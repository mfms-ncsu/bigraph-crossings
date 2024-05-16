/// @file IntSet.cpp
/// @brief Set of integers for use with graph adjacency lists.
/// @author Matt Stallmann
/// @date 7 Jul 1998
/// $Id: IntSet.cpp 62 2007-08-03 19:16:20Z mfms $

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

using namespace std;

#include<iostream>
#include<assert.h>
#include<float.h>               // DBL_MAX

#include"defs.h"
#include"IntSet.h"

const int SENTINEL = -1;

double IntSet::the_ratio = 1.0;

bool IntSet::true_median = false;

ostream & operator << (ostream& out, IntNode * L)
{
  out << "[";
  for ( IntNode * ptr = L; ptr; ptr = ptr->rest() ) {
    out << ptr->vertex();
    if ( ptr->rest() ) out << " ";
  }
  out << "]";
  return out;
}

void
IntSet::sort()
  // POST: my_array is sorted in descending order
{
  // do an insertion sort (p points to item to be inserted, q points to the
  // position being considered for insertion)
  for ( int * p = my_array + cardinality() - 2; p >= my_array; --p ) {
    int * q = p;
    int to_be_inserted = *p;
    while ( *(q + 1) > to_be_inserted ) {
      *q = *(q + 1);
      ++q;
    }
    *q = to_be_inserted;
  }
}

static int length(IntNode * L)
{
  int retval = 0;
  for ( IntNode * ptr = L; ptr; ptr = ptr->rest() ) ++retval;
  return retval;
}

IntSet::IntSet(IntNode * L)
{
  int cardinality = length(L);
  my_array = new int[cardinality + 1];
  int index = 0;
  for ( IntNode * ptr = L; ptr; ptr = ptr->my_rest )
    my_array[index++] = ptr->my_vertex;
  my_array[index] = SENTINEL;
}

IntSet::IntSet(const IntSet & other)
{
  my_array = new int[other.cardinality() + 1];
  int * p = other.my_array;
  int * q = my_array;
  while ( (*q++ = *p++) != SENTINEL );
}

// destructor defined inline

int
IntSet::cardinality() const
{
  // compute cardinality of this set
  int retval = 0;
  for ( int * p = my_array; *p != SENTINEL; ++p ) ++retval;
  return retval;
}

const int IntSet::DUMMY = -2;

void
IntSet::renumber(int i, int j)
{
  int * p = my_array;
  while ( *p != i ) ++p;
  *p = j;
}

double
IntSet::average()
{
#ifdef DEBUG
  cerr << "    IntSet::average(" << the_ratio << ", " << true_median <<
   ")" << endl << "      set = " << *this << endl; 
#endif
  double median = 0.0, mean = 0.0;
  double retval = -1.0;
  int size = cardinality();
  if ( size == 0 ) {
    cerr << "WARNING -- IntSet::average(): size of IntSet == 0." << endl;
    retval = DBL_MAX;
  }
  else {
    if ( the_ratio < 1.0 ) {
      sort();
      if ( size % 2 ) // odd number of items
        median = my_array[size / 2];
      else if ( true_median )
        median = (my_array[size / 2 - 1] + my_array[size / 2]) / 2.0;
      else median = my_array[size / 2] + MEDIAN_CORRECTION;
    }
    if ( the_ratio > 0.0 ) {
      for ( int * p = my_array; *p != SENTINEL; ++p )
        mean += *p;
      mean /= size;
    }
    retval = the_ratio * mean + (1.0 - the_ratio) * median;
  }
#ifdef DEBUG
  cerr << "    IntSet::average done, retval = " << retval << endl;
#endif
  return retval;
}

void
IntSet::sort(const int * key, bool descending)
{
  // do an insertion sort (p points to item to be inserted, q points to the
  // position being considered for insertion)
  for ( int * p = my_array + cardinality() - 2; p >= my_array; --p ) {
    int * q = p;
    int to_be_inserted = *p;
    if ( descending ) {
      while ( SENTINEL != *(q + 1) && key[*(q + 1)] > key[to_be_inserted] ) {
        *q = *(q + 1);
        ++q;
      }
    }
    else {
      while ( SENTINEL != *(q + 1) && key[*(q + 1)] < key[to_be_inserted] ) {
        *q = *(q + 1);
        ++q;
      }
    }
    *q = to_be_inserted;
  }
}

ostream & operator << (ostream & out, const IntSet & S)
{
  if ( S.my_array ) {
    out << "[";
    int * p = S.my_array;
    while( *p != SENTINEL ) {
      out << *p++;
      if ( *p != SENTINEL ) out << " ";
    }
    out << "]";
  }
  else out << "[*** not valid ***]";
  return out;
}

bool
Iterator::valid()
{
  return SENTINEL != *my_ptr;
}

// CLASSINV (for GapCounts):
//   my_counts maintains a histogram of integers from sets encountered
//   during update operations as follows: 
//     my_counts[2 * i] == # of elements > my_set.my_array[i] and
//                                       < my_set.my_array[i-1] if i > 0
//     my_counts[2 * i + 1] == # of elements == my_set.my_array[i]

GapCounts::GapCounts(IntSet& S): my_set(S)
{
  S.sort();
  int size = S.cardinality();
  my_counts = new int[2 * size + 1];
  for ( int i = 0; i <= 2 * size; i++ ) my_counts[i] = 0;
}

// destructor defined in line

void
GapCounts::update(IntSet& S)
{
  S.sort();
  int * this_set_ptr = my_set.my_array;
  int * cnt_ptr = my_counts;
  int * other_set_ptr = S.my_array;
  while ( *other_set_ptr != SENTINEL ) {
    if ( *other_set_ptr > *this_set_ptr ) {
      ++(*cnt_ptr);
      ++other_set_ptr;
    }
    else if ( *other_set_ptr == *this_set_ptr ) {
      ++cnt_ptr; ++(*cnt_ptr); ++cnt_ptr;
      ++this_set_ptr;
      ++other_set_ptr;
    }
    else { // *other_set_ptr < *this_set_ptr 
      ++cnt_ptr; ++cnt_ptr;
      ++this_set_ptr;
    }
  }
}

int
GapCounts::change_in_crossings() const
{
  int retval = 0;
  int size = my_set.cardinality();
  int multiplier = size;
  for ( int * p = my_counts; p <= my_counts + 2 * size; ++p ) {
    retval += *p * multiplier;
    --multiplier;
  }
  return retval;
}

ostream & operator << (ostream & out, const GapCounts & C)
{
  out << "<";
  int * cnt_ptr = C.my_counts;
  for ( Iterator iter = C.my_set; iter.valid(); iter.next() ) {
    out << *cnt_ptr++;
    out << " |" << *cnt_ptr++ << "| ";
  }
  out << *cnt_ptr;
  out << ">";
  return out;
}


//  [Last modified: 2007 07 27 at 21:14:14 GMT]
