##: Makefile for programs related to edge crossing minimization
## version 1.08 (the version used for the JEA paper)
## (Uses GNU-style syntax, also works on Sun OS)
## @author Matt Stallmann, 29 Jun 1999
## $Id: Makefile 50 2007-07-27 21:32:23Z mfms $

# Copyright (C) 2001,2007  Matthias Stallmann.
# Contact: matt_stallmann@ncsu.edu
#
#  This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#  This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License along
# with this program (file COPYING.txt); if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301
# USA.

#  3 Jan 2000 - added computation of a lower bound to gstats
# 18 Mar 2001 - added time2, program that times iterative improvement heuristics
# 29 Jul 2002 - added dot2dre, a program that converts .dot files to
# .dre format (for the dreadnaught graph isomorphism program)

# fstats: a quick and dirty program for getting min, average, max, and
#         standard deviation from a collection of data
#
# gstats: a program that gathers statistics about a bigraph
#
# sbg_cn_eval: a simple program that counts the number of crossings in a dot
#          file given the ordering specified in an ord file
#
# sbg_treat: main program for all treatments for the JEA paper (see
#          comments in heuristic.cpp for optional command-line arguments)
#           Usage: sbg_treat -tr=<treatment> <dot file> <ord file>
#
# dot2dre: reads a .dot file and outputs a corresponding .dre file
# (dreadnaught format for graph isomorphism testing).

# The following are deprecated:
#
# exp2: experimental program for trying out varying numbers of iterations on
#       adaptive insertion and getting convergence data (args like exp2)
#
# time2: like sbg_treat, except that it outputs timing data about iterative
#        improvement phase and does not create other output

DFLAGS =								  # used to add debugging flags
CC	= g++
CFLAGS = -c -g -O3 -Wall $(DFLAGS) # use these for efficient execution
#CFLAGS      = -c -g -Wall -DTRACE # use these when debugging and testing

# all programs that can be created
PROGRAMS = sbg_cn_eval sbg_treat fstats gstats dot2dre

# object files common to all heuristics
OBJECTS = BiGraph.o IntSet.o Sums.o dot.o ord.o

# header files common to all heuristics
HEADERS = BiGraph.hxx IntSet.h dot.h ord.h

.SUFFIXES: .cxx
.cxx.o: ; $(CC) $(CFLAGS) $*.cxx
.cpp.o: ; $(CC) $(CFLAGS) $*.cpp

all: $(PROGRAMS)

fstats: fstats.o; $(CC) -o fstats fstats.o

gstats: gstats.o tree_lower_bound.o $(OBJECTS) CmdLine.o Makefile\
; $(CC) -o gstats gstats.o tree_lower_bound.o $(OBJECTS) CmdLine.o

sbg_cn_eval: sbg_cn_eval.o $(OBJECTS) Makefile\
; $(CC) -o sbg_cn_eval sbg_cn_eval.o $(OBJECTS)

sbg_treat: heuristic.o bfs_heuristic.o plain_bfs.o adaptive_insert.o\
			  SortingHeuristic.o $(OBJECTS) CmdLine.o Makefile\
; $(CC) -o sbg_treat heuristic.o plain_bfs.o bfs_heuristic.o\
   adaptive_insert.o SortingHeuristic.o $(OBJECTS) CmdLine.o

dot2dre: dot2dre.o $(OBJECTS) Makefile\
; $(CC) -o dot2dre dot2dre.o $(OBJECTS)

# exp2 and time2 are used only for experimentation
exp2: exp2.o bfs_heuristic.o plain_bfs.o adaptive_insert.o\
			  $(OBJECTS) CmdLine.o\
; $(CC) -o exp2 exp2.o plain_bfs.o bfs_heuristic.o\
   adaptive_insert.o SortingHeuristic.o $(OBJECTS) CmdLine.o

time2: time2.o bfs_heuristic.o plain_bfs.o adaptive_insert.o\
			  SortingHeuristic.o $(OBJECTS) CmdLine.o\
; $(CC) -o time2 time2.o plain_bfs.o bfs_heuristic.o\
   adaptive_insert.o SortingHeuristic.o $(OBJECTS) CmdLine.o

# source dependencies

fstats.o: fstats.cxx Makefile

gstats.o: gstats.cxx $(HEADERS) CmdLine.h Makefile

sbg_cn_eval.o: sbg_cn_eval.cxx $(HEADERS) Makefile

dot2dre.o: dot2dre.cxx $(HEADERS) Makefile

BiGraph.o: BiGraph.cxx IntSet.h Sums.hxx $(HEADERS) defs.h Makefile

IntSet.o: IntSet.cpp IntSet.h defs.h Makefile

Sums.o: Sums.cxx Sums.hxx Makefile

dot.o: dot.cxx dot.h defs.h Makefile

ord.o: ord.cxx ord.h defs.h Makefile

plain_bfs.o: plain_bfs.cxx plain_bfs.h Queue.hxx Makefile

bfs_heuristic.o: bfs_heuristic.cxx bfs_heuristic.h Queue.hxx Makefile

adaptive_insert.o: adaptive_insert.cxx adaptive_insert.h $(HEADERS) Makefile

SortingHeuristic.o: SortingHeuristic.cpp SortingHeuristic.h $(HEADERS) Makefile

CmdLine.o: CmdLine.cpp CmdLine.h Makefile

heuristic.o: heuristic.cpp SortingHeuristic.h Median.h Barycenter.h\
	adaptive_insert.h plain_bfs.h bfs_heuristic.h defs.h\
	 $(HEADERS) CmdLine.h Makefile

# !!! the next two don't have all the dependencies right
exp2.o: heuristic.cpp SortingHeuristic.h adaptive_insert.h plain_bfs.h\
		  bfs_heuristic.h defs.h $(HEADERS) CmdLine.h Makefile\
; $(CC) $(CFLAGS) -DTRACK_ITERATIONS -o exp2.o heuristic.cpp

time2.o: heuristic.cpp SortingHeuristic.h adaptive_insert.h plain_bfs.h\
		   bfs_heuristic.h defs.h Timer.hxx $(HEADERS) CmdLine.h Makefile\
; $(CC) $(CFLAGS) -DTIMING -o time2.o heuristic.cpp

clean: ; rm -f *.o *~ $(PROGRAMS)
