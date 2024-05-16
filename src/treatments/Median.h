/// @file Median.h
/// @brief Class that implements the median heuristic
/// @date 2007/07/03
/// $Id: Median.h 51 2007-07-28 01:35:34Z mfms $

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

#ifndef MEDIAN_H
#define MEDIAN_H

using namespace std;

#include"SortingHeuristic.h"
#include"IntSet.h"

class Median: public SortingHeuristic {
protected:
  /// Use average of two middle elements instead of smaller with a
  /// tie-breaker that makes even sets greater than odd sets.
  bool use_true_median;

  virtual bool sortNodes() {
    /// @todo The placement of ratio in IntSet is awkward.  Perhaps it makes
    /// sense to get rid of in-between ratios entirely, and let the averaging
    /// method for IntSet have a parameter that determines the proper value.
    /// Or even two separate methods!
    IntSet::setRatio(0.0);

    if( use_true_median ) {
      IntSet::setTrueMedian();
    }
    return SortingHeuristic::sortNodes();
  }

public:
  /// Create an instance to work on the given BiGraph
  Median(BiGraph & bigraph, bool stable, bool no_improvement,
         bool true_median): SortingHeuristic(bigraph, stable, no_improvement),
                            use_true_median(true_median) { }
};

#endif

// Local Variables: ***
//  mode:c++ ***
// End: ***

//  [Last modified: 2007 07 28 at 01:35:02 GMT]
