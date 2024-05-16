/// @file BiGraph.hxx
/// @brief Bipartite graph class for crossing number problem (header)
/// @author Matt Stallmann, 12 Jun 1998
/// $Id: BiGraph.hxx 65 2007-08-08 18:45:24Z mfms $

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

//  2 Jan 1999 - changes to accomodate reading ordering from a separate file

// 27 Jan 1999 - changed best_insertion so that it reports the change in
// number of crossings; also got rid of "to_the_right" and
// "allow_equality".

// 14 Apr 1999 - added get_node_number() and get_name()

// 29 Jun 1999 - got rid of permute()

// 29 Jul 2002 - added output_dre to allow us to use the dreadnaught
// program for testing isomorphism

#ifndef BIGRAPH_HXX
#define BIGRAPH_HXX

using namespace std;

#include"IntSet.h"
#include<iostream>

struct BGRecord;

// A bipartite graph consists of two sets of vertices V0 = {0, ..., n0-1} and
// V1 = {0, ..., n1-1}; each vertex in V0 is connected to a subset of the
// vertices in V1 (and vice versa)

class BiGraph {
  BGRecord * my_graph;
public:
  BiGraph();
  // POST: this is an empty bipartite graph (with some preliminary memory
  //       allocation)

  BiGraph(const BiGraph &);     // copy constructor
  BiGraph & operator = (const BiGraph &); // assignment operator
  ~BiGraph();

  void read_with_order( istream & dot, istream & ord );
  // POST: this graph has been extracted in "dot" format from 'dot'
  //       using the layer ordering specified in "ord" format in 'ord'

  void read( istream & dot );
  // alternate version when no .ord file is given

  int n0() const;
  int n1() const;
  int number_of_edges() const;

  int degree(int i) const;
  // PRE: 0 <= i < n0
  // POST: retval == degree of vertex i in V0

  int get_node_number( const string name ) const;
  // POST: retval == current node number in V0 of node with the given name,
  //                 or -1 if no such node exists

  const string get_name( int i ) const;
  // POST: retval == name of node i in V0

  int max_degree_vertex() const;
  // POST: retval == maximum degree vertex in V0

  int min_degree_vertex() const;
  // POST: retval == minimum degree vertex in V0

  void output_order( ostream& out, const string graph_name, 
                                   const string treatment_name );
  // POST: out contains the current layer ordering for this graph in ord
  //       format
 
  void output_dre( ostream& out );
  // POST: out contains the graph in .dre (dreadnaught) format, a very
  // simple format in which nodes are numbered 0 through n-1; the first
  // two lines are
  //      n=number_of_nodes $=0 g
  //      $$
  // followed by a line of the form
  //      node_number: x_1 x_2 ... x_k;
  // where x_1, x_2, ..., x_k are the number of the k neighbors of the node
  // whose number is node_number.  Edges from higher numbers to lower
  // numbers can be omitted for undirected (so the output for a bigraph
  // looks the same whether we consider it directed or undirected).  We
  // number the nodes on layer 0 from 0 to n_0 - 1, those on layer 1 from
  // n_0 to n_0 + n_1 - 1.

  void switch_sides();
  // switch V0 and V1 (for the algorithms, V0 is the side that can be
  // permuted)

  void sort(int * key);
  // PRE:  key[i] is defined for each i = 0, ..., n0 - 1 
  // POST: vertices of V0 have been permuted so that the relationship
  //       between i and key[i] is maintained and 
  //       key[0] <= key[1] <= ... <= key[n0 - 1].
  //       This implies that the permutation of the key array has also been
  //       modified. 

  Iterator adjacency_list(int i);
  // PRE: 0 <= i < n0
  // POST: retval == an iterator for the adjacency list of vertex i in V0

  BackIterator backwards_adjacency_list(int i);
  // PRE: 0 <= i < n0
  // POST: retval == a backwards iterator for the adjacency list of vertex
  //                 i in V0 

  void sort(int i, const int * key, bool descending = false);
  // PRE: 0 <= i < n0 && key[j] is defined for each j in adjacency_list(i) 
  // POST: the adjacency list of vertex i in V0 has been arranged so that
  //       key[x_1] <= key[x_2] <= ... <= key[x_k], where x_1, ..., x_k is
  //       the sequence emitted by the iterator for it.
  //       If descending == true, key[x_1] >= key[x_2] >= ... >= key[x_k].
  
  void insert(int i, int j);
  // PRE: 0 <= i, j < n0
  // POST: vertex i in V0 has been inserted after vertex j (before if j < i)

  void swap(int i, int j);
  // PRE: 0 <= i <= j < n0
  // POST: vertex i in V0 has been exchanged with vertex j
  // NOT IMPLEMENTED!

  unsigned long long crossings() const;
  // POST: retval == current number of edge crossings

  int change_in_crossings(int i, int j) const;
  // PRE: 0 <= i, j < n0
  // POST: retval == change in # of edge crossings after the insert(i, j)

  double average(int i) const;
  // PRE: 0 <= i < n0
  // POST: retval == (biased) median of items in adjacency list for vertex
  //                 i in V0
  //                 (ratio is the ratio of mean to the mean/median mix,
  //                 i.e. 0.0 represents all median, 1.0 all mean)
 
  int best_insertion( int i, int & change_in_crossings ) const;
  // PRE: 0 <= i < n0
  // POST: retval == the j != i for which change_in_crossings(i, j) is
  //                 minimal, or IntSet::DUMMY if no such j exists
  //                 (i.e. when there's only one node on this side) 
  //       change_in_crossings == change_in_crossings(i, j) for that j

  int best_swap(int i) const;
  // PRE: 0 <= i < n0
  // POST: retval == the j for which swap(i, j) gives the smallest number
  //                 of crossings 
  // NOT IMPLEMENTED!

  friend ostream& operator << (ostream& out, const BiGraph& bg);
};

#endif

// Local Variables: ***
//  mode:c++ ***
// End: ***

//  [Last modified: 2007 08 08 at 18:39:37 GMT]
