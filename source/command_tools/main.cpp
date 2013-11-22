//
//  main.cpp
//  Pipeliner
//
//  Copyright (c) 2013 Bruno Nevado. All rights reserved.
//
// for linux, compile with g++ -Wall -g -std=c++0x -pthread -O3  *.cpp -o Pipeliner


#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h> // rand, srand
#include <time.h>
#include <cstring>
#include <thread>
#include <unordered_map>

#include "fasta.h"
#include "ms.h"
#include "common.h"
#include "vcf.h"
#include "summarise.h"
#include "sfscode.h"
#include "ngh.h"

using namespace std;

// definitions

int writeRandomSeq(vector<string> * ,  vector<string> *);

int desinterleave ( vector<string> * ,  vector<string> * );

int ms2fas ( vector<string> * ,  vector<string> * );

int vcf2fas ( vector<string> * ,  vector<string> * );

int resizeFasta  ( vector<string> * ,  vector<string> * );

int getAncestral ( vector<string> * ,  vector<string> * );

int seedMS ( vector<string> * ,  vector<string> * );

int maskReference ( vector<string> * ,  vector<string> * );

int summarisePipeline ( vector<string> * ,  vector<string> * );

int sfs2fas ( vector<string> * ,  vector<string> * );

int extract_gff_from_fas ( vector<string> * ,  vector<string> * );

int nghcaller ( vector<string> * ,  vector<string> * );


// MAIN

int main(int argc, char* argv[])
{
    string version = "0.2.0 20112013";
    // 20112013 : added outcome rr_ro to summarisePipeline
    // 24102013 : added option to ignore multi-allelic SNPs in input vcf (if set, will consider only RR, RA1 and A1A1)
    vector <string> args;
    vector <string> values;
    //cout << "Welcome to Pipeliner.c++\n";
    
    if( argc == 1 ){
        cerr << "ERROR (" << argv[0] << "): called without command? Use 'Pipeliner help' for info\n";
        exit(1);
    }
    else if ( !strncmp (argv[1] , "help", 3 ) ){
        help();
        cout << " Pipeliner command tools version " << version << endl;
    }
    else if ( argc == 2 || argc % 2 ){
        cout << "ERROR (" << argv[0] << "): arguments missing / wrong? Use 'Pipeliner help' for info\n";
        exit(1);
        
    }
    else {
        // seed the random number generator for whichever function.
        unsigned int seed = (unsigned)time(NULL);
        cout << "Running Pipeliner.c++. Random seed for the run (if appropriate): " << seed << "\n";
        srand(seed);
        
        for( int i = 2; i < argc; i += 2 )
        {
            args.push_back(argv[i]);
            values.push_back(argv[i+1]);
        }
        
        if(  strncmp (argv[1] , "writeRandomSeq", 5 ) == 0 ){
            writeRandomSeq( &args, &values );
        }
        else if(  strncmp (argv[1] , "desinterleave", 5 ) == 0 ){
            desinterleave( &args, &values );
        }
           else if(  strncmp (argv[1] , "ms2fas", 5 ) == 0 ){
            ms2fas( &args, &values );
        }
        else if(  strncmp (argv[1] , "vcf2fas", 5 ) == 0 ){
            vcf2fas( &args, &values );
        }
        else if(  strncmp (argv[1] , "nghcaller", 5 ) == 0 ){
            nghcaller(&args, &values);
        }
        else if(  strncmp (argv[1] , "resizeFasta", 5 ) == 0 ){
            resizeFasta( &args, &values );
        }
        else if(  strncmp (argv[1] , "getAncestralSeq", 5 ) == 0 ){
            getAncestral(&args, &values);
        }
        else if(  strncmp (argv[1] , "seedMS", 5 ) == 0 ){
            seedMS(&args, &values);
        }
        else if(  strncmp (argv[1] , "maskReference", 5 ) == 0 ){
            maskReference(&args, &values);
        }
        else if(  strncmp (argv[1] , "summarisePipeline", 5 ) == 0 ){
            summarisePipeline(&args, &values);
        }
        else if(  strncmp (argv[1] , "sfs2fas", 5 ) == 0 ){
            sfs2fas(&args, &values);
        }
        else if(  strncmp (argv[1] , "extract_gff", 5 ) == 0 ){
            extract_gff_from_fas(&args, &values);
        }
       else
        {
            cout << "ERROR (" << argv[0] << "): Fail to understand command " << argv[1] <<"\n";
            exit(1);
        }
        
    }
    return (0);
}


// FUNCTIONS

int writeRandomSeq (vector<string> *inargs, vector<string> *invalues ){
    
    // CHECK ARGS
    
    string outfile = "NA", name = "NA";
    unsigned int len = 0;
    bool verbose = false;
    
    for( unsigned int i = 0; i <  inargs->size()  ; i++ ){
        if( strncmp (inargs->at(i).c_str(), "-", 1) ){
            cout << "ERROR (writeRandomSeq): fail to understand argument " << inargs->at(i) << "\n";
            exit(1);
        }
        else if( !strncmp (inargs->at(i).c_str(), "-outfile", 2) ){
            outfile = invalues->at(i).c_str();
        }
        else if(!strncmp (inargs->at(i).c_str(), "-name", 2)){
            name = invalues->at(i).c_str();
        }
        else if(!strncmp (inargs->at(i).c_str(), "-length", 2)){
            len = atoi(invalues->at(i).c_str());
        }
        else if(!strncmp (inargs->at(i).c_str(), "-verbose", 3) ){
            verbose = stoi(invalues->at(i).c_str());
        }
        
        else{
            cout << "ERROR (writeRandomSeq): fail to understand argument value pair: " << inargs->at(i) << " " << invalues->at(i) << "\n";
        }
    }
    
    if(outfile == "NA"){
        cout << "ERROR(writeRandomSeq): outfile missing\n";
        exit(1);
    }
    if(name == "NA"){
        name = "ancestral";
        
    }
    if(len == 0){
        cout << "ERROR(writeRandomSeq): sequence length missing\n";
        exit(1);
        
    }
    // ARGS OK
    
    cout << "  Running writeRandomSeq\n";
    
    if(verbose){
        cout << "  Output file: " << outfile << "; Sequence name: " << name << "; Sequence length: " << len << "\n";
    }
    fasta afasta (1);
    
    afasta.write_random(outfile, name, len);
    
    cout << "  Done writing ancestral sequence to " << outfile << "\n";
    return 0;
}

