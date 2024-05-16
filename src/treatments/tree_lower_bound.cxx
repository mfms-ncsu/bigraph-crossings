/// @file  tree_lower_bound.cxx
/// @brief implementation of lower bound for trees
/// @author Matt Stallmann
/// @date 2000/01/03
/// $Id: tree_lower_bound.cxx 62 2007-08-03 19:16:20Z mfms $

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

#include<assert.h>
#include<limits.h>

#include"tree_lower_bound.h"

class LBTree: public BiGraph {  // adds data structures needed for
                                // computing lower bounds
public:
  LBTree( const BiGraph & bg );
  ~LBTree();
  void switch_sides();
  void visit( int u );          // visit node u (in a dfs)

  // the following assume that visit(u) has been done
  int depth( int u ) const;     // number of edges on longest path to a
                                // leaf from node u
  int longest( int u ) const;   // number of edges in longest path that
                                // goes through u
  int leaves( int u ) const;    // number of leaves in the subtree that
                                // includes u
private:
  enum { ZERO, ONE } current_side;
  bool * side_zero_mark;
  bool * side_one_mark;
  bool * this_side_mark;
  int * side_zero_depth;
  int * side_one_depth;
  int * this_side_depth;
  int * side_zero_longest;
  int * side_one_longest;
  int * this_side_longest;
  int * side_zero_leaves;
  int * side_one_leaves;
  int * this_side_leaves;
};

LBTree::LBTree( const BiGraph & bg ): BiGraph( bg )
{
  current_side = ZERO;
  this_side_mark = side_zero_mark = new bool[ n0() ];
  this_side_depth = side_zero_depth = new int[ n0() ];
  this_side_longest = side_zero_longest = new int[ n0() ];
  this_side_leaves = side_zero_leaves = new int[ n0() ];
  for ( int i = 0; i < n0(); ++i ) {
    side_zero_mark[ i ] = false;
    side_zero_depth[ i ] = 1;   // default values for a leaf
    side_zero_longest[ i ] = 0;
    side_zero_leaves[ i ] = 1;
  }
  side_one_mark = new bool[ n1() ];
  side_one_depth = new int[ n1() ];
  side_one_longest = new int[ n1() ];
  side_one_leaves = new int[ n1() ];
  for ( int i = 0; i < n1(); ++i ) {
    side_one_mark[ i ] = false;
    side_one_depth[ i ] = 1;   // default values for a leaf
    side_one_longest[ i ] = 0;
    side_one_leaves[ i ] = 1;
  }
}

LBTree::~LBTree()
{
  delete [] side_zero_mark;
  delete [] side_zero_depth;
  delete [] side_zero_longest;
  delete [] side_zero_leaves;

  delete [] side_one_mark;
  delete [] side_one_depth;
  delete [] side_one_longest;
  delete [] side_one_leaves;
}

void 
LBTree::switch_sides()
{
  BiGraph::switch_sides();
  if ( ZERO == current_side ) {
    current_side = ONE;
    this_side_mark = side_one_mark;
    this_side_depth = side_one_depth;
    this_side_longest = side_one_longest;
    this_side_leaves = side_one_leaves;
  }
  else {
    current_side = ZERO;
    this_side_mark = side_zero_mark;
    this_side_depth = side_zero_depth;
    this_side_longest = side_zero_longest;
    this_side_leaves = side_zero_leaves;
  }
}

void
LBTree::visit( int u )
{
#ifdef TRACE
  cerr << "->LBTree::visit, u = " << u
       << ", side = " << current_side << endl;
#endif
  this_side_mark[ u ] = true;
  int current_max_depth = 0;    // depth of deepest subtree so far
  int current_2nd_depth = 0;    // depth of 2nd deepest subtree so far
  int leaf_count = 0;           // number of leaves in subtrees so far
  Iterator adj_list = adjacency_list( u );
  switch_sides();
  while ( adj_list.valid() ) {
    int neighbor = adj_list.vertex();
    if ( ! this_side_mark[neighbor] ) { // new tree edge
      visit( neighbor );
      if ( this_side_depth[ neighbor ] > current_max_depth ) {
        current_2nd_depth = current_max_depth;
        current_max_depth = this_side_depth[ neighbor ];
      }
      else if ( this_side_depth[ neighbor ] > current_2nd_depth ) {
        current_2nd_depth = this_side_depth[ neighbor ];
      }
      leaf_count += this_side_leaves[ neighbor ];
    }
    adj_list.next();
  }
  switch_sides();

  this_side_depth[ u ] = current_max_depth + 1;
  this_side_longest[ u ] = current_max_depth + current_2nd_depth;
  if ( degree( u ) == 1 ) ++leaf_count; // current node is a leaf
  this_side_leaves[ u ] = leaf_count;

#ifdef TRACE
  cerr << " depth[ " << u << " ] = " << this_side_depth[ u ] << endl;
  cerr << " longest[ " << u << " ] = " << this_side_longest[ u ] << endl;
  cerr << " leaves[ " << u << " ] = " << this_side_leaves[ u ] << endl;
  cerr << "<-LBTree::visit, u  = " << u
       << ", side = " << current_side << endl;
#endif
}

int
LBTree::depth( int i ) const
{
  return this_side_depth[ i ];
}

int
LBTree::longest( int i ) const
{
  return this_side_longest[ i ];
}

int
LBTree::leaves( int i ) const
{
  return this_side_leaves[ i ];
}

int tree_lower_bound( BiGraph & bg )
{
  LBTree T = bg;
  T.visit( 0 );
  int longest_path = 0;
  for ( int i = 0; i < T.n0(); ++i ) {
    longest_path = T.longest( i ) > longest_path ? T.longest( i ) :
      longest_path;
  }
  T.switch_sides();
  for ( int i = 0; i < T.n0(); ++i ) {
    longest_path = T.longest( i ) > longest_path ? T.longest( i ) :
      longest_path;
  }
  T.switch_sides();
  int leaves = T.leaves( 0 );
  return T.number_of_edges() - longest_path - leaves + 2;
}

//  [Last modified: 2007 08 03 at 19:01:06 GMT]
