/// @file IntIterator.cxx
/// @brief
///  Based on recIterator.C in Perry/Levin, Section 12.7
/// @author Matt Stallmann
/// @date 1993
/// $Id: IntIterator.cxx 68 2007-08-08 20:07:39Z mfms $

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

#include "IntIterator.hxx"

IntIterator :: IntIterator(const IntList& aList) : theList_(aList),
                                                   cursor_(aList.head_->link_)
{ }

void IntIterator :: reset()
{
   cursor_ = theList_.head_->link_;
}

int IntIterator :: next()
{
   int x = cursor_->info_;           // ERROR if cursor_ is null
   cursor_ = cursor_->link_;
   return x;
}

bool IntIterator :: more() const
{
   return (cursor_ != 0);
}

//  [Last modified: 2007 08 08 at 19:50:58 GMT]