int desinterleave (vector<string> *inargs, vector<string> *invalues ){
    
    // CHECK ARGS
    
    string infile = "NA", outfile = "NA", force_names = "NA";
    bool verbose = false;
    
    for(unsigned int i = 0; i <  inargs->size() ; i++ ){
        if( strncmp (inargs->at(i).c_str(), "-", 1) ){
            cout << "ERROR (desinterleave): fail to understand argument " << inargs->at(i) << "\n";
            exit(1);
        }
        else if( !strncmp (inargs->at(i).c_str(), "-outfile", 2) ){
            outfile = invalues->at(i).c_str();
        }
        else if(!strncmp (inargs->at(i).c_str(), "-infile", 2)){
            infile = invalues->at(i).c_str();
        }
        else if(!strncmp (inargs->at(i).c_str(), "-verbose", 2) ){
            verbose = stoi(invalues->at(i).c_str());
        }
        else if(!strncmp (inargs->at(i).c_str(), "-force_names", 3) ){
            force_names = invalues->at(i).c_str();
        }
        
        else{
            cout << "ERROR (desinterleave): fail to understand argument value pair: " << inargs->at(i) << " " << invalues->at(i) << "\n";
        }
    }
    
    if(outfile == "NA"){
        cout << "ERROR(desinterleave): outfile missing\n";
        exit(1);
    }
    if(infile == "NA"){
        cout << "ERROR(desinterleave): sequence name missing\n";
        exit(1);
        
    }
    
    // ARGS OK
    
    cout << "  Running desinterleave\n";
    
    fasta afasta (10);
    afasta.read_fasta_file(infile);
    if(verbose){
        cout << "  Input file: " << infile << "; Output file: " << outfile << "\n  ";
        afasta.info_to_stdout();
    }
    
    int aligned = afasta.is_aligned();
    if( aligned != 0){
        cout << "WARNING (doSNPfiles): infile " << afasta.input_file() << " does not seem "
        << "to be aligned (offending sequence: " << aligned +1 << ")\n";
    }
    if (force_names != "NA" ) {
        if( verbose )
            cout << "  setting all names to " << force_names << endl;
        
        afasta.set_all_names_to(force_names);
    }

    afasta.write_to_file(outfile);
    
    
    cout << "  Desinterleaved fasta file written to " << outfile << "\n";
    return 0;
}

int ms2fas (vector<string> *inargs, vector<string> *invalues ) {
    
    // CHECK ARGS
    
    string in_ms = "NA", out_ms = "NA", in_anc = "NA", out_fas = "NA", force_names = "NA";
    bool verbose = false;
    
    for(unsigned int i = 0; i < inargs->size() ; i++ ){
        if( strncmp (inargs->at(i).c_str(), "-", 1) ){
            cout << "ERROR (ms2fas): fail to understand argument " << inargs->at(i) << "\n";
            exit(1);
        }
        else if( !strncmp (inargs->at(i).c_str(), "-in_ms", 6) || !strncmp (inargs->at(i).c_str(), "-msin", 5) ){
            in_ms = invalues->at(i).c_str();
        }
        else if(!strncmp (inargs->at(i).c_str(), "-in_anc", 6) || !strncmp (inargs->at(i).c_str(), "-ancestral", 4)){
            in_anc = invalues->at(i).c_str();
        }
        else if(!strncmp (inargs->at(i).c_str(), "-out_fas", 6) || !strncmp (inargs->at(i).c_str(), "-outfas", 6)){
            out_fas = invalues->at(i).c_str();
        }
        else if(!strncmp (inargs->at(i).c_str(), "-out_ms", 6) || !strncmp (inargs->at(i).c_str(), "-outms", 6)){
            out_ms = invalues->at(i).c_str();
        }
        else if(!strncmp (inargs->at(i).c_str(), "-verbose", 2) ){
            verbose = stoi(invalues->at(i).c_str());
        }
        else if(!strncmp (inargs->at(i).c_str(), "-force_names", 3) ){
            force_names = invalues->at(i).c_str();
        }
      
        else{
            cout << "ERROR (ms2fas): fail to understand argument value pair: " << inargs->at(i) << " " << invalues->at(i) << "\n";
        }
    }
    
    if(in_ms == "NA"){
        cout << "ERROR(ms2fas): ms infile missing\n";
        exit(1);
    }
    if(in_anc == "NA"){
        cout << "ERROR(ms2fas): ancestral fasta file missing\n";
        exit(1);
    }
    if(out_fas == "NA"){
        cout << "ERROR(ms2fas): output fasta file missing\n";
        exit(1);
        
    }
    
    // ARGS OK
    
    cout << "  Running ms2fas\n";
    
    fasta ancestralfasta (1);
    ancestralfasta.read_fasta_file(in_anc);
    if(verbose){
        cout << "  Ancestral: ";
        ancestralfasta.info_to_stdout();
    }
    
    ms ams (10); // the 10 is to reserve space for matrix. does it matter?
    ams.read_ms_file(in_ms);
    
    if(verbose){
        cout << "  ms: ";
        ams.info_to_stdout();
    }
    ams.remove_multi_hits( ancestralfasta.num_bases() );
    
    
    fasta newfasta ( int (ams.num_lines() ));
    newfasta.build_from_ms ( ams, ancestralfasta );
    
    int aligned = newfasta.is_aligned();
    if( aligned != 0){
        cout << "ERROR (ms2fas): Error converting to ms infile " << in_ms << "\n";
        exit(1);
    }
    
    if (force_names != "NA" ) {
        if( verbose )
            cout << "  setting all names to " << force_names << endl;
        
        newfasta.set_all_names_to(force_names);
    }
    newfasta.write_to_file(out_fas);
    cout << "  Data converted to fasta format and written to file " << out_fas;
    if (out_ms != "NA"){
        ams.write_to_file(out_ms);
        cout << " (new ms data written to file " << out_ms << ")";
    }
    cout << "\n";
    
    return(0);
}

