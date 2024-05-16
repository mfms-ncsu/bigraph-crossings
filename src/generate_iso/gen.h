/**
   @file gen.h
   @brief Structures needed for gen_ref.c
   @author Debabrata Ghosh and Matthias Stallmann
   @date 1999
   $Id: gen.h 41 2007-07-26 17:13:28Z mfms $
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

#define WIRENAME_LENGTH 16
#define FILENAME_LENGTH 50
#define MAX_LINE_LENGTH 1000
#define MAX_IN_DEGREE 5


  /** structure for the sink nodes, called gates or cells in the context of VLSI **/

typedef struct
  {
      /** these graphs have max. in-degree = 5 **/
    char inp[MAX_IN_DEGREE+1][WIRENAME_LENGTH]; /** names of the adjacent node **/

    char op[WIRENAME_LENGTH];    /** name of the node **/

  } gate;


/*  [Last modified: 2007 07 26 at 17:07:04 GMT] */
