/// @file Timer.hxx
/// @brief Header file for getting timing info
/// @author Matt Stallmann
/// @date 1989/06/27
/// $Id: Timer.hxx 66 2007-08-08 19:09:27Z mfms $

//  Revision History:
// 06/27/89 Created
// 03/07/90 Use "times.h" instead of "vtimes.h"
//     to improve compatability (wkc)
// 07/03/90 Allow host name uo tp 40 char's (it was 20 char's)
// 11/19/90 Change comment style   
// 06/19/96 Adapted to C++

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

#ifndef TIMER_HXX
#define TIMER_HXX

using namespace std;

#include <sys/types.h>
#include <sys/times.h>
#include <iostream>

class Timer {
   struct tms buffer_;           // buffer for timing info
   unsigned long startTime_;     // most recent start time
   unsigned long runTime_;       // time between most recent start/stop
   enum { IDLE, RUNNING } state_;
public:
   Timer(): startTime_(0), runTime_(0), state_(IDLE) { }

   void start() {                // start timing
#ifdef DEBUG
     cerr << "-> Timer::start, startTime_ = " << startTime_ << ", runTime_ = "
      << runTime_ << ", state_ = " << state_ << endl;
#endif
      if (state_ != IDLE) {
         cerr << endl << "Timer::start -- already started." << endl;
         abort();
      }
      state_ = RUNNING;
      times(&buffer_);
      startTime_ = buffer_.tms_utime;
      runTime_ = 0;
#ifdef DEBUG
     cerr << "<- Timer::start, startTime_ = " << startTime_ << ", runTime_ = "
      << runTime_ << ", state_ = " << state_ << endl;
#endif
   }

   void stop() {                 // stop timing
#ifdef DEBUG
     cerr << "->Timer::stop, startTime_ = " << startTime_ << ", runTime_ = "
      << runTime_ << ", state_ = " << state_ << endl;
#endif
      if (state_ != RUNNING) {
         cerr << endl << "Timer::stop -- no matching start." << endl;
         abort();
      }
      state_ = IDLE;
      times(&buffer_);
      runTime_ = buffer_.tms_utime - startTime_;
      startTime_ = 0;
#ifdef DEBUG
     cerr << "<-Timer::stop, startTime_ = " << startTime_ << ", runTime_ = "
      << runTime_ << ", state_ = " << state_ << endl;
#endif
   }

   double seconds() { // return time in seconds between most recent start/stop
                      //  or seconds since last start, if no intervening stop
      if ( RUNNING ) {
         times(&buffer_);
         return (buffer_.tms_utime - startTime_) / 60.0;
      }
      return (runTime_ / 60.0);
   }
};
#endif

//  [Last modified: 2007 08 08 at 19:02:45 GMT]