int vcf2fas (vector<string> *inargs, vector<string> *invalues ) {
    
    // CHECK ARGS
    
    string in_vcf = "NA", in_ref = "NA", in_sites = "NA", out_fas = "NA", names = "NA";
    int sites_col = 1, append=0, nt = 1;
    bool verbose = false, ignore_multihits = false ;
    for(unsigned int i = 0; i < inargs->size(); i++ ){
        if( strncmp (inargs->at(i).c_str(), "-", 1) ){
            cout << "ERROR (vcf2fas): fail to understand argument " << inargs->at(i) << "\n";
            exit(1);
        }
        else if( !strncmp (inargs->at(i).c_str(), "-in_vcf", 6) || !strncmp (inargs->at(i).c_str(), "-vcf", 5) ){
            in_vcf = invalues->at(i).c_str();
        }
        else if( !strncmp (inargs->at(i).c_str(), "-in_ref", 6) || !strncmp (inargs->at(i).c_str(), "-ref", 5) ){
            in_ref = invalues->at(i).c_str();
        }
        else if( !strncmp (inargs->at(i).c_str(), "-in_sites", 6) || !strncmp (inargs->at(i).c_str(), "-sites", 5) ){
            in_sites = invalues->at(i).c_str();
        }
        else if( !strncmp (inargs->at(i).c_str(), "-out_fas", 6) || !strncmp (inargs->at(i).c_str(), "-out", 5) ){
            out_fas = invalues->at(i).c_str();
        }
        else if( !strncmp (inargs->at(i).c_str(), "-names", 6)  ){
            names = invalues->at(i).c_str();
        }
        else if( !strncmp (inargs->at(i).c_str(), "-sites_col", 6) || !strncmp (inargs->at(i).c_str(), "-column", 3)  ){
            sites_col = atoi(invalues->at(i).c_str());
        }
        else if( !strncmp (inargs->at(i).c_str(), "-append", 6)  ){
            append = atoi(invalues->at(i).c_str());
        }
        else if(!strncmp (inargs->at(i).c_str(), "-threads", 2) ){
            nt = stoi(invalues->at(i).c_str());
        }
        else if(!strncmp (inargs->at(i).c_str(), "-verbose", 2) ){
            verbose = stoi(invalues->at(i).c_str());
        }
        else if(!strncmp (inargs->at(i).c_str(), "-ignoreMH", 6) ){
            ignore_multihits = stoi(invalues->at(i).c_str());
        }
      
        else{
            cout << "ERROR (vcf2fas): fail to understand argument value pair: " << inargs->at(i) << " " << invalues->at(i) << "\n";
        }
        
    }
    
    if(in_vcf == "NA"){
        cout << "ERROR(vcf2fas): vcf infile missing\n";
        exit(1);
    }
    if(in_ref == "NA"){
        cout << "ERROR(vcf2fas): reference infile missing\n";
        exit(1);
    }
    if(in_sites == "NA"){
        cout << "ERROR(vcf2fas): sites infile missing\n";
        exit(1);
    }
    if(out_fas == "NA"){
        cout << "ERROR(vcf2fas): fasta output file missing\n";
        exit(1);
    }
    
    
    vector <string> all_in_sites, all_names;
    
    msplit( in_sites, ",", &all_in_sites );
    
    // ARGS OK
    
    cout << "  Running vcf2fas (using " << nt << " thread(s) )\n";
    
    // VCF FILE
    vcf avcf(in_vcf, ignore_multihits);
    
    if( names != "NA" ){
        msplit( names, ",", &all_names );
    }
    else{
        for(int i = 0; i < avcf.num_inds(); i++){
            all_names.push_back( avcf.get_names(i) );
        }
    }
    
    if( int ( avcf.num_inds() ) != int ( all_names.size() ) || int ( avcf.num_inds() ) != int ( all_in_sites.size() ) ){
        cerr << "ERROR(vcf2fas): number of individuals in vcf file " << in_vcf << " (" << avcf.num_inds()
        << ") does not match number of sites files (" << all_in_sites.size()
        << ") or number of names provided (" << all_names.size() << ")\n";
        exit(1);
    }
    
    avcf.slurp_file();
    if(verbose){
        cout << "  Input vcf: ";
        avcf.info_to_stdout();
        if(ignore_multihits){
            std::cout << "Only first alternative allele considered!" << std::endl;
        }
    }
    
    // REFERENCE
    fasta referencefasta (1);
    referencefasta.read_fasta_file(in_ref);
    if(verbose){
        cout << "  Reference: ";
        referencefasta.info_to_stdout();
    }
    
    // NEW FASTA OBJECT
    fasta newfasta ( int(avcf.num_inds() ),  int(referencefasta.num_bases() ) );
    
    // BEGIN OF PARALLEL PART
    if(nt == 1){
        for(int i = 0; i < avcf.num_inds(); i++ ){
            newfasta.build_from_vcf( avcf , referencefasta, all_in_sites.at(i), all_names.at(i), sites_col, i );
        }
    }
    else{
        std::vector<std::thread> th;
        int num_diploids = avcf.num_inds(), cind= 0;
        int num_inds_per_th = ((num_diploids % nt )== 0) ? num_diploids /nt : (num_diploids /nt)+1;
        for(int cth = 1; cth <= nt; cth++){
            for(int launch = 1; launch <= num_inds_per_th; launch++){
                cind++;
                if( cind > num_diploids)
                    break;
                th.push_back( std::thread(&fasta::build_from_vcf,&newfasta, avcf, referencefasta, all_in_sites.at(cind-1), all_names.at(cind-1), sites_col, cind-1 ) );
               // th.push_back( std::thread(&fasta::build_from_vcf,&newfasta, avcf, referencefasta, all_in_sites.at(cind-1), all_names.at(cind-1), sites_col, cind-1) );
            }
            if( cind > num_diploids)
                break;
        }
        
        for(unsigned int ind = 0; ind <  th.size()  ; ind++)
            th.at(ind).join();
    }
    
    // END OF PARALLEL PART
    
    int aligned = newfasta.is_aligned();
    if( aligned != 0){
        cerr << "ERROR (vcf2fas): Error converting to fasta format (sites files contain non-unique values?).\n";
        exit(1);
    }
    
    newfasta.write_to_file( out_fas, append );
    cout << "  Data converted to fasta format and written to file " << out_fas << "\n";
    
    return(0);
}

