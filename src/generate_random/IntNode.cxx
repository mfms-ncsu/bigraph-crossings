/// @file IntNode.cxx
/// @brief
/// based on recNode.C in Perry/Levin, Section 12.7
/// @author Matt Stallmann
/// @date 1993
/// $Id: IntNode.cxx 68 2007-08-08 20:07:39Z mfms $

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

#include "IntNode.hxx"

IntNode :: IntNode(int i, IntNode* p) : info_(i), link_(p)
{ }
 
IntNode* IntNode :: copy() const
{
   if (link_ != 0)
      return new IntNode(info_, link_->copy());
   return new IntNode(info_, 0);
}
           
IntNode :: ~IntNode()
{
   delete link_;
}                      

bool IntNode :: containsAfter(int x) const
{
   if (link_ == 0 || x < link_->info_) // Termination: the chain is empty or x 
      return false;                    //   is to small to be in the chain.
   else if (link_->info_ == x)         // Termination: x is in the next node.
      return true;
   else                                // Recursion: pass the message to the
      return link_->containsAfter(x);  //   next node. (delegation)
}

void IntNode :: insertAfter(int x)
{
   if (link_ == 0 || x <= link_->info_)  // Termination: insert x here.
      link_ = new IntNode(x, link_);
   else                                  // Recursion: pass the message to the 
      link_->insertAfter(x);             //   next node.x
}

void IntNode :: removeAfter(int x)
{
   if (link_ == 0 || x < link_->info_)  // Termination: x is not in the list.
      return;
   else if (x == link_->info_) {        // Termination: x is in the next node
      IntNode* p = link_;               //   Remove the node.
      link_ = link_->link_;
      p->link_ = 0;
      delete p;
   }
   else                                 // Recursion: pass the message to the  
      link_->removeAfter(x);            //   next node.
}


//  [Last modified: 2007 08 08 at 19:51:23 GMT]
