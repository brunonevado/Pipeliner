//
//  ms.cpp
//  Pipeliner
//
//  Copyright (c) 2013 Bruno Nevado. GNU license.
//


#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <cstring>

#include "ms.h"


ms::ms ( int a ){
    matrix.reserve(a);
    positions.reserve(a);
    
}


void ms::info_to_stdout () {
    
    cout << "Infile: " << this->input_file() << " (" << this->num_lines() << " individuals, " <<
    this->num_seg_sites() << " segregating sites)\n";
    
}

// READ MS FROM FILE (SINGLE DATA MATRIX)

void ms::read_ms_file ( string in ){
    
    vector <string> readfile;
    infile = in;
    string line;
    ifstream infile (in.c_str());
    if (!infile.is_open()){
        cerr << "ERROR (read_ms_file): Unable to open for reading file " << in << "\n";
        exit(1);
    }
    while( !infile.eof() ){
        getline(infile, line);
        readfile.push_back(line);
    }
    
    // CHECKS
    if( strncmp( readfile[5].c_str(), "positions: ", 11 ) != 0 ){
        cerr << "ERROR (read_ms_file): ms infile " << in << " line 6 seems malformatted (should start with 'positions: ')\n";
        exit(1);
    }
    if( strncmp( readfile[4].c_str(), "segsites: ", 10 ) != 0 ){
        cerr << "ERROR (read_ms_file): ms infile " << in << " line 5 seems malformatted (should start with 'segsites: ')\n";
        exit(1);
    }
    
    // DONE CHECKS. fill header, positions & matrix
    
    header.push_back(readfile[0].c_str());
    header.push_back(readfile[1].c_str());
    header.push_back(readfile[2].c_str());
    header.push_back(readfile[3].c_str());
    
    stringstream add_filename;
    add_filename << in;
    string new_head = header[0] + ". File modified from " + add_filename.str() + ".";
    header[0] = new_head;
    vector <string> ssites;
    msplit(readfile[5].c_str(), " ", &ssites);
    double a = 10, b= -num_dec_for_output, c = pow(a,b); // this is to shift mutations in position 0 to position 1 (below)
    for( unsigned int i = 1; i < ssites.size(); i++){
        if (ssites[i].size() < 1) continue;
        // next line adds positions of mutations to positions vector. the conditional is to avoid mutations at site 0
        positions.push_back((atof ( ssites[i].c_str() ) == 0) ? 1 * c : atof ( ssites[i].c_str()));
    }
    
    vector <string> nsegsites;
    unsigned long ns = 0;
    msplit(readfile[4].c_str(), " ", &nsegsites);
    for( unsigned int i = 1; i < nsegsites.size(); i++){
        if (nsegsites[i].size() < 1) continue;
        ns = atof ( nsegsites[i].c_str() );
    }
    
    if ( ns != this->num_seg_sites() ){
        cerr << "ERROR (read_ms_file): number of segsites (" << ns << ") and length of positions ("
        << this->num_seg_sites() << ") does not match in infile " << in << "\n";
        exit(1);
    }
    
    for( unsigned int i = 6; i < readfile.size() ; i++){
        if (readfile[i].size() < 1 ){
            if( matrix.size() == 0 ){
                cerr << "ERROR(read_ms_file): malformatted infile " << in << "\n";
                exit(1);
            }else{
                break;
            }
        }
        else{
            matrix.push_back(readfile[i].c_str());
        }
        
    }
    
}

// WRITE MS TO FILE

void ms::write_to_file ( string out ){
    ofstream outputFile;
    outputFile.open(out.c_str());
    if( !outputFile.is_open() ){
        cerr << "ERROR (write_to_file): unable to open for output file " << out << "\n";
        exit(1);
    }
    for( unsigned int i = 0; i < header.size(); i++)
        outputFile << header[i] << "\n";
    
    outputFile << "segsites: " << this->num_seg_sites() << "\n" << "positions:";
    for ( unsigned int i = 0; i < this->num_seg_sites(); i++)
        outputFile << " " << fixed << setprecision(num_dec_for_output) << positions[i];
    
    outputFile << "\n";
    
    for( unsigned int i = 0; i < this->num_lines(); i ++)
        outputFile << matrix[i] << "\n";
    
    outputFile.close();
    
}

// CHECK AND REMOVE MULTIPLE HITS IN MS

void ms::remove_multi_hits( unsigned long dec ){
    
    unsigned int mhs = 0;
    
    for(long site = positions.size() - 2 ; site >= 0; site--){
        int a = round(positions.at(site)*dec);
        int b = round(positions.at(site+1)*dec);
        if(  a ==  b ){
            mhs++;
            positions.erase(positions.begin()+site);
            for( unsigned int ind = 0; ind < matrix.size(); ind++)
                matrix[ind].erase(matrix[ind].begin()+site);
        }
    }
    if (mhs){
        stringstream nummh;
        nummh << mhs;
        string new_head = header[0] + " Removed " + nummh.str() + " multiple hits.";
        header[0] = new_head;
        cout << "Found and removed " << mhs << " multiple hit(s)\n";
    }
}