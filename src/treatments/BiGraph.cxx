/// @file BiGraph.cxx
/// @brief Bipartite graph class for crossing number problem.
/// @author Matt Stallmann
/// @date 12 Jun 1998
/// $Id: BiGraph.cxx 65 2007-08-08 18:45:24Z mfms $

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

// Modifications up to 18 Aug 1998 (added lots of functionality)

// 29 Dec 1998 - changed so that order of nodes on a layer is in a
// different file and so that node names are stored as part of this class

// 14 Apr 1999 - added get_node_number() and get_name() for BiGraph class

// 20 May 2002 - added storage of graph name to BGRecord

// 29 Jul 2002 - added the output_dre() method

// 27 Sep 2004 - fixed an uninitialized pointer (my_name in BGRecord) 

#include<stdlib.h>              // abort()
#include<limits.h>
#include<iostream>
#include<string>
#include<sstream>
#include<assert.h>

#include"defs.h"
#include"BiGraph.hxx"
#include"Sums.hxx"
#include"IntSet.h"
#include"dot.h"
#include"ord.h"

const int INITIAL_BUFFER_SIZE = 2;   // initial size of arrays holding info
                                     // about vertices (to be doubled when
                                     // necessary)  

// Each adjacency list is stored as an array of integers, the first element
// gives the length and the rest are the adjacencies on the other side.

struct BGRecord {
  string my_name;               // graph name (used for debugging)
  int my_n0;
  int my_n1;                    // number of nodes on each "side"
  int my_number_of_edges;
  int my_V0_buffer_size;
  int my_V1_buffer_size;        // actual size of arrays holding info about
                                // V0 or V1 (respectively) - always a power
                                // of 2.  
  string * my_V0_name;
  string * my_V1_name;          // names of nodes on each "side"
  IntSet ** my_V0;              // V0's adjacency lists (each list is a
                                //  pointer to an IntSet)
  IntSet ** my_V1;              // V1's adjacency lists

  IntNode ** my_V0_list;
  IntNode ** my_V1_list;        // adjacency lists in linked-list form
                                // (used only when adding edges) 

  // the following are used to keep track of how this graph has been
  // altered (permutations of each side are stored in integer arrays)
  int * my_V0_permutation;
  int * my_V1_permutation;
  bool sides_switched;

  BGRecord( int n0 = 0, int n1 = 0 );

  ~BGRecord();

  // used by copy constructor and overloaded assignment for BiGraphs
  BGRecord * copy() const;
  // POST: retval == (ptr to) a freshly allocated copy of this BGRecord

  const string get_graph_name();
  // POST: retval == the name of the graph

  void put_name( const string name );
  // POST: name of this graph is 'name'

  int get_node_number( const string stg, int side = 0 ) const;
  // POST: retval == node number corresponding to 'stg' on the given side
  //                 (or -1 if no node of that name exists)

  int add_node( const string stg, int side = 0 );
  // PRE: 'stg' is not already the name of a node on the given side
  // POST: a new node with name 'stg' has been created on the given side
  //       retval == number of the new node

  void begin_adding_edges();
  // POST: adjacency lists are in linked-list format to accomodate new
  //       edges

  void end_adding_edges();
  // POST: adjacency lists are converted to array format and both arrays of
  //       linked lists are deallocated and set to 0

  void add_edge( const string u, const string v,
                 bool allow_expansion = true );
  // POST: edge (u,v) has been added (and node u or v has been added if not
  //       already present and allow_expansion == true)

  void switch_sides();
  // PRE: not in the middle of adding edges (i.e. between a begin_ and end_
  //      adding_edges pair)
  // POST: all data on the V0 side is on the V1 side and vice-versa,
  //       sides_switched == the opposite of what it was at the beginning
};

