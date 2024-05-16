/// @file Queue.hxx
/// @brief
/// Queue of integers, implemented as a linked list, used for breadth-first
/// search.
/// @author Matt Stallmann
/// @date 1998/07/31
/// $Id: Queue.hxx 66 2007-08-08 19:09:27Z mfms $

/// @todo This can be implemented as an stl queue.

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

class QNode {
  friend class Queue;
  int my_item;
  QNode * my_next;
  QNode(int item, QNode * next = 0): my_item(item), my_next(next) {}
  ~QNode() { delete my_next; }
};

class Queue {
  QNode * my_front;             // items are removed at this end
  QNode * my_rear;              // new items are added at this end
  int my_length;                // number of items
public:
  Queue(): my_front(0), my_rear(0), my_length(0) {}
  ~Queue() { delete my_front; }

  bool is_empty() { return 0 == my_front; }

  int length() { return my_length; }

  void add(int item)
    {
      if ( 0 == my_rear ) {
        my_front = my_rear = new QNode(item);
      }
      else {
        my_rear->my_next = new QNode(item);
        my_rear = my_rear->my_next;
      }
      ++my_length;
    }

  void remove()
    {
      assert( 0 != my_front && 0 != my_rear );
      if ( my_front == my_rear ) {
        delete my_front;
        my_front = my_rear = 0;
      }
      else {
        QNode * to_be_deleted = my_front;
        my_front = my_front->my_next;
        to_be_deleted->my_next = 0;
        delete to_be_deleted;
      }
      --my_length;
    }

  int front() const
    {
      return my_front->my_item;
    }
};


//  [Last modified: 2007 08 08 at 18:55:37 GMT]
