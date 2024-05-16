/// @file Sums.cxx
/// @brief
/// Data structure for efficient retrieval and update of sums of
///    contiguous integers in an indexed list, used to compute
///    crossing numbers
/// @author Matt Stallmann
/// @date circa 1998
/// $Id: Sums.cxx 66 2007-08-08 19:09:27Z mfms $

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
#include<assert.h>
#include"Sums.hxx"

class SumTree {
public:
  int my_value;
};                              // for now just use an array

Sums::Sums(int n): my_size(n)
{
  my_tree = new SumTree[n];
  for ( int i = 0; i < n; i++ ) my_tree[i].my_value = 0;
}

Sums::~Sums()
{
  delete [] my_tree;
}

void
Sums::increment(int index)
{
#ifdef DEBUG
  cerr << "      Sums::increment(" << index << ")" << endl;
#endif
  assert( 0 <= index && index < my_size );
  ++my_tree[index].my_value;
#ifdef DEBUG
  cerr << "       array is ";
  for ( int i = 0; i < my_size; i++ ) cerr << my_tree[i].my_value << " ";
  cerr << endl;
#endif
}

int
Sums::sum(int LB, int UB) const
{
#ifdef DEBUG
  cerr << "      Sums::sum(" << LB << ", " << UB << ")" << endl;
#endif
  assert( 0 <= LB && UB < my_size );
  int retval = 0;
  for ( int i = LB; i <= UB; i++ ) retval += my_tree[i].my_value;
#ifdef DEBUG
  cerr << "       retval = " << retval << endl;
#endif
  return retval;
}

//  [Last modified: 2007 08 08 at 18:58:46 GMT]
