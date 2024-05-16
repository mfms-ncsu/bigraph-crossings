//     Copyright (C) 2001  Matthias Stallmann, Debabrata Ghosh.
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

#include<stdio.h>
#include<stdlib.h>
#ifdef HAVE_MALLOC_H
#include<malloc.h>
#endif
#include<math.h>
#include<string.h>
#include<limits.h>

#include<sys/times.h>
#include<sys/types.h>


#define WIRENAME_LENGTH 32
#define FILENAME_LENGTH 512
#define MAX_LINE_LENGTH 2000
#define MAX_NODE (1 << 16) 
#define MAX_OUT_DEGREE (1 << 8)



typedef struct
  {
    char name[WIRENAME_LENGTH];   /** name of this source node **/
    short out_degree;             /** no. of edges adjacent to it **/
    unsigned short *adj_list;     /** which sink nodes are adjacent to it,  **/
                                  /** given just in terms of array index of **/
                                  /** the sink node it is adjacent to       **/
    unsigned short *adj_list_bak; /** this is just a back-up of adj_list    **/
  } node;


int main (argc, argv)


int   argc;
char *argv[];
{
 int nmb_scr;   /** No. of scrambled versions to be generated **/
 int scr_idx;   /** Index for scrmabled versions **/
 int nmb_source_node, nmb_sink_node;
 int source_idx, sink_idx;
 int i,j,k,l, m, len;
 int temp;
 int verbose=0;
 int start_comment, end_comment, start_qual, end_qual; 
       /** used to check any comment, qualifier etc. in dot **/
 int write_back_to_seed_file;
 int *new_src_ary;  /** stores the new order of the source nodes **/
 int *new_sink_ary; /** stores the new order of the sink nodes **/
 int *map_sink_ary; /** gives the position of the j-th sink node in the new_sink_ary  **/
 int *param;  /** to pass on to the search_and_insert function **/
 unsigned short Xi[3], Xi_orig[3];
 char root_name[FILENAME_LENGTH];
 char buff[MAX_LINE_LENGTH];
 char name[100][WIRENAME_LENGTH];
 node *source_node;  /** Array of source nodes **/
 node *sink_node;
 FILE *r_file, *w_file, *dot_file, *ord_file;
 if(argc != 6)
 {
   fprintf(stderr, "Usage: scramble_dot dot_file n seed_file verbose (0/1) modify_seed_file (0/1)\n");
   fprintf(stderr, " where n is the number of scrambled copies\n");
   exit(1);
 }

  nmb_scr=atoi(argv[2]);
  verbose=atoi(argv[4]);
  write_back_to_seed_file=atoi(argv[5]);

/** Extract the root file-name from argv[1], ensure that it has a .dot extension **/
  len = strlen(argv[1]);
   for(i=len-4; i<len; i++) root_name[i-len+4]=argv[1][i];
    root_name[i-len+4]='\0';
   if( strcmp(root_name,".dot") != 0) {
    fprintf(stderr,"The file_name must have a '.dot' extension (you gave %s)\n", root_name); exit(2)
;
   }
   else {
    for(i=0; i<len-4; i++) root_name[i]=argv[1][i];
     root_name[len-4]='\0';
   }
   if(verbose) printf("%s would be the root_name for naming files\n", root_name);
/*** End of extracting root file-name ***/


/** Now read the seed file ***/
    r_file = fopen(argv[3], "r");
    if(r_file==NULL)
    {fprintf(stderr, "Cannot open file %s for reading\n", argv[3]); exit(2);}

    for(l=0;l<=2;l++)
    {
      fscanf(r_file, "%d\n", &temp);
      Xi[l] = (unsigned short) temp;
      if(verbose) printf("Xi[%d]=%d\n", l, Xi[l]);
    }
    fclose(r_file);
/** end of reading seed file **/


/***** Memory allocation chore  ***/
  if (  (source_node=(node *)(  calloc( (MAX_NODE+1),(sizeof(node)) )  )  ) == NULL)
        {fprintf(stderr,"Error in mallocation of *source_node\n");exit(2);};
   for(i=0; i<MAX_NODE+1; i++) {
       if (  (source_node[i].adj_list=(unsigned short *)(  calloc( (MAX_OUT_DEGREE),(sizeof(unsigned
 short)) )  )  ) == NULL)
             {fprintf(stderr,"Error in mallocation of *source_node[%d].adj_list\n",i);exit(2);};
   }
  if (  (sink_node=(node *)(  calloc( (MAX_NODE+1),(sizeof(node )) )  )  ) == NULL)
        {fprintf(stderr,"Error in mallocation of *sink_ndoe\n");exit(2);};

  if (  (param=(int *)(  calloc( (2),(sizeof(int )) )  )  ) == NULL)
        {fprintf(stderr,"Error in mallocation of *param\n");exit(2);};

/*** end of memory allocation chore **/

/*** Now you can start reading the dot file and buildng up the array ****/

  if( (r_file=fopen(argv[1],"r")) == NULL) {
    fprintf(stderr,"Can't open file %s for reading\n", argv[1]);
    exit(1);
  }

  nmb_source_node=0;
  nmb_sink_node=0;
  fgets(buff,MAX_LINE_LENGTH,r_file);
  while(  !(feof(r_file)) ) {
    if (strstr(buff,"->")!=NULL) {
      l=0; k=0; start_comment=0; end_comment=0; start_qual=0; end_qual=0;
      for(i=0; i<strlen(buff); i++) {
        /** First check if there is a comment on this line **/
         if(i>0 && (buff[i-1]=='/' && buff[i]=='*') ){
             start_comment=1;
         }
         if(i>0 && (buff[i-1]=='*' && buff[i]=='/') && start_comment==1)
            {end_comment=1; start_comment=0; }
        /** also check if there is any other qualifying thing, such as color etc. for an edge **/
         if( buff[i]=='[') {start_qual=1; printf("Qual started\n");}
         if( buff[i]==']' && start_qual==1) {end_qual=1; start_qual=0; printf("Qual ended\n");}

         if(  !(
                       buff[i]==' '   /** skip all blank spaces **/
                    || buff[i]=='\t'  /** skip all occurences of tab **/
                    || buff[i]=='-'   /** skip all occurences of '-' **/
                    || buff[i]=='>'   /** skip all occurences of '>' **/ 
                    || buff[i]==';'   /** skip all occurences of ';' **/
                    || buff[i]=='/'   /** skip all occurences of '/' **/
                    || buff[i]==']'   /** skip all occurences of ']' **/
               ) 
            && (start_comment!=1)  /** skip everything under comment **/
            && (start_qual!=1)     /** skip everything under qualifying stuff **/
           ) {
           name[l][k]=buff[i];
           k++;
         }
         else if (buff[i]=='-' ||  buff[i]==';') {
           name[l][k]='\0'; k=0; l++;
           /** each time you see a ';', an edge ends. Store the SOURCE of this
               edge in 0 or an even numbered position of the array name[], and
               store the SINK at an odd-numbered location. Also, reset k
               so that next new edge can be started **/ 
         }
      } /** end of "for(i=0; i<strlen(buff); i++)" **/

    /** Once you have all the vertices of the edge(s) in the array name[], 
        these vertices have to be put in the appropriate place in the 
        source_node or sink_node array and the appropriate elements
        updated in the structures. Remember that the odd-numbered
        elements in name[] are sinks and even-numbered ones are sources.
    **/

 
    for(i=0; i< (l+1)/2; i++) {

          /** Search if the SOURCE/SINK node is already there or not, 
             if not put it in the array */

            search_and_insert(source_node, name[i*2], nmb_source_node, param);
             nmb_source_node=param[0];
             source_idx = param[1];
            search_and_insert(sink_node, name[i*2+1], nmb_sink_node, param);
             nmb_sink_node=param[0];
             sink_idx = param[1];

          /** Now update the adjacency list of each source node **/

           source_node[source_idx].adj_list[source_node[source_idx].out_degree]=sink_idx;
           source_node[source_idx].out_degree++;
     }


    } /** end of "if (strstr(buff,"->")!=NULL)" **/

    fgets(buff,MAX_LINE_LENGTH,r_file);
  } /** end of "while(  !(feof(r_file)) )" **/


 if(verbose) {
   printf("nmb_source_node=%d, nmb_sink_node=%d\n", nmb_source_node, nmb_sink_node);
   printf("\nThe source nodes are in the following order:\n");
    for(i=0; i<nmb_source_node; i++) printf(" %s", source_node[i].name);
      printf("\n\n");
   printf("The sink nodes are in the following order:\n");
    for(i=0; i<nmb_sink_node; i++) printf(" %s", sink_node[i].name);
      printf("\n\n");
   printf("The edges are: \n");
    for(i=0; i<nmb_source_node; i++) {
      for(j=0; j<source_node[i].out_degree; j++) {
       printf("%s -> %s; \n", source_node[i].name, sink_node[source_node[i].adj_list[j]].name);
      }
    }
 }


  if (  (new_src_ary=(int *)(  calloc( (nmb_source_node),(sizeof(int)) )  )  ) == NULL)
        {fprintf(stderr,"Error in mallocation of *new_src_ary\n");exit(2);};
  if (  (new_sink_ary=(int *)(  calloc( (nmb_sink_node),(sizeof(int)) )  )  ) == NULL)
        {fprintf(stderr,"Error in mallocation of *new_sink_ary\n");exit(2);};
  if (  (map_sink_ary=(int *)(  calloc( (nmb_sink_node),(sizeof(int)) )  )  ) == NULL)
        {fprintf(stderr,"Error in mallocation of *map_sink_ary\n");exit(2);};

   for(i=0; i<nmb_source_node; i++) {
       if ((source_node[i].adj_list_bak=(unsigned short *)
                (calloc((source_node[i].out_degree),(sizeof(unsigned short)))))==NULL)
         {fprintf(stderr,"mallocation error for *source_node[%d].adj_list_bak\n",i);exit(2);};
       for(j=0; j<source_node[i].out_degree; j++) 
           source_node[i].adj_list_bak[j] = source_node[i].adj_list[j]; 
   }


for(scr_idx=1; scr_idx<=nmb_scr; scr_idx++) {

  /*** First get the actual adj_list from the backup  **/
   for(i=0; i<nmb_source_node; i++) {
       for(j=0; j<source_node[i].out_degree; j++) 
           source_node[i].adj_list[j] = source_node[i].adj_list_bak[j]; 
   }

  sprintf(buff,"%s_scr_%04d.dot", root_name, scr_idx);
  if(  (dot_file=fopen(buff,"w")) == NULL) {
   fprintf(stderr,"Can't open file %s for writing\n", buff); exit(2);
  }
  sprintf(buff,"%s_scr_%04d.dot.ORD", root_name, scr_idx);
  if(  (ord_file=fopen(buff,"w")) == NULL) {
  fprintf(stderr,"Can't open file %s for writing\n", buff); exit(2);
  }

  fprintf(dot_file, "digraph %s_scr_%04d /* seed: %u %u %u */ {\n",  root_name, scr_idx,
            Xi[0], Xi[1], Xi[2]);
  fprintf(ord_file, "# Natural ordering for graph %s_scr_%04d\n",  root_name, scr_idx);
  fprintf(ord_file, "# seed: %u %u %u\n", Xi[0], Xi[1], Xi[2]);

    /** Scramble the node orders. We just store the info about the new order **/

      for(i=0; i<nmb_source_node; i++) new_src_ary[i]=i;
        random_permutation(new_src_ary, nmb_source_node, Xi);
      for(i=0; i<nmb_sink_node; i++) new_sink_ary[i]=i;
        random_permutation(new_sink_ary, nmb_sink_node, Xi);
      for(i=0; i<nmb_sink_node; i++)  {
          map_sink_ary[new_sink_ary[i]] = i;
      }

      if(verbose) {
        printf("\nNew order of SOURCE nodes is:\n");
        for(i=0; i<nmb_source_node; i++)  printf(" %d(%s)", new_src_ary[i],
                                   source_node[new_src_ary[i]].name);
         printf("\n\n");
        printf("New order of SINK nodes is:\n");
        for(i=0; i<nmb_sink_node; i++)  printf(" %d(%s)", new_sink_ary[i],
                                   sink_node[new_sink_ary[i]].name);
         printf("\n\n");
    for(i=0; i<nmb_sink_node; i++)printf("i=%d, new_sink_ary=%d, map_sink_ary=%d\n",
        i, new_sink_ary[i], map_sink_ary[i]);
      }


     /** Now update the index no.. of the adj_list of each source node **/
      for(i=0; i<nmb_source_node; i++) {
        for(j=0; j<source_node[i].out_degree; j++)  {
          source_node[i].adj_list[j] = map_sink_ary[ source_node[i].adj_list[j] ];
        }
         /** sort the adj_list in ascending order since new order is in place **/
        sort_small(source_node[i].adj_list, source_node[i].out_degree);
      }


  /** Print out the orders in the order file **/
  fprintf(ord_file, "# Ordering for layer 0\n0 { \n");
    for(i=0; i<nmb_source_node; i++) {
        k=new_src_ary[i];
     fprintf(ord_file, " %s",  source_node[k].name);
    }
  fprintf(ord_file, "\n} \n# End of ordering for layer 0\n");

  fprintf(ord_file, "# Ordering for layer 1\n1 { \n");
 for(i=0; i<nmb_sink_node; i++) {
        k=new_sink_ary[i];
     fprintf(ord_file, " %s",  sink_node[k].name);
    }
  fprintf(ord_file, "\n} \n# End of ordering for layer 1\n");

  /** Print out the edge list ***/
     for(i=0; i<nmb_source_node; i++) {
        k=new_src_ary[i];
        for(j=0; j<source_node[k].out_degree; j++)  {
          l = source_node[k].adj_list[j];
          m = new_sink_ary[l];
          fprintf(dot_file, "%s -> %s;\n", source_node[k].name, sink_node[m].name);
        }
     }



 fprintf(dot_file, "}\n");
 fclose(dot_file);
 fclose(ord_file);
}


 if(write_back_to_seed_file!=0) { /** default is write_back to seed_file **/
    w_file = fopen(argv[3], "w");
    if(w_file==NULL)
    {fprintf(stderr, "Cannot open file %s for writing\n", argv[3]); exit(2);}

    for(l=0; l<3; l++) fprintf(w_file,"%u\n", Xi[l]);

    fclose(w_file);
 }

exit(0);
}

