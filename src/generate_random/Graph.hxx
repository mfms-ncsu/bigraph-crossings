/// @file Graph.hxx
/// @brief
/// Header for Graph ADT (used for generating random connected
///             graphs but may have other applications later)
/// @author Matt Stallmann
/// @date 25 Jun 1999
/// $Id: Graph.hxx 43 2007-07-26 17:34:42Z mfms $

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

#ifndef GRAPH_HXX
#define GRAPH_HXX

#include"IntList.hxx"
#include"IntIterator.hxx"

class Graph {
friend class EdgeList;
public:
  Graph( int n );               // construct an n-node graph with no edges
  ~Graph();
  int number_of_nodes() const;
  int number_of_edges() const;
  void add_edge( int i, int j );
  bool is_edge( int i, int j ) const;
private:
  int my_n;                     // number of nodes
  int my_m;                     // number of edges
  IntList * my_adj;             // array of adjacency lists
};

class EdgeList {
public:
  EdgeList( const Graph & G, int i ); // for node i
  bool more() const;                  // are there more edges?

  // the following assume that more() is true!
  void next();                        // move to the next one
  int head() const;                   // return head of current edge
  int tail() const;                   // return tail of current edge
  // when listing edges for node i, tail() is always i and head() is the
  // other node

  // to call the iterator in a loop, do
  //    EdgeList EL( G, i );
  //    while ( EL.more() ) {
  //      EL.next();
  //      // do something with EL.head() and EL.tail()
  //    }

private:
  const Graph & my_graph;
  int my_node;                  // current node
  int my_other_node;            // the other end of the current edge
  IntIterator my_adj;           // current position in adjacency list
};
#endif

//  [Last modified: 2007 07 26 at 17:33:15 GMT]