BGRecord::BGRecord( int n0, int n1 ): 
  my_n0(n0), my_n1(n1),
  my_number_of_edges(0),
  my_V0_buffer_size(INITIAL_BUFFER_SIZE),
  my_V1_buffer_size(INITIAL_BUFFER_SIZE),
  my_V0(0), my_V1(0),
  my_V0_list(0), my_V1_list(0),
  my_V0_permutation(0), my_V1_permutation(0), sides_switched(false)
{
  while ( my_V0_buffer_size < my_n0 ) my_V0_buffer_size *= 2;
  my_V0_name = new string[ my_V0_buffer_size ];
  my_V0 = new IntSet *[ my_V0_buffer_size ];
  for ( int i = 0; i < my_V0_buffer_size; ++i ) my_V0[ i ] = 0;
  my_V0_permutation = new int[ my_V0_buffer_size ];
  for ( int i = 0; i < my_V0_buffer_size; ++i ) my_V0_permutation[ i ] = i;

  while ( my_V1_buffer_size < my_n1 ) my_V1_buffer_size *= 2;
  my_V1_name = new string[ my_V1_buffer_size ];
  my_V1 = new IntSet *[ my_V1_buffer_size ];
  for ( int i = 0; i < my_V1_buffer_size; ++i ) my_V1[ i ] = 0;
  my_V1_permutation = new int[ my_V1_buffer_size ];
  for ( int i = 0; i < my_V1_buffer_size; ++i ) my_V1_permutation[ i ] = i;
}

BGRecord::~BGRecord()
{
  if ( my_V0 ) {
    for ( int i = 0; i < my_n0; i++ ) {
      delete my_V0[i];
    }
    delete [] my_V0_name;
    delete [] my_V0;
    delete [] my_V0_permutation;
  }
  if ( my_V1 ) {
    for ( int i = 0; i < my_n1; i++ ) {
      delete my_V1[i];
    }
    delete [] my_V1_name;
    delete [] my_V1;
    delete [] my_V1_permutation;
  }
}

BGRecord *
BGRecord::copy() const
{
  assert( 0 == my_V0_list && 0 == my_V1_list );
       // shouldn't copy a graph being altered
  BGRecord * retval = new BGRecord(my_n0, my_n1);
  retval->my_number_of_edges = my_number_of_edges;
  for ( int i = 0; i < my_n0; i++ ) {
    retval->my_V0_name[i] = my_V0_name[i];
    retval->my_V0[i] = new IntSet(*my_V0[i]);
    retval->my_V0_permutation[i] = my_V0_permutation[i];
  }
  for ( int i = my_n0; i < my_V0_buffer_size; ++i ) {
    retval->my_V0[i] = 0;
    retval->my_V0_permutation[i] = i;
  }

  for ( int i = 0; i < my_n1; i++ ) {
    retval->my_V1_name[i] = my_V1_name[i];
    retval->my_V1[i] = new IntSet(*my_V1[i]);
    retval->my_V1_permutation[i] = my_V1_permutation[i];
  }
  for ( int i = my_n1; i < my_V1_buffer_size; ++i ) {
    retval->my_V1[i] = 0;
    retval->my_V1_permutation[i] = i;
  }

  retval->sides_switched = sides_switched;
  return retval;
}

const string
BGRecord::get_graph_name()
{
  return my_name;
}

void
BGRecord::put_name( const string name )
{
#ifdef DEBUG
  cerr << "BGRecord::put_name(" << name << "), my_name = " << my_name <<
    endl;
#endif
  my_name = name;
}

int
BGRecord::get_node_number( const string stg, int side ) const
{
  string* array = ( 0 == side ) ? my_V0_name : my_V1_name;
  int limit = ( 0 == side ) ? my_n0 : my_n1;
  int i = 0;
  for ( ; i < limit && stg != array[i]; ++i ) {
  }
  return ( i < limit ) ? i : -1;
}

