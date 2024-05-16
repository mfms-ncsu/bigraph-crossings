/// @file AdaptiveInsertion.h
/// @brief Class that encapsulates the adaptive insertion heuristic.
/// @author Matt Stallmann
/// @date 2007/07/27
/// $Id: AdaptiveInsertion.h 58 2007-08-02 22:00:14Z mfms $

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

#ifndef ADAPTIVEINSERTION_H
#define ADAPTIVEINSERTION_H

using namespace std;

#include"BiGraph.hxx"
#include"IterativeHeuristic.h"
#include"adaptive_insert.h"

class AdaptiveInsertion: public IterativeHeuristic {
protected:
  /// Maximum number of iterations (this is the only reasonable termination
  /// criterion for adaptive insertion)
  unsigned my_iteration_limit;
  /// Number of iterations so far
  unsigned iterations;
  /// Current number of crossings
  unsigned long long crossings;
public:
  /// Create an instance to work on the given BiGraph
  AdaptiveInsertion(BiGraph & bigraph, unsigned iteration_limit):
    IterativeHeuristic(bigraph), my_iteration_limit(iteration_limit) {
    iterations = 0;
  }

  /// Run the heuristic once for each side
  virtual void runHeuristic() {
    ++iterations;
    myBigraph.switch_sides();
    crossings += adaptive_insert(myBigraph);
    myBigraph.switch_sides();
    crossings += adaptive_insert(myBigraph);
  }

  /// Return true if it's appropriate to keep going, based on the termination
  /// criterion
  virtual bool keepGoing() const {
    return iterations <= my_iteration_limit;
  }
};

#endif

// Local Variables: ***
//  mode:c++ ***
// End: ***

//  [Last modified: 2007 08 02 at 21:57:43 GMT]
