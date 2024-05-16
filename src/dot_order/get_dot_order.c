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

#include "structs.h"

int main (argc, argv)
 
 int   argc;
 char *argv[];
{
int nmb_layer;
char buff[300];
char name[300];
char word[100];
char gname[100];
int i,j, k, start, start_gname, end;
FILE *r_file;
FILE *w_file;

 if(argc!=4)
 {
  fprintf(stderr,"Usage: get_dot_order (1)dot_file_name, (2)ord_file_name (3)nmb_of_layers\n");
  exit(1);
 }

  nmb_layer=atoi(argv[3]);


/**************************************************************************/
/*  We need to first give a dot run. To do so, we prepare a separate dot  */
/* file since we want to have the optimized graph in the landscape format */
/* so that we can use the x-coordinate to determine the layer that a node */
/* belongs to, and the y-coordinate to determine the position of the node */
/* in that particular layer.                                              */ 
/**************************************************************************/

  if( (r_file=fopen(argv[1],"r")) == NULL) {
   fprintf(stderr,"Can't open file %s for reading\n", argv[1]); exit(2);
  }
  sprintf(buff, "%s.new", argv[1]);
  if( (w_file=fopen(buff, "w")) == NULL) {
   fprintf(stderr,"Can't open file %s for writing\n", buff); exit(2);
  }

  fgets(buff,300,r_file);
  while (  !feof(r_file)  ) {
    if( strstr(buff, "digraph") != NULL) {



/*
      j=0; start=0; k=0; end=1; start_gname=0;

      for(i=0; i<strlen(buff); i++) {

            if( i>0 && ( (buff[i]==' ' && buff[i-1]!=' ') || buff[i]=='{') )  {
               end=1;
            }
            else end=0;

            if( buff[i]!=' ') {
                 if(start_gname==1) {
                    gname[k] =  buff[i];
                    k++;
                 }
                 else { 
                    word[k] =  buff[i]; 
                    k++;
                 }
            }



            if(end==1) {
              if(start_gname==1) {
                  gname[k]='\0'; k=0; 
                  start_gname=0;
              }
              else {word[k]='\0'; k=0; 
              }
              if( strcmp(word, "digraph")==0 ) {
                  start_gname=1;
                  sprintf(word,"");
              }
            }

           
      }
     fprintf(w_file, "digraph \"%s\" {\n", gname);
*/


     fprintf(w_file, "%s", buff);
     fprintf(w_file, "size=\"10,7.5\";\n");
     fprintf(w_file, "center=1;\n");
     fprintf(w_file, "orientation=landscape;\n");
    }
    else if( strstr(buff, "->") ) {
     fprintf(w_file, "%s", buff);
    }
    fgets(buff,300,r_file);
  }
   fprintf(w_file, "}\n");

  fclose(r_file);
  fclose(w_file);

  sprintf(buff, "dot -Tdot %s.new -o %s.new.op", argv[1], argv[1]);
  system(buff);
/**********  End of submitting the dot run thru' a system call  ************/



get_dot_order(argv[1], argv[2], nmb_layer-1);


/**** Get rid of all the intermediate files produced ***/

  sprintf(buff, "\\rm %s.new %s.new.op", argv[1], argv[1]);
  system(buff);

exit(0);
}
 