int
BGRecord::add_node( const string stg, int side )
{
#ifdef DEBUG
  cerr << "BGRecord::add_node, stg = " << stg << ", side = " << side << endl;
#endif
  if ( 0 == side ) {
    if ( my_n0 >= my_V0_buffer_size ) { // need to reallocate arrays
      my_V0_buffer_size *= 2;

      string* temp_name = new string[ my_V0_buffer_size ];
      for ( int i = 0; i < my_n0; ++i ) temp_name[ i ] = my_V0_name[ i ];
      delete [] my_V0_name;
      my_V0_name = temp_name;

      IntSet ** temp = new IntSet *[ my_V0_buffer_size ];
      for ( int i = 0; i < my_n0; ++i ) temp[ i ] = my_V0[ i ];
      for ( int i = my_n0; i < my_V0_buffer_size; ++i ) temp[ i ] = 0;
      delete [] my_V0;
      my_V0 = temp;
      
      if ( my_V0_list ) {
        IntNode ** temp = new IntNode *[ my_V0_buffer_size ];
        for ( int i = 0; i < my_n0; ++i ) temp[ i ] = my_V0_list[ i ];
        for ( int i = my_n0; i < my_V0_buffer_size; ++i ) temp[ i ] = 0;
        delete [] my_V0_list;
        my_V0_list = temp;
      }

      int * temp_permutation = new int[ my_V0_buffer_size ];
      for ( int i = 0; i < my_n0; ++i )
        temp_permutation[ i ] = my_V0_permutation[ i ];
      for ( int i = my_n0; i < my_V0_buffer_size; ++i )
        temp_permutation[ i ] = i;
      delete [] my_V0_permutation;
      my_V0_permutation = temp_permutation;
    } // reallocation of arrays
    my_V0_name[ my_n0 ] = stg;
    return my_n0++;
  }
  else if ( 1 == side ) {
    if ( my_n1 >= my_V1_buffer_size ) { // need to reallocate arrays
      my_V1_buffer_size *= 2;

      string* temp_name = new string[ my_V1_buffer_size ];
      for ( int i = 0; i < my_n1; ++i ) temp_name[ i ] = my_V1_name[ i ];
      delete [] my_V1_name;
      my_V1_name = temp_name;

      IntSet ** temp = new IntSet *[ my_V1_buffer_size ];
      for ( int i = 0; i < my_n1; ++i ) temp[ i ] = my_V1[ i ];
      for ( int i = my_n1; i < my_V1_buffer_size; ++i ) temp[ i ] = 0;
      delete [] my_V1;
      my_V1 = temp;

      if ( my_V1_list ) {
        IntNode ** temp = new IntNode *[ my_V1_buffer_size ];
        for ( int i = 0; i < my_n1; ++i ) temp[ i ] = my_V1_list[ i ];
        for ( int i = my_n1; i < my_V1_buffer_size; ++i ) temp[ i ] = 0;
        delete [] my_V1_list;
        my_V1_list = temp;
      }

      int * temp_permutation = new int[ my_V1_buffer_size ];
      for ( int i = 0; i < my_n1; ++i )
        temp_permutation[ i ] = my_V1_permutation[ i ];
      for ( int i = my_n1; i < my_V1_buffer_size; ++i )
        temp_permutation[ i ] = i;
      delete [] my_V1_permutation;
      my_V1_permutation = temp_permutation;
    } // reallocation of arrays
    my_V1_name[ my_n1 ] = stg;
    return my_n1++;
  }
  else assert( "BGRecord::add_node, bad side number" && false );
  return -1;
}

void
BGRecord::begin_adding_edges()
{
  assert( 0 == my_V0_list && 0 == my_V1_list ); // assumes no modification
                                                // currently underway
  // initialize adjacency lists for the two sides
  // CAUTION: does not work if there are my_V0 or my_V1 already have
  //          adjacency lists in place
  my_V0_list = new IntNode *[ my_V0_buffer_size ];
  for ( int i = 0; i < my_V0_buffer_size; i++ ) my_V0_list[i] = 0;
  my_V1_list = new IntNode *[ my_V1_buffer_size ];
  for ( int i = 0; i < my_V1_buffer_size; i++ ) my_V1_list[i] = 0;
}

void
BGRecord::end_adding_edges()
{
  assert( my_V0_list && my_V1_list ); // make sure lists are present
  my_V0 = new IntSet *[ my_V0_buffer_size ];
  for ( int i = 0; i < my_n0; i++ ) my_V0[ i ] = new IntSet( my_V0_list[ i ] );
  for ( int i = my_n0; i < my_V0_buffer_size; ++i ) my_V0[ i ] = 0;
  my_V1 = new IntSet *[ my_V1_buffer_size ];
  for ( int i = 0; i < my_n1; i++ ) my_V1[ i ] = new IntSet( my_V1_list[ i ] );
  for ( int i = my_n1; i < my_V1_buffer_size; ++i ) my_V1[ i ] = 0;
  
  // deallocate linked lists
  for ( int i = 0; i < my_V0_buffer_size; ++i ) delete my_V0_list[ i ];
  for ( int i = 0; i < my_V1_buffer_size; ++i ) delete my_V1_list[ i ];
  delete [] my_V0_list;
  my_V0_list = 0;
  delete [] my_V1_list;
  my_V1_list = 0;
}

