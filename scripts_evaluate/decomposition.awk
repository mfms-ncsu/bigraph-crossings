#: decomposition.awk - an awk program for creating the DeltaGraph input for
#:    the decomposition diagram representation of a particular bigraph
#:    class (use with csh script decomposition)
# Matt Stallmann, 29 May 2000

# INPUT: two tab-delimited columns, with column 1 = treatment name and
#        column 2 = crossing number (min or average)
# OUTPUT: 9 columns as follows: an index column with numbers 1-5, 6 columns
#         representing the lines of the diagram, a column for dot,
#         and a column for tr23.

{
  if ( $1 ~ /TR23/ ) $1 = "TR23";
  value[$1] = $2;
}

END {
  printf "2\t%g\n", value["TR09"];
  printf "1\t%g\t%g\n", value["TR03"], value["TR03"];
  printf "3\t%g\t%g\t%g\t\t\t\t%g\t%g\n", value["TR17"],\
	 value["TR00"], value["TR07"], value["DOT"], value["TR23"];
  printf "2\t\t%g\t%g\t%g\n", value["TR01"], value["TR01"], value["TR09"];
  printf "4\t\t\t%g\t%g\t%g\n", value["TR15"], value["TR06"], value["TR06"];
  printf "3\t\t\t\t%g\t%g\t%g\n", value["TR07"], value["TR00"], value["TR17"];
  printf "5\t\t\t\t\t%g\t%g\n", value["TR14"], value["TR14"];
  printf "4\t\t\t\t\t\t%g\n", value["TR15"];
}