int resizeFasta (vector<string> *inargs, vector<string> *invalues ){
    
    
    // CHECK ARGS
    
    string infile = "NA", outfile = "NA", aff = "NA";
    unsigned int start = 0, end = 0;
    bool verbose = false;
    for(unsigned int i = 0; i < inargs->size(); i++ ){
        if( strncmp (inargs->at(i).c_str(), "-", 1) ){
            cout << "ERROR (resizeFasta): fail to understand argument " << inargs->at(i) << "\n";
            exit(1);
        }
        else if( !strncmp (inargs->at(i).c_str(), "-infile", 3)  ){
            infile = invalues->at(i) ;
        }
        else if( !strncmp (inargs->at(i).c_str(), "-outfile", 3) ){
            outfile =  invalues->at(i);
        }
        else if( !strncmp (inargs->at(i).c_str(), "-start", 3) ){
            start =  atoi( invalues->at(i).c_str() );
        }
        else if( !strncmp (inargs->at(i).c_str(), "-end", 3) ){
            end =  atoi( invalues->at(i).c_str() );
        }
        else if( !strncmp (inargs->at(i).c_str(), "-regions", 3) ){
            aff =  invalues->at(i);
        }
        else if(!strncmp (inargs->at(i).c_str(), "-verbose", 2) ){
            verbose = stoi(invalues->at(i).c_str());
        }
        
        else{
            cout << "ERROR (resizeFasta): fail to understand argument value pair: " << inargs->at(i) << " " << invalues->at(i) << "\n";
        }
        
    }
    
    // CHECK ARGS
    if(infile == "NA"){
        cout << "ERROR(resizeFasta): input file missing\n";
        exit(1);
    }
    if(outfile == "NA" && aff == "NA"){
        cout << "ERROR(resizeFasta): output file obligatory without regions file\n";
        exit(1);
    }
    
    
    
    if( ( start == 0 && end == 0) && aff == "NA"  ){
        cout << "ERROR(resizeFasta): need start and/or end and/or regions file (at least 1 must be present)\n";
        exit(1);
    }
    
    cout << "  Running resizeFasta\n";
    fasta afasta(1);
    afasta.read_fasta_file(infile);
    
    if(verbose){
        cout <<   "  Input fasta: ";
        afasta.info_to_stdout();
    }
    
    if ( aff == "NA" ){
        if (start == 0)
            start = 1;
        
        if(end == 0)
            end = afasta.num_bases();
        
        afasta.resize_matrix(start, end);
        
        if(verbose)
            cout << "  Region to output: " << start << "-" << end << endl;
        
        //afasta.info_to_stdout();
        
        afasta.write_to_file(outfile);
        cout << "  New fasta file written to " << outfile << endl;
    }
    
    
    else{
        if(verbose)
            cout << "  Outputing regions defined in infile " << aff << endl;
        string line;
        ifstream infile_regions (aff.c_str());
        if( !infile_regions.is_open() ) {
            cout << "ERROR (resizeFasta): Unable to open for output regions file " << aff << endl;
            exit(1);
        }
        
        while (getline(infile_regions,line)) {
            
            vector <string> fields;
            fields.clear();
            msplit( line, "\t", &fields );
            
            if (fields.size() != 3)
                return 0;
            
            unsigned int tstart = atoi( fields.at(0).c_str());
            unsigned int tend = atoi( fields.at(1).c_str());
            string tempoutfile = fields.at(2);
            
            fasta atempfasta (afasta.num_lines());
            atempfasta = afasta;
            atempfasta.resize_matrix(tstart, tend);
            
            atempfasta.write_to_file(tempoutfile);
            cout << "  New fasta file in region " << tstart << ":" << tend  << " written to fasta file " << tempoutfile << endl;
        }
        
    }
    
    return 0;
}

int getAncestral (vector<string> *inargs, vector<string> *invalues ){
    
    // CHECK ARGS
    string infile = "NA", outfile = "NA";
    bool verbose = false;
    for(unsigned int i = 0; i < inargs->size(); i++ ){
        if( strncmp (inargs->at(i).c_str(), "-", 1) ){
            cout << "ERROR (getAncestralSeq): fail to understand argument " << inargs->at(i) << "\n";
            exit(1);
        }
        else if( !strncmp (inargs->at(i).c_str(), "-infile", 3)  ){
            infile = invalues->at(i) ;
        }
        else if( !strncmp (inargs->at(i).c_str(), "-outfile", 3) ){
            outfile =  invalues->at(i);
        }
        else if(!strncmp (inargs->at(i).c_str(), "-verbose", 2) ){
            verbose = stoi(invalues->at(i).c_str());
        }
        
        else{
            cout << "ERROR (getAncestralSeq): fail to understand argument value pair: " << inargs->at(i) << " " << invalues->at(i) << "\n";
        }
    }
    
    // CHECK ARGS
    if(infile == "NA"){
        cout << "ERROR(getAncestralSeq): input file missing\n";
        exit(1);
    }
    if(outfile == "NA" ){
        cout << "ERROR(getAncestralSeq): output file missing\n";
        exit(1);
    }
    cout << "  Running getAncestralSeq\n";
    fasta afasta(1);
    afasta.read_fasta_file(infile);
    if(verbose){
        cout <<   "  Input fasta: ";
        afasta.info_to_stdout();
    }
    if (afasta.num_bases() < 1){
        cerr << "ERROR(getAncestralSeq): no sequence found in input file (" << infile << ")?\n";
        exit(1);
    }
    
    if (afasta.num_lines() != 1){
        cerr << "ERROR(getAncestralSeq): input file should contain a single sequence!\n";
        exit(1);
    }
    
    afasta.write_to_file(outfile);
    cout << "  New fasta written to " << outfile << endl;
    
    return 0;
}

