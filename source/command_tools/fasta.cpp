//
//  fasta.cpp
//  Pipeliner
//
//  Created by Bruno Nevado on 17/06/2013.
//  Copyright (c) 2013 Bruno Nevado. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <cstring>
#include <string>
#include <locale>

#include "fasta.h"

// FASTA OBJECT CONSTRUCTOR

fasta::fasta ( int num_inds, int len ){
    if(len == 0){
        names.reserve(num_inds);
        matrix.reserve(num_inds);
    }
    else{
        matrix.resize(num_inds);
        names.resize(num_inds);
        
        for(int i = 0; i < num_inds; i++)
            matrix.at(i).resize(len);
        /*
         string newline (len , NULL);
         for(int i = 0; i < num_inds; i++)
         matrix.push_back(newline);
         
         
         */
    }
}

// FASTA OBJECT CALCULATE ALLELE COUNT

int fasta::calc_dac ( char alt , unsigned int site ) {
    int freq = 0;
    
    for(int ind = 1; ind < int ( matrix.size() ); ind++){
        if( matrix[ind].at(site-1) == alt){
            freq++;
        }
    }
    return freq;
}

// FASTA OBJECT OUTPUT SNP FILES


void fasta::outputSNPfiles( int cind, string prefix ){
    
    ofstream streamHomo, streamHetero;
    
    stringstream cindstr;
    cindstr << cind;
    string outfileHomo = prefix + ".Ind" + cindstr.str() + ".fixed";
    string outfileHetero = prefix + ".Ind" + cindstr.str() + ".snps";
    
    streamHomo.open(outfileHomo.c_str());
    if( !streamHomo.is_open() ){
        cerr << "ERROR (outputSNPfiles): unable to open for output file " << outfileHomo << "\n";
        exit(1);
    }
    streamHetero.open(outfileHetero.c_str());
    if( !streamHetero.is_open() ){
        cerr << "ERROR (outputSNPfiles): unable to open for output file " << outfileHetero << "\n";
        exit(1);
        
    }
    int i = (cind -1) * 2 + 1;
    for( unsigned int j = 0; j < this->num_bases(); j++ ){
        
        if(matrix[0][j] == matrix[i][j] && matrix[i][j] == matrix[i+1][j])
            continue;
        // Fixed diff
        if ( matrix[0][j] != matrix[i][j] && matrix[i][j] == matrix[i+1][j] ){
            streamHomo << j+1 << "\t" << matrix[0][j] << "\t" << matrix[i][j] << "\t" << matrix[i+1][j]
            << "\t" << calc_dac( matrix[i][j] , j+1 ) << "\n";
            
        }
        // heterozygous SNP
        else if ( (matrix[0][j] != matrix[i][j] && matrix[0][j] == matrix[i+1][j] )
                 || (matrix[0][j] == matrix[i][j] && matrix[0][j] != matrix[i+1][j]) ){
            streamHetero << j+1 << "\t" << matrix[0][j] << "\t" << matrix[i][j] << "\t" << matrix[i+1][j]
            << "\t" << calc_dac( ( matrix[0][j] == matrix[i+1][j] ?  matrix[i][j] :  matrix[i+1][j] ) , j+1 ) << "\n";
        }
        else{
            cout << "WARNING: position " << j+1 << " is triallelic (ref:" << matrix[0][j] << ",individual " << cind << ":" << matrix[i][j] << matrix[i+1][j] << ")\n" ;
        }
        
    }
    streamHomo.close();
    streamHetero.close();
    
    
}


// FASTA CLASS SPEAKING FUNCTION

void fasta::info_to_stdout () {
    
    cout << "Infile: " << this->input_file() << " (" << this->num_lines() << " sequences, " <<
    this->num_bases() << " bp long)\n";
    
    /*
     cout << "Name of sequences: ";
     for(int i = 0; i < this->num_lines(); i++)
     cout << this->names[i] << " ";
     cout << "\n";
     
     cout<< "And the data matrix is:\n";
     for(int i = 0; i < matrix.size(); i++)
     cout << matrix[i] << "\n";
     
     
     */
}

// FASTA CLASS FUNCTION: READ FASTA FILE

