/**
   @file gen_grid_bfly_hcube.c
   @brief generates grids, butterflies, or hypercubes as bipartite graphs in
   .dot format.
   @author Debabrata Ghosh and Matthias Stallmann
   @date 1999
 */

//     Copyright (C) 2001,2007  Matthias Stallmann, Debabrata Ghosh.
//     Contact: matt_stallmann@ncsu.edu
//
//     This program is free software; you can redistribute it and/or modify
//     it under the terms of the GNU General Public License as published by
//     the Free Software Foundation; either version 2 of the License, or
//     (at your option) any later version.
//
//     This program is distributed in the hope that it will be useful,
//     but WITHOUT ANY WARRANTY; without even the implied warranty of
//     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//     GNU General Public License for more details.
//
//     You should have received a copy of the GNU General Public License along
//     with this program (file COPYING.txt); if not, write to the Free Software
//     Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301
//     USA.

#include "gen_grid_bfly_hcube.h"

/*** This function converts a decimal no. into an n-bit  binary word, where n is given  **/
int decimal2binary(dec_no, nmb_bits, binary_word)
 
     int dec_no;
     int nmb_bits;
     char binary_word[];
{
  int i;
  int dividend, result,  remainder;

  dividend = dec_no; remainder=0;
  for(i=0; i<nmb_bits-1; i++) {
    remainder = dividend%2;
    dividend = dividend/2;
    if(remainder==0) binary_word[i]='0'; else binary_word[i]='1';
    if(i==nmb_bits-2) {if(dividend==0) binary_word[i+1]='0'; else binary_word[i+1]='1';}
  }

  binary_word[nmb_bits]='\0';

  return(0);
}                                   /* end of decimal2binary         */

int logbase2(int nmb)
{
  return( (int) (log(nmb)/log(2) ) );
}

/** checks the parity of binary words, 0=even parity, 1=odd parity **/
int check_parity(char binary_word[], int nmb_bits)
{
  int i, parity=0;
  
  for(i=0; i<nmb_bits; i++)
    if( binary_word[i]=='1') parity++;

  return(parity%2);
}

/** 
    Given the binary addresses of two nodes, this function checks 
    whether they would be adjacent in  H'cube or not 
**/
int hcube_adjacent(char word1[], char word2[], int nmb_bits)
{
  int i, yes;
  int diff=0;
  for(i=0; i<nmb_bits; i++) {
    if( word1[i]!=word2[i]) { diff++; }
  }

  if(diff==1) yes=1; else yes=0;
  return(yes);
}


int connect_hypercube(gate *cell_node, net *net_node, 
                      int nmb_cell, int nmb_net, int nmb_bits) 
{

  /** Two nodes will be adjacent in a H'Cube if their binary addresses
      differ only in 1 bit position **/

  int i, j;
  int ip_idx=0;

  for(i=0; i<nmb_cell; i++) {
    ip_idx=0;
    for(j=0; j<nmb_net; j++) {
      if(hcube_adjacent(cell_node[i].op, net_node[j].name, nmb_bits)==1)  {
        strcpy(cell_node[i].inp[ip_idx], net_node[j].name);
        net_node[j].out_degree++;
        ip_idx++;
      }
    }

    if(ip_idx != nmb_bits) {
      fprintf(stderr,"Error: Cell %d (%s) has only %d inputs named (should be %d)\n",
              cell_node[i].dec_op, cell_node[i].op, ip_idx, nmb_bits);
      exit(3);
    }
    cell_node[i].in_degree=nmb_bits;
  }

  return(0);
}

int adjacent_butterfly(gate *cell_node, net *net_node, int cell_idx, int net_idx, int nmb_bits) {

  int i, yes;
  int ce,ne;
  int condn;
  int check_bit;

  /** each b'fly edge is of the form (i,w)->(i+1,w') where w' is either
      identical to w or differ from w at the i-th bit position. Note that
      i is stored in the element 'other_idx' and an edge can exist from
      a net-node (source) to a cell-node (sink) only. So, when two nodes
      differ in the 'other_idx' by 1, we determine which bit will be the
      check_bit. If the w-address (expressed in binary) stored in the 
      'name' element of net_node or 'op' element of cell_node are identical
      or differ only at the bit position 'other_idx', we form an edge.
  **/

  ce = cell_node[cell_idx].other_idx;
  ne = net_node[net_idx].other_idx;


  if(ne-ce==1) { check_bit=ce; condn=1;}
  else if(ce-ne==1) { check_bit=ne; condn=1;}
  else condn=0;

  yes=condn;
  if( condn ) {
 
    for(i=0; i<nmb_bits; i++) {
      if(cell_node[cell_idx].op[i] != net_node[net_idx].name[i] && i!=check_bit) yes=0;
    }
  }

  return(yes);
}