void
BGRecord::add_edge( const string u, const string v, bool allow_expansion )
{
  assert( my_V0_list && my_V1_list ); // make sure list arrays are allocated
  int i = get_node_number( u, 0 );
  if ( i < 0 ) {
    if ( allow_expansion ) i = add_node( u, 0 );
    else {
      cerr << "Read error in .dot file: node " << u
           << " on layer 0 not listed in .ord file." << endl;
      abort();
    }
  }
  int j = get_node_number( v, 1 );
  if ( j < 0 ) {
    if ( allow_expansion ) j = add_node( v, 1 );
    else {
      cerr << "Read error in .dot file: node " << v
           << " on layer 1 not listed in .ord file." << endl;
      abort();
    }
  }

  my_V0_list[ i ] = new IntNode( j, my_V0_list[ i ] );
  my_V1_list[ j ] = new IntNode( i, my_V1_list[ j ] );
  ++my_number_of_edges;
}

void
BGRecord::switch_sides()
{
  int tmp_node_count = my_n0;
  my_n0 = my_n1;
  my_n1 = tmp_node_count;

  int tmp_buffer_size = my_V0_buffer_size;
  my_V0_buffer_size = my_V1_buffer_size;
  my_V1_buffer_size = tmp_buffer_size;

  string* tmp_names = my_V0_name;
  my_V0_name = my_V1_name;
  my_V1_name = tmp_names;

  IntSet ** tmp_set = my_V0;
  my_V0 = my_V1;
  my_V1 = tmp_set;

  assert( 0 == my_V0_list && 0 == my_V1_list );

  int * tmp_permutation = my_V0_permutation;
  my_V0_permutation = my_V1_permutation;
  my_V1_permutation = tmp_permutation;

  sides_switched = sides_switched ? false : true;
}


// *** BiGraph member functions

BiGraph::BiGraph()
{
  my_graph = new BGRecord;
}

void
BiGraph::read( istream & dot )
{
  my_graph->begin_adding_edges();

  // initialize the dot file
  init_dot( dot );

  // add edges to the adjacency lists as they are read from the dot file
  {
    const char * u;
    const char * v;
    while ( next_edge( dot, u, v ) )
      my_graph->add_edge( u, v );
  }
  my_graph->end_adding_edges();
}

void
BiGraph::read_with_order( istream & dot, istream & ord )
{
  // make sure this graph has not been initialized previously
  assert( 0 == my_graph->my_n0 && 0 == my_graph->my_n1 );

  // read ordering information from the ord stream
  {
    char buffer[NAME_LENGTH];
    get_graph_name( ord, buffer );
    my_graph->put_name( buffer );
#ifdef DEBUG
    cerr << "Reading .ord info for graph " << my_graph->get_graph_name() <<
      endl;
#endif 
    int layer;
    while ( next_layer( ord, layer ) ) {
#ifdef DEBUG
      cerr << "Reading .ord info for layer " << layer << endl;
#endif
      if ( 0 > layer || 1 < layer ) {
        cerr << "Bad layer number " << layer << " in .ord file." << endl;
        abort();
      }
      char * node = 0;
      while ( next_node( ord, node ) ) {
        if ( my_graph->get_node_number( node, layer ) < 0 )
          my_graph->add_node( node, layer );
        else cerr << "Warning: repeated node " << node << " on layer "
                  << layer << " of graph " << my_graph->get_graph_name()
                  << " (ignored)." << endl; 
      }
    }
  }

  my_graph->begin_adding_edges();

  // initialize the dot file
  init_dot( dot );

  // add edges to the adjacency lists as they are read from the dot file
  {
    const char * u;
    const char * v;
    while ( next_edge( dot, u, v ) )
      my_graph->add_edge( u, v, false );
  }

  my_graph->end_adding_edges();
}

