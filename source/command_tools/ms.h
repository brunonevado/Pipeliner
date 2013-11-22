//
//  ms.h
//  Pipeliner
//
//  Copyright (c) 2013 Bruno Nevado. All rights reserved.
//

#ifndef __Pipeliner__ms__
#define __Pipeliner__ms__

#include <iostream>
#include <vector>

#include "common.h"

using namespace std;


// ms DATASET CLASS

class fasta;

class ms {
    vector <string> matrix;
    vector <double> positions;
    vector <string> header;
    enum { num_dec_for_output = 6 };
    string infile;
public:
    ms( int );
    unsigned long num_lines () const {return matrix.size();}
    unsigned long num_seg_sites () const {return positions.size();}
    
    string input_file (){return infile;}
    void info_to_stdout();
    void read_ms_file ( string in ) ;
    void write_to_file ( string out );
    void remove_multi_hits( unsigned long len );
    friend class fasta;
};




#endif