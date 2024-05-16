/// @file Rand.hxx
/// @brief
/// Definition of a class of random number streams; each stream
///            can be initialized from an istream or an array of 3 short
///            int's and (optionally) written to an ostream via the
///            overloaded operator. Uses rand48 library.
/// @author Matt Stallmann
/// @date circa 1990
/// $Id: Rand.hxx 68 2007-08-08 20:07:39Z mfms $

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

#ifndef RAND_HXX
#define RAND_HXX

using namespace std;

#include<stdlib.h>
#include<iostream>

class Rand {
public:
  Rand( istream & in ) { in >> my_x[ 0 ] >> my_x[ 1 ] >> my_x[ 2 ]; }
  Rand( unsigned short x[] )
   { for ( int i = 0; i < 3; ++i ) my_x[ i ] = x[ i ]; }
  operator int() { return nrand48( my_x ); }
  int integer( int n ) { return (nrand48( my_x ) % n); }
  double real( double lb, double ub )
   { return lb + (ub - lb) * erand48( my_x ); }
  bool boolean( double r ) { return (erand48( my_x ) < r); }
  friend ostream & operator << ( ostream & out, const Rand & R );
private:
  unsigned short my_x[ 3 ];
};

inline ostream & operator << ( ostream & out, const Rand & R )
{
  out << R.my_x[0] << " " << R.my_x[1] << " " << R.my_x[2];
  return out;
}

#endif

//  [Last modified: 2007 08 08 at 20:06:38 GMT]