int seedMS (vector<string> *inargs, vector<string> *invalues ){
    
    // CHECK ARGS
    
    string folder = "NA";
    
    for(unsigned int i = 0; i < inargs->size(); i++ ){
        if( strncmp (inargs->at(i).c_str(), "-", 1) ){
            cout << "ERROR seedMS: fail to understand argument " << inargs->at(i) << "\n";
            exit(1);
        }
        else if( !strncmp (inargs->at(i).c_str(), "-folder", 3)  ){
            folder = invalues->at(i) ;
        }
        
        else{
            cout << "ERROR (seedMS): fail to understand argument value pair: " << inargs->at(i) << " " << invalues->at(i) << "\n";
        }
        
    }
    
    string out = folder + "/seedms", randnums;
    randnums +=  to_string ( static_cast<long long> ( rand()) ) + " " + to_string (static_cast<long long> (rand()) ) + " " + to_string (static_cast<long long> (rand()) );
    
    ofstream outfile;
    outfile.open(out);
    
    if( !outfile.is_open() ) {
        cerr << "ERROR (seedMS): Unable to open for output  file " << out << endl;
        exit(1);
    }
    
    outfile << randnums << endl;
    
    outfile.close();
    cout << "  ms seed written to " << out << endl;
    
    return 0;
}

int maskReference (vector<string> *inargs, vector<string> *invalues ) {
    
    // CHECK ARGS
    
    string in_vcf = "NA", in_ref = "NA", out = "NA";
    bool verbose = false;
    for(unsigned int i = 0; i < inargs->size(); i++ ){
        if( strncmp (inargs->at(i).c_str(), "-", 1) ){
            cout << "ERROR (maskReference): fail to understand argument " << inargs->at(i) << "\n";
            exit(1);
        }
        else if( !strncmp (inargs->at(i).c_str(), "-in_vcf", 6) || !strncmp (inargs->at(i).c_str(), "-vcf", 5) ){
            in_vcf = invalues->at(i).c_str();
        }
        else if( !strncmp (inargs->at(i).c_str(), "-in_ref", 6) || !strncmp (inargs->at(i).c_str(), "-ref", 5) ){
            in_ref = invalues->at(i).c_str();
        }
        else if( !strncmp (inargs->at(i).c_str(), "-out_fas", 6) || !strncmp (inargs->at(i).c_str(), "-outfile", 5) ){
            out = invalues->at(i).c_str();
        }
        else if(!strncmp (inargs->at(i).c_str(), "-verbose", 2) ){
            verbose = stoi(invalues->at(i).c_str());
        }
        
        else{
            cout << "ERROR (maskReference): fail to understand argument value pair: " << inargs->at(i) << " " << invalues->at(i) << "\n";
        }
        
    }
    
    if(in_vcf == "NA"){
        cout << "ERROR(maskReference): vcf infile missing\n";
        exit(1);
    }
    if(in_ref == "NA"){
        cout << "ERROR(maskReference): reference infile missing\n";
        exit(1);
    }
    if(out == "NA"){
        cout << "ERROR(maskReference): fasta output file missing\n";
        exit(1);
    }
    
    
    cout << "  Running maskReference\n";
    
    // VCF FILE
    vcf avcf(in_vcf, false );
    avcf.slurp_file();
    if(verbose){
        cout << "  Input vcf: ";
        avcf.info_to_stdout();
    }
    
    
    // REFERENCE
    fasta referencefasta (1);
    referencefasta.read_fasta_file(in_ref);
    if(verbose){
        cout << "  Reference: ";
        referencefasta.info_to_stdout();
    }
    
    vector < int > positions_to_mask = avcf.give_positions();
    
    for (unsigned int i = 0; i < positions_to_mask.size(); i++) {
        referencefasta.mask_base(positions_to_mask.at(i), 'n');
    }
    
    referencefasta.write_to_file( out );
    
    cout << "  Reference masked and written to " << out << "\n";
    
    return(0);
}


