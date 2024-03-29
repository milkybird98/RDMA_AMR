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

#ifndef BLOCK_H
#define BLOCK_H

typedef struct {
   int number;
   int level;
   int refine;
   int new_proc;
   int parent;       // if original block -1,
                     // else if on node, number in structure
                     // else (-2 - parent->number)
   int parent_node;
   int child_number;
   int nei_refine[6];
   int nei_level[6];  /* 0 to 5 = W, E, S, N, D, U; use -2 for boundary */
   int nei[6][2][2];  /* negative if off processor (-1 - proc) */
   int cen[3];
   double ****array;
} block;
extern block *blocks;

typedef struct {
   int number;
   int level;
   int parent;      // -1 if original block
   int parent_node;
   int child_number;
   int refine;
   int child[8];    // n if on node, number if not
                    // if negative, then onnode child is a parent (-1 - n)
   int child_node[8];
   int cen[3];
} parent;
extern parent *parents;

typedef struct {
   int number;     // number of block
   int n;          // position in block array
} sorted_block;
extern sorted_block *sorted_list;
extern int *sorted_index;

extern int my_pe;
extern int num_pes;

extern int max_num_blocks;
extern int target_active;
extern int target_max;
extern int target_min;
extern int num_refine;
extern int uniform_refine;
extern int x_block_size, y_block_size, z_block_size;
extern int num_vars;
extern int comm_vars;
extern int init_block_x, init_block_y, init_block_z;
extern int reorder;
extern int npx, npy, npz;
extern int inbalance;
extern int refine_freq;
extern int report_diffusion;
extern int checksum_freq;
extern int stages_per_ts;
extern int error_tol;
extern int num_tsteps;
extern int stencil;
extern int report_perf;
extern int plot_freq;
extern int lb_opt;
extern int block_change;
extern int code;
extern int permute;
extern int nonblocking;
extern int refine_ghost;

extern int max_num_parents;
extern int num_parents;
extern int max_active_parent;
extern int cur_max_level;
extern int *num_blocks;
extern int *local_num_blocks;
extern int *block_start;
extern int num_active;
extern int max_active_block;
extern int global_active;
extern int x_block_half, y_block_half, z_block_half;
extern double tol;
extern double *grid_sum;
extern int *p8, *p2;
extern int mesh_size[3];
extern int max_mesh_size;
extern int *from, *to;
extern int msg_len[3][4];
extern int local_max_b;
extern int global_max_b;

extern int num_objects;
typedef struct {
   int type;
   int bounce;
   double cen[3];
   double orig_cen[3];
   double move[3];
   double orig_move[3];
   double size[3];
   double orig_size[3];
   double inc[3];
} object;
extern object *objects;

extern int num_dots;
extern int max_num_dots;
extern int max_active_dot;
typedef struct {
   int cen[3];
   int number;
   int n;
   int proc;
   int new_proc;
} dot;
extern dot *dots;

#endif
