This directory contains two programs gen_ref.c and gen_grid_bfly_hcube.c
which generate VLSI-type graphs and graphs more common in communication
area respectively. 

To create the executables, use the gcc commands:
gcc -lm gen_ref.c -o gen_ref
gcc -lm gen_grid_bfly_hcube.c -o gen_grid_bfly_hcube

1. gen_ref:
===========

Generates VLSI-type graphs, i.e., sparse graphs. Its usage is as follows:
     gen_ref size type_of_graph name_of_graph

Where
    size is explained below
    type_of_graph = Gxy, with x = 0,1,2 and y = 0,1 (explained below)
    the dot file produced is name_of_graph.dot and order file is
     name_of_graph.dot.ORD 

Each graph has 

(1) n = 3 * (2 ^ size) - 2  "cell" nodes (one side of the bipartition)
(2) 5n edges, distributed equally among the cell nodes
(3) 4n + 1 "net" nodes (the other side of the bipartition) for G0y graphs
    3n / 2 + 2 net nodes for G1y graphs
    floor[(n - 2) / 4] + 5 net nodes for G2y graphs
    where y = 0,1 corresponding to small or large # of crossings, respectively.


2. gen_grid_bfly_hcube
=======================

Usage:
   gen_grid_bfly_hcube n type_of_graph (grid,bfly,hcube) name_of_graph

Where
    n is a power of 2
    # of nodes = n for grid
                 n (lg n + 1) for butterfly
                 n for hypercube

Edges are formed based on the standard for these graphs.

Scripts
=======

Some scripts facilitate generation of these graphs and 64 scrambled versions
of each, please see under the scripts directory.

For example, scripts gen_00, gen_01, gen_10, gen_11, gen_20, gen_21 
will generate isomorphism classes (with 64 members) of 
G_k_00, G_k_01, G_k_10, G_k_11, G_k_20, G_k_21 respectively where
k = 2 through 7.

Similarly, gen_grid, gen_hcube and gen_bfly will generate isomorphism 
classes of grid, hypercube and butterfly graphs with n = 2^k where k
varies from 
 2 through 7 for butterfly
 2 through 9 for hypercube
 2 through 10 for grid.

IMPORTANT: You must have a seed_file at the directory from where you are
going to fire up the script. The seed_file consists of three 
unsigned short integers (range 0 to 65535) in three consecutive lines.
