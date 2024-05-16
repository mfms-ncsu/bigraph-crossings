/// @file dot.h
/// @brief
/// Module for reading files in .dot format
/// @author Matt Stallmann
/// @date 1998/07/17
/// $Id: dot.h 66 2007-08-08 19:09:27Z mfms $

// modified 13-AUG-1998 to handle output as well as input
// 29 Dec 1998 - reads only the string representation of vertices
//               (converting vertices to integers is done in the BiGraph class)

/// @todo Should be encapsulated as a class.

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

#ifndef DOT_H
#define DOT_H

using namespace std;

#include<iostream>

void init_dot( istream & in );
// PRE: 'in' is at the beginning of a dot file description
// POST: 'in' is at the beginning of the first statement inside {}'s

bool next_edge( istream & in, const char * & u, const char * & v );
// PRE: 'in' is at the beginning of a dot statement S
// POST: 'in' is just beyond the next statement starting with S that
//       describes an edge; (u,v) is the edge, where u and v are C-strings
//       (not allocated and not guaranteed to be there after the next call
//       to this function)
//       retval == true if and only if there is another statement
//                 describing an edge  

// To extract edges from a dot file in stdin, use the following type of loop:
//
//     const char * u;
//     const char * v;
//     init_dot(cin);
//     while ( next_edge( cin, u, v ) ) {
//       // do something with edge (u,v)
//     }

void dot_preamble( ostream & out, const char * graph_name,
                   const char * seed_info = "" );
// POST: the preamble for a graph with the given name has been emitted to
//       'out' and the seed info added as a comment

void end_dot( ostream & out );
// POST: output indicating the end of a dot graph has been sent to 'out'

void output_edge( ostream & out,
                  const char * u, const char * v );
// POST: a dot statement for the edge (u,v) has been sent to 'out'

#endif

/*  [Last modified: 2007 08 08 at 19:07:24 GMT] */

// Local Variables: ***
//  mode:c++ ***
// End: ***

