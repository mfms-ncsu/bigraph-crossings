/// @file gstats.cxx
/// @brief
/// Reports various statitics about a bigraph in dot format
/// @author Matt Stallmann
/// @date 1999/06/29
/// $Id: gstats.cxx 67 2007-08-08 19:28:31Z mfms $

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

//  3 Jan 2000 - added a tree-based lower bound calculation

// INPUT: one command line argument giving the name of a dot file, plus the
//        following optional flags:
//           -ord=<ord file> if you want to check that the ord file matches
//                           the dot file
//           -L              if you want labelled output
//           -v              if you want degree of each node reported

// OUTPUT: A single line of text and numerical data containing the
//         following tab-separated fields:
//            file name without .dot extension
//            n0 = number of nodes on layer 0
//            n1 = number of nodes on layer 1
//            m = number of edges
//            minimum degree for a layer 0 node
//            average degree for a layer 0 node
//            maximum degree for a layer 0 node
//            standard deviation in degree for layer 0 nodes
//            minimum degree for a layer 1 node
//            average degree for a layer 1 node
//            maximum degree for a layer 1 node
//            standard deviation in degree for layer 1 nodes
//            lower bound based on how many edges in excess of a comb
//          With -L flag all these are labelled and on separate lines

// 23 May 2001 - got rid of DBL_MAX (portability to Linux on IBM 390)

using namespace std;

#include<iostream>
#include<iomanip>
#include<fstream>
#include<string>
#include<limits.h>              // INT_MAX
#include<assert.h>
#include<math.h>                // sqrt
#include<stdlib.h>              // exit()

#include"BiGraph.hxx"
#include"CmdLine.h"
#include"tree_lower_bound.h"

int main(int argc, char ** argv)
{
  CmdLine args( argc, argv );

  // get input file name and open it
  const string input_file_name = args.getFile( 1 );
  ifstream input_file;
  if ( input_file_name.length() != 0 )
    input_file.open(input_file_name.c_str());
  else {
    cerr << "Usage: " << args.getFile( 0 ) << " dot_file" << endl;
    cerr << "       optional flags:" << endl;
    cerr << "         -ord=ord_file to check consistency" << endl;
    cerr << "         -v            to report degree of each node" << endl;
    cerr << "         -L            to get labelled output" << endl;
    exit( 1 );
  }

  if ( ! input_file ) {
    cerr << "Unable to open file " << args.getFile( 1 )
     << " for input" << endl;
    exit( 2 );
  }

  // get values of optional flags
  const string ord_file_name = args.flagPresent( "ord" ) ?
   args.stringFlag( "ord" ) : 0;
  bool verbose = args.flagPresent( "v" ) ? true : false;
  bool labelled = args.flagPresent( "L" ) ? true : false;

  // read graph in dot format from file
  BiGraph bg;
  if ( ord_file_name.length() != 0 ) {
    ifstream ord_file(ord_file_name.c_str());
    if ( ! ord_file ) {
      cerr << "Unable to open file " << ord_file_name
       << " for input" << endl;
      exit( 3 );
    }
    bg.read_with_order( input_file, ord_file );
    ord_file.close();
  }
  else {
    bg.read( input_file );
  }
  input_file.close();

  if ( 0 == bg.n0() || 0 == bg.n1() ) {
    cerr << "Input error results in empty graph, file "
         << input_file_name << endl;
    exit( 4 );
  }

  // Compute statistics for both sides
  int min_deg[ 2 ] = { INT_MAX, INT_MAX };
  int max_deg[ 2 ] = { 0, 0 };
  double avg_deg[ 2 ] = { -1.0, -1.0 };
  double standard_deviation[ 2 ] = { -1.0, -1.0 };
  for ( int side = 0; side < 2; ++side ) {
    double total_deg = 0.0;
    double sum_of_squares = 0.0;
    int number_of_nodes = bg.n0();
    for ( int i = 0; i < number_of_nodes; ++i ) {
      int degree = bg.degree( i );
      if ( verbose )
       cout << "side = " << side << ", node = " << bg.get_name( i )
        << ", degree = " << degree << endl;
      total_deg += degree;
      sum_of_squares += degree * degree;
      if ( degree < min_deg[ side ] )
       min_deg[ side ] = degree;
      if ( degree > max_deg[ side ] )
       max_deg[ side ] = degree;
    }
    avg_deg[ side ] = total_deg / number_of_nodes;
    standard_deviation[ side ] = sqrt( sum_of_squares / number_of_nodes
                                   - avg_deg[ side ] * avg_deg[ side ] );
    bg.switch_sides();
  }

  // compute lower bound
  int lower_bound = tree_lower_bound( bg );

  // get file name without .dot suffix
  int extension_position = input_file_name.rfind(".");
  string name = input_file_name.substr(0, extension_position);

  // output summary information
  if ( labelled ) {
    cout << "BiGraph, name = " << name
         << endl << " nodes: layer 0 = " << bg.n0()
         << ", layer 1 = " << bg.n1()
         << "; " << bg.number_of_edges() << " edges."
         << endl << " layer 0 statistics:" 
         << endl << "   minimum degree = " << min_deg[ 0 ]
         << endl << "   maximum degree = " << max_deg[ 0 ]
         << endl << "   average degree = " << avg_deg[ 0 ]
         << endl << "   standard deviation = " << standard_deviation[ 0 ]
         << endl << " layer 1 statistics:" 
         << endl << "   minimum degree = " << min_deg[ 1 ]
         << endl << "   maximum degree = " << max_deg[ 1 ]
         << endl << "   average degree = " << avg_deg[ 1 ]
         << endl << "   standard deviation = " << standard_deviation[ 1 ]
         << endl << " Lower bound = " << lower_bound
         << endl;
  }
  else {
    cout << name
         << "\t" << bg.n0()
         << "\t" << bg.n1()
         << "\t" << bg.number_of_edges()
         << "\t" << min_deg[ 0 ]
         << "\t" << avg_deg[ 0 ]
         << "\t" << max_deg[ 0 ]
         << "\t" << standard_deviation[ 0 ]
         << "\t" << min_deg[ 1 ]
         << "\t" << avg_deg[ 1 ]
         << "\t" << max_deg[ 1 ]
         << "\t" << standard_deviation[ 1 ]
         << "\t" << lower_bound
         << endl;
  }
}

//  [Last modified: 2007 08 08 at 19:25:52 GMT]
