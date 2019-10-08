# miniAMR mini-application
A proxy version of miniAMR based on ffRDMA,which is a homemade mpi implementation over fake rdma sockset.

***

## miniAMR overview:

miniAMR applies a stencil calculation on a unit cube computational  domain,
which is divided into blocks. The blocks all have the same number of cells
in each direction and communicate ghost values with neighboring blocks. With
adaptive mesh refinement, the blocks can represent different levels of
refinement in the larger mesh. Neighboring blocks can be at the same level
or one level different, which means that the length of cells in neighboring
blocks can differ by only a factor of two in each direction. The calculations
on the variables in each cell is an averaging of the values in the chosen
stencil. The refinement and coarsening of the blocks is driven by objects
that are pushed through the mesh. If a block intersects with the surface
or the volume of an object, then that block can be refined. There is also
an option to uniformly refine the mesh. Each cell contains a number of
variables, each of which is evaluated indepently.

***

## Build dependency:
+ ffrdma
+ librdmacm
+ libibverbs
+ cmake (version higher than 3.0.0)

***


## Building miniAMR:

To make the code,:
+ first, update the submodule to get correct ffrdma library
+ then, go into ffrdma directory, type 'mkdir build && cd build', and type 'cmake .. && make -j'
+ then, go back to miniAMR directory, type 'make' in the directory containing the source.

The enclosed Makefile is configured for ffrdma version compilation.
Other compiler or other machines will need changes in the CFLAGS
variable to correspond with the flags available for the compiler being used.

## Running miniAMR:
arguments consists by to parts: one is ffrdma argument, and another is origin miniAMR argument, for the latter, plz refer to the README.old, for the ffrdma arguments:
+ --r_myip : The ip address of this process
+ --r_myport : The port of this process
+ --r_hostmap : The hostmap for all clients
+ -- : at the last place, followed with origin miniAMR arguments