void fasta::read_fasta_file( std::string infas){
    int cind = 0;
    matrix.clear();
    std::locale loc;
    std::string line;
    std::ifstream infile_fas (infas.c_str());
    if (infile_fas.is_open())
    {
        infile = infas;
        while ( ! infile_fas.eof() )
        {
            getline(infile_fas, line);
            
            if( line[0] == '>' ){
                cind++;
                matrix.resize(cind);
                std::string name = line.substr(1);
                names.push_back(name);
            }
            else {
                matrix.at(cind-1).append( line );
            }
        }
    }
    
    else {
        std::cerr << "ERROR (read_fasta_file): Unable to open infile " << infas << "\n" ;
        exit(1);
    }
    infile_fas.close();
    
    for (unsigned int l = 0; l < matrix.size(); l++) {
        for (unsigned s = 0; s < matrix.at(l).length(); s++) {
            matrix.at(l).at(s) = tolower(matrix.at(l).at(s), loc);
        }
    }
    
}

// CHECK WHETHER ALL SEQS HAVE SAME LENGTH
int fasta::is_aligned () {
    unsigned int previous_len = 0;
    for(unsigned int i = 0; i < matrix.size() ; i++){
        unsigned int current_len = 0;
        for (unsigned int j = 0; j <  matrix.at(i).size() ; j++) {
            if ( matrix[i][j] != char (NULL)  ){
                current_len++;
            }
            
        }
        if( i == 0){
            previous_len = current_len;
            continue;
        }
        else if (current_len != previous_len){
            return (i);
        }
    }
    
    return 0;
}

// WRITES RANDOM SEQUENCE TO FILE

void fasta::write_random( string out, string name, unsigned int len ){
    
    
    ofstream outputFile;
    char bases[4] = { 'A', 'C', 'G', 'T' };
    
    outputFile.open(out.c_str());
    
    if( !outputFile.is_open() ){
        cerr << "ERROR (write_random): unable to open for output file " << out << "\n";
        exit(1);
    }
    
    outputFile << ">" << name << "\n";
    
    for( unsigned int site = 0; site < len; site++){
        outputFile << bases[ rand() % 4 ];
    }
    outputFile << "\n";
    outputFile.close();
    
}

// WRITE ALIGNMENT TO FILE

void fasta::write_to_file( string out, int append ){
    locale loc;
    ofstream outputFile;
    
    if( append == 0 )
        outputFile.open(out.c_str());
    else
        outputFile.open(out.c_str(), ios::app );
    
    if( !outputFile.is_open() ){
        cerr << "ERROR (write_to_file): unable to open for output file " << out << "\n";
        exit(1);
    }
    if( names.size() == matrix.size() ){
        for( unsigned int i = 0; i < matrix.size(); i++){
            outputFile << ">" << names[i] << "\n";
            for (unsigned int site = 0; site < matrix.at(i).length(); site++) {
                outputFile << toupper(matrix[i][site],loc); //  matrix[i][site];
            }
            outputFile << endl;
        }
    }
    else{
        for( unsigned int i = 0; i < matrix.size(); i++){
            outputFile << ">i" << i+1 << "\n"; //
            for (unsigned int site = 0; site < matrix.at(i).length(); site++) {
                outputFile << toupper(matrix[i][site],loc); //  matrix[i][site];
            }
            outputFile << endl;
        }
    }
    outputFile.close();
    
}

void fasta::remove_multi_hits(){
    
    vector <unsigned int> mh_pos;
    
    for ( unsigned int site = 0; site < this->num_bases(); site++){
        char ref = this->matrix[0][site] , alt = 'N';
        for( unsigned int ind = 1; ind < this->num_lines(); ind++ ){
            if( matrix[ind][site] != ref ){
                if (alt == 'N'){
                    alt = matrix[ind][site];
                }
                else if ( matrix[ind][site] != alt ){
                    mh_pos.push_back(site);
                    
                }
            }
        }
        
    }
    
    if( mh_pos.size() > 0 ){
        cout << "  Multiple hits found: " << mh_pos.size()
        << ". Sites masked as invariable: ";
        for( unsigned long mh = 0; mh < mh_pos.size(); mh++ ){
            cout << " " << mh_pos[mh]+1;
            for( unsigned int ind = 1; ind < matrix.size(); ind++){
                matrix[ind][mh_pos[mh]] = matrix[0][mh_pos[mh]];
                
            }
        }
        cout << "\n";
    }
    else{
        cout << "  No multiple hits found\n";
    }
    
}

// BUILD FROM MS CLASS

