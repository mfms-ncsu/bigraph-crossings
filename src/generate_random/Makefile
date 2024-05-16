## Makefile for programs generating random bipartite graphs in .dot format
## (Uses GNU-style syntax, also works on Sun OS)
## @author Matt Stallmann, 25 Jun 1999
## $Id$

##     Copyright (C) 2001,2007  Matthias Stallmann.
##     Contact: matt_stallmann@ncsu.edu
##
##     This program is free software; you can redistribute it and/or modify
##     it under the terms of the GNU General Public License as published by
##     the Free Software Foundation; either version 2 of the License, or
##     (at your option) any later version.
##
##     This program is distributed in the hope that it will be useful,
##     but WITHOUT ANY WARRANTY; without even the implied warranty of
##     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
##     GNU General Public License for more details.
##
##     You should have received a copy of the GNU General Public License along
##     with this program (file COPYING.txt); if not, write to the Free Software
##     Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301
##     USA.

CC				= g++
CFLAGS        = -c -g -O3 -Wall
#CFLAGS      = -c -g -Wall -DDEBUG

# object files common to all generators
OBJECTS = Graph.o IntList.o IntNode.o IntIterator.o dot.o ord.o

# object files used for command-line processing
CMD_OBJECTS = CmdLine.o

# header files common to all heuristics
HEADERS = Graph.hxx IntList.hxx IntNode.hxx IntIterator.hxx\
          dot.h ord.h Rand.hxx defs.h

# header files used for command-line processing
CMD_HEADERS = CmdLine.h

#.SUFFIXES: .cxx.o
#.cxx.o: ; $(CC) $(CFLAGS) $*.cxx
#.cpp.o: ; $(CC) $(CFLAGS) $*.cpp

all: randomBiGraph

randomBiGraph: randomBiGraph.o $(OBJECTS) $(CMD_OBJECTS) Makefile\
; $(CC) -o randomBiGraph randomBiGraph.o $(OBJECTS) $(CMD_OBJECTS)

randomBiGraph.o: randomBiGraph.cxx $(HEADERS) $(CMD_HEADERS) Makefile\
; $(CC) $(CFLAGS) randomBiGraph.cxx 

Graph.o: Graph.cxx Graph.hxx IntList.hxx IntNode.hxx IntIterator.hxx Makefile\
; $(CC) $(CFLAGS) Graph.cxx

dot.o: dot.cxx dot.h Makefile\
; $(CC) $(CFLAGS) dot.cxx

ord.o: ord.cxx ord.h Makefile\
; $(CC) $(CFLAGS) ord.cxx

IntNode.o: IntNode.cxx IntNode.hxx Makefile\
; $(CC) $(CFLAGS) IntNode.cxx

IntList.o: IntList.cxx IntList.hxx IntNode.hxx Makefile\
; $(CC) $(CFLAGS) IntList.cxx

IntIterator.o: IntIterator.cxx IntIterator.hxx IntList.hxx IntNode.hxx\
               Makefile\
; $(CC) $(CFLAGS) IntIterator.cxx

CmdLine.o: CmdLine.cpp CmdLine.h Makefile\
; $(CC) $(CFLAGS) CmdLine.cpp

clean: ; rm -f *.o *~ randomBiGraph
