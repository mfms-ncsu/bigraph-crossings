/// @file SortingHeuristic.h
/// @brief Base class for all the averaging heuristics for edge crossing
/// minimization, e.g., median and barycenter; simplifies the control
/// @author Matt Stallmann
/// @date 2007/07/27
/// $Id: SortingHeuristic.h 57 2007-08-01 20:44:54Z mfms $

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

#ifndef SORTINGHEURISTIC_H
#define SORTINGHEURISTIC_H

using namespace std;

#include"BiGraph.hxx"
#include"IterativeHeuristic.h"

class SortingHeuristic: public IterativeHeuristic {
protected:
  /// Determine whether the sort will be stable
  bool stable_sort;
  /// Stop iterating when number of crossings fails to decrease, as opposed
  /// to continuing until there is no change.
  bool stop_when_no_improvement;
  /// true if the order has changed during the most recent iteration
  bool order_has_changed;
  /// Keeps track of crossing number before the most recent iteration
  unsigned long long crossings_before;

  /// Generic method to sort nodes using appropriate criteria (stability, for
  /// example).  Virtual because subclasses may want to set different
  /// parameters (e.g., ratio and true_median)
  ///
  /// Return true if the order has changed.
  virtual bool sortNodes();

public:
  /// Create an instance to work on the given BiGraph
  SortingHeuristic(BiGraph & bigraph, bool stable, bool no_improvement):
    IterativeHeuristic(bigraph), stable_sort(stable),
    stop_when_no_improvement(no_improvement) { }

  /// Run the heuristic once for each side
  virtual void runHeuristic();

  /// Return true if it's appropriate to keep going, based on the termination
  /// criterion
  virtual bool keepGoing() const;

  /// Make the sorting unstable (might be more efficient and give better
  /// results)
  void makeUnstable() { stable_sort = false; }

  /// Make the sorting stable (default)
  void makeStable() { stable_sort = true; }

  /// Ask heuristic to terminate when there are no improvements
  void stopWhenNoImprovement() { stop_when_no_improvement = true; }

  /// Ask heuristic to continue until there are no changes (the default)
  void stopWhenNoChanges() { stop_when_no_improvement = false; }
  
  /// Needed in case a subclass has a destructor
  virtual ~SortingHeuristic() {}
};

#endif

// Local Variables: ***
//  mode:c++ ***
// End: ***

//  [Last modified: 2007 08 01 at 20:23:06 GMT]
