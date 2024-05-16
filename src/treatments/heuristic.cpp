/// @file heuristic.cpp
/// @brief Implements heuristics for edge crossings.
/// @author Matt Stallmann
/// @date 21 Aug 1998
/// $Id: heuristic.cpp 75 2015-05-17 19:54:36Z mfms $
/// Treatments:
///
/// - treatment 1: median, no preprocessing
/// - treatment 3: barycenter, no preprocessing 
/// - treatment 4: adaptive insertion, no preprocessing
/// - treatment 6: breadth first search 
/// - treatment 7: breadth first search + median
/// - treatment 9: breadth first search + barycenter
/// - treatment 14: guided breadth-first search
/// - treatment 15: guided breadth-first search + median
/// - treatment 17: guided breadth-first search + barycenter
/// - treatment 23: guided breadth-first search + adaptive insertion/barycenter 
/// - treatment 24: median, stop when no improvement, unstable sort 
/// - treatment 25: barycenter, stop when no improvement, unstable sort 
/// - treatment 26: median, stop when no improvement, stable sort
/// - treatment 27: barycenter, stop when no improvement, stable sort
/// - treatment 28: true median, stop when no improvement, unstable sort
/// - treatment 30: true median, stop when no improvement, unstable sort
///
/// True median means use the average of the two middle elements when there
/// is an even number.  The other median heuristics use the smaller of the
/// two elements, but ensure that the median of an even set is treated as
/// larger than that of an odd set if the two happen to be equal.

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

//  6 Jan 1999 - modified to handle ordering info from a separate file and
//               to take flags from the command line 
//  8 Jan 1999 - modified so that only valid ALENEX treatments can be
//               executed and a .ord file is required
// 23 Jan 1999 - added capability to gather intermediate data by setting
//               either the CURVE or TRACK_ITERATIONS compile-time flag
// 25 Jan 1999 - added ability to gather timing info on iterative
//               improvement by setting the TIMING compile-time flag
//               (average time per iteration is printed)
// 31 May 1999 - converted so that .itr file shows total iterations so far
//               instead of iterations without improvement 
// 26 Jun 1999 - added ASSUME_CONVERGENCE option (so that averaging
//               heuristics can be allowed to run until they converge) 
// 29 Jun 1999 - fixed bug that caused averaging heuristics not to save the
//               most recent copy unless tracking iterations 
// 17 Sep 1999 - fixed it so that number of iterations defaults to 1024 and 
//               the averaging heuristics pay attention to the -iterations
//               flag
//  9 Nov 1999 - changed it so that treatments 5, 11, and 19 use pure
//               barycenter instead of a mix of median and barycenter
// 12 Mar 2001 - fixed to allow several ways to stop iterating
// 13 Mar 2001 - added ability to choose between stable and unstable sort
//               (and added new treatments for presumed median/barycenter
//               algorithms reported by Juenger and Mutzel)
// 14 Mar 2001 - added treatments 26, 27, 28, and 30 (to deal with all
//               variations of median and barycenter with stable and
//               unstable sorts)
// 17 Mar 2001 - fixed timing so that both total time and time per
//               iteration are output when timing iterative improvement
// 22 May 2001 - fixed bug that omitted printout for TRACK_ITERATIONS when
//               UNTIL_CONVERGE was the termination condition
// 24 Jul 2007 - created a simpler interface for iterative improvement
//               heuristics based on abstract classes

// Usage: argv[0] -tr=N [-iterations=I] graph.dot order.ord
//        currently, argv[0] is unfold2 or exp2 (the latter keeps track of
//        iterations that lead to improvement).

using namespace std;

#include<iostream>
#include<iomanip>
#include<fstream>
#include<string>
#include<sstream>
#include<assert.h>
#include<stdlib.h>              // exit(), abort()

#include"defs.h"
#include"BiGraph.hxx"
#include"dot.h"
#include"plain_bfs.h"
#include"bfs_heuristic.h"
#include"IterativeHeuristic.h"
#include"Median.h"
#include"Barycenter.h"
#include"AdaptiveInsertion.h"
#include"CmdLine.h"

#ifdef TIMING
#include"Timer.hxx"
#endif

/// Maximum iterations for adaptive insertion
const unsigned int ADAPTIVE_INSERTION_ITERATIONS = 2048;

