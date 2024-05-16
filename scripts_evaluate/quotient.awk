#: quotient.awk - transforms two fields to one as $1/$2 (x if undefined, 1
#: in the special case of 0/0)

# Matt Stallmann, 01 Mar 2000

{
  if ( $2 ) q1 = $1 / $2;
  else if ( $1 ) q1 = "x";
  else q1 = 1;
  print q1;
}
