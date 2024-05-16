/// @file adaptive_insert.h
/// @brief header for adaptive insertion heuristic
/// @author Matt Stallmann
/// @date 3-AUG-1998
/// $Id: adaptive_insert.h 65 2007-08-08 18:45:24Z mfms $

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

#include"BiGraph.hxx"

/// runs the adaptive insertion heuristic and returns the net change in
/// crossings
int adaptive_insert( BiGraph & bg );

// Local Variables: ***
//  mode:c++ ***
// End: ***

/*  [Last modified: 2007 08 08 at 18:03:58 GMT] */

