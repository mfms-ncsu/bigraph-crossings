/// @file defs.h
/// @brief
/// Definitions common to all edge crossing heuristic source files
/// @author Matt Stallmann
/// @date Summer 1998  
/// $Id: defs.h 66 2007-08-08 19:09:27Z mfms $

// 14 Feb 2000 - got rid of MAX_VERTICES and MAX_EDGES, replacing static
//               arrays in plain_bfs.cxx and bfs_heuristic.cxx by dynamic
//               ones. 

//     Copyright (C) 2001 Matthias Stallmann.
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

using namespace std;
#include<string>

const double MEDIAN_CORRECTION = 0.5;
                                // fudge factor to guarantee that medians
                                // of even sets are sorted correctly wrt
                                // those of odd sets (anything less than 1
                                // will do)

const string TREATMENT_PREFIX = "_tr";
                                // what comes right before the treatment
                                // number in the output file name
const int TREATMENT_FIELD_WIDTH = 4;
                                // length of field in which to embed
                                // treatment number when it's appended to
                                // the graph name
const string ORD_SUFFIX = ".ord";
                                // suffix for ordering files

const int NAME_LENGTH = 511;    // maximum length of a node name
const int LINE_LENGTH = 75;     // maximum length of a line in a .ord file

extern bool TRUE_MEDIAN;        // true if you take average of two middle
                                // elements when taking median of an even
                                // number (only true for TR24)  

/*  [Last modified: 2007 08 08 at 19:05:08 GMT] */

// Local Variables: ***
//  mode:c++ ***
// End: ***

