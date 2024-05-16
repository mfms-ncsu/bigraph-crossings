/// @file IterativeHeuristic.h
/// @brief Abstract class for all heuristics that iterate until some
/// condition is satisfied while updating the order on each side during an
/// iteration.
/// @author Matt Stallmann
/// @date 2007/07/27
/// $Id: IterativeHeuristic.h 48 2007-07-27 17:57:59Z mfms $

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

#ifndef ITERATIVEHEURISTIC_H
#define ITERATIVEHEURISTIC_H

using namespace std;

#include"BiGraph.hxx"

class IterativeHeuristic {

protected:
  /// The BiGraph to which the heuristic is applied
  BiGraph & myBigraph;
public:
  /// Create an instance to work on the given BiGraph
  IterativeHeuristic(BiGraph & bigraph): myBigraph(bigraph) {
  }

  /// Run one iteration of the heuristic
  virtual void runHeuristic() = 0;

  /// True if termination criterion has not been met.  This allows each type
  /// of heuristic to manage its own termination decision.
  virtual bool keepGoing() const = 0;

  /// Needed in case a subclass has a destructor
  virtual ~IterativeHeuristic() {}
};

#endif

// Local Variables: ***
//  mode:c++ ***
// End: ***

//  [Last modified: 2007 07 27 at 17:55:55 GMT]
