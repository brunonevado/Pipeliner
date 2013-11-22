//
//  vcf.h
//  Pipeliner
//
//  Copyright (c) 2013 Bruno Nevado. All rights reserved.
//

#ifndef __Pipeliner__vcf__
#define __Pipeliner__vcf__

#include <iostream>
#include <vector>
#include <fstream>

#include "common.h"
#include "fasta.h"

using namespace std;
class fasta;
class ms;

// VCF  CLASS

class vcf {
    int inds;
    unsigned int lines_processed;
    bool ignoremh; // if true, will only consider first alternative allele - if best PL is other than RR, RA1 or A1A1, will code missing
    vector <string> header;
    string infile;
    vector <string> names;
    vector <string> matrix;
    vector < vector < unsigned int  >  > indel_sites,  ambiguous_sites;
    vector <string> get_genotypes(string cline);
    vector <int> positions;
    string chromosome;
public:
    vcf( string infile, bool ignore_mh );
    string get_names( int index ) { return names.at(index) ;};
    int num_inds () {return inds;}
    void info_to_stdout();
    vector < vector < unsigned int  >  > give_ambiguous () { return ambiguous_sites; };
    vector < vector < unsigned int  >  > give_indels () { return indel_sites; };
    void slurp_file();
    vector <int> give_positions () { return positions; } ;
    string give_infile (){ return infile; };
    
    friend class fasta;
    
};


#endif /* defined(__Pipeliner__vcf__) */
