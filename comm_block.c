// ************************************************************************
//
// miniAMR: stencil computations with boundary exchange and AMR.
//
// Copyright (2014) Sandia Corporation. Under the terms of Contract
// DE-AC04-94AL85000 with Sandia Corporation, the U.S. Government 
// retains certain rights in this software.
//
// This library is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as
// published by the Free Software Foundation; either version 2.1 of the
// License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
// Questions? Contact Courtenay T. Vaughan (ctvaugh@sandia.gov)
//                    Richard F. Barrett (rfbarre@sandia.gov)
//
// ************************************************************************

#include <mpi.hpp>

#include "block.h"
#include "comm.h"
#include "proto.h"

// This routine uses the communication structure for communicating ghost
// values to communicate the new processors of blocks that will be moving
// in the mesh during load balancing.  This coupled with blocks communicating
// to neighboring blocks on processor allows reconstruction of the structure
// used to communicate ghost values.
void comm_proc(void)
{
   int i, j, k, n, offset, dir, which, face, face_case, err, type;
   int *send_int = (int *) send_buff;
   int *recv_int = (int *) recv_buff;
   block *bp;

   for (dir = 0; dir < 3; dir++) {
      type = 10 + dir;

      for (i = 0; i < num_comm_partners[dir]; i++) {
         if (nonblocking)
            offset = comm_index[dir][i];
         else
            offset = 0;
         for (n = 0; n < comm_num[dir][i]; n++)
            send_int[offset+n] =
                        blocks[comm_block[dir][comm_index[dir][i]+n]].new_proc;
         // 由于RDMA不存在传统阻塞概念，所以无论是否有“nonblocking”，调用一致
         if (nonblocking)
            RDMA_Send(&send_int[comm_index[dir][i]], comm_num[dir][i],
                      R_TYPE_INT, comm_partner[dir][i]);
         else
            RDMA_Send(&send_int[0], comm_num[dir][i], R_TYPE_INT,
                     comm_partner[dir][i]);
      }

      // malloc一个状态表，用于标记该rank的数据是否被接收
      int rev_status = (int *)malloc(sizeof(int)*num_comm_partners[dir]);
      for(int i=0;i<num_comm_partners[dir];i++){
         rev_status[i]=0;
      }
      // 用于记录已经接收多少个rank的数据
      int count = 0;

      while (1) {
         //err = MPI_Waitany(num_comm_partners[dir], request, &which, &status);
         // 如果全部接收，则已完成，退出循环
         if(count >= num_comm_partners[dir]) break;

         // 遍历所有rank，试图接收数据
         for(which=0;which<num_comm_partners[dir];which++){
            // 该rank的数据已经接收，跳过
            if(rev_status[which]) continue;

            // 试图接收数据
            int rc = RDMA_Irecv(&recv_int[comm_index[dir][which]], comm_num[dir][which],
            R_TYPE_INT, comm_partner[dir][which]);

            // rc=0代表接收正常，进行数据处理
            if(rc == 0){
               // 将该rank的标志位置1
               rev_status[which] = 1;
               // count自增
               count++;

               for (n = 0; n < comm_num[dir][which]; n++) {
                  face = dir*2+(comm_face_case[dir][comm_index[dir][which]+n] >= 10);
                  bp = &blocks[comm_block[dir][comm_index[dir][which]+n]];
                  j = k = 0;
                  face_case = comm_face_case[dir][comm_index[dir][which]+n]%10;
                  if (face_case >= 6) {
                     j = ((face_case+2)/2)%2;
                     k = face_case%2;
                  }
                  bp->nei[face][j][k] = -1 - recv_int[comm_index[dir][which]+n];
               }
            }
         }
      }

      // 释放状态表
      free(rev_status);

      /*
       *  if (nonblocking)
       *     for (i = 0; i < num_comm_partners[dir]; i++)
       *        err = MPI_Waitany(num_comm_partners[dir], s_req, &which, &status);
       */
   }
}