//void fasta::build_from_ms (ms in, fasta ancestral){
void fasta::build_from_ms ( const ms& in, const fasta& ancestral){
    
    
    infile = "Converted from ms file " + in.infile;
    
    vector <char> alternative;
    vector <int> segsites_int;
    alternative.reserve(in.num_seg_sites());
    segsites_int.reserve(in.num_seg_sites());
    
    //char alternative[ancestral.num_bases()] = 'A';
    char bases[4] = { 'a', 'c', 'g', 't' };
    
    // prepare alternative sequence
    for( unsigned int mut = 0; mut < in.num_seg_sites(); mut++){
        int site = round(in.positions.at(mut) * ancestral.num_bases() );
        segsites_int.push_back(site);
        char adraw = bases[ rand() % 4 ];
        while ( adraw == ancestral.matrix[0].at(site)){
            adraw = bases[ rand() % 4 ];
        }
        
        alternative.push_back(adraw);
    }
    
    for ( unsigned int line = 0 ; line < in.num_lines(); line++ ){
        names.push_back("Individual");
        int curr_mutation = 0;
        string growing_seq;
        
        for( unsigned int site = 0; site < ancestral.num_bases(); site++){
            if(binary_search(segsites_int.begin(), segsites_int.end(), site+1  )){
                
                if( in.matrix[line].at(curr_mutation) == '0')
                {
                    growing_seq.push_back(ancestral.matrix[0].at(site));
                }
                else
                {
                    growing_seq.push_back(alternative[curr_mutation]);
                }
                curr_mutation++;
            }
            else{
                growing_seq.push_back(ancestral.matrix[0].at(site) );
            }
        }
        matrix.push_back(growing_seq);
        growing_seq.clear();
    }
    
}


// FOR KICKS


void fasta::forkicks(){
    ofstream outputFile;
    outputFile.open("table.txt");
    
    double sites=0;
    double start = 1;
    for( unsigned int site = 0; site < this->num_bases(); site++){
        int present=0;
        for( unsigned int seq = 0; seq < this->num_lines(); seq++){
            
            if(matrix[seq][site] != 'n'){
                present++;
            }
            if(present == int ( this->num_lines() ) )
                sites++;
            
        }
        
        if(( site % 500000) == 0 ){
            outputFile << fixed << start << "\t" << site << "\t" << sites << "\n";
            start=site;
            sites=0;
            
        }
        
        
    }
    
    outputFile.close();
    
}

// BUILD FROM VCF + SITES

void fasta::build_from_vcf (const vcf& objvcf, const fasta& reference, string sites_file, string name, int column, int ind ){
    
    infile = "Converted from vcf file " + objvcf.infile + " (chromosome: " + objvcf.chromosome + ")";
    
    unsigned int previous_site = 0;
    string new_haplo;
    ifstream infile_sites(sites_file.c_str());
    names.at(ind) = name;
    
    int c = 0;
    if( !infile_sites.is_open()){
        cerr << "ERROR (fasta, build_from_vcf): Unable to open for reading sites file " << sites_file << "\n";
        exit(1);
    }
    else{
        string line;
        while( getline(infile_sites, line ) ){
            c++;
        }
        infile_sites.close();
        if( c == 0){
            cout << "WARNING (fasta, build from vcf): sites file " << sites_file << " is empty" << endl;
        }
    }
    
    infile_sites.open(sites_file.c_str());

    string sites_line;
    unsigned int index_vcf = 0;
    
    for( unsigned int site_reference = 0; site_reference < reference.num_bases(); site_reference++ ){
       
        if( infile_sites.eof() || c == 0 ){
            while( site_reference < reference.num_bases() ){
                new_haplo.push_back('n');
                site_reference++;
                
            }
            break;
        }

        while ( sites_line.size() == 0  ){
            getline(infile_sites, sites_line);
        }
        vector <string> temp;
        msplit( sites_line, "\t", &temp );
        unsigned int site_sites = atoi( temp.at(column-1).c_str());
        temp.clear();
        if( site_sites < previous_site ){
            
            cerr << "ERROR (fasta, build from vcf): sites file " << sites_file << " does not appear to be sorted "
            << "(offending sites: " << previous_site << " and " << site_sites << "). Consider using 'sort -n -u' \n";
            exit(1);
        }else{
            previous_site = site_sites;
        }
        if ( site_sites > (site_reference + 1 ) ){
            new_haplo.push_back('n');
            continue;
        }
        
        else if ( site_sites == (site_reference + 1 ) ){
            
              
            // SITE WITH SOME DATA
            while( index_vcf < (unsigned) objvcf.positions.size()  && ( (unsigned) objvcf.positions.at(index_vcf) < ( site_reference + 1 ))  ){
                index_vcf++;
                
            }
            
            if( index_vcf >= (unsigned) objvcf.positions.size() ||  (unsigned) objvcf.positions.at(index_vcf) != ( site_reference + 1 ) ){
                new_haplo.push_back( reference.matrix[0][site_reference] );
                
            }
            else{
                new_haplo.push_back( objvcf.matrix[index_vcf][ind] );
                index_vcf++;
            }
            
            getline(infile_sites, sites_line);
            
        }
        
    }
    
    for( unsigned int i = 0; i < new_haplo.size(); i++ ){
        matrix[ind][i] = new_haplo[i];
    }
    
    new_haplo.clear();
    
    infile_sites.close();
}


