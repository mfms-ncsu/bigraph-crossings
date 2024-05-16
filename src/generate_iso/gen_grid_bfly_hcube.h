/**
   @file gen_grid_bfly_hcube.h
   @brief Structures needed for gen_grid_bfly_hcube.c
   @author Debabrata Ghosh and Matthias Stallmann
   @date 1999
   $Id: gen_grid_bfly_hcube.h 41 2007-07-26 17:13:28Z mfms $
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

#define WIRENAME_LENGTH 20
#define FILENAME_LENGTH 512
#define MAX_LINE_LENGTH 1000
#define MAX_IN_DEGREE (1 << 8)
#define MAX_OUT_DEGREE (1 << 8)



/** gate is the structure for the sink nodes, so called because of use in VLSI **/
typedef struct
{
  char inp[MAX_IN_DEGREE][WIRENAME_LENGTH]; /** name of each incident net **/
  char op[WIRENAME_LENGTH]; /** name of this vertex in binary. For butterfly, need to 
                                add another index called 'other_idx' **/
  int dec_op;  /** name of this vertex in decimal **/
  int other_idx;  /** used for butterfly only, each node is of the form (i,w) where
                      i is stored in decimal form and w in binary. The parameter
                      'other_idx' correspond to i **/
  int in_degree;
} gate;

/** net is the structure for the source nodes, so called because of use in VLSI **/
typedef struct
{
  char name[WIRENAME_LENGTH]; /** name of this vertex in binary **/
  int dec_name;   /** index of this vertex (in decimal), as given by the connectivity **/
  /** pattern of the graph being generated **/
  int other_idx;  /** see description in the structure 'gate' **/
  int out_degree;
} net;

/*  [Last modified: 2007 07 26 at 17:11:35 GMT] */