BiGraph::BiGraph(const BiGraph & other):
  my_graph( other.my_graph->copy() ) { }

BiGraph &
BiGraph::operator = (const BiGraph & rhs)
{
  if ( this == &rhs ) return *this;

  // deallocate memory associated with current graph, if any
  delete my_graph;

  // copy from rhs to this graph
  my_graph = rhs.my_graph->copy();

  return *this;
}

BiGraph::~BiGraph()
{
  delete my_graph;
}

int
BiGraph::n0() const
{
  return my_graph->my_n0;
}

int
BiGraph::n1() const
{
  return my_graph->my_n1;
}

int
BiGraph::number_of_edges() const
{
  return my_graph->my_number_of_edges;
}

int
BiGraph::get_node_number( const string name ) const
{
  return my_graph->get_node_number( name );
}

const string
BiGraph::get_name( int i ) const
{
  return my_graph->my_V0_name[ my_graph->my_V0_permutation[ i ] ];
}

int
BiGraph::degree(int i) const
{
  return my_graph->my_V0[i]->cardinality();
}

int
BiGraph::max_degree_vertex() const
{
  int max_degree = 0;
  int retval = -1;
  for ( int i = 0; i < my_graph->my_n0; ++i ) {
    int degree = my_graph->my_V0[i]->cardinality();
    if ( degree > max_degree ) {
      max_degree = degree;
      retval = i;
    }
  }
  assert( 0 <= retval );
  return retval;
}

int
BiGraph::min_degree_vertex() const
{
  int min_degree = my_graph->my_n1 + 1;
  int retval = -1;
  for ( int i = 0; i < my_graph->my_n0; ++i ) {
    int degree = my_graph->my_V0[i]->cardinality();
    if ( degree < min_degree ) {
      min_degree = degree;
      retval = i;
    }
  }
  assert( 0 <= retval );
  return retval;
}

void
BiGraph::output_order( ostream & out, const string graph_name,
                                const string treatment_name )
{
  ord_preamble( out, graph_name.c_str(), treatment_name.c_str() );

  if ( my_graph->sides_switched ) {
    switch_sides();
  }

  // output node order on each layer
  begin_layer( out, 0 );
  for ( int i = 0; i < my_graph->my_n0; ++i ) {
    output_node(out,
                my_graph->my_V0_name[my_graph->my_V0_permutation[i]].c_str());
  }
  end_layer(out);

  begin_layer( out, 1 );
  for ( int i = 0; i < my_graph->my_n1; ++i ) {
    output_node(out,
                my_graph->my_V1_name[my_graph->my_V1_permutation[i]].c_str());
  }
  end_layer(out);
}

void
BiGraph::output_dre( ostream & out )
{
  // preamble gives total number of nodes, specifies that first node is
  // numbered 0, and prepares for the remainder of the graph to be read
  out << "n=" << (n0() + n1()) << " $=0 g" << endl;
  // each subsequent line has a node number, followed by a colon, followed
  // by a space-delimited list of neighbors, followed by a semicolon
  for ( int i = 0; i < n0(); ++i ) {
    out << i << ":";
    for ( Iterator neighbors = adjacency_list( i );
          neighbors.valid();
          neighbors.next() ) {
      out << " " << (n0() + neighbors.vertex());
    }
    out << ";" << endl;
  }
}

void
BiGraph::switch_sides()
{
  my_graph->switch_sides();
}

void
BiGraph::sort(int * key)
{

#ifdef DEBUG
  cerr << "-> BiGraph::sort -" << endl;
  cerr << " current permutation is";
  for ( int i = 0; i < my_graph->my_n0; ++i )
   cerr << " " << my_graph->my_V0_permutation[ i ];
  cerr << endl;
  cerr << "                 key is";
  for ( int i = 0; i < my_graph->my_n0; ++i )
   cerr << " " << key[ i ];
  cerr << endl;
#endif

  for ( int i = my_graph->my_n0 - 2; i >= 0; --i ) {
    int to_be_inserted = key[i];
    int j = i + 1;
    while ( j < my_graph->my_n0 && to_be_inserted > key[j] ) {
      // BUG FIX: used to be to_be_inserted < key[j], which gives reverse
      //          order and doesn't matter as far as crossing heuristics are
      //          concerned (but does matter in other applications) 
      key[j - 1] = key[j];
      ++j;
    }
    key[j - 1] = to_be_inserted;
    insert(i, j - 1);
  }

#ifdef DEBUG
  cerr << " current permutation is";
  for ( int i = 0; i < my_graph->my_n0; ++i )
   cerr << " " << my_graph->my_V0_permutation[ i ];
  cerr << endl;
  cerr << "<- BiGraph::sort -" << endl;
#endif

}

