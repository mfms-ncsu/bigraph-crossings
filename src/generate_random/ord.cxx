/// @file ord.cxx
/// @brief implementation of utility functions that read and write .ord
///           files (node ordering on layers of a graph)
/// @author Matt Stallmann
/// @date 1 Jan 1999
/// $Id: ord.cxx 68 2007-08-08 20:07:39Z mfms $

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

using namespace std;

#include"defs.h"
#include"ord.h"
#include<stdlib.h>              // abort()
#include<iostream>
#include<string.h>
#include<ctype.h>
#include<assert.h>

// characters with special meaning
const char NULL_CHAR = '\0';
const char BLANK_CHAR = ' ';
const char COMMENT_CHAR = '#';
const char END_OF_LINE = '\n';
const char OPEN_LIST = '{';
const char CLOSE_LIST = '}';

static char name_buffer[NAME_LENGTH];  // used to report graph name
static bool valid_name = false;        // true if an actual name was found

bool eat_space_and_comments( istream & in )
  // POST: 'in' is at the first non-blank character after its initial
  //       position (with comments excluded);
  //       retval == true iff there is another non-blank character before
  //                 the end of file 
{
  static bool first_comment = true; // name appears at the end of first comment
  enum { NOT_IN_COMMENT, IN_COMMENT } local_state = NOT_IN_COMMENT;
  char ch;
  int index = 0;
  while ( in.get( ch ) ) {
#ifdef DEBUG
    cerr << "..local state = " << local_state << ", ch = " << ch << endl;
#endif
    switch ( local_state ) {
    case NOT_IN_COMMENT:
      if ( COMMENT_CHAR == ch ) local_state = IN_COMMENT;
      else if ( ! isspace( ch ) ) {
        in.putback( ch );
        return true;
      }
      break;
    case IN_COMMENT:
      if ( END_OF_LINE == ch ) local_state = NOT_IN_COMMENT;
      // if this is the first comment line, save the last "word" -- it's
      // the name of the graph.
      if( first_comment ) {
        if ( END_OF_LINE == ch ) {
          name_buffer[index] = NULL_CHAR;
          if( index > 0 ) {
            valid_name = true;
#ifdef DEBUG
            cerr << "....valid name [" << name_buffer << "]" << endl;
#endif
          }
          first_comment = false;
        }
        else if ( BLANK_CHAR == ch ) {
          index = 0;
        }
        else {
          name_buffer[index++] = ch;
#ifdef DEBUG
          cerr << "....add to buffer " << ch << ", index = " << index << endl;
#endif
        }
      } // if this is the first comment
      break;
    default: assert( "bad local state" && false );
    }
  }
  return false;
}

bool get_graph_name( istream & in, char * buffer ) {
  eat_space_and_comments( in );
#ifdef DEBUG
  cerr << "<- get_graph_name returns [" << name_buffer << "], valid = " <<
    valid_name << endl;
#endif
  if( valid_name ) {
    strcpy( buffer, name_buffer );
    return true;
  }
  return false;
}

static enum { OUTSIDE_LAYER,
              LAYER_NUMBER,
              INSIDE_LAYER } state = OUTSIDE_LAYER;

static int hold_layer = -1;     // most recent layer number encountered

bool next_layer( istream & in, int & layer )
{
  layer = -1;
  char ch;
  while ( eat_space_and_comments( in ) && in.get( ch ) ) {
#ifdef DEBUG
    cerr << "..state = " << state << ", ch = " << ch << endl;
#endif
    switch ( state ) {
    case OUTSIDE_LAYER:
      in.putback( ch );         // put back first digit of expected int
      in >> layer;
      hold_layer = layer;
      state = LAYER_NUMBER;
      break;
    case LAYER_NUMBER:
      if ( OPEN_LIST == ch ) {
        state = INSIDE_LAYER;
        assert( layer >= 0 );
        return true;
      }
      else {
        cerr << endl << "Read error in .ord file: " << OPEN_LIST
             << " expected, reading " << ch << " instead." << endl;
        abort();
      }
      break;
    case INSIDE_LAYER:
      if ( CLOSE_LIST == ch )
        state = OUTSIDE_LAYER; 
      break;
    default: assert( "bad state" && false );
    }
  }
  return false;
}

bool next_node( istream & in, char * & node )
{
  assert( INSIDE_LAYER == state );
  if ( ! eat_space_and_comments( in ) ) {
    cerr << endl << "Read error in .ord file: unexpected end of file" <<
      endl << "while reading nodes in layer " << hold_layer << endl;
    abort();
  }
  char buffer[ NAME_LENGTH + 1 ];
  int index = 0;
  char ch;
  while ( in.get( ch ) ) {
#ifdef DEBUG
    cerr << "..state = " << state << ", ch = " << ch << endl;
#endif
    if ( CLOSE_LIST == ch || COMMENT_CHAR == ch || isspace( ch ) ) {
      in.putback( ch );
      if ( 0 == index )
        return false;
      buffer[ index ] = '\0';
      node = new char[ strlen( buffer ) + 1 ];
      strcpy( node, buffer );
      return true;
    }
    else {
      assert( index < NAME_LENGTH );
      buffer[ index++ ] = ch;
    }
  }
  assert( false );              // should never get here
  return false;
}

static int current_column = 0;  // keeps track of column while printing
static int number_of_nodes = 0; // number of nodes on current line
static int output_layer = -1;   // current layer during output

void ord_preamble( ostream & out, const char * graph_name,
                                  const char * treatment_name,
                   bool natural )
{
  if ( natural ) {
    out << "#: Natural ordering for graph " << graph_name << endl;
    out << "#: Seed " << treatment_name << endl << endl;
  }
  else {
    out << "#: Ordering for graph " << graph_name << endl;
    out << "#: Treatment " << treatment_name << endl << endl;
  }
}

void begin_layer( ostream & out, int layer )
{
  out << layer << " { # start of ordering for layer " << layer << endl;
  output_layer = layer;
  current_column = 0;
  number_of_nodes = 0;
}

void end_layer( ostream & out )
{
  assert( 0 <= output_layer );
  if ( 0 < number_of_nodes ) out << endl;
  out << "} # end of ordering for layer " << output_layer << endl << endl;
  output_layer = -1;
}

void output_node( ostream & out, const char * node )
{
  assert( 0 <= output_layer );
  if ( 0 < number_of_nodes
       && LINE_LENGTH <= current_column + int(strlen( node )) ) {
    out << endl;
    current_column = 0;
    number_of_nodes = 0;
  }
  if ( 0 < number_of_nodes ) {
    out << " ";
    ++current_column;
  }
  out << node;
  current_column += strlen( node );
  ++number_of_nodes;
}

//  [Last modified: 2007 08 03 at 19:08:02 GMT]
