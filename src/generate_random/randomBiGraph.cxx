/// @file randomBiGraph.cxx
/// @brief program to generate a random connected bipartite
///              graph with a given number of nodes specified for
///              each side and a given number of edges
/// @author Matt Stallmann @date 24 Jun 1999
/// $Id: randomBiGraph.cxx 68 2007-08-08 20:07:39Z mfms $

// 28 Jun 1999 - changed to use of flags
// 22 Nov 1999 - fixed a bug in the generation of 2D trees
// 24 Nov 1999 - added ability to generate trees with dimension > 1 and <
//               2, where dimension 1 + r means that points are chosen in a 
//               rectangle with aspect ratio r
// 05 Mar 2001 - added ability to generate random graphs that are not
//               necessarily connected
// 03 Apr 2001 - added a test to make sure not too many edges were being
//               asked for (and issued a warning)
// 20 Jan 2003 - added 'return 0' at the end of main to ensure that 0 is
//               returned for normal execution (otherwise the perl scripts
//               that rely on this might screw up) 
// 13 Apr 2006 - replaced strstream with stringstream
// 23 Jul 2007 - replaced CmdLine with updated version

// INPUT (as command line flags appearing in any order):
//     -n0=<number of nodes on layer 0>
//     -n1=<number of nodes on layer 1> (defaults to n0)
//     -m=<number of edges>             (defaults to n0 + n1 - 1)
//     -name=<name of class>
//     -N=<number of "mutants">         (defaults to 32)
//     -seed=<name of seed file>        (defaults to "../seed")
//     -c=<connectedness>               (0 means not necessarily connected,
//                                       -1 means each node has a neighbor,
//                                       1, the default, means connected)
//     -d=<dimension of spanning tree>  (0, the default,
//                                          means completely random,
//                                       1 means based on nearest neighbors
//                                         on a line segment => comb
//                                       2 means nearest neighbor in a
//                                         square => variance in degree is
//                                         less than for 0;
//                                       1 + r means nearest neighbor in a
//                                             rectangle with aspect ratio r)

// OUTPUT: a bipartite graph described in a file in DOT format and an
//         accompanying file in ord format with a .ORD extension (to
//         distiguish it from heursitic generated .ord files) 

using namespace std;

#include<string>
#include<iostream>
#include<fstream>
#include<iomanip>
#include<sstream>
#include<stdlib.h>
#include<limits.h>
#include<assert.h>
#include"defs.h"
#include"dot.h"
#include"ord.h"
#include"Rand.hxx"
#include"Graph.hxx"
#include"IntList.hxx"
#include"IntIterator.hxx"
#include"CmdLine.h"

void BiPrim( Graph & G, const int * X, const int * Y, int n0, int n1,
             Rand & rstream );
// PRE: G is an empty graph with n0 + n1 nodes; X and Y are vectors of
//      length n0 + n1 giving coordinates for each node of G, or they may
//      be null pointers
// POST: G is a minimum spanning tree on the complete graph whose nodes are
//       at the given coordinates using the infinity norm (1D if Y = 0, or
//       use the rstream to generate completely random weights if both X
//       and Y are 0).

void add_random_edges( Graph & G, int n0, int num_edges,
                       Rand & random_stream );
// POST: G is what it was initially, but with the given number of random
//       edges added (the added edges all go from nodes < n0 to nodes >=
//       n0).

void print_dot_output( ostream & out, const char * graph_name,
                       const char * seed_info,
                       const Graph & G, int n0 );
// PRE: G is a non-empty graph && 0 < n0 <= G.number_of_nodes()
// POST: G has been inserted into out in DOT format

void print_ord_output( ostream & out, const char * graph_name,
                       const char * seed_info,
                       const Graph & G, int n0 );
// PRE: G is a non-empty graph && 0 < n0 <= G.number_of_nodes()
// POST: the nodes of G have been inserted into out in ORD format