int summarisePipeline (vector<string> *inargs, vector<string> *invalues ) {
    
    // CHECK ARGS
    
    string in_vcf = "NA", in_fasta = "NA", in_sites = "NA", out_sum = "NA", tag = "NA", err_file = "NA";
    int sites_col = 1,  nt = 1;
    bool append=false, verbose = false, ignore_multihits = false ;

    for(unsigned int i = 0; i < inargs->size(); i++ ){
        if( strncmp (inargs->at(i).c_str(), "-", 1) ){
            cout << "ERROR (summarisePipeline): fail to understand argument " << inargs->at(i) << "\n";
            exit(1);
        }
        else if( !strncmp (inargs->at(i).c_str(), "-in_vcf", 6) || !strncmp (inargs->at(i).c_str(), "-vcf", 5) ){
            in_vcf = invalues->at(i).c_str();
        }
        else if( !strncmp (inargs->at(i).c_str(), "-in_fasta", 6) || !strncmp (inargs->at(i).c_str(), "-fasta", 5) ){
            in_fasta = invalues->at(i).c_str();
        }
        else if( !strncmp (inargs->at(i).c_str(), "-in_sites", 6) || !strncmp (inargs->at(i).c_str(), "-sites", 5) ){
            in_sites = invalues->at(i).c_str();
        }
        else if( !strncmp (inargs->at(i).c_str(), "-out_sum", 6) || !strncmp (inargs->at(i).c_str(), "-out", 5) ){
            out_sum = invalues->at(i).c_str();
        }
        else if( !strncmp (inargs->at(i).c_str(), "-tag", 6)  ){
            tag = invalues->at(i).c_str();
        }
        else if( !strncmp (inargs->at(i).c_str(), "-sites_col", 6) || !strncmp (inargs->at(i).c_str(), "-column", 3)  ){
            sites_col = atoi(invalues->at(i).c_str());
        }
        else if( !strncmp (inargs->at(i).c_str(), "-append", 6)  ){
            append = ( atoi(invalues->at(i).c_str())  == 1 ) ? true : false;
        }
        else if(!strncmp (inargs->at(i).c_str(), "-verbose", 2) ){
            verbose = stoi(invalues->at(i).c_str());
        }
        else if(!strncmp (inargs->at(i).c_str(), "-errors", 2) ){
            err_file = invalues->at(i).c_str();
        }
        else if(!strncmp (inargs->at(i).c_str(), "-ignoreMH", 6) ){
            ignore_multihits = stoi(invalues->at(i).c_str());
        }
     /*
         else if(!strncmp (inargs->at(i).c_str(), "-threads", 2) ){
         nt = stoi(invalues->at(i).c_str());
         }
         */
        
        else{
            cout << "ERROR (summarisePipeline): fail to understand argument value pair: " << inargs->at(i) << " " << invalues->at(i) << "\n";
        }
        
    }
    
    if(in_vcf == "NA"){
        cout << "ERROR (summarisePipeline): vcf infile missing\n";
        exit(1);
    }
    if(in_fasta == "NA"){
        cout << "ERROR(summarisePipeline): original fasta infile missing\n";
        exit(1);
    }
    if(in_sites == "NA"){
        cout << "ERROR (summarisePipeline): sites infile missing\n";
        exit(1);
    }
    if(tag == "NA"){
        cout << "ERROR (summarisePipeline): tag missing\n";
        exit(1);
    }
    if(out_sum == "NA"){
        cout << "ERROR (summarisePipeline): summary output file missing\n";
        exit(1);
    }
    
    // ARGS OK
    
    cout << "  Running summarisePipeline\n";
    
    vector <string> all_in_sites;
    vector <string> all_in_vcfs;
    
    msplit( in_sites, ",", &all_in_sites );
    msplit( in_vcf, ",", &all_in_vcfs );
    
    vector <vcf> vcf_objs;
    unsigned int total_inds = 0;
    
    for( unsigned int avcf_index = 0; avcf_index < all_in_vcfs.size(); avcf_index++ ){
       // vcf_objs.push_back( vcf::vcf(all_in_vcfs.at(avcf_index)) );
        vcf_objs.push_back( vcf(all_in_vcfs.at(avcf_index), ignore_multihits) );
        total_inds += vcf_objs.at(avcf_index).num_inds();
    }
    
    
    if( total_inds != all_in_sites.size() ){
        cerr << "ERROR(summarisePipeline): number of individuals in vcf files " << in_vcf << " (" << total_inds
        << ") does not match number of sites files (" << all_in_sites.size()
        << ")\n";
        exit(1);
    }
    
    //
    
    for( unsigned int avcf_index = 0; avcf_index < all_in_vcfs.size(); avcf_index++ ){
        vcf_objs.at(avcf_index).slurp_file();
        if(verbose){
            cout << "  Input vcf: ";
            vcf_objs.at(avcf_index).info_to_stdout();
        }
    }
    
    // ORIGINAL
    fasta original_fasta (10);
    original_fasta.read_fasta_file(in_fasta);
    if(verbose){
        cout << "  Input original fasta file: " ;
        original_fasta.info_to_stdout();
        // SITES
        cout << "  Sites file (s) : " << all_in_sites.at(0);
        for (unsigned int j = 1; j < all_in_sites.size(); j++ ) {
            cout << ", " << all_in_sites.at(j);
        }
        cout << "\n";
    }
    
    
    vector <fasta> all_fastas;
    vector < vector < unsigned int  >  >  hold_indels, hold_ambs;
    vector <string> vcf_names;
    
    for( unsigned int avcf_index = 0; avcf_index < all_in_vcfs.size(); avcf_index++ ){
        
        //all_fastas.push_back( fasta::fasta(vcf_objs.at(avcf_index).num_inds(),  int(original_fasta.num_bases() )) );
        all_fastas.push_back( fasta(vcf_objs.at(avcf_index).num_inds(),  int(original_fasta.num_bases() )) );
   
        // BEGIN OF PARALLEL PART, transforming into fasta format
        if(nt == 1){
            for(int i = 0; i < vcf_objs.at(avcf_index).num_inds(); i++ ){
                hold_indels.push_back( vcf_objs.at(avcf_index).give_indels().at(i) );
                hold_ambs.push_back( vcf_objs.at(avcf_index).give_ambiguous().at(i) );
                vcf_names.push_back(vcf_objs.at(avcf_index).give_infile());
                all_fastas.at(avcf_index).build_from_vcf( vcf_objs.at(avcf_index) , original_fasta, all_in_sites.at(hold_indels.size()-1), vcf_objs.at(avcf_index).get_names(i) , sites_col, i);
            }
        }
        else{
            cout << "Parallel onde day!\n";exit(1);
        }
    }
    for( unsigned int fasta_index = 1; fasta_index < all_fastas.size(); fasta_index++)
        all_fastas.at(0).append_seq(all_fastas.at(fasta_index));
    
    all_fastas.erase( all_fastas.begin()+1, all_fastas.end() );
    
    int aligned = all_fastas.at(0).is_aligned();
    if( aligned != 0  ){
        cerr << "ERROR (summarisePipeline): Error converting data (sites files contain non-unique values?).\n";
        exit(1);
    }
    

    // BEGIN OF PARALLEL PART, sumarising
    summarise asummary( original_fasta.num_lines() - 1, all_fastas.at(0).num_bases() );

    if(nt == 1){
        for( unsigned int iind = 0; iind < total_inds; iind++ ){
            
            asummary.summarise_ind( original_fasta,  all_fastas.at(0),   iind ,
                                   hold_ambs.at(iind) ,
                                   hold_indels.at(iind) ,
                                   tag, vcf_names.at(iind), all_in_sites.at(iind), err_file );
            
        }
    }
    else{
        cout << "Parallel onde day!\n";exit(1);
    }
    asummary.print(out_sum, append);
    cout << "  Summary written to outfile "<< out_sum << endl;
    return(0);
}

 
int sfs2fas (vector<string> *inargs, vector<string> *invalues ){
    
    
    string infile = "NA", outfile = "NA", force_names = "NA";
    unsigned int npops = 0, inds_per_pop = 0, rep_to_fetch = 0, pop_ref = 0, pop_int = 0, num_diploids = 0;
    bool verbose = false;
    
    // CHECK ARGS
    
    for(unsigned int i = 0; i < inargs->size(); i++ ){
        if( strncmp (inargs->at(i).c_str(), "-", 1) ){
            cout << "ERROR (sfs2fas): fail to understand argument " << inargs->at(i) << "\n";
            exit(1);
        }
        else if( !strncmp (inargs->at(i).c_str(), "-infile", 3) ){
            infile = invalues->at(i).c_str();
        }
        else if( !strncmp (inargs->at(i).c_str(), "-outfile", 3) ){
            outfile = invalues->at(i).c_str();
        }
        else if( !strncmp (inargs->at(i).c_str(), "-npops", 4) || !strncmp (inargs->at(i).c_str(), "-num_pops", 5) ){
            npops = stoi(invalues->at(i).c_str());
        }
        else if( !strncmp (inargs->at(i).c_str(), "-pop_size", 7) ){
            inds_per_pop = stoi(invalues->at(i).c_str());
        }
        else if( !strncmp (inargs->at(i).c_str(), "-replicate", 4) ){
            rep_to_fetch = stoi(invalues->at(i).c_str());
        }
        else if(!strncmp (inargs->at(i).c_str(), "-reference", 4) || !strncmp (inargs->at(i).c_str(), "-pop_ref", 6) ){
            pop_ref = stoi(invalues->at(i).c_str());
        }
        else if(!strncmp (inargs->at(i).c_str(), "-sample", 4) || !strncmp (inargs->at(i).c_str(), "-pop_sample", 7) ){
            pop_int = stoi(invalues->at(i).c_str());
        }
        else if(!strncmp (inargs->at(i).c_str(), "-nsamples", 4) || !strncmp (inargs->at(i).c_str(), "-ndiploids", 6) ){
            num_diploids = stoi(invalues->at(i).c_str());
        }
        else if(!strncmp (inargs->at(i).c_str(), "-verbose", 2) ){
            verbose = stoi(invalues->at(i).c_str());
        }
        else if(!strncmp (inargs->at(i).c_str(), "-force_names", 3) ){
            force_names = invalues->at(i).c_str();
        }
        
        else{
            cout << "ERROR (sfs2fas): fail to understand argument value pair: " << inargs->at(i) << " " << invalues->at(i) << "\n";
        }
        
    }
    
    if(infile == "NA"){
        cout << "ERROR(sfs2fas): sfs_code infile missing\n";
        exit(1);
    }
    if(outfile == "NA"){
        cout << "ERROR(sfs2fas): fasta output file missing\n";
        exit(1);
    }
    if(inds_per_pop == 0){
        cout << "ERROR(sfs2fas): popsize missing\n";
        exit(1);
    }
    if(npops == 0){
        cout << "ERROR(sfs2fas): number of populations missing\n";
        exit(1);
    }
    if(rep_to_fetch == 0){
        cout << "ERROR(sfs2fas): replicate to fetch missing\n";
        exit(1);
    }
    
    cout << "  Running sfs2fas\n";
    
    fasta afasta = sfs2fas ( infile, outfile, npops, inds_per_pop, rep_to_fetch, verbose );
    
    int aligned = afasta.is_aligned();
    if( aligned != 0){
        cout << "ERROR (sfs2fas): Error converting sfs_code to fasta (resulting matrix is not aligned)\n";
        exit(1);
    }
    if (force_names != "NA" ) {
        if( verbose )
            cout << "  setting all names to " << force_names << endl;
        
        afasta.set_all_names_to(force_names);
    }
    
    if( pop_ref == 0 || pop_int == 0 || num_diploids == 0 ){
        if(verbose)
            cout << "  Missing pop_ref, pop_sample or nsamples, outputting full replicate to " << outfile << endl;
        
        afasta.write_to_file(outfile);
        
        if(!verbose)
            cout << "  Fasta file written to outfile "<< outfile << endl;
    }
    else{
        if(verbose)
            cout << "  Sampling 1 haplotype from population " << pop_ref << " and " << num_diploids
            << " diploid individuals from population " << pop_int << endl;
        
        vector < int> to_output0;
        to_output0.push_back( ( pop_ref - 1 ) * inds_per_pop * 2 );  // reference sequence
        int istart = ( pop_int - 1 ) * inds_per_pop * 2 + 2 ;         // sample starts after first diploid individual
        for( unsigned int i = istart; i < istart + num_diploids * 2; i++ )
            to_output0.push_back(i);
        
        
        fasta outfasta( num_diploids * 2 + 1 );
        outfasta.new_fasta_from_inds(afasta, to_output0);
        
        outfasta.write_to_file(outfile);
        
        cout << "  Fasta file written to outfile "<< outfile << endl;
    }
    return (0);
    
}


