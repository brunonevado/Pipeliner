//
//  sfscode.cpp
//  Pipeliner
//
//  Copyright (c) 2013 Bruno Nevado. All rights reserved.
//

#include <fstream>
#include <vector>
#include <cstring>

#include "sfscode.h"

fasta sfs2fas ( string infile, string outfile,
               unsigned int npops,
               unsigned int ninds_ppop,
               unsigned int rep_num1,
               bool verbose
               ){
    
    ifstream filehandle;
    filehandle.open(infile.c_str());
    if(!filehandle.is_open()){
        cerr << "ERROR (sfs2fas vcf): Unable to open infile " << infile << "\n" ;
        exit(1);
    }
    
    vector <string> mutations;
    string cline;
    unsigned int crep = 0, clocus = 0;
    bool get = false;
    // read all data for selected replicate
    while( getline(filehandle, cline) ){
        if(clocus > 1){
            cout << "WARNING (sfs2fas): More than 1 loci found in infile (only first locus considered)\n";
            break;
        }
        if( strncmp(cline.c_str(), "//iteration", 3) == 0){
            crep++;
            if (crep == rep_num1)
                get = true;
            else if ( crep > rep_num1 )
                break;
            
            continue;
        }
        if(get){
            if( strncmp(cline.c_str(), ">", 1) == 0){
                clocus++;
                continue;
            }
            if( strncmp(cline.c_str(), "Nc:", 3) == 0 || strncmp(cline.c_str(), "MALES:", 3) == 0 ){
                continue;
            }
            vector<string> fields;
            msplit( cline , ";",  &fields );
            for(unsigned int i = 0; i < fields.size(); i++){
                if( fields.at(i).size() > 0 )
                    mutations.push_back(fields.at(i));
            }
        }
    }
    filehandle.close();
    
    if(crep < rep_num1){
        cout << "ERROR (sfs2fas): replicate number to fetch (" << rep_num1 << ") out of bounds (found " << crep << " replicates in infile "
        << infile << ")" << endl;
        exit(1);
    }
    
    
    if(verbose)
        cout << "  File " << infile << " read, fetched replicate " << rep_num1 << ", found "
        << mutations.size() - 1 << " mutations (length of simulated region: "
        << mutations.at(0).size() << ")" <<  endl;
    
    // build ancestral fasta for selected replicate
    fasta afasta( npops * ninds_ppop * 2 );
    string name = "replicate " + to_string(static_cast<long long> ( rep_num1 ) ) + " from infile " + infile;
    afasta.set_infile(infile);
    for(unsigned int ipop = 0; ipop < npops; ipop++){
        for( unsigned int ihaplo = 0; ihaplo < ninds_ppop * 2 ; ihaplo++){
            string name = "it" + to_string ( static_cast<long long> (rep_num1 - 1))
            + "pop" + to_string ( static_cast<long long> (ipop))
            + "ind" + to_string ( static_cast<long long> (ihaplo))
            + "locus0";
            afasta.append_from_vector(mutations.at(0), name);
        }
    }
    // apply mutations
    for (unsigned int mutation = 1; mutation < mutations.size(); mutation++) {
        // loops starts at 1 because 0 contains fasta sequence
        vector<string> fields;
        msplit( mutations.at(mutation), ",",  &fields );
        //std::cout << " MUTATION AT : " <<  fields.at(2).c_str() << endl;
        unsigned int site = atoi (fields.at(2).c_str()); // site where mutation occurs, 0-based
        char alternative = fields.at(6).at(0);
        
        for (unsigned int mutant = 12; mutant < fields.size() ; mutant++) {
            vector<string> fields2;
            msplit( fields.at(mutant), ".",  &fields2 );
            // fields2 contains popindex, affected_chr
            if( fields2.at(1) == "-1" ){
                // population is fixed for alternative
                unsigned int istart = ( atoi (fields2.at(0).c_str()) * ninds_ppop * 2);
                unsigned int iend = istart + ninds_ppop * 2;
                for (unsigned int ind = istart ; ind < iend; ind++ ) {
                    afasta.mask_base( site + 1 , alternative, ind+1 ); // sites are 1-based in mask_base
                }
            }
            else{
                unsigned int ind = ( atoi (fields2.at(0).c_str()) * ninds_ppop * 2 ) + atoi (fields2.at(1).c_str());
                afasta.mask_base( site + 1 , alternative, ind+1 ); // sites are 1-based in mask_base
            }
        }
    }
    return afasta;
    
    
}