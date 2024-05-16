/// @file CmdLine.cpp
/// @brief Implementation of command-line parser
/// @author Matt Stallmann
/// @date 16 Dec 1998
/// $Id$

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

//   1 Apr 1999 - changed it so that a flag without an = sign is
//                automatically set to 0 (so its presence can be tested) 
//  29 Jun 1999 - added getFile( int ) method
//   3 Nov 2004 - modified it to use STL instead of Jo Perry's utilities

#include"CmdLine.h"
#include<stdlib.h>              // abort()
#include<iostream>
#include<sstream>
#include<assert.h>

// private data:
//  string_list my_files;
//  string_table my_flags;

static void process_flag( const string str, string_table & flag_table )
{
  string flag_name = "";
  string flag_value = "";
  size_t eq_pos = str.find( "=" );
  if ( eq_pos == string::npos ) {
    flag_name = str;
    flag_value = "0";
  }
  else {
    flag_name = str.substr( 0, eq_pos );
    flag_value = str.substr( eq_pos + 1, str.length() - eq_pos - 1 ); 
  }
  flag_table[ flag_name ] = flag_value; // latest value of a flag counts if
                                        // there are multiple occurrences
}

CmdLine::CmdLine( int argc, char * * argv )
{
  while ( argc ) {
    if ( '-' == (*argv)[0] ) {
      if ( (*argv)[1] ) process_flag( *argv + 1, my_flags );
      else my_files.push_back( "" );
    }
    else my_files.push_back( *argv );
    --argc;
    ++argv;
  }
}

CmdLine::~CmdLine()
{
  // no action required - all dynamic data is buried in the contained objects
}

bool
CmdLine::flagsAreLegal( const string_set legal_flags ) const
{
  bool retval = true;
  // check each flag
  for( string_table::const_iterator current = my_flags.begin();
       current != my_flags.end(); ++current ) {
    if( legal_flags.find( current->first ) == legal_flags.end() ) {
      retval = false;
      cerr << " Unknown option: " << current->first << endl;
    }
  }
  return retval;
}

unsigned
CmdLine::numberOfFiles() const
{
  return (my_files.size() - 1); // exclude argv[0]
}

string_list 
CmdLine::getFiles() const
{
  return my_files;
}

const string
CmdLine::getFile( unsigned i )
{
  assert( i <= my_files.size() );
  return my_files[ i ];
}

bool
CmdLine::flagPresent( const string flag ) const
{
  if ( my_flags.find( flag ) != my_flags.end() ) {
    return true;
  }
  else {
    return false;
  }
}

const string 
CmdLine::stringFlag( const string flag ) const
{
  string_table::const_iterator position = my_flags.find( flag );
  if ( position == my_flags.end() ) {
    cerr << "Command-line flag: -" << flag << " was not present" << endl;
    abort();
  }
  return position->second;
}

int
CmdLine::intFlag( const string flag ) const
{
  const string str_val = stringFlag( flag );
  istringstream conversion_stream( str_val );
  int retval = 0;
  if( ! (conversion_stream >> retval) ) {
    cerr << "Flag value is expected to be integer, flag = -" << flag
         << ", value = " << str_val << endl;
    abort();
  }
  string left_overs = "";
  if( conversion_stream >> left_overs ) {
    cerr << "Garbage at end of integer flag value, flag = -" << flag
         << ", value = " << retval << ", garbage is " << left_overs <<
      endl;
    abort();
  }
  return retval;
}

double
CmdLine::doubleFlag( const string flag ) const
{
  const string str_val = stringFlag( flag );
  istringstream conversion_stream( str_val );
  double retval = 0;
  if( ! (conversion_stream >> retval) ) {
    cerr << "Flag value is expected to be double, flag = -" << flag
         << ", value = " << str_val << endl;
    abort();
  }
  string left_overs = "";
  if( conversion_stream >> left_overs ) {
    cerr << "Garbage at end of integer flag value, flag = -" << flag
         << ", value = " << retval << ", garbage is " << left_overs <<
      endl;
    abort();
  }
  return retval;
}

bool
CmdLine::boolFlag( const string flag ) const
{
  const string str_val = stringFlag( flag );
  bool retval = false;
  if ( str_val != "0" && str_val != "false"
       && str_val != "no" ) { // not false
    if ( str_val != "1" && str_val != "true"
         && str_val != "yes" ) { // not true
      cerr << "Invalid boolean flag value, flag = " << flag << ", value = "
           << str_val << endl;
      abort();
    }
    else retval = true;         // == "1"
  }
  return retval;
}


//  [Last modified: 2015 05 16 at 13:02:20 GMT]
