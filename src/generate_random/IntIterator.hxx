/// @file IntIterator.hxx
/// @brief
/// Based on recIterator.H in Perry/Levin, Section 12.7
/// @author Matt Stallmann
/// @date 1993 
/// $Id: IntIterator.hxx 68 2007-08-08 20:07:39Z mfms $

//     Copyright (C) 2001  Matthias Stallmann.
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

#ifndef _INTITERATOR_H
#define _INTITERATOR_H

#include "IntNode.hxx"
#include "IntList.hxx"

// General iterator to access all of the items in a IntList.
// When the IntList is modified, the IntIterator must be reset before 
//   subsequent use.
class IntIterator {
public:
   IntIterator(const IntList& aList); // Iterates over aList. Iteration begins
                                      //  with the first list item.
   int next();                // Next list item.
                              //   ASSUME: There is a next item.
   bool more() const;         // Are there more list items?
   void reset();              // Reset the iterator to begin with the 
                              //   first list item.
protected:
   const IntList& theList_;
   const IntNode* cursor_;
};

#endif

//  [Last modified: 2007 08 08 at 19:50:45 GMT]
