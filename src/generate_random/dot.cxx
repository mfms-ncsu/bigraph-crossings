/// @file dot.cxx
/// @brief
/// Module for reading files in .dot format
/// @author Matt Stallmann
/// @date 1998/07/17
/// $Id: dot.cxx 68 2007-08-08 20:07:39Z mfms $

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

#include<stdlib.h>              // abort()
#include<iostream>
#include<string.h>
#include<assert.h>
#include<ctype.h>

#include"dot.h"
#include"defs.h"

void init_dot( istream & in )
{
  char ch = '\0';
  while ( ( in.get( ch ) ) && ch != '{' ); // ignore everything up to and
                                           // including first {
  if ( ch != '{' ) {
    cerr << "Input error in .dot file: no { found." << endl;
    abort();
  }
}

static char first_node[ NAME_LENGTH + 1 ];
static char second_node[ NAME_LENGTH + 1 ];

static bool parse_statement( const char * buffer, const char * & u,
                                                  const char * & v )
// POST: retval == true iff this statement declares an edge (u,v)
{
#ifdef DEBUG
  cerr << " parsing statement ... " << buffer << endl;
#endif
  const char * arrow_pos = strstr(buffer, "->");
  if ( ! arrow_pos ) return false;
  for ( int k = 0; k < arrow_pos - buffer; k++ )
   first_node[k] = buffer[k];
  first_node[arrow_pos - buffer] = '\0';
  strcpy( second_node, arrow_pos + 2 );
#ifdef DEBUG
  cerr << " node names are: " << first_node << ", " << second_node <<
   endl;
#endif
  u = first_node;
  v = second_node;
  return true;
}

static void recursive_expression(istream & in)
  // PRE: 'in' is just beyond a '{'
  // POST: 'in' is just beyond the matching '}'
{
  char ch;
  for ( in.get(ch); ch != '}'; in.get(ch) ) {
    if ( '{' == ch ) recursive_expression(in);
  }
}

bool next_edge( istream & in, const char * & u, const char * & v )
{
  char ch;
  char buffer[ 2 * NAME_LENGTH + 3 ]; // leave room for two node names and
                                      // arrow 
  int index = 0;
  while ( in.get(ch) ) {
    switch ( ch ) {
    case ';':
      buffer[ index ] = '\0';
#ifdef DEBUG
      cerr << endl;
#endif
      if ( parse_statement( buffer, u, v ) ) return true;
      index = 0;
      break;
    case '{':
      recursive_expression( in );
      break;
    case '}':
      return false;
    default:
      if ( ! isspace( ch ) ) {
        buffer[index++] = ch;
#ifdef DEBUG
        cerr << ch;
#endif
      }
    }
  }
  cerr << "Warning: .dot file ended without a closing }" << endl;
  return false;
}

void dot_preamble( ostream & out, const char * graph_name, const char *
                  info )
{
  out << "digraph " << graph_name << " /* " << info
   << " */ {" << endl;
}

void end_dot( ostream & out )
{
  out << "}" << endl;
}

void output_edge( ostream & out, const char * u, const char * v )
{
  out << u << " -> " << v << ";" << endl;
}

//  [Last modified: 2007 08 08 at 19:08:41 GMT]
