/// @file fstats.cxx 
/// @brief
/// Compile simple statistics for experimental data
/// @author Matt Stallmann
/// @date 1999/07/28 
/// $Id: fstats.cxx 67 2007-08-08 19:28:31Z mfms $

// input is a list of integer or real numbers (no intervening text),
// separated by arbitrary white space; output gives min, mean, max, and
// standard deviation, in that order, on the same line, separated by tabs.

// to use the "with errorbars" option in gnuplot for a file containing a
// list of lines generated by this program, do either:
//    cut -f 2,4 | awk '{ print NR, $1, $2; }'
// to get "x y ydelta" format or:
//    cut -f 1-3 | awk '{ print NR, $2, $1, $3; }'
// for "x y ymin ymax" format.

// 23 May 2001 - got rid of DBL_MAX (portability to Linux on IBM 390)

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
#include<math.h>
#include<stdlib.h>              // exit()

int main()
{
  double data;
  if ( ! ( cin >> data ) ) {
    cerr << "fstats -- Warning: no input data given" << endl;
    exit(1);
  }

  double min_data = data;
  double max_data = data;
  double total_data = data;
  double sum_of_squares = data * data;
  int number_of_experiments = 1;
  while ( cin >> data ) {
    ++number_of_experiments;
    total_data += data;
    sum_of_squares += data * data;
    if ( data < min_data )
      min_data = data;
    if ( data > max_data )
      max_data = data;
  }
  double mean = total_data / number_of_experiments;
  double standard_deviation = sqrt( sum_of_squares / number_of_experiments
                                   - mean * mean );
  cout << min_data << "\t" << mean << "\t" << max_data << "\t"
        << standard_deviation << endl; 
}

//  [Last modified: 2007 08 08 at 19:24:20 GMT]