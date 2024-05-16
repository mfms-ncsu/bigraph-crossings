/// @file bfs_heuristic.cxx
/// @brief implementation of bfs-based heuristic: guided breadth-first search
///
/// @author Matt Stallmann
/// @date 1998/08/03
/// $Id: bfs_heuristic.cxx 65 2007-08-08 18:45:24Z mfms $

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

#include"bfs_heuristic.h"
#include"Queue.hxx"

static int * side_zero_distance;
static int * side_one_distance;
static int * this_side_distance;
static int * side_zero_depth;
static int * side_one_depth;
static int * this_side_depth;
static int * side_zero_parent;
static int * side_one_parent;
static int * this_side_parent;
static int * side_zero_sequence;
static int * side_one_sequence;
static int * this_side_sequence;

static int last_vertex = -1;    // vertex at greatest distance from the
                                // start during first BFS 
static int max_distance = -1;   // maximum distance from start vertex in
                                // first BFS 

static void init_bfs(const BiGraph & bg)
{
  side_zero_distance = new int[ bg.n0() ];
  side_zero_depth = new int[ bg.n0() ];
  side_zero_parent = new int[ bg.n0() ];
  side_zero_sequence = new int[ bg.n0() ];
  for ( int i = 0; i < bg.n0(); ++i ) {
    side_zero_distance[i] = INT_MAX;
    side_zero_depth[i] = 0;
    side_zero_parent[i] = IntSet::DUMMY;
    side_zero_sequence[i] = 0;
  }
  side_one_distance = new int[ bg.n1() ];
  side_one_depth = new int[ bg.n1() ];
  side_one_parent = new int[ bg.n1() ];
  side_one_sequence = new int[ bg.n1() ];
  for ( int i = 0; i < bg.n1(); ++i ) {
    side_one_distance[i] = INT_MAX;
    side_one_depth[i] = 0;
    side_one_parent[i] = IntSet::DUMMY;
    side_one_sequence[i] = 0;
  }
  this_side_distance = side_zero_distance;
  this_side_depth = side_zero_depth;
  this_side_parent = side_zero_parent;
  this_side_sequence = side_zero_sequence;
}

static void switch_sides()
{
  if ( this_side_distance == side_zero_distance ) this_side_distance =
                                                    side_one_distance;
  else this_side_distance = side_zero_distance;
  if ( this_side_depth == side_zero_depth ) this_side_depth =
                                              side_one_depth;
  else this_side_depth = side_zero_depth;
  if ( this_side_parent == side_zero_parent ) this_side_parent =
                                                side_one_parent;
  else this_side_parent = side_zero_parent;
  if ( this_side_sequence == side_zero_sequence ) this_side_sequence =
                                                    side_one_sequence;
  else this_side_sequence = side_zero_sequence;
}

static void trace_path(int vertex, int depth)
  // POST: every vertex on the path from 'vertex' to root has depth at
  //       least 'depth' 
{
  int path_vertex = vertex;
  int side_switches = 0;    // number of times we switched sides
  while ( IntSet::DUMMY != path_vertex
          && depth > this_side_depth[path_vertex] ) {
#ifdef TRACE
    cerr << "     traversing path, vertex = " << path_vertex
         << ", depth = " << depth << endl;
#endif
    this_side_depth[path_vertex] = depth;
    path_vertex = this_side_parent[path_vertex];
    switch_sides();
    ++side_switches;
  }
  // get back to correct side if path length was odd
  if ( 0 != side_switches % 2 ) switch_sides();
}