int extract_gff_from_fas (vector<string> *inargs, vector<string> *invalues ){
    
    
    // CHECK ARGS
    
    string infile = "NA", outfile = "NA", aff = "NA";
    // unsigned int start = 0, end = 0;
    bool verbose = false;
    for(unsigned int i = 0; i < inargs->size(); i++ ){
        if( strncmp (inargs->at(i).c_str(), "-", 1) ){
            cout << "ERROR (extract_gff): fail to understand argument " << inargs->at(i) << "\n";
            exit(1);
        }
        else if( !strncmp (inargs->at(i).c_str(), "-infile", 3)  ){
            infile = invalues->at(i) ;
        }
        else if( !strncmp (inargs->at(i).c_str(), "-outfile", 3) ){
            outfile =  invalues->at(i);
        }
        else if( !strncmp (inargs->at(i).c_str(), "-regions", 3) ){
            aff =  invalues->at(i);
        }
        else if(!strncmp (inargs->at(i).c_str(), "-verbose", 2) ){
            verbose = stoi(invalues->at(i).c_str());
        }
        
        else{
            cout << "ERROR (extract_gff): fail to understand argument value pair: " << inargs->at(i) << " " << invalues->at(i) << "\n";
        }
        
    }
    
    // CHECK ARGS
    if(infile == "NA"){
        cout << "ERROR(extract_gff): input file missing\n";
        exit(1);
    }
    if(outfile == "NA" ){
        cout << "ERROR(extract_gff): output file missing\n";
        exit(1);
    }
    
    
    
    if( aff == "NA"  ){
        cout << "ERROR(extract_gff): regions file missing)\n";
        exit(1);
    }
    
    cout << "  Running extract_gff\n";
    fasta afasta(1);
    afasta.read_fasta_file(infile);
    
    if(verbose){
        cout <<   "  Input fasta: ";
        afasta.info_to_stdout();
    }
    
    
    
    
    if(verbose)
        cout << "  Outputing regions defined in infile " << aff << endl;
    string line;
    ifstream infile_regions (aff.c_str());
    if( !infile_regions.is_open() ) {
        cout << "ERROR (extract_gff): Unable to open for output regions file " << aff << endl;
        exit(1);
    }
    
    fasta newfasta( afasta.num_lines() );
    bool first = true;
    while (getline(infile_regions,line)) {
         
        vector <string> fields;
        fields.clear();
        msplit( line, "\t", &fields );
        
        if (fields.size() != 3){
            cout << "ERROR (extract_gff): regions file malformatted" << endl;
            exit(1);
        }
        
        unsigned int tstart = atoi( fields.at(0).c_str());
        unsigned int tend = atoi( fields.at(1).c_str());
       
        if ( tend > afasta.num_bases() ){
            cout << "ERROR(extract_gff): region " << tstart << ":" << tend << " is out of bounds (length of alignment is "
            << afasta.num_bases() << ")" << endl;
            exit(1);
        }
        
        if(first){
            first = false;
            newfasta = afasta;
            newfasta.resize_matrix(tstart, tend);
          
        }
        else{
        
         
        fasta atempfasta (afasta.num_lines());
        atempfasta = afasta;
        atempfasta.resize_matrix(tstart, tend);
        
        newfasta.concatenate_alignments(atempfasta);
    }
    }
    
    
    newfasta.write_to_file(outfile);
    cout << "  New fasta file written to fasta file " << outfile << endl;

    
    
    return 0;
}