// FASTA ADD SEQUENCE OPTION

void fasta::append_seq( fasta in, bool fill ){
    // assuming both fasta objects are aligned within each other, and adding a the end
    // of the shortest 'Ns'
    if(fill){
        
        if( in.num_bases() < this->num_bases() ){
            string missing ( this->num_bases() - in.num_bases(), 'n');
            for (unsigned int nline = 0 ; nline < in.num_lines() ; nline++) {
                for (unsigned int base = 0; base < missing.size(); base++) {
                    in.matrix.at(nline).push_back(missing.at(base));
                }
            }
        }
        else if (in.num_bases() > this->num_bases()){
            string missing ( in.num_bases() - this->num_bases(), 'n');
            for (unsigned int nline = 0 ; nline < this->num_lines() ; nline++) {
                for (unsigned int base = 0; base < missing.size(); base++) {
                    matrix.at(nline).push_back(missing.at(base));
                }
            }
        }
        
    }
    else if ( in.num_bases() != this->num_bases() ){
        cerr << "ERROR (fasta, add_seq): sequence to add has length different from alignment ("
        << in.num_bases() << " and " << this->num_bases() << ")\n";
        exit(1);
    }
    
    
    
    for (unsigned int i = 0; i < in.num_lines(); i++ ) {
        matrix.push_back(in.matrix.at(i).c_str());
    }
    
    infile = infile + " + " + in.infile;
    
    if( this->num_lines() == this->num_names() && in.num_names() > 0 ){
        
        for (unsigned int i = 0; i < in.num_lines(); i++ ) {
            
            names.push_back(in.names.at(i));
        }
    }
    
    
}


void fasta::build_from_ngh( unsigned int site, string col_to_add ){
    //  cout << matrix.at(0).size() << " and " << site <<  "\n";
    
    while (matrix.at(0).size() < ( site - 1 ) ) {
        for (unsigned int i = 0; i < col_to_add.size(); i++) {
            matrix.at(i).push_back('n');
        }
    }
    
    for (unsigned int i = 0; i < col_to_add.size(); i++) {
        matrix.at(i).push_back(col_to_add.at(i));
    }
}


void fasta::resize_matrix( unsigned int start, unsigned int end ){
    for( unsigned int line = 0; line < matrix.size(); line++ ){
        string sub = matrix.at(line).substr(start - 1, end - start + 1  );
        matrix.at(line) = sub;
    }
    
}

void fasta::mask_base ( unsigned int site1, char newchar, int index1){
    
    if ( site1 == 0 ) {
        cerr << "ERROR (fasta::mask_base): site number should be 1-based" << endl;
        exit(1);
    }
    if( index1 == -1 ){
    for (unsigned int line = 0; line < matrix.size(); line++) {
        
        if (site1  > matrix.at(line).size() ) {
            cerr << "ERROR (fasta::mask_base): site to mask (" <<  site1 <<") out of bounds" << endl;
            exit(1);
        }
        else{
            matrix.at(line).at( site1 - 1 ) = tolower ( newchar );
        }
    }
    
    }
    else{
        if (site1  > matrix.at(index1 -1 ).size() ) {
            cerr << "ERROR (fasta::mask_base): site to mask (" <<  site1 <<") out of bounds" << endl;
            exit(1);
        }
        else{
            matrix.at(index1 -1).at( site1 - 1 ) = tolower ( newchar );
        }
    }
}


void fasta::new_fasta_from_inds ( fasta &infasta, vector <int> index0 ){

    infile = infasta.infile;
    for (unsigned int line = 0; line < index0.size() ; line++ ) {
        matrix.push_back( infasta.matrix.at(index0.at(line)));
       names.push_back( infasta.names.at(index0.at(line)));
    }
}

 
int fasta::concatenate_alignments( fasta & al2){
    if( matrix.size() != al2.num_lines()){
        cerr << "ERROR (concatenate fasta): number of sequences do not match!"  << endl;
        exit(1);
    }
    
    for ( unsigned int i = 0; i < matrix.size(); i++ ) {
        matrix.at(i).append( al2.show_seq(i) );
    }
    return 0;
}