int get_dot_order(char dot_file_name[], char ord_file_name[], int max_lev)
{
int i,j,k;
int idx;
int start;
int x_idx;
int count, y_val, x_val; 
int gottcha;
float tmpf1, tmpf2;

dot_node_struct **node_ary; /** 2-D structure for storing the info read from dot file **/

unsigned int *x_ary;      /** layers will be determined by the x_values. These are  **/
unsigned int *x_ary_bak;  /** stored in the x_ary. x_ary_bak is a back-up for x_ary.**/

unsigned int *x_ary_cnt;  /** x_ary_cnt[i] = no. of nodes seen with x_value = x_ary[i] **/
unsigned int *x_ary_map;  /** Finally, the x_values will be sorted to find the actual  **/
                          /** layer no. x_ary_map[i] says layer i belongs to which     **/
                          /** x_ary.                                                   **/

char buff[300];
char name[300];
char node_name[WIRENAME_LENGTH];
FILE *r_file;
FILE *w_file;

/*************** Memory allocation chore  ********************************/

if ( (x_ary=(unsigned int*)calloc(max_lev+1, sizeof(unsigned int))) == NULL )
 { fprintf(stderr, "Error in memory callocation for x_ary\n"); exit(2); }
if ( (x_ary_bak=(unsigned int*)calloc(max_lev+1, sizeof(unsigned int))) == NULL )
 { fprintf(stderr, "Error in memory callocation for x_ary_bak\n"); exit(2); }
if ( (x_ary_map=(unsigned int*)calloc(max_lev+1, sizeof(unsigned int))) == NULL )
 { fprintf(stderr, "Error in memory callocation for x_ary_map\n"); exit(2); }
if ( (x_ary_cnt=(unsigned int*)calloc(max_lev+1, sizeof(unsigned int))) == NULL )
 { fprintf(stderr, "Error in memory callocation for x_ary_cnt\n"); exit(2); }


      if ((node_ary=(dot_node_struct **)(  calloc( (max_lev+1),(sizeof(dot_node_struct *)) ) ))==NULL)
                     {fprintf(stderr,"Error in callocation of **node_ary\n");exit(2);};
            for(i=0; i<=max_lev; i++) {
             if((node_ary[i]=(dot_node_struct *)(calloc(MAX_NODES_AT_ONE_LEVEL,(sizeof(dot_node_struct)))))==NULL)
                       {fprintf(stderr, "Error in mallocation of *node_ary[%d]\n", i);  exit(3);}
            }

/*************** End of memory allocation ********************************/


  sprintf(buff, "%s.new.op", dot_file_name);
  if( (r_file=fopen(buff,"r"))  ==  NULL) {
    fprintf(stderr,"Can't open file %s for reading\n", buff); exit(2);
  }

  x_idx=0;
  fgets(buff,300,r_file);
  while(  !feof(r_file)  ) {
    if(
            strstr(buff,"pos=") != NULL  /** Take only the lines which give position **/
        &&  strstr(buff,"->") == NULL    /** but not the lines which gives position of edges **/
      )  
      
      {
        idx=0; start=0;
            /** ignore all leading white spaces **/
        for(k=0; k<strlen(buff); k++) {
          if( buff[k]!=' ' && buff[k]!='\t' && start==0) start=1;
          if( start==1) {name[idx]=buff[k]; idx++;}
        }
        name[idx]='\0';

       /** now extract the x and y co-ordinates **/
       sscanf(name, "%s [pos=\"%d,%d\", width=\"%f\", height=\"%f\"];\n", 
             node_name, &y_val, &x_val, &tmpf1, &tmpf2);

/*
printf("Scanned: name=%s, x_val=%d, y_val=%d\n", node_name, x_val, y_val);
*/

       /** Check if the x_value has already occured (already stored in some x_ary[i]) **/
       gottcha=0;
       for(j=0; j<=max_lev; j++) {
         if(x_val==x_ary[j]) {gottcha=1; k=j; break;}
       }

       /** If this x_value is not seen before, put it in x_ary, and increment the index
           for x_ary. **/
       if(gottcha==0) { k=x_idx; x_ary[x_idx]=x_val; x_idx++; }

/*
printf("k=%d, x_ary_cnt[k]=%d\n", k, x_ary_cnt[k]);
*/

       /** stroe the node name and its y_co-ordinate in the appropriate strcuture  **/
       strcpy(node_ary[k][x_ary_cnt[k]].name, node_name);
       node_ary[k][x_ary_cnt[k]].y_val=y_val;
       x_ary_cnt[k]++;

      /** Error checking **/
       if(x_ary_cnt[k] > MAX_NODES_AT_ONE_LEVEL-1) {
         fprintf(stderr, "I can handle at the max %d nodes at each layer\n", 
               MAX_NODES_AT_ONE_LEVEL);
         fprintf(stderr, "At the layer corresponding to x_value=%d, I get %d nodes\n",
                x_ary[k], x_ary_cnt[k]);
         fprintf(stderr, "If you think there is no other problem, you may change the parameter \nMAX_NODES_AT_ONE_LEVEL in the file structs.h and recompile.\n"); 
         exit(10);
       }
      /** End of error checking **/

      }
    fgets(buff,300,r_file);
  }

  fclose(r_file);


  /** sort the node_ary strcuture at each level based on the y_value  ***/
  for(i=0; i<=max_lev; i++) { sort_small(node_ary[i], x_ary_cnt[i]); }


  /** back up x_ary into x_ary_bak  **/
 for(i=0; i<=max_lev; i++) { x_ary_bak[i]=x_ary[i];}


  /** sort x_ary in descending order, becauase dot produces higher levels with lower x_value **/
 sort_desc_order(x_ary, max_lev);

 

 /** Now find out from which x_ary each layer came (original x_ary is stored in x_ary_bak) **/
 for(i=0; i<=max_lev; i++) {
   gottcha=0;
   for(j=0; j<=max_lev; j++) {
     if(x_ary_bak[j]==x_ary[i]) { x_ary_map[i]=j; gottcha=1; break;}
   }
   if(gottcha==0){fprintf(stderr,"Can't find a mapping for x_ary[%d]=%d\n",i,x_ary[i]);exit(10);}
 }


/*
 for(i=0; i<=max_lev; i++) printf("Lev %d has a x-val=%d\n", i, x_ary[i]);
*/

  if( (w_file=fopen(ord_file_name, "w"))  == NULL) {
   fprintf(stderr,"Can't open file %s for wrting\n", ord_file_name);
   exit(2);
  }
  
  fprintf(w_file, "# Natural ordering for graph %s\n", dot_file_name);

  for(i=0; i<=max_lev; i++) {
    k=x_ary_map[i]; 
    fprintf(w_file, "# Ordering for layer %d\n", i);
    fprintf(w_file, "%d {\n", i);
    count=0;
    for(j=0; j<x_ary_cnt[k]; j++) {
/*
      printf("lev=%d, sr_nmb=%d, Node=%s (x_val=%d)\n", k, j, node_ary[k][j].name, x_ary[k]);
*/
      fprintf(w_file, " %s", node_ary[k][j].name);
      count += (strlen(node_ary[k][j].name)) ;
      if(count>60) {fprintf(w_file, "\n"); count=0;} /** Put a new-line for readability **/
    }
    fprintf(w_file, "\n}\n");
    fprintf(w_file, "# End of ordering for layer %d\n", i);
  }

  fclose(w_file);

return(0);
}


int sort_small(dot_node_struct *array, int length)
{
int i,j; 
dot_node_struct to_be_inserted;

  for ( i = 1; i < length; ++i ) {
            j = i - 1;
            to_be_inserted = array[ i ];
            while ( j >= 0 && to_be_inserted.y_val < array[ j ].y_val ) {
              array[ j + 1 ] = array[ j ];
              --j;
            }
            array[ j + 1 ] = to_be_inserted;
          }
 
}


int sort_desc_order(unsigned int *array, int length)
{
int i,j; 
unsigned int to_be_inserted;

  for ( i = 1; i < length; ++i ) {
            j = i - 1;
            to_be_inserted = array[ i ];
            while ( j >= 0 && to_be_inserted > array[ j ] ) {
              array[ j + 1 ] = array[ j ];
              --j;
            }
            array[ j + 1 ] = to_be_inserted;
          }
 
}

/*  [Last modified: 2007 08 08 at 20:11:05 GMT] */