static void bfs_depth(BiGraph & bg, int start_vertex)
{
  Queue Q;
  int current_distance = 0;
  this_side_distance[start_vertex] = 0;
  Q.add(start_vertex);
  Q.add(IntSet::DUMMY);
  while ( 1 < Q.length() ) {
    int current_vertex = Q.front();
#ifdef TRACE
    cerr << "   Removing vertex " << current_vertex << " from queue." << endl;
#endif
    Q.remove();
    if ( IntSet::DUMMY == current_vertex ) {
      switch_sides();
      bg.switch_sides();
      ++current_distance;
      Q.add(IntSet::DUMMY);
    }
    else {                      // a real vertex in V0
      last_vertex = current_vertex;
      Iterator adj_list = bg.adjacency_list(current_vertex);
      bool interior_node = false; // true if current vertex is interior
                                  // node of the BFS tree
      switch_sides();
      while ( adj_list.valid() ) {
        int neighbor = adj_list.vertex();
        if ( INT_MAX == this_side_distance[neighbor] ) { // new tree edge
#ifdef TRACE
          cerr << "     Adding neighbor vertex " << neighbor << " to queue."
               << endl;
#endif
          this_side_parent[neighbor] = current_vertex;
          this_side_distance[neighbor] = current_distance + 1;
          Q.add(neighbor);
          interior_node = true;
        }
        else {                  // cross edge - cycle found
/*** Omitted 28-AUG-1998
          switch_sides();
          if ( neighbor != this_side_parent[current_vertex] ) {
            switch_sides();
            int cycle_depth = current_distance +
              this_side_distance[neighbor] + 1;
            trace_path(neighbor, cycle_depth);
            switch_sides();
            trace_path(current_vertex, cycle_depth);
          }
          switch_sides();
*** 28-AUG-1998 ***/
        }
        adj_list.next();
      }
      switch_sides();
      if ( ! interior_node ) {  // leaf => update depth along path to root
        trace_path(current_vertex, current_distance);
      }
    }
  }
  // get back to side zero if last vertex has odd distance
  max_distance = this_side_distance[last_vertex];
  if ( 0 != max_distance % 2 ) {
    switch_sides();
    bg.switch_sides();
  }
}

static void bfs_sequence(BiGraph & bg, int start_vertex)
{
  if ( 0 != max_distance % 2 ) {
    switch_sides();
    bg.switch_sides();
  }
  Queue Q;
  int current_distance = 0;
  int sequence_number = 1;
  this_side_sequence[start_vertex] = 1;
  Q.add(start_vertex);
  Q.add(IntSet::DUMMY);
  while ( 1 < Q.length() ) {
    int current_vertex = Q.front();
#ifdef TRACE
    cerr << "   Removing vertex " << current_vertex << " from queue." << endl;
#endif
    Q.remove();
    if ( IntSet::DUMMY == current_vertex ) {
      ++current_distance;
      switch_sides();
      bg.switch_sides();
      Q.add(IntSet::DUMMY);
    }
    else {                      // a real vertex in V0
      Iterator adj_list = bg.adjacency_list(current_vertex);
      switch_sides();
      while ( adj_list.valid() ) {
        int neighbor = adj_list.vertex();
        if ( 0 == this_side_sequence[neighbor] ) { // new tree edge
#ifdef TRACE
          cerr << "     Adding neighbor vertex " << neighbor << " to queue."
               << endl;
#endif
          this_side_sequence[neighbor] = ++sequence_number;
          Q.add(neighbor);
        }
        adj_list.next();
      }
      switch_sides();
    }
  }
  // get back to original side if parity is odd (started on side one XOR
  // went an odd distance from the start)
  if ( 0 != (current_distance + max_distance) % 2 ) {
    switch_sides();
    bg.switch_sides();
  }
}

void bfs_heuristic(BiGraph & bg)
{
  init_bfs(bg);

  // compute depth = maximum distance of any BFS tree descendant for each
  // vertex 
#ifdef TRACE
  cerr << " First BFS, bg = " << bg << endl;
#endif
  int start_vertex = bg.max_degree_vertex();

  bfs_depth(bg, start_vertex);

  // sort every adjacency list by descreasing distance and then by
  // increasing depth (we're on side zero now, so the adjacencent vertices
  // are on side one) 
  for ( int i = 0; i < bg.n0(); i++ ) {
    bg.sort(i, side_one_distance, true);
    bg.sort(i, side_one_depth);
  }
  bg.switch_sides();
  // (now we're on side one with adjecent vertices on side 0)
  for ( int i = 0; i < bg.n0(); i++ ) {
    bg.sort(i, side_zero_distance, true);
    bg.sort(i, side_zero_depth);
  }
  bg.switch_sides();

  // do another BFS, starting with vertex at greatest distance from the
  // start vertex
#ifdef TRACE
  cerr << " Second BFS, bg = " << bg << endl;
#endif
  bfs_sequence(bg, last_vertex);

  // sort vertices on both sides by sequence number
  bg.sort(side_zero_sequence);
  switch_sides(); bg.switch_sides();
  bg.sort(side_one_sequence);
  switch_sides(); bg.switch_sides();
}

//  [Last modified: 2007 08 08 at 18:37:52 GMT]
