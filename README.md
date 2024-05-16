# bigraph-crossings
    Software for Stallmann, Brglez, Ghosh: "Heuristics, Experimental..."

               SBG_Software, Version 1.3, first version released 2004/09/28
                   Bipartite Graph Crossing Minimization

     _________________________________________________________________

      *** INSTALLATION *** EXECUTION *** EXAMPLE (to illustrate use of
               scripts) *** DATA CONVENTIONS AND FORMATS ***
     _________________________________________________________________

   This is the table of contents for the software used in the experiments
   for Heuristics, Experimental Subjects, and Treatment Evaluation in
   Bigraph Crossing Minimization, by M. Stallmann, F. Brglez, and
   D. Ghosh, in JEA 2001. BibTeX entry:

   ```
   @article{2001-JEA-Stallmann,
    author  = "M. Stallmann and F. Brglez and D. Ghosh",
    title   = "{Heuristics, Experimental Subjects, and Treatment Evaluation
                  in Bigraph Crossing Minimization}",
    journal = {Journal on Experimental Algorithmics},
    year    = 2001,                                                            
    volume  = 6,                                                           
    number  = 8,                                                           
}
   ```

   All important information is available from `index.html`

   COPYRIGHT:
   This software is freely available for academic teaching and research
   under the following conditions:
   
    1. The above paper should be cited in any published work that makes
       use of the software.
    2. Any bugs or modifications should be reported to Matt Stallmann
       (mfms@ncsu.edu).
    3. Copies of the software or any part of it are not distributed for
       profit or commercial advantage.
    4. The ACM copyright notice printed below be included on the home
       page of any distribution that includes this software.
     _________________________________________________________________

   ACM Copyright Notice:
   Permission to make digital or hard copies of part or all of this work
   for personal or classroom use is granted without fee provided that
   copies are not made or distributed for profit or commercial advantage
   and that copies bear this notice and the full citation on the first
   page. Copyrights for componenets of this work owned by others than ACM
   must be honored. Abstracting with credit is permitted. To copy
   otherwise, to republish, to post on servers or to redistribute to
   lists, requires prior specific permission and/or a fee.
     _________________________________________________________________

  Subdirectories

     * bin - where binary executables are stored after installation,
       unless otherwise specified; all executables have names beginning
       with the prefix sbg_ (first initials of the authors of the paper).
       Before installation, this directory should be empty.
     * data - where experimental data and results are stored; this can be
       modified by setting the DATA_DIRECTORY environment variable before
       running the appropriate scripts (see README files in the
       subdirectories cotaining scripts) -- initially empty.
     * DATA_SUMMARIES - where summary data from the JEA paper is stored.
     * processed - contains data summary files and other files that can
       be used to generate charts with standard spreadsheet programs (see
       README in scripts_evaluate for more details) -- initially empty.
     * scripts_generate - scripts that generate bigraph classes for the
       experiments.
     * scripts_execute - scripts that run a specified treatment(s) on
       specified bigraph classes.
     * scripts_evaluate - scripts that collect experimental data and
       package it in a form that can be used by charting software.
     * src - source code for the binary executables.
     _________________________________________________________________
