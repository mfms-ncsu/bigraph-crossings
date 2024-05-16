Some small examples for testing the crossing number heuristics.  Each has a
.dot file describing the bigraph and a .ord file giving the initial order.
Running sbg_cn_eval on x.dot x.ord gives the initial cost.  The best order
obtained by treatment yy is in file x_tr00yy.ord and its cost is obtained
using sbg_cn_eval x.dot x_tr00yy.ord. 

A bash script for running all treatments on all the graphs is (other
treatments can be added or some can be omitted):

rm -f *_tr00??.ord
rm -f current_output
for file in *.dot; do
  base=`basename $file .dot`
  for treatment in 01 03 04 06 07 09 14 15 17 23 24 25 26 27 28 30; do
    ../sbg_treat -tr=$treatment $file $base.ord
    ../sbg_cn_eval $file ${base}_tr00$treatment.ord
  done
done > current_output
# compare with expected output
diff current_output expected_output

Output from sbg_cn_eval is shown for various treatments in cases where they
do not all agree.

p1 -- a path of length 1 (initial cost = 0, opt = 0); obvious boundary case

p4 -- a path of length 4 (init = 1, opt = 0)

c4 -- a cycle of length 4 (init = opt = 1)

c6 -- a cycle of length 6 (init = 6, opt = 2) 

g_1_xy -- graphs based on VLSI patterns; these graphs have
       4 nodes on one side

       17 and 8 nodes on the other side for x = 0 and 1, respectively
       (density increases with increasing x; this is accomplished by
       decreasing the number of nodes) 

       20 edges

       y = 0 if crossing number is small, 1 if it's large

G_1_00 results (init =  100, opt  =    0):

g_1_00_tr0001   9
g_1_00_tr0003   9
g_1_00_tr0006   11
g_1_00_tr0007   9
g_1_00_tr0009   9
g_1_00_tr0014   0
g_1_00_tr0015   0
g_1_00_tr0017   0

G_1_01 results (init =   86, best =    5):

g_1_01_tr0001   16
g_1_01_tr0003   5
g_1_01_tr0006   8
g_1_01_tr0007   5
g_1_01_tr0009   5
g_1_01_tr0014   5
g_1_01_tr0015   5
g_1_01_tr0017   5

G_1_10 results (init =   63, best =   24):

g_1_10_tr0001   24
g_1_10_tr0003   24
g_1_10_tr0006   28
g_1_10_tr0007   24
g_1_10_tr0009   24
g_1_10_tr0014   28
g_1_10_tr0015   24
g_1_10_tr0017   24

G_1_11 results (init =   98, best =   29):

g_1_11_tr0001   29
g_1_11_tr0003   29
g_1_11_tr0006   38
g_1_11_tr0007   30
g_1_11_tr0009   29
g_1_11_tr0014   29
g_1_11_tr0015   29
g_1_11_tr0017   29

G_3_2y -- more VLSI graphs, larger and denser: (22,10) and 110 edges

G_3_20 results (init = 2827, best =  950):

g_3_20_tr0001   950
g_3_20_tr0003   950
g_3_20_tr0006   1149
g_3_20_tr0007   950
g_3_20_tr0009   950
g_3_20_tr0014   1072
g_3_20_tr0015   950
g_3_20_tr0017   950

G_3_21 results (init = 2475, best = 1454):

g_3_21_tr0001   1470
g_3_21_tr0003   1686
g_3_21_tr0006   1715
g_3_21_tr0007   1579
g_3_21_tr0009   1462
g_3_21_tr0014   1885
g_3_21_tr0015   1493
g_3_21_tr0017   1454

butterfly_32 -- a 32-node butterfly graph (init = 533, best = 142)

butterfly_32_tr0001     208
butterfly_32_tr0003     144
butterfly_32_tr0006     167
butterfly_32_tr0007     150
butterfly_32_tr0009     142
butterfly_32_tr0014     160
butterfly_32_tr0015     150
butterfly_32_tr0017     142

cube_16 -- a 16-node hypercube (init = 196, best = 116)

cube_16_tr0001  122
cube_16_tr0003  116
cube_16_tr0006  118
cube_16_tr0007  116
cube_16_tr0009  118
cube_16_tr0014  118
cube_16_tr0015  116
cube_16_tr0017  118

grid_16 -- a 16-node square grid (init = 125, best = 33)

grid_16_tr0001  73
grid_16_tr0003  33
grid_16_tr0006  37
grid_16_tr0007  36
grid_16_tr0009  33
grid_16_tr0014  35
grid_16_tr0015  34
grid_16_tr0017  33

tree_100 -- a random tree with 100 nodes on each side (init = 9517, best = 372)

tree_100_tr0001 2014
tree_100_tr0003 709
tree_100_tr0006 814
tree_100_tr0007 499
tree_100_tr0009 418
tree_100_tr0014 608
tree_100_tr0015 375
tree_100_tr0017 372

tree_075 -- an example where treatment 3 differs from treatment 27, showing
that cost is not strictly decreasing for the barycenter heuristic (also true
of tree_100, to an even greater extent)