int connect_butterfly(gate *cell_node, net *net_node, int nmb_cells, int nmb_net, int nmb_bits)

{
  int i,j, inp_idx;
  char buff[200];

  /**
     See how the edges are formed in the function adjacent_butterfly 
  **/

  for(i=0; i<nmb_cells; i++) {
    inp_idx=0;
    for(j=0; j<nmb_net; j++) {
      if(adjacent_butterfly(cell_node, net_node, i, j, nmb_bits)==1)  {
        sprintf(cell_node[i].inp[inp_idx], "%d_%s", net_node[j].other_idx, net_node[j].name);
        net_node[j].out_degree++;
        inp_idx++;
      }
    }
    sprintf(buff, "%d_%s", cell_node[i].other_idx, cell_node[i].op);
    strcpy(cell_node[i].op, buff);
    cell_node[i].in_degree=inp_idx;
  }
  for(j=0; j<nmb_net; j++) {
    sprintf(buff, "%d_%s", net_node[j].other_idx, net_node[j].name);
    strcpy(net_node[j].name, buff);
  }
  return(0);
}

/** (x1,y1) and (x2,y2) are adjacent in a grid if |x1-x2|=1 or |y1-y2|=1
    but not both at the same time **/

int adjacent_grid(gate *cell_node, net *net_node, int cell_idx, int net_idx) {
  int net_i,net_j,yes;
  int cell_i,cell_j;

  sscanf(cell_node[cell_idx].op, "%d_%d", &cell_i, &cell_j);
  sscanf(net_node[net_idx].name, "%d_%d", &net_i, &net_j);

  if( 
     ( abs( cell_i - net_i ) == 1 || abs( cell_j - net_j ) == 1)
     &&  !( abs( cell_i - net_i ) + abs( cell_j - net_j ) > 1)
      ) {
    yes=1;
  }
  else  {
    yes=0;
  }

  return(yes);
}

int connect_grid(gate *cell_node, net *net_node, int nmb_cells, int nmb_net, int nmb_bits)
{
  int i,j, inp_idx;

  for(i=0; i<nmb_cells; i++) {
    inp_idx=0;
    for(j=0; j<nmb_net; j++) {
      if(adjacent_grid(cell_node, net_node, i, j)==1)  {
        strcpy(cell_node[i].inp[inp_idx], net_node[j].name);
        net_node[j].out_degree++;
        inp_idx++;
      }
    }
    cell_node[i].in_degree=inp_idx;
  }
  return(0);
}

