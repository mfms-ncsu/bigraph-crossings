/**
   @file gen_ref.c
   @brief Generates VLSI-type graphs, i.e., sparse graphs with the following
   characteristics.
   @author Debabrata Ghosh and Matthias Stallmann
   @date 1999
   $Id: gen_ref.c 75 2015-05-17 19:54:36Z mfms $
   
   Its usage is as follows:
     gen_ref size type_of_graph name_of_graph

   Where
     size is explained below
     type_of_graph = Gxy, with x = 0,1,2 and y = 0,1 (explained below)
     the dot file produced is name_of_graph.dot and order file is
     name_of_graph.dot.ORD 

Each graph has 

-# n = 3 * (2 ^ size) - 2  "cell" nodes (one side of the bipartition)
-# 5n edges, distributed equally among the cell nodes
-# 4n + 1 "net" nodes (the other side of the bipartition) for G0y graphs
    3n / 2 + 2 net nodes for G1y graphs
    floor[(n - 2) / 4] + 5 net nodes for G2y graphs
    where y = 0,1 corresponding to small or large # of crossings, respectively.
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

#include "gen.h"

int connect_G00(gate *cell_node, int k, int nmb_node, int nmb_net) {

  int i;
  
  for(i=0; i<nmb_node; i++) {
    sprintf(cell_node[i].inp[1], "b%d", 4*i);
    sprintf(cell_node[i].inp[2], "b%d", 4*i+1);
    sprintf(cell_node[i].inp[3], "b%d", 4*i+2);
    sprintf(cell_node[i].inp[4], "b%d", 4*i+3);
    sprintf(cell_node[i].inp[5], "b%d", 4*i+4);
    sprintf(cell_node[i].op, "a%d", i);
  }

  return(0);
}

int connect_G01(gate *cell_node, int k, int nmb_node, int nmb_net) {

  int i;

  for(i=0; i<nmb_node; i++) {
    if(i==0)
      {
        sprintf(cell_node[i].inp[1], "b0");
        sprintf(cell_node[i].inp[2], "b1");
        sprintf(cell_node[i].inp[3], "b2");
        sprintf(cell_node[i].inp[4], "b3");
        sprintf(cell_node[i].inp[5], "b4");
        sprintf(cell_node[i].op, "a0");
      }
    else if(i>=1 && i<=3)
      {
        sprintf(cell_node[i].inp[1], "b%d", i+1);
        sprintf(cell_node[i].inp[2], "b%d", 4*i+1);
        sprintf(cell_node[i].inp[3], "b%d", 4*i+2);
        sprintf(cell_node[i].inp[4], "b%d", 4*i+3);
        sprintf(cell_node[i].inp[5], "b%d", 4*i+4);
        sprintf(cell_node[i].op, "a%d", i);
      }
    else if(i>3)
      {
        sprintf(cell_node[i].inp[1], "b%d", 2*i-3);
        sprintf(cell_node[i].inp[2], "b%d", 4*i+1);
        sprintf(cell_node[i].inp[3], "b%d", 4*i+2);
        sprintf(cell_node[i].inp[4], "b%d", 4*i+3);
        sprintf(cell_node[i].inp[5], "b%d", 4*i+4);
        sprintf(cell_node[i].op, "a%d", i);
      }
  }

  return(0);
}

int connect_G10(gate *cell_node, int k, int nmb_node, int nmb_net) {

  int i;

  for(i=0; i<nmb_node; i++) {
    sprintf(cell_node[i].inp[1], "b%d", 3*(i/2));
    sprintf(cell_node[i].inp[2], "b%d", 3*(i/2)+1);
    sprintf(cell_node[i].inp[3], "b%d", 3*(i/2)+2);
    sprintf(cell_node[i].inp[4], "b%d", 3*(i/2)+3);
    sprintf(cell_node[i].inp[5], "b%d", 3*(i/2)+4);
    sprintf(cell_node[i].op, "a%d", i);
  }

  return(0);
}

int connect_G11(gate *cell_node, int k, int nk, int nmb_net) {

  int i;

  for(i=0; i<nk; i++) {
    if(i%2==0) {
      sprintf(cell_node[i].inp[1], "b%d", i);
      sprintf(cell_node[i].inp[2], "b%d", i+1);
      sprintf(cell_node[i].inp[3], "b%d", i+2);
      sprintf(cell_node[i].inp[4], "b%d", i+3);
      sprintf(cell_node[i].inp[5], "b%d", nk+2 + i%(nk/2) );
      sprintf(cell_node[i].op, "a%d", i);
    }
    else {
      sprintf(cell_node[i].inp[1], "b%d", i-1);
      sprintf(cell_node[i].inp[2], "b%d", i);
      sprintf(cell_node[i].inp[3], "b%d", i+1);
      sprintf(cell_node[i].inp[4], "b%d", i+2);
      sprintf(cell_node[i].inp[5], "b%d", nk+2 + i%(nk/2) );
      sprintf(cell_node[i].op, "a%d", i);
    }
  }

  return(0);
}
int connect_G20(gate *cell_node, int k, int nmb_node, int nmb_net) {

  int i;
  
  for(i=0; i<nmb_node; i++) {
    sprintf(cell_node[i].inp[1], "b%d", i/4);
    sprintf(cell_node[i].inp[2], "b%d", i/4+1);
    sprintf(cell_node[i].inp[3], "b%d", i/4+2);
    sprintf(cell_node[i].inp[4], "b%d", i/4+3);
    sprintf(cell_node[i].inp[5], "b%d", i/4+4);
    sprintf(cell_node[i].op, "a%d", i);
  }

  return(0);
}

int connect_G21(gate *cell_node, int k, int nk, int nmb_net) {

  int i;
  
  for(i=0; i<nk; i++) {
    sprintf(cell_node[i].inp[1], "b%d", i/8);
    sprintf(cell_node[i].inp[2], "b%d", i/8+1);
    sprintf(cell_node[i].inp[3], "b%d", i/8+2);
    sprintf(cell_node[i].inp[4], "b%d", i/8+3);
    sprintf(cell_node[i].inp[5], "b%d", (nk-6)/8 + 4 + (i/4)%( (nk-6)/8 + 2)  );
    sprintf(cell_node[i].op, "a%d", i);
  }

  return(0);
}

int main (argc, argv)
     int   argc;
     char *argv[];
{
  gate *cell_node; /** array of sink nodes. These are called cell nodes in VLSI **/
  int k, nk; /** k is the parameter used to specify the size of the
                 graph, nk=no. of vertices, derived from k; nk=3*2^k-2 **/
  int i,j, m, len;
  int nmb_edge; 
  int nmb_net; /** nmb of source nodes, also called nets in VLSI **/
  int nmb_net_0y, nmb_net_1y, nmb_net_2y; /** nmb of nets for G0y, G1y, G2y graphs **/
  FILE *dot_file;
  FILE *ord_file;
  char buff[FILENAME_LENGTH];

  if(argc != 4)
    {
      fprintf(stderr, "Usage: gen_ref k type_of_graph (Gxy, x=0,1,2,; y=0,1) name_of_graph\n"); 
      exit(1);
    }

  k=atoi(argv[1]);
  nk = (int)(3*pow(2,k))-2;
  nmb_edge = 5*nk;
  nmb_net_0y = 4*nk+1;
  nmb_net_1y = 3*nk/2+2;
  nmb_net_2y = (nk-2)/4+5;

  if (  (cell_node=(gate *)(  calloc( (nk+1),(sizeof(gate)) )  )  ) == NULL)
    {fprintf(stderr,"Error in mallocation of *cell_node\n");exit(2);}; 


  /** these are simple routines for connecting the nodes, it will be
      evident from the functions how the edges are formed for different
      types of graph (G00, G01, G10, G11, G20, G21) **/

  if(strcmp(argv[2], "G00")==0) {
    nmb_net = nmb_net_0y;
    connect_G00(cell_node, k, nk, nmb_net);
  }
  else if(strcmp(argv[2], "G01")==0) {
    nmb_net = nmb_net_0y;
    connect_G01(cell_node, k, nk, nmb_net);
  }
  else if(strcmp(argv[2], "G10")==0) {
    nmb_net = nmb_net_1y;
    connect_G10(cell_node, k, nk, nmb_net);
  }
  else if(strcmp(argv[2], "G11")==0) {
    nmb_net = nmb_net_1y;
    connect_G11(cell_node, k, nk, nmb_net);
  }
  else if(strcmp(argv[2], "G20")==0) {
    nmb_net = nmb_net_2y;
    connect_G20(cell_node, k, nk, nmb_net);
  }
  else if(strcmp(argv[2], "G21")==0) {
    nmb_net = nmb_net_2y;
    connect_G21(cell_node, k, nk, nmb_net);
  }
  else {
    fprintf(stderr, "Invalid graph type : %s\n", argv[2]); exit(3);
  }
  


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
    fprintf(ord_file, " b%d", i);  /** net nodes are just called "a<index_no.>" **/
    sprintf(buff, " b%d", i);
    len += strlen(buff);
    if(len > 72 && i<nmb_net-1) {fprintf(ord_file,"\n"); len=0;}
  }
  fprintf(ord_file, "\n} \n# End of ordering for layer 0\n");

  fprintf(ord_file, "# Ordering for layer 1\n1 { \n");
  len=0;
  for(i=0; i<nk; i++) {
    fprintf(ord_file, " a%d", i); /** this is the order of the cell nodes **/
    sprintf(buff, " a%d", i);
    len += strlen(buff);
    if(len > 72 && i<nk-1) {fprintf(ord_file,"\n"); len=0;}
  }
  fprintf(ord_file, "\n} \n# End of ordering for layer 1\n");

  /** The edges written out in the dot file should be sorted by the order
      of the source nodes (net nodes) and edges adjacent to the same 
      source node (net-node) should be sorted by the order of the sink
      (cell) nodes. **/

  for(i=0; i<nmb_net; i++) { /** this is a slow method, but speed is not paramount here **/
    sprintf(buff, "b%d", i); /** this is the name of the net node (source node) **/
    for(j=0; j<nk; j++) {
      for(m=1; m<=MAX_IN_DEGREE; m++) {
        if(strcmp(buff, cell_node[j].inp[m])==0)
          fprintf(dot_file, "%s -> %s;\n", buff, cell_node[j].op);
      }
    }
  }

  fprintf(dot_file, "}\n");
  fclose(dot_file);
  fclose(ord_file);

  exit(0);
}

/*  [Last modified: 2015 05 17 at 19:05:31 GMT] */
