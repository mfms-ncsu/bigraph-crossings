/// @file IntList.hxx 
/// @brief
/// based on recList.H in Perry/Levin, Section 12.7
/// @author Matt Stallmann
/// @date 1993
/// $Id: IntList.hxx 68 2007-08-08 20:07:39Z mfms $

/// @todo The use of hand-crafted software for linked lists of integers no
/// longer makes sense.  The classes IntList, IntNode, and IntIterator should
/// be replaced by an STL list. 

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

#ifndef _INTLIST_H
#define _INTLIST_H

#include "IntNode.hxx"

// An ordered list of integers. List data is in ascending order.
class IntList {
public:
   IntList();                               // Constructs an empty list.
   IntList(const IntList& r);               // Constructs a deep copy of r.
   IntList& operator=(const IntList& rhs);  // Assigns a deep copy of rhs.
   ~IntList();                  // Deallocates list resources.

   void add(int x);             // Adds x into its proper position.
   void remove(int x);          // Removes the first copy of x, 
                                //   if it exists.
   bool contains(int x) const;  // Is x a list item?
   bool isEmpty() const;        // Is the list empty?

friend class IntIterator;
protected:
   IntNode* head_;
};

#endif

//  [Last modified: 2007 08 08 at 19:50:25 GMT]