int main (argc, argv)


     int   argc;
     char *argv[];
{
  gate *cell_node;/** structure for SINK nodes, also called cell/gate in VLSI **/
  net *net_node;  /** structure for SOURCE nodes, also called net-nodes in VLSI **/
  int n, k, w;
  int i,j,m, len;
  int max_x, max_y; /** used for calculating max. x,y index in grid **/
  int max_i, max_w; /** used for calculating max. i,w index in butterfly **/
  int nmb_vert, nmb_bits, nmb_edge, nmb_cell, nmb_net;
  int in_degree, cell_idx, net_idx;
  FILE *dot_file;
  FILE *ord_file;
  char buff[FILENAME_LENGTH];
  char binary_word[20];


  if(argc != 4)
    {
      fprintf(stderr, "Usage: gen_grid_bfly_hcube n type_of_graph (grid,bfly,hcube) name_of_graph\n"); 
      fprintf(stderr, "   where number of nodes is n (grid,hcube) or n(lg n + 1) (bfly)\n"); 
      exit(1);
    }

  n=atoi(argv[1]);


  nmb_bits=logbase2(n);

  /** 
      First calculate the no. of edges, source/sink vertices (nets/cells
      respectively) in_degree (if possible to calculate), max value of indices
      (for grid/b'fly) etc.
  **/
   
  if(strcmp(argv[2], "hcube")==0) {
    nmb_cell = n/2;
    nmb_edge = n*logbase2(n)/2;
    nmb_net = n/2;
    in_degree=logbase2(n);
  }
  else if(strcmp(argv[2], "grid")==0) {
    if( (logbase2(n) )%2 == 0)  {
      max_x = max_y = sqrt(n);
      nmb_edge = 2*(n- sqrt(n));
    } 
    else {
      max_x = sqrt(2*n);
      max_y = sqrt(n/2);
      nmb_edge = 2*n-3*n/2 ;
    }
    in_degree=4;
    nmb_cell=nmb_net=n/2;
  }
  else if(strcmp(argv[2], "bfly")==0) {
    nmb_vert=n*(logbase2(n)+1);
    nmb_edge=2*n*logbase2(n);
    max_i = logbase2(n)+1;
    max_w = n;
    nmb_cell = (max_i/2)*max_w;
    nmb_net = (max_i - (max_i/2) )*max_w;
    nmb_bits = logbase2(max_w);
  }
  else {
    fprintf(stderr, "Invalid graph type: %s\n", argv[2]);
    exit(3);
  }

  /**
     Allocate memory based on the number of nets/cells needed 
  **/

  if (  (cell_node=(gate *)(  calloc( (nmb_cell),(sizeof(gate)) )  )  ) == NULL)
    {fprintf(stderr,"Error in mallocation of *cell_node\n");exit(2);}; 

  if (  (net_node=(net *)(  calloc( (nmb_net+1),(sizeof(net)) )  )  ) == NULL)
    {fprintf(stderr,"Error in mallocation of *net_node\n");exit(2);}; 


  /** 
      Now designate some nodes as net (source) nodes and others as cell (sink)
      nodes. This designation is well-defined, and it is based on the structure
      of the graph. This step basically forms the two sets of nodes which will
      be later connected.
  **/


  if(strcmp(argv[2],"hcube")==0) {
    cell_idx=0; net_idx=0;
    /** For hypercube, each node is indexed from 0 to n-1 and the binary equivalent
        of this index is the address. When this address has an odd-parity we
        call the node a cell-node (source), otherwise a net-node (sink) **/
    for(i=0; i<n; i++) {
      decimal2binary(i, nmb_bits, binary_word); 
      if( check_parity(binary_word, nmb_bits) == 1) {
        strcpy( cell_node[cell_idx].op, binary_word); 
        /** The binary address will constitute the actual name **/
        cell_node[cell_idx].dec_op = i; /** just copy the index no. to dec_op **/
        cell_idx++;  /** count how many cells are being assigned **/
      }
      else {
        strcpy( net_node[net_idx].name, binary_word);
        /** The binary address will constitute the actual name **/
        net_node[net_idx].dec_name = i; /** dec_name is equivalent to dec_op of cells **/
        net_idx++;  /** count how many cells are being assigned **/
      }
    }
  }
  else if(strcmp(argv[2],"grid")==0) {

    /*** Each node in a grid has a (x,y) co-ordinate. When x+y is even, we
         call it a net-node, when odd, we call it a cell-node. Name is
         a concatenation of x and y co-orinates, with a '_' in between. **/
    cell_idx=0; net_idx=0;
    for(i=0; i<max_x; i++) {
      for(j=0; j<max_y; j++) {
        if(  (i+j)%2 == 0)  {
          sprintf( net_node[net_idx].name, "%d_%d", i,j);
          net_idx++;
        }
        else {
          sprintf( cell_node[cell_idx].op, "%d_%d", i,j);
          cell_idx++;
        }
      }
    }
    /** double-check if a proper no. of cells and nets have been assigned **/
    if(cell_idx != nmb_cell || net_idx!=nmb_net) {
      fprintf(stderr,"Problem with grid: nmb_net=%d, net_idx=%d, nmb_cell=%d, cell_idx=%d\n",
              nmb_net, net_idx, nmb_cell, cell_idx);
      exit(5);
    }
  }
  else if(strcmp(argv[2],"bfly")==0) {
    /** Each vertex in a butterfly has two indices (i,w). w is expressed as
        a binary word and stored in the 'name' or 'op' elenment of the
        structures for a net/cell node. The i-index is stored in the
        element 'other_idx' of both the structures. All nodes with an
        even i are called net-nodes, cell-nodes are with odd i. **/

    cell_idx=0; net_idx=0;
    for(i=0; i<max_i; i++)  {
      for(w=0; w<max_w; w++)  {
        if(i%2==0) { /** then it's a net node **/
          net_node[net_idx].other_idx=i;
          decimal2binary(w, nmb_bits, binary_word); 
          strcpy( net_node[net_idx].name, binary_word);
          net_idx++;
        }
        else {
          cell_node[cell_idx].other_idx=i;
          decimal2binary(w, nmb_bits, binary_word); 
          strcpy( cell_node[cell_idx].op, binary_word);
          cell_idx++;
        }
      }
    }
    /** double-check for any discrepancy in the no. of cell/net nodes **/
    if(cell_idx != nmb_cell || net_idx!=nmb_net) {
      fprintf(stderr,"Problem with bfly: nmb_net=%d, net_idx=%d, nmb_cell=%d, cell_idx=%d\n",
              nmb_net, net_idx, nmb_cell, cell_idx);
      exit(5);
    }
  }
  else {
    fprintf(stderr, "Invalid graph type: %s\n", argv[2]); exit(3);
  }


  /** Now do the actual connections **/
  if(strcmp(argv[2],"hcube")==0)
    connect_hypercube(cell_node, net_node, nmb_cell, nmb_net, nmb_bits); 
  else if(strcmp(argv[2],"grid")==0)
    connect_grid(cell_node, net_node, nmb_cell, nmb_net, nmb_bits); 
  else if(strcmp(argv[2],"bfly")==0)
    connect_butterfly(cell_node, net_node, nmb_cell, nmb_net, nmb_bits); 
  


  /** Once all connections have been updated in the data structure,
      we can write out them in the .dot and .dot.ORD files. The
      rest of the main function does just this output operation **/
  
  sprintf(buff,"%s.dot", argv[3]);
  if( (dot_file=fopen(buff,"w"))==NULL) {
    fprintf(stderr,"Can't open file %s for writing\n", buff); exit(2);
  }

  fprintf(dot_file, "digraph %s /* seed: 0 0 0 */ {\n", argv[3]);

  sprintf(buff,"%s.dot.ORD", argv[3]);
  if( (ord_file=fopen(buff,"w"))==NULL) {
    fprintf(stderr,"Can't open file %s for writing\n", buff); exit(2);
  }

  fprintf(ord_file, "# Natural ordering for graph %s\n", argv[3]);
  fprintf(ord_file, "# seed: 0 0 0\n");

  /** Print out the orders in the order file **/
  fprintf(ord_file, "# Ordering for layer 0\n0 { \n");
  len=0;
  for(i=0; i<nmb_net; i++) {
    fprintf(ord_file, " b%s", net_node[i].name);
    len += ( strlen(net_node[i].name) +1 );
    if(len > 72 && i<nmb_net-1) {fprintf(ord_file,"\n"); len=0;}
  }
  fprintf(ord_file, "\n} \n# End of ordering for layer 0\n");

  fprintf(ord_file, "# Ordering for layer 1\n1 { \n");
  len=0;
  for(i=0; i<nmb_cell; i++) {
    fprintf(ord_file, " a%s", cell_node[i].op);
    len += ( strlen(cell_node[i].op)+1);
    if(len > 72 && i<nmb_cell-1) {fprintf(ord_file,"\n"); len=0;}
  }
  fprintf(ord_file, "\n} \n# End of ordering for layer 1\n");

  /** The edges written out in the dot file should be sorted by the order
      of the source nodes (net nodes) and edges adjacent to the same 
      source node (net-node) should be sorted by the order of the sink
      (cell) nodes. **/

  for(i=0; i<nmb_net; i++) { /** this is a slow method, but speed is not paramount here **/
    for(j=0; j<nmb_cell; j++) {
      for(m=0; m<cell_node[j].in_degree; m++) {
        if(strcmp(net_node[i].name, cell_node[j].inp[m])==0)
          fprintf(dot_file, "b%s -> a%s;\n", net_node[i].name, cell_node[j].op);
        /** add a prefix 'b' to all net-nodes, 'a' to all cell-nodes, just to 
            make them easy to identify  **/
      }
    }
  }

  fprintf(dot_file, "}\n");
  fclose(dot_file);
  fclose(ord_file);

  exit(0);
}

/*  [Last modified: 2015 05 17 at 18:45:36 GMT] */
