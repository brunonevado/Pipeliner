//
//  summarise.h
//  Pipeliner
//
//  Copyright (c) 2013 Bruno Nevado. All rights reserved.
//

#ifndef __Pipeliner__summarise__
#define __Pipeliner__summarise__

#include <iostream>
#include <vector>

#include "fasta.h"
#include "vcf.h"

//class fasta;

using namespace std;

class summarise {
private:
    vector <int> total_screened, total_calls;
    // possible outcomes for homozygous SNPs
    vector < vector< int > > total_aa, aa_seen, aa_missed, aa_aa, aa_rr, aa_ra, aa_ao, aa_ro, aa_oo, aa_oo2 ;
    // possible outcomes for heterozygous SNPs
    vector < vector< int > > total_ra, ra_seen, ra_missed, ra_ra, ra_rr, ra_aa, ra_ao, ra_ro, ra_oo, ra_oo2 ;
    // possible outcomes for invar positions
    vector < int >  rr_ro, rr_oo, rr_oo2 ;
    // and ambiguous/indel calls
    vector < vector< int > > aa_amb, aa_indel, ra_amb, ra_indel;
    vector < int > invar_amb, invar_indel, total_amb;
    vector <string> tags ;
    int num_inds , len;
    // to display info in output
    vector <string> input_vcfs, input_sites;
public:
    summarise(unsigned int num_haplos, int len);
    void summarise_ind( fasta &preseq, fasta &posseq, int index0_ind,
                       vector < unsigned int  > ambiguous,
                       vector < unsigned int  > indels ,
                       string tag, string vcf_file, string sites_file, string err_file );
    void print ( string outfile, bool append );
    
};





/*
 class summarise {
 private:
 fasta original , called ;
 int ref_index_in_fas;
 public:
 summarise( fasta &input, fasta &result, int index_out );
 
 
 };
 
 */


#endif /* defined(__Pipeliner__summarise__) */
