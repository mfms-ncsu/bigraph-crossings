/// @file SortingHeuristic.cpp
/// @brief Base class for all the averaging heuristics for edge crossing
/// minimization, e.g., median and barycenter.
/// @author Matt Stallmann
/// @date 2007/07/27
/// $Id: SortingHeuristic.cpp 52 2007-07-28 01:42:14Z mfms $

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

#include<float.h>               // DBL_MAX
#include<assert.h>
#include"SortingHeuristic.h"
#include"IntSet.h"

bool SortingHeuristic::sortNodes() {
  // the value to be returned; will be made true as soon as a change is
  // detected
  bool order_changed = false;

  // Create an auxiliary array for the values on which the nodes will be
  // sorted.
  double * keys = new double[myBigraph.n0() + 1];
  for ( int i = 0; i < myBigraph.n0(); i++ )
    keys[i] = myBigraph.average(i);
  keys[myBigraph.n0()] = DBL_MAX;  // sentinel value

  // sort by keys (insertion sort)
  for ( int i = myBigraph.n0() - 2; i >= 0; --i ) {
    double to_be_inserted = keys[i];
    int j = i;
    if ( stable_sort ) {
      while ( to_be_inserted > keys[j + 1] ) {
        order_changed = true;
        keys[j] = keys[j + 1];
        ++j;
      }
    }
    else { // unstable sort
      while ( to_be_inserted >= keys[j + 1] ) {
        if( to_be_inserted > keys[j + 1] ) order_changed = true;
        keys[j] = keys[j + 1];
        ++j;
      }
    }
    keys[j] = to_be_inserted;
    myBigraph.insert(i, j);
  }
  delete [] keys;
  return order_changed;
}

void SortingHeuristic::runHeuristic() {
  // an unstable sort may not lead to termination if we wait until there is
  // no change in order
  assert( stable_sort || stop_when_no_improvement );

  if( stop_when_no_improvement )
    crossings_before = myBigraph.crossings();
  myBigraph.switch_sides();
  bool order_changed_side_one = sortNodes();
  myBigraph.switch_sides();
  bool order_changed_side_two = sortNodes();
  order_has_changed = order_changed_side_one || order_changed_side_two;
}

bool SortingHeuristic::keepGoing() const {
  if( stop_when_no_improvement )
    return myBigraph.crossings() < crossings_before;
  else
    return order_has_changed;
}

//  [Last modified: 2007 07 28 at 01:40:25 GMT]