Iterator
BiGraph::adjacency_list(int i)
{
  assert( 0 <= i && i < my_graph->my_n0 );
  return *my_graph->my_V0[i];
}

BackIterator
BiGraph::backwards_adjacency_list(int i)
{
  assert( 0 <= i && i < my_graph->my_n0 );
  return *my_graph->my_V0[i];
}

void
BiGraph::sort(int i, const int * key, bool descending)
{
  assert( 0 <= i && i < my_graph->my_n0 );
  my_graph->my_V0[i]->sort(key, descending);
}

unsigned long long
BiGraph::crossings() const
{
  unsigned long long retval = 0;
  Sums edge_counts(my_graph->my_n1);
  for ( int i = 0; i < my_graph->my_n0; i++ ) {
    for ( Iterator adj_of_i = *my_graph->my_V0[i]; adj_of_i.valid();
          adj_of_i.next() ) {
      int j = adj_of_i.vertex();
      int increment = edge_counts.sum(j + 1, my_graph->my_n1 - 1);
      retval += increment;
    }
    for ( Iterator adj_of_i = *my_graph->my_V0[i]; adj_of_i.valid();
          adj_of_i.next() ) {
      edge_counts.increment(adj_of_i.vertex());
    }
  }
  return retval;
}

void 
BiGraph::insert(int i, int j)
{
  if (i == j ) return;

  assert( 0 <= i && i < my_graph->my_n0 && j < my_graph->my_n0 );

  // Renumber V0 vertices in adj lists on the V1 side (do this first!)
  // 1. Change i to DUMMY in all the adjacency lists that contain i
  for ( Iterator i_adj = *my_graph->my_V0[i]; i_adj.valid(); i_adj.next() )
    my_graph->my_V1[i_adj.vertex()]->renumber(i, IntSet::DUMMY);

  if ( i < j ) {
    // 2a. Change k to k-1 for all lists that contain k (k = i+1, ... , j)
    for ( int k = i + 1; k <= j; ++k ) {
      for ( Iterator k_adj = *my_graph->my_V0[k]; k_adj.valid(); k_adj.next() )
        my_graph->my_V1[k_adj.vertex()]->renumber(k, k - 1);
    }
  }
  else { // i > j
    // 2b. Change k to k+1 for all lists that contain k (k = i-1, ... , j)
    for ( int k = i - 1; k >= j; --k ) {
      for ( Iterator k_adj = *my_graph->my_V0[k]; k_adj.valid(); k_adj.next() )
        my_graph->my_V1[k_adj.vertex()]->renumber(k, k + 1);
    }
  }

  // 3. Change DUMMY's to j's
  for ( Iterator i_adj = *my_graph->my_V0[i]; i_adj.valid(); i_adj.next() )
    my_graph->my_V1[i_adj.vertex()]->renumber(IntSet::DUMMY, j);

  // shift adjacency lists and permutations in V0
  IntSet * tmp_set_ptr = my_graph->my_V0[i];
  int original_i = my_graph->my_V0_permutation[i]; 
  if ( i < j ) {
    for ( int k = i + 1; k <= j; ++k ) {
      my_graph->my_V0[k - 1] = my_graph->my_V0[k];
      my_graph->my_V0_permutation[k - 1] = my_graph->my_V0_permutation[k];
    }
  }
  else { // i > j
    for ( int k = i - 1; k >= j; --k ) {
      my_graph->my_V0[k + 1] = my_graph->my_V0[k];
      my_graph->my_V0_permutation[k + 1] = my_graph->my_V0_permutation[k];
    }
  }
  my_graph->my_V0[j] = tmp_set_ptr;
  my_graph->my_V0_permutation[j] = original_i;
}