int nghcaller(vector<string> *inargs, vector<string> *invalues ){
    
    // CHECK ARGS
    string mindep = "3", maxdep = "100", platform = "33", baseq = "20", out_fas = "NA", root = "NA";
    bool strict = true, verbose = false;
    unsigned int minreads = 6;
    
    string outfile = "NA";
    
    for(unsigned int i = 0; i < inargs->size(); i++ ){
        if( strncmp (inargs->at(i).c_str(), "-", 1) ){
            cout << "ERROR (nghcaller): fail to understand argument " << inargs->at(i) << "\n";
            exit(1);
        }
        else if( !strncmp (inargs->at(i).c_str(), "-mindep", 4)  ){
            mindep = invalues->at(i) ;
        }
        else if( !strncmp (inargs->at(i).c_str(), "-maxdep", 4) ){
            maxdep =  invalues->at(i);
        }
        else if( !strncmp (inargs->at(i).c_str(), "-platform", 4) ){
            platform =  invalues->at(i);
        }
        else if( !strncmp (inargs->at(i).c_str(), "-baseq", 4) ){
            baseq = invalues->at(i);
        }
        else if( !strncmp (inargs->at(i).c_str(), "-threshold", 4) ){
            minreads =  atoi (invalues->at(i).c_str() );
        }
        else if( !strncmp (inargs->at(i).c_str(), "-root", 4) ){
            root = invalues->at(i);
        }
        else if( !strncmp (inargs->at(i).c_str(), "-out_fas", 6) ){
            out_fas = invalues->at(i);
        }
        else if( !strncmp (inargs->at(i).c_str(), "-strictness", 4)  ){
            ( atoi (invalues->at(i).c_str() )) == 1 ? strict = true : strict = false;
        }

        else if(!strncmp (inargs->at(i).c_str(), "-verbose", 2) ){
            verbose = stoi(invalues->at(i).c_str());
        }
        
        else{
            cout << "ERROR (nghcaller): fail to understand argument value pair: " << inargs->at(i) << " " << invalues->at(i) << "\n";
        }
        
    }
    
    // CHECK ARGS
    
    if(out_fas == "NA" ){
        cout << "ERROR(nghcaller): output file unspecified (-out_fas)\n";
        exit(1);
    }
    
      
    // ARGS OK
    
    cout << "  Running nGHcaller\n";
    
    string lineInput;
    
    
    // for debuggin purposes
    //ifstream infile_mpileup ("test.small.mpileup");
   // ifstream infile_mpileup ("./input_files/test.with_ref.mpileup");
    //   ifstream infile_mpileup ("toyrun.mpileup");
    
    ngh angh( mindep, maxdep, platform, baseq, minreads, strict );
    
    
    
    getline(cin,lineInput);
    
    //getline(infile_mpileup,lineInput);
    
    vector <string> fields;
    msplit(lineInput, "\t" , &fields);
    angh.set_num_inds( (fields.size() - 3 )/3 );
    
    if(verbose){
        cout << "  Settings for SNP calling: ";
        angh.info_to_stdout();
        cout << endl;
    }
    
    fasta nameless_fasta(1);
    nameless_fasta.set_num_inds(2*(fields.size() - 3 )/3 );
    
    ngh_parse_return a_parsed_line = angh.parse_mpileup(lineInput);
    nameless_fasta.build_from_ngh( a_parsed_line.site,  angh.snp_call( a_parsed_line  ) );
    
   while (getline(cin,lineInput)) {
        
         //while (getline(infile_mpileup,lineInput)) {
        
        ngh_parse_return a_parsed_line = angh.parse_mpileup(lineInput);
        nameless_fasta.build_from_ngh( a_parsed_line.site,  angh.snp_call( a_parsed_line  ) );
        
    }
    
    // Root with reference or outgroup
    
    fasta outgroup(1);
    
    if( root != "NA" ){
        outgroup.read_fasta_file(root);
        nameless_fasta.append_seq(outgroup, true);
    }
    if(verbose){
        cout << "  SNP calling done" << endl;
    }
    
    if (out_fas != "NA") {
        nameless_fasta.write_to_file(out_fas);
        cout << "  Output written to fasta file " << out_fas << endl;
    }
  
    return (0);
    
}








