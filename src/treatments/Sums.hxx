/// @file Sums.hxx
/// @brief
/// Data structure for efficient retrieval and update of sums of
///    contiguous integers in an indexed list, used to compute
///    crossing numbers
/// @author Matt Stallmann
/// @date circa 1998
/// $Id: Sums.hxx 66 2007-08-08 19:09:27Z mfms $

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

using namespace std;

#include<iostream>

class SumTree;

// In some comments below, the integers in the list are referred to as
//      A[0], ... , A[my_size-1]

class Sums {
  int my_size;                  // number of integers in the list
  SumTree * my_tree;            // binary tree for storing integers and
                                // partial sums
public:
  Sums(int n);
  // POST: the list has n integers indexed 0 to n-1 and all are == 0

  ~Sums();
  // POST: memory for the list has been deallocated

  int size() const { return my_size; }

  void increment(int index);
  // PRE: 0 <= index < size() 
  // POST: A[index] == A[index]<before> + 1

  int sum(int LB, int UB) const;
  // PRE: 0 <= LB && UB < size()
  // POST: retval == A[LB] + A[LB+1] + ... + A[UB]
};

//  [Last modified: 2007 08 08 at 18:58:57 GMT]

// Local Variables: ***
//  mode:c++ ***
// End: ***

