//
//  sfscode.h
//  Pipeliner
//
//  Copyright (c) 2013 Bruno Nevado. GNU license.
//

#ifndef __Pipeliner__sfscode__
#define __Pipeliner__sfscode__

#include <iostream>
#include "fasta.h"
#include "common.h"

// converts replicate number rep_num1 to fasta format
fasta sfs2fas ( string infile, string outfile,
               unsigned int npops,
               unsigned int ninds_ppop,
               unsigned int rep_num1,
               bool verbose = true
               );

#endif /* defined(__Pipeliner__sfscode__) */
