/// @file Barycenter.h
/// @brief Class that implements the barycenter heuristic
/// @date 2007/07/03
/// $Id: Barycenter.h 65 2007-08-08 18:45:24Z mfms $

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

#ifndef BARYCENTER_H
#define BARYCENTER_H

using namespace std;

#include"SortingHeuristic.h"
#include"IntSet.h"

class Barycenter: public SortingHeuristic {
protected:
  virtual bool sortNodes() {
    /// @todo The placement of ratio in IntSet is awkward.  Perhaps it makes
    /// sense to get rid of in-between ratios entirely, and let the averaging
    /// method for IntSet have a parameter that determines the proper value.
    /// Or even two separate methods!
    IntSet::setRatio(1.0);
    return SortingHeuristic::sortNodes();
  }

public:
  /// Create an instance to work on the given BiGraph
  Barycenter(BiGraph & bigraph, bool stable, bool no_improvement):
    SortingHeuristic(bigraph, stable, no_improvement) { }
};

#endif

// Local Variables: ***
//  mode:c++ ***
// End: ***

//  [Last modified: 2007 08 08 at 18:07:42 GMT]
