/// @file defs.h
/// @brief
/// definitions common to all source files for randomBiGraph
/// @author Matt Stallmann
/// @date 1999
/// $Id: defs.h 68 2007-08-08 20:07:39Z mfms $

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

const int NODE_FIELD_WIDTH = 4; // length of field in which to embed
                                // numbers of nodes (should be >= number of
                                // digits in MAX_VERTICES).
const int MUTANT_FIELD_WIDTH = 4;
                                // used for encoding sequence numbers of
                                // subjects in a graph class
const int NAME_LENGTH = 511;    // maximum length of a vertex name
const int LINE_LENGTH = 75;     // maximum length of a line in a .ord file
const int DEFAULT_CLASS_SIZE = 32;

/*  [Last modified: 2007 08 08 at 19:55:52 GMT] */

// Local Variables: ***
//  mode:c++ ***
// End: ***
