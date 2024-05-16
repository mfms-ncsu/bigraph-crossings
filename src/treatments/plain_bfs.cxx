/// @file plain_bfs.cxx
/// @brief implementation of a breadth-first search
/// @author Matt Stallmann
/// @date 3 Aug 1998

// last modification, 28 Aug 1998

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

#include"plain_bfs.h"
#include"Queue.hxx"

static bool * side_zero_mark;
static bool * side_one_mark;
static bool * this_side_mark;
static int * side_zero_sequence;
static int * side_one_sequence;
static int * this_side_sequence;

static enum Side { ZERO, ONE } current_side = ZERO;

static void init_bfs(const BiGraph & bg)
{
  current_side = ZERO;
  side_zero_mark = new bool[ bg.n0() ];
  side_zero_sequence = new int[ bg.n0() ];
  for ( int i = 0; i < bg.n0(); ++i ) {
    side_zero_mark[i] = false;
    side_zero_sequence[i] = 0;
  }
  side_one_mark = new bool[ bg.n1() ];
  side_one_sequence = new int[ bg.n1() ];
  for ( int i = 0; i < bg.n1(); ++i ) {
    side_one_mark[i] = false;
    side_one_sequence[i] = 0;
  }
  this_side_mark = side_zero_mark;
  this_side_sequence = side_zero_sequence;
}

static void switch_sides(BiGraph & bg)
{
  bg.switch_sides();
  if ( ZERO == current_side ) {
    current_side = ONE;
    this_side_mark = side_one_mark;
    this_side_sequence = side_one_sequence;
  }
  else {
    current_side = ZERO;
    this_side_mark = side_zero_mark;
    this_side_sequence = side_zero_sequence;
  }
}

static void bfs_sequence(BiGraph & bg, int start_vertex)
  // POST: every vertex in the same connected component as start_vertex is
  //       marked and has been given a sequence number;
{
#ifdef TRACE
  cerr << "   bfs_sequence, start_vertex = " << start_vertex
       << ", side = " << current_side << endl;
#endif
  Side beginning_side = current_side;
  int sequence_number = 0;
  Queue Q;
  this_side_mark[start_vertex] = true;
  Q.add(start_vertex);
  Q.add(IntSet::DUMMY);
  while ( 1 < Q.length() ) {
    int current_vertex = Q.front();
#ifdef TRACE
    cerr << "   Removing vertex " << current_vertex
         << " from queue, side = " << current_side << endl;
#endif
    Q.remove();
    if ( IntSet::DUMMY == current_vertex ) {
      switch_sides(bg);
      Q.add(IntSet::DUMMY);
    }
    else {                      // a real vertex in V0
      this_side_sequence[current_vertex] = ++sequence_number;
      Iterator adj_list = bg.adjacency_list(current_vertex);
      switch_sides(bg);
      while ( adj_list.valid() ) {
        int neighbor = adj_list.vertex();
        if ( ! this_side_mark[neighbor] ) { // new tree edge
#ifdef TRACE
          cerr << "     Adding neighbor vertex " << neighbor << " to queue."
               << endl;
#endif
          this_side_mark[neighbor] = true;
          Q.add(neighbor);
        }
        adj_list.next();
      }
      switch_sides(bg);
    }
  }
  // get back to correct side if necessary
  if ( current_side != beginning_side ) switch_sides(bg);
#ifdef TRACE
  cerr << "  End of bfs_max_degree, side = " << current_side << endl;
#endif
}

void plain_bfs(BiGraph & bg)
{
  init_bfs(bg);
  bfs_sequence(bg,
               /*** 28-AUG-1998 (use a "random" vertex instead of max
                 degree as the start vertex)
                 bg.max_degree_vertex()
                 ***/
               0);

  // sort vertices on both sides by sequence number
  bg.sort(side_zero_sequence);
  switch_sides(bg);
  bg.sort(side_one_sequence);
  switch_sides(bg);
}

//  [Last modified: 2007 08 03 at 19:05:30 GMT]