int main( int argc, char * argv [] )
{
  if ( argc < 2 ) {
    cerr << "Usage: " << argv[0] << endl
<< "     -n0=<number of nodes on layer 0>" << endl
<< "     -n1=<number of nodes on layer 1> (defaults to n0)" << endl
<< "     -m=<number of edges>             (defaults to n0 + n1 - 1)" << endl
<< "     -name=<name of random class being created>" << endl
<< "     -N=<number of class members>     (defaults to 32)" << endl
<< "     -seed=<name of seed file>        (defaults to ../seed)" << endl
<< "     -c=<connectedness>               (0 means not necessarily connected,"
<< endl
<< "                                       -1 means each node has a neighbor,"
<< endl
<< "                                       1, the default, means connected)"
<< endl
<< "     -d=<dimension of spanning tree>  (0, the default, means completely random"
<< endl
<< "                                       1 means based on nearest neighbors" 
<< endl
<< "                                         on a line segment => comb"
<< endl
<< "                                       1 + r, where 0 < r <= 1.0, means"
<< endl
<< "                                         nearest neighbor in a"
<< endl
<< "                                         rectangle with aspect ratio r)"
<< endl;
    exit( 1 );
  }

  CmdLine args( argc, argv );
  int n0;
  if ( args.flagPresent( "n0" ) )
   n0 = args.intFlag( "n0" );
  else {
    cerr << "Need to specify number of layer 0 nodes by -n0=??" << endl;
    exit( 2 );
  }
  int n1 = args.flagPresent( "n1" ) ? args.intFlag( "n1" ) : n0;
  int m = args.flagPresent( "m" ) ? args.intFlag( "m" ) : n0 + n1 - 1;
  int number_of_subjects = args.flagPresent( "N" ) ? args.intFlag( "N" )
   : DEFAULT_CLASS_SIZE;

  if ( n0 <= 0 || n1 <= 0 || m <= 0 ) {
    cerr << "Bad graph parameters: n0 = " << n0 << ", n1 = " << n1
     << ", m = " << m << endl;
    exit( 3 );
  }

  int connectedness = args.flagPresent( "c" ) ? args.intFlag( "c" ) : 1;
  if ( connectedness != 0 && connectedness != 1 ) {
    cerr << "Warning: connectedness " << connectedness
         << " not implemented, setting to default 1." <<
     endl;
    connectedness = 1;
  }
  if ( connectedness == 1 && m < n0 + n1 - 1 ) {
    cerr << "Warning: asking for a connected graph with " << (n0 + n1)
         << " nodes and " << m << " edges." << endl;
    cerr << "Setting conectedness to 0." << endl;
    connectedness = 0;
  }
  if ( m > n0 * n1 ) {
    cerr << "Warning: asking for more edges than are possible with "
         << "n0 = " << n0 << ", n1 = " << n1 << endl;
    cerr << "Original value of m = " << m << ", setting m to " << n0 * n1
         << endl;
    m = n0 * n1;
  }
  
  double dimension = args.flagPresent( "d" ) ? args.doubleFlag( "d" ) : 0.0;
  if ( dimension > 2.0 ) {
    cerr << "Warning: dimension > 2 not implemented, setting to 2." <<
     endl;
    dimension = 2.0;
  }

  string class_name;
  if ( args.flagPresent( "name" ) )
    class_name = args.stringFlag( "name" );
  else {
    cerr << "Need to specify a name for this class by -name=??" << endl;
    exit( 4 );
  }
  string seed_file = args.flagPresent( "seed" ) ?
    args.stringFlag( "seed" ) : "../seed";
  ifstream input_random_seed( seed_file.c_str() );
  if ( ! input_random_seed ) {
    cerr << "Unable to open seed file " << seed_file << " for input." <<
     endl;
    exit( 5 );
  }

  Rand random_stream( input_random_seed );
  input_random_seed.close();
  
  for ( int subject = 1; subject <= number_of_subjects; ++subject ) {
    ostringstream base_name;
    base_name << class_name << "_" << setfill( '0' )
     << setw( MUTANT_FIELD_WIDTH ) << subject;

    string dot_file_name = base_name.str() + ".dot";
    ofstream dot_output( dot_file_name.c_str() );
    if ( ! dot_output ) {
      cerr << "Unable to open file " << dot_file_name << " for output."
       << endl;
      exit( 6 );
    }

    string ord_file_name = dot_file_name + ".ORD";
    ofstream ord_output( ord_file_name.c_str() );
    if ( ! ord_output ) {
      cerr << "Unable to open file " << ord_file_name << " for output."
       << endl;
      exit( 7 );
    }

    Rand initial_random_stream = random_stream;

    // give each node a location in one or two dimensions, if appropriate
    Graph G( n0 + n1 );
    int * X = 0;
    int * Y = 0;
    if ( connectedness == 1 && dimension > 0 ) {
      X = new int[ n0 + n1 ];
      for ( int i = 0; i < n0 + n1; ++i )
       X[ i ] = random_stream;
      if ( dimension > 1 ) {
        Y = new int[ n0 + n1 ];
        for ( int i = 0; i < n0 + n1; ++i )
         Y[ i ] = static_cast<int>( (dimension - 1.0) * random_stream );
      }
    }

    // compute a minimum spanning tree using "infinity" norm (if appropriate)
    int edges_to_add = m;
    if ( connectedness == 1 ) {
      BiPrim( G, X, Y, n0, n1, random_stream );
      edges_to_add -= n0 + n1 - 1;
    }

    delete [] X;
    delete [] Y;

    // add other random edges
    add_random_edges( G, n0, edges_to_add, random_stream );

    // print output files
    ostringstream seed;
    seed << initial_random_stream;
    print_dot_output( dot_output, base_name.str().c_str(),
                      seed.str().c_str(), G, n0 );
    print_ord_output( ord_output, base_name.str().c_str(),
                      seed.str().c_str(), G, n0 );
  }

  // rewrite random seed
  ofstream output_random_seed( seed_file.c_str() );
  if ( ! output_random_seed ) {
    cerr << "Warning: random seed was not updated!" << endl;
  }
  else {
    output_random_seed << random_stream << endl;
  }
  return 0;
}

