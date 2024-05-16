/// @file sbg_cn_eval.cxx
/// @brief reports number of crossings for a graph, given an ord file
/// @author Matt Stallmann
/// @date 2 Jan 1999
/// $Id: sbg_cn_eval.cxx 67 2007-08-08 19:28:31Z mfms $

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

#include<stdlib.h>              // exit()
#include<iostream>
#include<iomanip>
#include<fstream>
#include<string.h>
#include<assert.h>

#include"BiGraph.hxx"
#include"dot.h"

int main(int argc, char ** argv)
{
  if ( argc != 3 ) {
    cerr << "Usage: " << argv[0] << " graph.dot layer_order.ord" << endl;
    exit(1);
  }

  // get input file name and open it
  ifstream input_file( argv[1] );
  if ( ! input_file ) {
    cerr << "Unable to open file " << argv[1] << " for input" << endl;
    exit(2);
  }

  // get .ord file name and open it
  ifstream ord_file( argv[2] );
  if ( ! ord_file ) {
    cerr << "Unable to open file " << argv[2] << " for order" << endl;
    exit(3);
  }

  // read graph in dot format from file
  BiGraph bg;
  bg.read_with_order( input_file, ord_file );

  if ( 0 == bg.n0() || 0 == bg.n1() ) {
    cerr << "Input error results in empty graph, file "
         << argv[1] << endl;
    exit(4);
  }

  // name of .ord file usually encodes both graph and treatment
  int name_length = strlen( argv[2] ) - 4; // without .ord suffix
  char * name = new char[ name_length + 1 ];
  for ( int i = 0; i < name_length; ++i )
   name[ i ] = argv[2][ i ];
  name[ name_length ] = '\0';
  cout << name << "\t" << bg.crossings() << endl;
  delete [] name;
}

//  [Last modified: 2007 08 08 at 19:27:06 GMT]
