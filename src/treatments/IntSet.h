/// @file IntSet.h
/// @brief Set of integers for representing adjacency lists in graphs
/// @author Matt Stallmann
/// @date 7 Jul 1998
/// $Id: IntSet.h 62 2007-08-03 19:16:20Z mfms $

/// @todo This obviously needs to be implemented as an std vector

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

// 12 Mar 2001 - modified so that average() returns INT_MAX when the set
//               is empty

#ifndef INTSET_H
#define INTSET_H

using namespace std;

#include<iostream>

class IntNode {                // list node (external representation of
                               // an IntSet is more flexible)
  int my_vertex;
  IntNode * my_rest;
  friend class IntSet;
public:
  IntNode(int i, IntNode * L = 0): my_vertex(i), my_rest(L) {}
  ~IntNode() { delete my_rest; }
  int vertex() const { return my_vertex; }
  IntNode * rest() const { return my_rest; }
};

ostream & operator << (ostream & out, IntNode * L);

class IntSet {
  /// Specifies the proportion of median (0.0) and mean (1.0) to be used.
  static double the_ratio;
  /// Specifies whether true median (average of two middle elements) is to be
  /// used in place of always choosing the
  static bool true_median;

  int * my_array;               // array for storing set members
  void sort();
  friend class Iterator;
  friend class BackIterator;
  friend class GapCounts;

  // don't allow assignment of IntSet's
  IntSet & operator = (const IntSet &);
public:

  IntSet(IntNode * L);
  // PRE: the integers in L are distinct
  // POST: this set contains the integers of L

  IntSet(const IntSet & other);

  ~IntSet() { delete [] my_array; }

  /// Set the ratio
  static void setRatio(double ratio) { the_ratio = ratio; }
  /// Specify that true median should be used (default is not)
  static void setTrueMedian() { true_median = true; }
  /// Specify that true median should not be used (default)
  static void setFudgedMedian() { true_median = false; }

  int cardinality() const;
  // POST: retval == cardinality of this set

  static const int DUMMY;
  // dummy value guaranteed not to be the same as any vertex

  void renumber(int i, int j);
  // PRE: i is in the set and j is not in the set
  // POST: j is in the set and i is not in the set

  /// POST: retval == weighted mean/median of this set (using the_ratio to
  ///                 determine the mean/median mix, i.e. 0.0 represents
  ///                 all median, 1.0 all mean)
  ///                 if true_median, the median of an even number
  ///                 of items is the mean of the two middle items;
  ///                 otherwise, it's the first item plus a constant
  ///                 adjustment (see defs.h)
  ///                 for a set of cardinality 0, retval == INT_MAX
  double average();

  void sort(const int * key, bool descending = false);
  // PRE: key[i] is defined for every i in this set
  // POST: elements of this set are arranged x_1, ..., x_k so that
  //       key[x_1] <= key[x_2] <= ... <= key[x_k]
  //       if descending == true, key[x_1] >= key[x_2] >= ... >= key[x_k]

  friend ostream & operator << (ostream & out, const IntSet & S);
};

// The Iterator (and BackIterator) class may be used in a loop as follows
// to access all vertices in the set S:
//      Iterator iter = S;
//      while ( iter.valid() ) {
//        // do something with iter.vertex()
//        iter.next();
//      }

class Iterator {
  int * my_ptr;                 // current position in IntSet array
public:
  Iterator(const IntSet& S): my_ptr(S.my_array) {}

  bool valid();
  // POST: retval == true if current position is still valid

  int vertex()
   // PRE: valid()
   { return *my_ptr; }

  void next()
   // PRE: valid()
   // POST: this iterator has advanced to the next element 
   { ++my_ptr; }
};

class BackIterator {
  int * my_ptr;                 // current position in IntSet array
  int * my_front;               // pointer to beginning of array
public:
  BackIterator(const IntSet& S): my_ptr(S.my_array + S.cardinality() - 1),
    my_front(S.my_array) {}

  bool valid()
    // POST: retval == true if current position is still valid
    { return my_ptr >= my_front; }

  int vertex()
    // PRE: valid()
    { return *my_ptr; }

  void next()
    // PRE: valid()
    // POST: this iterator has advanced to the next element 
    { --my_ptr; }
};

// The following class is specific to the crossing number
// problem. It is used to keep track of how many elements of other
// sets fall into each gap between elements of this set. The information
// is then used to compute the net change in number of crossings should
// this set be moved to the right of the others.

class GapCounts {
  IntSet my_set;                // the set whose gaps are being used
  int * my_counts;              // array for keeping track of the counts
public:
  GapCounts(IntSet& S);
  ~GapCounts() { delete [] my_counts; }
  void update(IntSet& S);
  int change_in_crossings() const;
  friend ostream & operator << (ostream & out, const GapCounts & C);
};

#endif

// Local Variables: ***
//  mode:c++ ***
// End: ***

//  [Last modified: 2007 07 27 at 18:00:29 GMT]
