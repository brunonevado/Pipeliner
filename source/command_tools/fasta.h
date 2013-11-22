//
//  fasta.h
//  Pipeliner
//
//  Copyright (c) 2013 Bruno Nevado. All rights reserved.
//

#ifndef __Pipeliner__fasta__
#define __Pipeliner__fasta__

#include <iostream>
#include <vector>
#include <string>

#include "vcf.h"
#include "ms.h"


using namespace std;
class vcf;
class ms;

// FASTA DATASET CLASS

class fasta {
    vector <string> matrix;
    vector <string> names;
    string infile;
public:
    fasta( int num_inds, int len = 0 );
    unsigned int num_lines () const {return int ( matrix.size() );}
    unsigned int num_bases () const {return int ( matrix[0].size() );}
    unsigned int num_names () const {return int ( names.size() );}
    void set_num_inds ( unsigned int value ) { matrix.resize(value); }
    void resize_matrix( unsigned int start, unsigned int end );
    void new_fasta_from_inds ( fasta &infasta, vector <int> index0 );
    void mask_base ( unsigned int site1, char newchar, int index1 = - 1 );
    int is_aligned();
    void set_infile (string title) { infile = title; }
    string input_file () const {return infile;}
    void outputSNPfiles ( int, string );
    void info_to_stdout();
    void read_fasta_file ( string in ) ;
    void write_random ( string out, string name, unsigned int len );
    void write_to_file ( string out, int append = 0 );
    void remove_multi_hits();
    int  calc_dac ( char allele , unsigned int position1 );
    void build_from_ngh( unsigned int site, string col_to_add );
    void build_from_ms ( const ms& inms, const fasta& ancestral );
    void build_from_vcf (const vcf& obj, const fasta& reference, string sites_file, string name, int column, int ind0 );
    void append_from_vector ( string in, string name ) { matrix.push_back(in) ; names.push_back(name); }
    void append_seq (fasta in , bool fill = false);
    void forkicks ();
    char show_base( unsigned int line0, unsigned int site0 ) { return matrix.at(line0).at(site0);} ;
    string show_seq ( int n ) { return matrix.at(n);};
    void set_all_names_to ( string in ) { for( unsigned int i = 0; i < names.size(); i++ ){ names.at(i) = in; } };
    int concatenate_alignments ( fasta & al2 );
    friend class ms;
    friend class vcf;
    friend class mse;
    
};

#endif