int random_int(int n, unsigned short Xi[])
{
int r;
  r = (int) (n*erand48(Xi));
return(r);
}

int random_permutation( int *array, int length, unsigned short Xi[])
{
int i,j, r, original_i;

  for ( i = 1; i < length; ++i ) {
    r = random_int(i + 1, Xi);
    /* put the original i-th element in position r, shifting elements
          r,...,i-1 one position to the right */
         original_i = array[ i ];
         for ( j = i - 1; j >= r; --j ) array[ j + 1 ] = array[ j ];
         array[ r ] = original_i;
  }
}


/**
    This function sorts the adjacent list of each source node based on the
    position of the adjacent (sink) node
***/
int sort_small(unsigned short *array, int length)
{
int i,j, to_be_inserted;
  
  for ( i = 1; i < length; ++i ) {
            j = i - 1;
            to_be_inserted = array[ i ];
            while ( j >= 0 && to_be_inserted < array[ j ] ) {
              array[ j + 1 ] = array[ j ];
              --j;
            }
            array[ j + 1 ] = to_be_inserted;
          }

}


     /** Search if the node is alreday there or not, if not put it in the array */
int search_and_insert(node *source_node, char this_node[], int nmb_node, int *param)
{
int already_there;
int i;
int node_idx;

      already_there=0;
      for(i=0; i<nmb_node; i++) {
         if( strcmp(source_node[i].name, this_node)==0) {
            already_there=1;
            node_idx=i;
            break;
         }
      }
      if( already_there==0 ) {
        node_idx=nmb_node;
        strcpy( source_node[node_idx].name, this_node);
        nmb_node++;
        if(nmb_node>MAX_NODE) {
          fprintf(stderr, "At present I can't handle more than %d nodes per side (you gave %d nodes)\n",
             MAX_NODE, nmb_node);
          fprintf(stderr, "Change the MAX_NODE parameter at the top of the scramble_dot.c file and recompile\n");
          exit(4);
        }
      }



param[0]=nmb_node;
param[1]=node_idx;

return(0);
}

/*  [Last modified: 2007 08 08 at 20:13:56 GMT] */
