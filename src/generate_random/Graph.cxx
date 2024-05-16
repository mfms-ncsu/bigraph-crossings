/// @file Graph.cxx
/// @brief
///   implementation of Graph ADT
/// @author Matt Stallmann
/// @date 25 Jun 1999
/// $Id: Graph.cxx 42 2007-07-26 17:28:44Z mfms $

//     Copyright (C) 2001,2007  Matthias Stallmann.
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

#include"Graph.hxx"
#include<iostream>
#include<assert.h>

// ****** Graph class implementation *******

// PRIVATE DATA:
//  int my_n;                     // number of nodes
//  int my_m;                     // number of edges
//  IntList * my_adj;             // array of adjacency lists

Graph::Graph( int n ): my_n( n ), my_m( 0 )
{
#ifdef DEBUG
  cerr << "-> Graph::Graph( " << n << " )" << endl;
#endif
  assert( n > 0 );
  my_adj = new IntList[ n ];
#ifdef DEBUG
  cerr << "<- Graph::Graph" << endl;
#endif
}

Graph::~Graph()
{
#ifdef DEBUG
  cerr << "-> Graph::~Graph()" << endl;
#endif
  delete [] my_adj;
#ifdef DEBUG
  cerr << "<- Graph::~Graph" << endl;
#endif
}

int
Graph::number_of_nodes() const
{
  return my_n;
}

int
Graph::number_of_edges() const
{
  return my_m;
}

void
Graph::add_edge( int i, int j )
{
#ifdef DEBUG
  cerr << "-> Graph::add_edge( " << i << ", " << j << " )" << endl;
#endif
  ++my_m;
  my_adj[ i ].add( j );
  my_adj[ j ].add( i );
#ifdef DEBUG
  cerr << "<- Graph::add_edge" << endl;
#endif
}

bool
Graph::is_edge( int i, int j ) const
{
#ifdef DEBUG
  cerr << "-> Graph::is_edge( " << i << ", " << j << " )" << endl;
#endif
  bool retval = my_adj[ i ].contains( j );
#ifdef DEBUG
  cerr << "<- Graph::is_edge, retval = " << retval << endl;
#endif
  return retval;
}


// ******** EdgeList class implementation ************

// PRIVATE DATA:
//  const Graph & my_graph;
//  int my_node;                  // current node
//  int my_other_node;            // the other end of the current edge
//  IntIterator my_adj;           // current position in adjacency list

EdgeList::EdgeList( const Graph & G, int i ): // for node i
my_graph( G ),
my_node( i ),
my_adj( G.my_adj[ i ] )
{
#ifdef DEBUG
  cerr << "-> EdgeList::EdgeList( G, " << i << " )" << endl;
#endif
#ifdef DEBUG
  cerr << "<- EdgeList::EdgeList" << endl;
#endif
}

bool
EdgeList::more() const                  // are there more edges?
{
  return my_adj.more();
}

void
EdgeList::next()                        // move to the next one
{
#ifdef DEBUG
  cerr << "-> EdgeList::next()" << endl;
#endif
  assert( more() );
  my_other_node = my_adj.next();
#ifdef DEBUG
  cerr << "<- EdgeList::next, head = " << my_other_node << ", tail = " <<
   my_node << endl;
#endif
}

int
EdgeList::head() const                   // return head of current edge
{
  return my_other_node;
}

int
EdgeList::tail() const                   // return tail of current edge
{
  return my_node;
}

//  [Last modified: 2007 07 26 at 17:25:47 GMT]
