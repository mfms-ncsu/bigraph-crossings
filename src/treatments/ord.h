/// @file ord.h
/// @brief header for utility functions that read and write .ord files (node
///          ordering on layers of a graph)
/// @author Matt Stallmann
/// @date 29 Dec 1998
/// $Id: ord.h 62 2007-08-03 19:16:20Z mfms $

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

// 20 May 2002 - added get_graph_name()

// To read a .ord file via the stream 'in', do the following:
//     int layer;
//     while ( next_layer( in, layer ) ) {
//       // initialize the layer
//       char * node = 0;
//       while ( next_node( in, node ) ) {
//         // do something with this node
//       }
//     }

#ifndef ORD_H
#define ORD_H

using namespace std;

#include<iostream>

bool get_graph_name( istream & in, char * buffer );
// PRE: 'in' is a valid istream for a .ord file
//      'buffer' is large enough to hold a graph name
// POST: 'in' is beyond the initial comments and white space
//       'buffer' contains the name of the graph, if any (last word of the
//       first comment line in the stream)
//       retval == true iff there was a nonblank comment before there was
//                 any meaningful input 

bool next_layer( istream & in, int & layer );
// PRE: 'in' is a valid istream for a .ord file
// POST: 'in' is at the first node of the next layer (if any)
//       'layer' == the number of the next layer (if any)
//       retval == true iff there is another layer

bool next_node( istream & in, char * & node );
// PRE: 'in' is a valid istream for a .ord file and is positioned somewhere
//      inside the list of nodes for a layer
// POST: 'in' is beyond the next node (if any)
//       'node' is an allocated string, the name of the next node (if any)
//       retval == true iff there is another node on the current layer

// output functions

void ord_preamble( ostream & out, const char * graph_name, const char *
                  treatment_name, bool natural = false );
// PRE: 'out' is a valid ostream
// POST: comments identifying the graph and treatment have been written on
//       'out' (if natural is true, the treatment is a random number seed)

void begin_layer( ostream & out, int layer );
// PRE: 'out' is a valid ostream
// POST: the encoding for the start of 'layer' has been written on 'out' 

void end_layer( ostream & out );
// PRE: 'out' is a valid ostream
// POST: the encoding for the end of the current layer has been written on
//       'out' 

void output_node( ostream & out, const char * node );
// PRE: 'out' is a valid ostream
// POST: the 'node' is listed next for the current layer on 'out' 

#endif

/*  [Last modified: 2007 08 03 at 19:06:50 GMT] */
