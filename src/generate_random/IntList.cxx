/// @file IntList.cxx
/// @brief
/// based on recList.C in Perry/Levin, Section 12.7
/// @author Matt Stallmann
/// @date 1993
/// $Id: IntList.cxx 68 2007-08-08 20:07:39Z mfms $

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

#include "IntList.hxx"

IntList :: IntList()
{
   head_ = new IntNode;
} 

IntList :: IntList(const IntList& old)
{
   head_ = old.head_->copy();
} 

IntList& IntList::operator=(const IntList& rhs)
{
   if (this != &rhs) {
      delete head_;
      head_ = rhs.head_->copy();
   }
   return *this;
}

IntList :: ~IntList()
{
   delete head_;
}

bool IntList :: isEmpty() const
{
   return (head_->link_ == 0);
}

void IntList :: add(int x)
{
   head_->insertAfter(x);
}

void IntList :: remove(int x)
{
   head_->removeAfter(x);
}

bool IntList :: contains(int x) const
{
   return head_->containsAfter(x);
}


//  [Last modified: 2007 08 08 at 19:52:26 GMT]