void
BiGraph::swap(int i, int j)
{
  cerr << "BiGraph::swap(" << i << ", " << j << ") - DISABLED" << endl;
  abort();
  /* !!! DISABLED - needs to be fixed to handle permutations
  if (i == j ) return;

  // set up swapping and non-swapping sides
  assert( 0 <= i && i < my_graph->my_n0 && j < my_graph->my_n0 );
  IntSet ** swapping_side = my_graph->my_V0;
  IntSet ** non_swapping_side = my_graph->my_V1;

  // Renumber vertices on non-shifting side (do this first!)
  // 1. Change i to DUMMY in all the adjacency lists that contain i
  for ( Iterator i_adj = *swapping_side[i]; i_adj.valid(); i_adj.next() )
   non_swapping_side[i_adj.vertex()]->renumber(i, IntSet::DUMMY);

  // 2. Change j to i for all lists that contain j
  for ( Iterator j_adj = *swapping_side[j]; j_adj.valid(); j_adj.next() )
   non_swapping_side[j_adj.vertex()]->renumber(j, i);

  // 3. Change DUMMY's to j's
  for ( Iterator i_adj = *swapping_side[i]; i_adj.valid(); i_adj.next() )
   non_swapping_side[i_adj.vertex()]->renumber(IntSet::DUMMY, j);

  // swap adjacency lists on the shifting side of the graph
  IntSet * tmp_set_ptr = swapping_side[i];
  swapping_side[i] = swapping_side[j];
  swapping_side[j] = tmp_set_ptr;
  !!! DISABLED */
}

int 
BiGraph::change_in_crossings(int i, int j) const
{
  assert( 0 <= i && i < my_graph->my_n0 && j < my_graph->my_n0 );

  GapCounts gaps(*my_graph->my_V0[i]);
  if ( i <= j ) {
    for ( int k = i + 1; k <= j; ++k ) {
      gaps.update(*my_graph->my_V0[k]);
    }
    return gaps.change_in_crossings();
  }
  else { // i > j
    for ( int k = i - 1; k >= j; --k ) {
      gaps.update(*my_graph->my_V0[k]);
    }
    return -gaps.change_in_crossings();
  }
}

double
BiGraph::average(int i) const
{
  return my_graph->my_V0[i]->average();
}

int
BiGraph::best_insertion(int i, int & change_in_crossings ) const
{
  assert( 0 <= i && i < my_graph->my_n0 );
  int retval = IntSet::DUMMY;
  change_in_crossings = INT_MAX;
  { // look for better insertion with j > i
    GapCounts gaps( *my_graph->my_V0[i] );
    for ( int k = i + 1; k < my_graph->my_n0; ++k ) {
      gaps.update( *my_graph->my_V0[k] );
      int current_change = gaps.change_in_crossings();
#ifdef DEBUG
      cerr << "      best_insertion: i = " << i << ", k = " << k
           << ", change = " << current_change << endl;
#endif
      if ( current_change < change_in_crossings ) {
        change_in_crossings = current_change;
        retval = k;
      }
    }
  }
  { // look for better insertion with j < i
    GapCounts gaps( *my_graph->my_V0[i] );
    for ( int k = i - 1; k >= 0; --k ) {
      gaps.update( *my_graph->my_V0[k] );
      int current_change = -gaps.change_in_crossings();
#ifdef DEBUG
      cerr << "      best_insertion: i = " << i << ", k = " << k
           << ", change = " << current_change << endl;
#endif
      if ( current_change < change_in_crossings ) {
        change_in_crossings = current_change;
        retval = k;
      }
    }
  }
  return retval;
}

int
BiGraph::best_swap(int i) const
{
  // NOT IMPLEMENTED
  return i;
}

// used only for tracing and debugging
ostream& operator << (ostream& out, const BiGraph & bg)
{
  out << "(";
  for ( int i = 0; i < bg.my_graph->my_n0; i++ ) {
    out << *bg.my_graph->my_V0[i];
  }
  out << ")" << endl;
  out << "V0 order = ";
  for ( int i = 0; i < bg.my_graph->my_n0; i++ ) {
    out << bg.get_name(i) << " ";
  }
  
  return out;
}

//  [Last modified: 2007 08 08 at 18:38:55 GMT]
