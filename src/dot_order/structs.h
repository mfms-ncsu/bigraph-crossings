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
#include<string.h>
#include<math.h>

#define MAX_NODES_AT_ONE_LEVEL 5000
  /** This is the maximum no. of nodes allowed at each layer of the **/
  /** graph. We are just setting this limit to 5000 for the time    **/
  /** being, and avoiding doing any re-allocation of memory. The    **/
  /** structures that are going to be calloc-ed with this parameter **/
  /** are not large (5000 of these structures won't eat up a lot of **/
  /** memory) and the program will exit gracefully whenever this    **/
  /** limit is exceeded.                                            **/

#define FILENAME_LENGTH 200     /** Max. size of a file-name        **/
#define WIRENAME_LENGTH 60      /** Max. size of the name of a node **/


/** This strcuture is for storing the relevant information as read  **/
/** from the DOT output written out in DOT format. Note that, we    **/
/** are not storing the x-coordinates here, since we'll put the     **/
/** graph in landscape format, so the x-coordinate will determine   **/
/** the rank, and we'll put the info. in appropriate array.         **/
typedef struct  {
  int y_val;     /** value of the y-co-ordinate **/
  char name[WIRENAME_LENGTH]; /** name of the node  **/
} dot_node_struct;


/*  [Last modified: 2015 05 17 at 19:52:12 GMT] */
