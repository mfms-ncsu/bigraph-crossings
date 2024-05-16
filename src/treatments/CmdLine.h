/// @file CmdLine.h
/// @brief Header for a class that parses the command line
/// @author Matt Stallmann
/// @date 16 Dec 1998
/// $Id: CmdLine.h 65 2007-08-08 18:45:24Z mfms $

//     Copyright (C) 2001,2007  Matthias Stallmann.
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

// HISTORY:
//   1 Apr 1999 - changed it so that a flag without an = sign is
//                automatically set to 0 (so its presence can be tested) 
//  29 Jun 1999 - added getFile( int ) method
//   3 Nov 2004 - updated this to use STL instead of Jo Perry's utilities.

#ifndef CMDLINE_H
#define CMDLINE_H

using namespace std;

#include<string>
#include<vector>
#include<map>
#include<set>

typedef map< string, string, less< string > > string_table;
typedef vector< string > string_list;
typedef set< string > string_set;

/// @brief Stores information about the command line and allows easy access
/// to it from a program.
///
/// ASSUMPTIONS:
///
///  -# Any command line argument beginning with '-' (except for '-' by
///   itself) is a flag; the - is followed by a legal C identifier, an '=',
///   and the value of the flag (no spaces); the value may be interpreted as
///   a string, an integer, a double, or a Boolean (0 or 1). A '-' followed
///   by an identifier and no '=' is entered with value 0.
///  -# Anything other than a flag is interpreted as a file name.
///  -# A '-' by itself represents standard input (is reported as the file
///   name "").

class CmdLine {
public:
  /// PRE: argc = number of command line arguments and argv = an array
  ///      containing the arguments
  /// POST: values of all flags have been stored and the file names,
  ///       beginning with the command name, are queued up in the queue
  ///       returned by getFiles() - an empty file name indicates standard
  ///       input (or can be interpreted as desired by the client) 
  CmdLine( int argc, char * * argv );

  ~CmdLine();                   // clean up allocated memory

  /// Returns true iff all the flags match those on the legal_flags list
  bool flagsAreLegal( const string_set legal_flags ) const;

  /// number of files on the command line (not including argv[0]!)
  unsigned numberOfFiles() const;

  /// POST: retval == a copy of the queue of file names
  string_list getFiles() const;

  /// PRE: i <= numberOfFiles()
  /// POST: retval == the i-th file name (i = 0 means the command name); if
  ///       there are no flags, this is equivalent to argv[i]
  const string getFile( unsigned i );

  /// POST: retval == true iff flag is present in command line
  bool flagPresent( const string flag ) const;

  /// POST: retval == the value of the given flag
  const string stringFlag( const string flag ) const;

  /// PRE: the value of flag is a legal integer
  /// POST: retval == value of flag interpreted as an integer
  int intFlag( const string flag ) const;

  /// PRE: the value of flag is a legal floating point number
  /// POST: retval == value of flag interpreted as an (double precision)
  ///                 floating point number
  double doubleFlag( const string flag ) const;

  /// PRE: the value of flag is 0 or 1
  /// POST: retval == value of flag interpreted as a Boolean
  bool boolFlag( const string flag ) const;

private:
  string_list my_files;
  string_table my_flags;
};

#endif

// Local Variables: ***
//  mode:c++ ***
// End: ***

//  [Last modified: 2007 08 08 at 18:40:58 GMT]
