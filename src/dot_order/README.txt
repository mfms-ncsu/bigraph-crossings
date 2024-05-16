This program writes out an order file as optimized by dot. To do so,
it does a system call for a dot run and asks dot to produce the
output in dot format (you'll need  dot in your path to run this
program). It then parses the output produced by dot to find out
the order that dot has optmized.

To compile the progrem, just use gcc:
gcc -lm get_dot_order.c -o get_dot_order

Usage: get_dot_order (1)dot_file_name, (2)ord_file_name (3)nmb_of_layers

dot_file_name is the input graph in dot format.
ord_file_name is the output file for the order.
nmb_of_layers is no. of layers in the graph.
