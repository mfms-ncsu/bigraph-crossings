/// @file adaptive_insert.cxx 
/// @brief implementation of (one iteration of) the adaptive
///                        insertion heuristic
/// @author Matt Stallmann
/// @date 03 Aug 1998
/// $Id: adaptive_insert.cxx 65 2007-08-08 18:45:24Z mfms $

// 18 Aug 1998 - modified to insert each item only once per iteration and
//               not allow insertions to do nothing. 
// 13 Mar 2000 - fixed a bug that caused a seg fault if there's only one
//               node on a side; got rid of NEIGHBORS_ONLY compile-time
//               option.

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
#include<limits.h>

#include"adaptive_insert.h"

static void insert(bool array[], int i, int j)
  // PRE: i and j are nonnegative and less than the length of the array
  // POST: array[i] has been inserted before array[j] (and other array
  //       elements shifted accordingly
{
  bool original_i = array[i]; 
  if ( i < j ) {
    for ( int k = i + 1; k <= j; ++k ) {
      array[k - 1] = array[k];
    }
  }
  else { // i > j
    for ( int k = i - 1; k >= j; --k ) {
      array[k + 1] = array[k];
    }
  }
  array[j] = original_i;
}

int adaptive_insert( BiGraph & bg )
{
  int retval = 0;               // change in number of crossings
  bool * was_inserted = new bool[bg.n0()];
  for ( int k = 0; k < bg.n0(); ++k ) was_inserted[k] = false;
  int i = bg.n0() - 1;
  while ( i >= 0 ) {
    if ( ! was_inserted[i] ) {
      int change = 0;
      int j = bg.best_insertion( i, change );
      if ( IntSet::DUMMY != j ) { // BUG FIX: 13 Mar 2000
        retval += change;
#ifdef TRACE
        cerr << "   insert(" << i << ", " << j << "), cumulative change = "
             << retval << endl;
#endif
        bg.insert( i, j );
        was_inserted[i] = true;
        insert( was_inserted, i, j );
      }
    }
    --i;
  }
  delete [] was_inserted;
  return retval;
}

//  [Last modified: 2007 08 08 at 18:06:15 GMT]