void iterate( IterativeHeuristic & iterative_improvement );

int main(int argc, char ** argv)
{
  CmdLine arguments( argc, argv );
  string_list files = arguments.getFiles();
  
  if ( files.size() != 3 || ! arguments.flagPresent( "tr" ) ) {
    cerr << "Usage: " << files[0]
         << " -tr=N [-iterations=I] graph.dot order.ord" << endl;
    exit(1);
  }

  /// initial ordering (none, bfs, or GBFS)
  string preprocessing = "";
  /// iterative improvement (none, med/bary, adapt. ins., adapt. ins. +
  /// barycenter
  string iterative_improvement = "";
  /// whether to use a stable sort for median/barycenter (unstable may give
  /// better results)
  bool stable_sort = true;
  /// whether to iterate only until there is no improvement (alternative is to
  /// keep iterating until there is no change at all); waiting until there is
  /// no change at all is more efficient because there is no need to perform
  /// the costly crossing number calculation after each iteration.
  bool terminate_when_no_improvement = false;
  /// Whether to use the "true" median
  bool true_median = false;

  // use "tr" flag to get treatment number
  const int treatment = arguments.intFlag( "tr" );

  // select various options based on treatment number
  switch ( treatment ) {
  case 0:                       // "placebo" - do nothing
  break;

  case 1:                       // median (stable sort, wait for convergence)
  iterative_improvement = "median";
  break;

  case 3:                       // barycenter (stable sort, convergence)
  iterative_improvement = "barycenter";
  break;

  case 4:                       // adaptive insertion
  iterative_improvement = "adaptive_insertion";
  break;

  case 6:                       // breadth-first search only
  preprocessing = "bfs";  
  break;

  case 7:                       // bfs + median
  preprocessing = "bfs";
  iterative_improvement = "median";
  break;

  case 9:                       // bfs + barycenter
  preprocessing = "bfs";
  iterative_improvement = "barycenter";
  break;

  case 14:                      // guided breadth-first search
  preprocessing = "guided_bfs";
  break;

  case 15:                      // gbfs + median
  preprocessing = "guided_bfs";
  iterative_improvement = "median";
  break;

  case 17:                      // gbfs + barycenter
  preprocessing = "guided_bfs";
  iterative_improvement = "barycenter";
  break;

  case 23:
  preprocessing = "guided_bfs";
  iterative_improvement = "combined";
  break;

  case 24:                      // median, unstable, separate even & odd
  iterative_improvement = "median";
  stable_sort = false;
  terminate_when_no_improvement = true;
  break;
  
  case 25:                      // barycenter, unstable
  iterative_improvement = "barycenter"; 
  stable_sort = false;
  terminate_when_no_improvement = true;
  break;
  
  case 26:                      // median, stable, separate even & odd
  iterative_improvement = "median";
  terminate_when_no_improvement = true;
  break;

  case 27:                      // barycenter, stable
  iterative_improvement = "barycenter"; 
  terminate_when_no_improvement = true;
  break;

  case 28:                      // median, unstable, average two middles
  iterative_improvement = "median";
  stable_sort = false;
  true_median = true;
  terminate_when_no_improvement = true;
  break;
  
  case 30:                      // median, stable, average two middles
  iterative_improvement = "median";
  true_median = true;
  terminate_when_no_improvement = true;
  break;
 
  default:
  cerr << treatment << " is not a valid treatment number." << endl;
  cerr << "Valid numbers are 1,3,6,7,9,14,15,17,23, 24-27, 28, and 30."
  << endl;
  exit(7);

  } // end, switch

  // check for iterations if specified on command line
  unsigned int adaptive_insertion_iterations;
  if ( arguments.flagPresent( "iterations" ) )
  adaptive_insertion_iterations = arguments.intFlag("iterations");
  else adaptive_insertion_iterations =  ADAPTIVE_INSERTION_ITERATIONS;

  // get input file name from file list and open it
  // (graph_name = input_file_name w/o the .dot extension)
  if ( files[1].length() == 0 ) {
    cerr << "Bad command-line option (a - with white space after it)" <<
     endl;
    exit(2);
  }

  string input_file_name = files[1];
  size_t position = input_file_name.rfind(".dot");
  if ( position == string::npos ) {
    cerr << "Bad .dot file name (expected a .dot extension)" << endl;
    exit(3);
  } 
  string graph_name = input_file_name.substr(0, position);

  ifstream input_file;
  input_file.open(input_file_name.c_str());
  if ( ! input_file ) {
    cerr << "Unable to open file " << input_file_name << " for input" << endl;
    exit(4);
  }

  // get .ord file name from file queue and open it
  ifstream ord_file;
  string ord_file_name = files[2];

  ord_file.open(ord_file_name.c_str());
  if ( ! ord_file ) {
    cerr << "Unable to open file " << ord_file_name << " for order" << endl;
    exit(5);
  }

  // read graph in dot format from file
  BiGraph bg;
  bg.read_with_order( input_file, ord_file );
  ord_file.close();
  input_file.close();

  if ( 0 == bg.n0() || 0 == bg.n1() ) {
    cerr << "Input error results in empty graph, file "
         << input_file_name << endl;
    exit(6);
  }

#ifdef TRACE
  cerr << "Graph = " << bg << endl;
  cerr << "Crossings = " << bg.crossings() << endl;
#endif

  // create a filename suffix from the treatment number
  ostringstream tr_stream;
  tr_stream << setfill('0') << setw(TREATMENT_FIELD_WIDTH) << treatment;
  const string TN = tr_stream.str();
  string treatment_name = TREATMENT_PREFIX + TN;

  if ( preprocessing == "bfs" )
    plain_bfs(bg);
  else if ( preprocessing == "guided_bfs" )
    bfs_heuristic(bg);

#ifdef TRACK_ITERATIONS
  string iteration_file_name = graph_name + treatment_name + ".itr";
  ofstream iteration_file(iteration_file_name.c_str());
  assert( iteration_file );
#endif

#ifdef CURVE
  string curve_file_name = graph_name + treatment_name + ".crv";
  ofstream curve_file(curve_file_name.c_str());
  assert( curve_file );
#endif

  // !!! int new_crossings = bg.crossings();

#ifdef TRACE
  cerr << "After preprocessing," << endl << "Graph = " << bg << endl;
  cerr << "Crossings = " << new_crossings << endl;
#endif

#ifdef CURVE
  curve_file << new_crossings << endl;
#endif
#ifdef TRACK_ITERATIONS
  iteration_file << 0 << "\t" << new_crossings << endl;
#endif

#ifdef TIMING
  Timer time;
  double total_time = 0.0;
  int number_of_iterations = 0;
  time.start();
#endif

  if( iterative_improvement == "median" ) {
    Median median(bg, stable_sort,
                  terminate_when_no_improvement, true_median);
    iterate(median);
  }
  else if( iterative_improvement == "barycenter" ) {
    Barycenter barycenter(bg, stable_sort, terminate_when_no_improvement);
    iterate(barycenter);
  }
  else if( iterative_improvement == "adaptive_insertion" ) {
    AdaptiveInsertion adaptive_insertion(bg, adaptive_insertion_iterations);
    iterate(adaptive_insertion);
  }
  else if( iterative_improvement == "combined" ) {
    bool improvement = true;
    while( improvement ) {
      AdaptiveInsertion adaptive_insertion(bg, adaptive_insertion_iterations);
      Barycenter barycenter(bg, stable_sort, terminate_when_no_improvement);
      unsigned long long crossings = bg.crossings();
      iterate(adaptive_insertion);
      iterate(barycenter);
      improvement = bg.crossings() < crossings;
    }
  }

#if 0
  if ( POSTP > 0 ) {
    int total_iterations = 0;
    bool change = false;     // "pure" averaging stops when there's no change

    do {
      // set (or select) iterative improvement heuristics to perform
      enum { INS = 0, AVG = 1 } begin_heuristic, end_heuristic;
      if ( POSTP & 2 ) begin_heuristic = INS;
      else begin_heuristic = AVG;
      if ( POSTP & 1 ) end_heuristic = AVG;
      else end_heuristic = INS;

      if ( termination_policy != UNTIL_MAX )
        change = false;

      ++total_iterations;
      for ( int heuristic = begin_heuristic;
            heuristic <= end_heuristic; ++heuristic ) {
      
        // apply the current heuristic to both sides
        for ( int side = 0; side < 2; ++side ) {
          bg.switch_sides();
          if ( AVG == heuristic ) {
            // just for testing the new code organization
            bool local_change = false;
            if( POSTP == 1 ) {
              /// @todo - make the iterative improvement into a separate
              /// method or class so that the abstract class is actually
              /// useful; also get rid of the global RATIO
              AvgHeuristic * avg_heur = NULL;
              if( RATIO == 0.0 )
                avg_heur = new Median(bg);
              else
                avg_heur = new Barycenter(bg);
              avg_heur->setStable();
              avg_heur->runHeuristic();
              local_change = avg_heur->orderHasChanged();
              delete avg_heur;
            }
            if ( termination_policy == UNTIL_CONVERGE ) {
              change = change || local_change;

#ifdef TRACK_ITERATIONS
#define UPDATE_CROSSINGS
#endif
#ifdef CURVE
#define UPDATE_CROSSINGS
#endif
#ifdef TRACE
#define UPDATE_CROSSINGS
#endif
#ifdef UPDATE_CROSSINGS 
              new_crossings = bg.crossings();
#endif

            }
            else {
              new_crossings = bg.crossings();
              change = ( new_crossings < best_crossings );
            }
          }
          else if ( INS == heuristic ) {
            new_crossings += adaptive_insert( bg );
          }
          else {
            cerr << "Bad iterative improvement heuristic: " << heuristic
             << endl;
            abort();
          }

#ifdef TRACE
          cerr << "    " << (side ? "__" : "^^") << " graph = " 
           << bg << endl;
          cerr << "    crossings = " << new_crossings << endl;
#endif
#ifdef CURVE
          curve_file << new_crossings << endl;
#endif
#ifdef TRACK_ITERATIONS
          if ( new_crossings < best_crossings
               || termination_policy == UNTIL_CONVERGE ) {
            iteration_file << total_iterations << "\t"
                           << new_crossings << endl;
          }
#endif
          // check to see if this is an improvement on the best so far
          // (not necessary for averaging heuristics that presumably
          // converge - BUG FIX 29 Jun 1999) 
          if ( new_crossings < best_crossings
               && termination_policy != UNTIL_CONVERGE ) {
            best_crossings = new_crossings;
            best_copy = bg;
          }
        }
      }
#ifndef ASSUME_CONVERGENCE
      if ( termination_policy == UNTIL_CONVERGE
           && total_iterations > MAX_ITERATIONS ) {
        cerr << "Treatment " << treatment << " failed to converge on graph "
         << graph_name << endl;
        cerr << "Number of iterations = " << total_iterations << endl;
        cerr << "Current crossing no. = " << bg.crossings() << endl;
        break;
      }
#endif
    } while ( ( termination_policy == UNTIL_MAX
                && total_iterations <= MAX_ITERATIONS )
              || change );

    // haven't kept track of best copy if waiting for convergence
    if ( termination_policy == UNTIL_CONVERGE ) {
      best_copy = bg;
    }

#ifdef TIMING
    time.stop();
    total_time = time.seconds();
    number_of_iterations = total_iterations;
#endif

  }                             // if ( POSTP > 0 )
#endif // if 0

#ifdef TIMING
  cout << graph_name << "\t" << total_time << "\t" << number_of_iterations
       << endl;
#else  // not timing
  // output the result in ord format (in an appropriately named file)
  string output_file_name = graph_name + treatment_name + ORD_SUFFIX;
  ofstream output_file;
  output_file.open(output_file_name.c_str());
  assert( output_file );

  // !!! best_copy is relevant for adaptive insertion
  //best_copy.output_order( output_file, graph_name, TN );
  bg.output_order( output_file, graph_name, TN );
  output_file.close();
#endif // not timing

#ifdef OUTPUT_CROSSING_NUMBER
  cout << graph_name << "\t" << treatment
       << "\t" << best_copy.crossings() << endl;
#endif

#ifdef CURVE
  curve_file.close();
#endif

#ifdef TRACK_ITERATIONS
  iteration_file.close();
#endif
}

void iterate( IterativeHeuristic & iterative_improvement ) {
  bool keep_going = true;
  while( keep_going ) {
    iterative_improvement.runHeuristic();
    keep_going = iterative_improvement.keepGoing();
  }
}

//  [Last modified: 2015 05 17 at 19:47:58 GMT]
