#: confidence.awk - awk script to generate 95% confidence intervals from data
#:                  where each row has the form:
#:                    label min mean max std_deviation
#:                  number of data points must be deduced from the label
#:                  (it's 64 unless label is tr??_N where N is the number)

BEGIN {
	 size = 64;						  # standard size
#   size = 100;                 # size used to replicate Junger-Mutzel results

	 # fill in t_table for most commonly used sizes
	 t_table[ 8 ] = 2.31;
	 t_table[ 10 ] = 2.23;
	 t_table[ 64 ] = 2.00;
	 t_table[ 100 ] = 1.98;
}

{
	 label = $1;
	 mean = $3;
	 std_deviation = $5;
	 local_size = size;
	 if ( label == "tr23" ) {			  # other class size for tr23
		  local_size = sqrt(size);
	 }

	 # calculate difference on each side of mean
	 if ( t_table[ local_size ] == 0 ) {
		  print "ERROR: bad class size for ", $1, $2, $3, $4, $5;
	 }
	 diff = t_table[ local_size ] * std_deviation / sqrt( local_size );
	 printf "%s\t%6.2f\t%6.2f\n", label, mean - diff, mean + diff;
}
