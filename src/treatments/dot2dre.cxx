/// @file dot2dre.cxx
/// @brief
/// Reads a .dot file and outputs the same file in
/// .dre format (for use with the dreadnaught isomorphism testing program)
/// @author Matt Stallmann
/// @date 2002/07/29 
/// $Id: dot2dre.cxx 67 2007-08-08 19:28:31Z mfms $

//     Copyright (C) 2007 Matthias Stallmann.
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

#include<stdlib.h>              // exit()
#include<iostream>
#include<iomanip>
#include<fstream>
#include<string.h>              // strlen(), strcpy(), rindex()
#include<strings.h>             // rindex() on Solaris
#include<assert.h>

#include"BiGraph.hxx"
#include"dot.h"

int main(int argc, char ** argv)
{
  if ( argc != 2 ) {
    cerr << "Usage: " << argv[0] << " graph.dot" << endl;
    exit(1);
  }

  // get input file name and open it
  char * input_file_name = argv[ 1 ];
  ifstream input_file( input_file_name );
  if ( ! input_file ) {
    cerr << "Unable to open file " << input_file_name << " for input" << endl;
    exit(2);
  }

  // convert the .dot extension to .dre to get output file name
  char * output_file_name = new char[ strlen( input_file_name ) + 1 ];
  strcpy( output_file_name, input_file_name );
  char * index_of_last_dot = rindex( output_file_name, '.' );
  *index_of_last_dot = '\0';    // effectively 'erase' the last dot and
                                // everything after it
  strcat( output_file_name, ".dre" );

  ofstream output_file( output_file_name );
  if ( ! output_file ) {
    cerr << "Unable to open file " << output_file_name << " for output"
         << endl;
    exit(3);
  }

  // read graph in dot format from file
  BiGraph bg;
  bg.read( input_file );

  if ( 0 == bg.n0() || 0 == bg.n1() ) {
    cerr << "Input error results in empty graph, file "
         << input_file_name << endl;
    exit(4);
  }

  bg.output_dre( output_file );
}

//  [Last modified: 2007 08 08 at 19:22:03 GMT]
