/// @mainpage
/// Source code for treatments used in bipartite graph crossing minimization.
///
/// To use these programs in standalone fashion, run 'make' and then copy the
/// executables to a bin directory on your path.  The executables are:
///
/// - fstats: a quick and dirty program for getting min, average, max, and
///         standard deviation from a collection of data
/// - gstats: a program that gathers statistics about a bigraph
/// - sbg_cn_eval: a simple program that counts the number of crossings
///                 in a dot file given the ordering specified in an ord file
/// - sbg_treat: main program for all treatments for the JEA paper (see
///          comments in heuristic.cpp for optional command-line arguments)
///   <br>  Usage: sbg_treat -tr=<var>treatment</var> <var>dot_file</var>
///                  <var>ord file</var>
/// - dot2dre: reads a .dot file and outputs a corresponding .dre file
/// (dreadnaught format for graph isomorphism testing).
///
/// The Examples subdirectory contains a set of small graphs for testing
/// purposes.  See <a href="../Examples/README.txt">the README.txt file</a>
/// for more details.
///
///     Copyright (C) 2001,2007 Matthias Stallmann.
///     Contact: matt_stallmann@ncsu.edu
///
///     This program is free software; you can redistribute it and/or modify
///     it under the terms of the GNU General Public License as published by
///     the Free Software Foundation; either version 2 of the License, or
///     (at your option) any later version.
///
///     This program is distributed in the hope that it will be useful,
///     but WITHOUT ANY WARRANTY; without even the implied warranty of
///     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
///     GNU General Public License for more details.
///
///     You should have received a copy of the GNU General Public License along
///     with this program (file COPYING.txt); if not, write to the Free Software
///     Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301
///     USA.

/// @file mainpage.h
/// @brief The "Main Page"
/// @author Matthias Stallmann
/// @date 2007/08/03
/// $Id: mainpage.h 71 2007-08-08 21:12:00Z mfms $ 

/*  [Last modified: 2007 08 08 at 21:10:12 GMT] */

// Local Variables: ***
//  mode:c++ ***
// End: ***