int weight( int i, int j, const int * X, const int * Y, Rand & rstream )
// return the distance from node i to node j in the infinity norm (to
// assure uniformity of distances)
{
  int xdiff = X ? abs( X[ i ] - X[ j ] ) : -1;
  int ydiff = Y ? abs( Y[ i ] - Y[ j ] ) : -1;
  int distance = (xdiff > ydiff) ? xdiff : ydiff;
  if ( -1 == distance ) distance = rstream;
  return distance;
}

void BiPrim( Graph & G, const int * X, const int * Y, int n0, int n1,
             Rand & rstream )
{
  assert( G.number_of_nodes() == n0 + n1 );
  assert( G.number_of_edges() == 0 );

  int edges_in_tree = 0;
  enum { UNSEEN, FRINGE, TREE }; // for use in status array
  int * status = new int[ n0 + n1 ];
  for ( int i = 0; i < n0 + n1; ++i ) status[ i ] = UNSEEN;
  IntList fringe;
  int * parent = new int[ n0 + n1 ]; // parent of this node - valid only
                                     // for fringe nodes
  int * cost = new int[ n0 + n1 ];   // weight of lowest weight edge to
                                     // this fringe node
                                     
  // start at node 0
  int current_node = 0;
  status[ 0 ] = TREE;

  while ( edges_in_tree < n0 + n1 - 1 ) {
    // update neighbors of current_node on other layer
    int start = current_node < n0 ? n0 : 0;
    int stop = current_node < n0 ? n0 + n1 : n0;
    for ( int other_node = start; other_node < stop; ++other_node ) {
      if ( FRINGE == status[ other_node ] ) {
        int new_cost = weight( current_node, other_node, X, Y, rstream );
        if ( new_cost < cost[ other_node ] ) {
          cost[ other_node ] = new_cost;
          parent[ other_node ] = current_node;
        }
      }
      else if ( UNSEEN == status[ other_node ] ) {
        status[ other_node ] = FRINGE;
        fringe.add( other_node );
        cost[ other_node ] = weight( current_node, other_node, X, Y, rstream );
        parent[ other_node ] = current_node;
      }
    }

    // find lowest cost node on the fringe and make it the new current_node
    assert( ! fringe.isEmpty() );
    int min_cost = INT_MAX;
    IntIterator fringe_itr = fringe;
    while ( fringe_itr.more() ) {
      int node = fringe_itr.next();
      if ( cost[ node ] < min_cost ) {
        min_cost = cost[ node ];
        current_node = node;
      }
    }

    // add the edge from current_node to its parent to the MST
    fringe.remove( current_node );
    status[ current_node ] = TREE;
    G.add_edge( current_node, parent[ current_node ] );
    ++edges_in_tree;
  }
  delete [] status;
  delete [] parent;
  delete [] cost;
}

void add_random_edges( Graph & G, int n0, int num_edges,
                       Rand & random_stream )
{
  int n1 = G.number_of_nodes() - n0;

  int potential_edges = n0 * n1 - G.number_of_edges();
  // number of edges that could still be added randomly
  int edges_remaining = num_edges;
  // number of edges that still need to be added randomly

  // keep adding edges with adjusted probability until no more edges remain
  // to be added
  for ( int i = 0; i < n0; i++ )
   for ( int j = n0; j < n0 + n1; j++ )
    if ( ! G.is_edge( i, j ) ) {
      if ( random_stream.boolean(double(edges_remaining) / potential_edges) ) {
        G.add_edge(i, j);
        --edges_remaining;
      }
      --potential_edges;
    }
}

void print_dot_output( ostream & out, const char * graph_name,
                       const char * seed_info,
                       const Graph & G, int n0 )
{
  dot_preamble( out, graph_name, seed_info );
  for ( int layer0_node = 0; layer0_node < n0; ++layer0_node ) {
    EdgeList edges( G, layer0_node );
    while ( edges.more() ) {
      edges.next();
      int layer1_node = edges.head() - n0;
      ostringstream layer0_name, layer1_name;
      layer0_name << "a" << setfill( '0' ) << setw( NODE_FIELD_WIDTH ) <<
        layer0_node;
      layer1_name << "b" << setfill( '0' ) << setw( NODE_FIELD_WIDTH ) <<
       layer1_node;
      output_edge( out, layer0_name.str().c_str(), layer1_name.str().c_str() );
    }
  }
  end_dot( out );
}

void print_ord_output( ostream & out, const char * graph_name,
                       const char * seed_info,
                       const Graph & G, int n0 )
{
  ord_preamble( out, graph_name, seed_info, true );
  begin_layer( out, 0 );
  for ( int i = 0; i < n0; ++i ) {
    ostringstream layer0_name;
    layer0_name << "a" << setfill( '0' ) << setw( NODE_FIELD_WIDTH ) << i;
    output_node( out, layer0_name.str().c_str() );
  }
  end_layer( out );

  begin_layer( out, 1 );
  for ( int i = 0; i < G.number_of_nodes() - n0; ++i ) {
    ostringstream layer1_name;
    layer1_name << "b" << setfill( '0' ) << setw( NODE_FIELD_WIDTH ) << i;
    output_node( out, layer1_name.str().c_str() );
  }
  end_layer( out );
}

//  [Last modified: 2015 05 17 at 22:44:45 GMT]
