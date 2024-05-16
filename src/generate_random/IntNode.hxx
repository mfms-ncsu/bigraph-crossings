/// @file IntNode.hxx
/// @brief
/// based on recNode.H in Perry/Levin, Section 12.7
/// @author Matt Stallmann
/// @date 1993 
/// $Id: IntNode.hxx 68 2007-08-08 20:07:39Z mfms $

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

#ifndef _INTNODE_H
#define _INTNODE_H


// Node type for an ordered linked list of integers with a dummy head node
class IntNode {
private:
   IntNode(int i = 0, IntNode* p = 0);   // Constructor.
   IntNode* copy() const;                // Returns a deep copy.
	~IntNode();                      // Deallocates the next node.

   void insertAfter(int x);              // Adds x in the proper position in
                                         //   the chain after this node.
   void removeAfter(int x);              // Removes the first occurrence of 
                                         //   x in the chain after this node.
   bool containsAfter(int x) const;      // Does the chain after this node 
                                         //   contain x?
   int info_;
   IntNode* link_;

friend class IntList;
friend class IntIterator;
};

#endif

//  [Last modified: 2007 08 08 at 19:51:12 GMT]
