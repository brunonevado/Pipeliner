//
//  common.cpp
//  Pipeliner
//
//  Copyright (c) 2013 Bruno Nevado. GNU license.
//

#include <fstream>
#include <algorithm>

#include "common.h"

// from IUPAC

vector <char> fromIUPAC(char in){
    vector <char> toreturn;
    
    if( in == 'a' || in == 'c' || in == 'g' || in == 't' || in == 'n' ){
        toreturn.push_back(in);
        toreturn.push_back(in);
        return toreturn;
    }
    else if ( in == 'm' ){
        toreturn.push_back('a');
        toreturn.push_back('c');
        return toreturn;
    }
    else if ( in == 'r' ){
        toreturn.push_back('a');
        toreturn.push_back('g');
        return toreturn;
    }
    else if ( in == 'w' ){
        toreturn.push_back('a');
        toreturn.push_back('t');
        return toreturn;
    }
    else if ( in == 's' ){
        toreturn.push_back('c');
        toreturn.push_back('g');
        return toreturn;
    }
    else if ( in == 'k' ){
        toreturn.push_back('g');
        toreturn.push_back('t');
        return toreturn;
    }
    else if ( in == 'y' ){
        toreturn.push_back('c');
        toreturn.push_back('t');
        return toreturn;
    }
    else{
        cerr << "ERROR (fromIUPAC): invalid code: " << in << "\n";
        exit(1);
    }
    
}

int sum_vec_ints ( vector <int> in ) {
    int sum = 0;
    for (unsigned int i = 0; i < in.size(); i++) {
        sum += in.at(i);
    }
    return(sum);
}


char toIUPAC(string in){
    //  cout << " called with " << in.at(0) << "\n";
    if( in.at(0) == in.at(1) ){
        return in.at(0);
    }
    else if ( (in.at(0) == 'a' && in.at(1) == 'c') || (in.at(1) == 'a' && in.at(0) == 'c') ){
        return 'm';
    }
    else if ( (in.at(0) == 'a' && in.at(1) == 'g') || (in.at(1) == 'a' && in.at(0) == 'g') ){
        return 'r';
    }
    else if ( (in.at(0) == 'a' && in.at(1) == 't') || (in.at(1) == 'a' && in.at(0) == 't') ){
        return 'w';
    }
    else if ( (in.at(0) == 'c' && in.at(1) == 'g') || (in.at(1) == 'c' && in.at(0) == 'g') ){
        return 's';
    }
    else if ( (in.at(0) == 't' && in.at(1) == 'g') || (in.at(1) == 't' && in.at(0) == 'g') ){
        return 'k';
    }
    else if ( (in.at(0) == 'c' && in.at(1) == 't') || (in.at(1) == 'c' && in.at(0) == 't') ){
        return 'y';
    }
    else{
        cerr << "ERROR (turnIUPAC): no code available for " << in.at(0) << in.at(1) << "\n";
        exit(1);
    }
    
    
}

unsigned int count_ignore_case ( char base, vector <char> DNAbases , vector <int> counts ){
    
    unsigned int count = 0;
    
    for (unsigned int i = 0; i < DNAbases.size() ; i++) {
        if ( DNAbases.at(i) == base || DNAbases.at(i) == toupper( base ) ) {
            count += counts.at(i);
        }
    }
    return count;
}

vector <char> get_uniques_lc ( vector <char> bases_to_check ){
    // from http://www.cplusplus.com/forum/general/14268/
    vector<char> result;
    vector<bool> duplicated(bases_to_check.size(), false);
    
    for (unsigned int i = 0; i < bases_to_check.size(); i++) {
        for (unsigned int j = i + 1 ; j < bases_to_check.size(); j++) {
            if (bases_to_check.at(i) == bases_to_check.at(j)) {
                duplicated.at(j) = true;
            }
        }
    }
    
    for (unsigned int i = 0; i < bases_to_check.size(); i++) {
        if (!duplicated.at(i)) {
            result.push_back(tolower(bases_to_check.at(i)));
        }
    }
    return result;
}

void msplit(string s , string delim,  vector<string> * output){
    
    //
    if( output->size() != 0 ){
        std::cerr << "WARNING: result vector for msplit is not empty!" << std::endl;
        for (unsigned int i = 0; i < output->size(); i++) {
            std::cerr << output->at(i) << " " ;
        }
        std::cerr << std::endl;
    }
    

    
    
    //
    unsigned long start = 0U;
    unsigned long end = s.find(delim);
    while (end != std::string::npos)
    {
        output->push_back(s.substr(start, end - start));
        start = end + delim.length();
        end = s.find(delim, start);
    }
    output->push_back(s.substr(start, end - start));
    
}

void msplit2( const string& s , string delim,  vector<string> * output){
    
    //
    if( output->size() != 0 ){
        std::cerr << "WARNING: result vector for msplit is not empty!" << std::endl;
        for (unsigned int i = 0; i < output->size(); i++) {
            std::cerr << output->at(i) << " " ;
        }
        std::cerr << std::endl;
    }
    

    //
    unsigned long start = 0U;
    unsigned long end = s.find(delim);
    while (end != std::string::npos)
    {
        output->push_back(s.substr(start, end - start));
        start = end + delim.length();
        end = s.find(delim, start);
    }
    output->push_back(s.substr(start, end - start));
    
}

void help(){
    cout << "###################\nCommands available:\n###################\n";
    
    cout << "\n  *help: print this message and exit (usage: Pipeliner help)\n";
    cout << "   ~~~~\n";
    
    //
    //
    
    cout << "\n  *writeRandomSeq\n";
    cout << "   ~~~~~~~~~~~~~~\n";
    cout << "\tusage: Pipeliner writeRandomSeq -outfile outfile.fas -length n [ -name seqname ]\n";
    cout << "\targuments: -outfile: file to save random sequence to\n";
    cout << "\t           -length: length of random sequence to create\n";
    cout << "\t           -name: name to use for sequence in output fasta file (def: ancestral)\n";
    cout << "\tdescription: Writes random sequence ( consisting of A,C,G,T) of -length bp into fasta file -outfile\n";
    
    //
    //
    
    cout << "\n  *desinterleave\n";
    cout << "   ~~~~~~~~~~~~~\n";
    cout << "\tusage: Pipeliner desinterleave -infile infile.fas -outfile outfile.fas\n";
    cout << "\targuments: -infile: input fasta file\n";
    cout << "\t           -outfile: file to save desinterleaved fasta file to\n";
    cout << "\tdescription: Desinterleave fasta file\n";
    
    //
    //
    
    cout << "\n  *resizeFasta\n";
    cout << "   ~~~~~~~~~~~\n";
    cout << "\tusage: Pipeliner resizeFasta -infile infile.fas [[ -outfile outfile.fas [ -start n ||  -end n ]  ]] [[ -regions regions.txt ]]\n";
    cout << "\targuments: -infile: input fasta file\n";
    cout << "\t           -outfile: file to save new fasta file to (will be overwritten)\n";
    cout << "\t           -regions: file containing regions to output. Will ignore -start -end and -output options, and use only -regions file\n";
    cout << "\t                      regions file should have 1 entry per line, with start, end and file_to_write_to (values should be tab separated)\n";
    cout << "\tdescription: Writes new fasta file in the region [start:end] (inclusive). If -start not given, starts from beginning of input,\n";
    cout << "\t             if no -end given, writes till the end of input, if both -start and -end missing raises an error.\n";
    cout << "\t             Alternativelly, uses a 'regions' file, with each line containing start and end positions, and file to output to\n";
    
    //
    //
    
    cout << "\n  *maskReference\n";
    cout << "   ~~~~~~~~~~~~~\n";
    cout << "\tusage: Pipeliner maskReference -in_vcf|vcf infile.vcf -in_ref|ref infile.fas  -out_fas|outfile outfile.fas\n";
    cout << "\targuments: -in_vcf|vcf: input vcf file (single or multiple individuals)\n";
    cout << "\t           -in_ref|ref: input reference file\n";
    cout << "\t           -out_fas|outfile: file to save new reference\n";
    cout << "\tdescription: All positions present in vcf infile are masked (changed to 'N') in reference sequence (and new file written to outfile).\n";
    
    //
    //
    
    cout << "\n  *ms2fas\n";
    cout << "   ~~~~~~\n";
    cout << "\tusage: Pipeliner ms2fas -in_ms|msin infile.ms -in_anc|ancestral ancestral.fas -out_fas|outfas output.fas [ -out_ms|outms output.ms ]\n";
    cout << "\targuments: -in_ms|msin: input ms file\n";
    cout << "\t           -in_anc|ancestral: input fasta file to use as ancestral\n";
    cout << "\t           -out_fas|outfas: file to save resulting fasta alignment\n";
    cout << "\t           -out_ms|outms: file to save resulting ms file (def: only writes fasta file)\n";
    cout << "\tdescription: Takes a ms file and an ancestral fasta file, and creates a fasta file for all individuals in input ms file\n";
    cout << "\t             If input ms file contains multiple hits (as judged after rounding segsites to length of ancestral sequence)\n";
    cout << "\t             only last hit for each site is kept. Outputs new ms file if -outms used\n";
    
    //
    //
    
    cout << "\n  *vcf2fas\n";
    cout << "   ~~~~~~~\n";
    cout << "\tusage: Pipeliner vcf2fas -in_vcf|vcf infile.vcf -in_ref|ref reference.fas -in_sites|sites infile.ind1.sites,infile.ind2.sites (...)\n";
    cout << "\t                            (...) -out_fas|out output.fas [ -names ind1,ind2 -sites_col|col 1 -ignoreMA 1 -append 1 -threads n ]\n";
    cout << "\targuments: -in_vcf|vcf: input vcf file (single or multiple individuals)\n";
    cout << "\t           -in_ref|ref: input reference fasta file\n";
    cout << "\t           -in_sites|sites: sites file, one per individual. Should contain, sorted and unique, the positions that are to be outputted\n";
    cout << "\t           -out_fas|out: file to save resulting fasta alignment\n";
    cout << "\t           -names: names to use for output fasta file sequences (def: take names from vcf file if present, otherwise use numbers from 1 to nind)\n";
    cout << "\t           -threads: number of threads to use (experimental)\n";
    cout << "\t           -append: if 1 append to output file, otherwise overwrite (def: overwrite)\n";
    cout << "\t           -sites_col|col: which column in infile.sites contain the site numbers (def: column 1)\n";
    cout << "\t           -ignoreMA: If set to 1, will only consider genotypes RR, RA1 and A1A1 - remaining genotypes are coded as missing (default: 0, i.e. all 10 possible genotypes are considered).\n";
    
    cout << "\tdescription: Takes a reference fasta file, a vcf file and 1 file per individual with sites to output (based e.g. on coverage level)\n";
    cout << "\t             and outputs a fasta file with all individuals present in the vcf file. In this output file, regions not present in the\n";
    cout << "\t             'sites' files will be marked missing ('n'), remaining positions will be coded according to genotype in vcf file\n";
    cout << "\t             (heterozygous genotypes are coded according to IUPAC). Indels in input vcf file are ignored and coded as missing";
    
    cout << "\n  *sfs2fas\n";
    cout << "   ~~~~~~~\n";
    cout << "\tusage: Pipeliner sfs2fas -infile infile.sfs_code -outfile outfile.fas -npops|num_pops n -pop_size n -replicate n (...)\n";
    cout << "\t                            (...)  [ -reference|pop_ref n -sample|pop_sample n -ndiploids|nsamples n -verbose 1 ]\n";
    cout << "\targuments: -infile: input sfs_code file. Can contain several populations, but all populations must have the same number of samples\n";
    cout << "\t           -outfile: file to save resulting fasta alignment\n";
    cout << "\t           -npops|num_pops: number of populations in input sfs_code file\n";
    cout << "\t           -pop_size: number of diploid individuals in each population (must be the same accross populations)\n";
    cout << "\t           -replicate: which replicate to extract from sfs_code infile\n";
    cout << "\t           -reference|pop_ref: which population to sample haplotype for reference\n";
    cout << "\t           -sample|pop_sample: which population to sample ndiploids from\n";
    cout << "\t           -ndiploids|nsamples: number of diploid individuals to sample from pop_sample\n";
    cout << "\tdescription: Converts to fasta format the replicate -replicate in input sfs_code file -infile.\n";
    cout << "\t             If present, -reference, -sample and -ndiploids must all be defined. If any of these is missing, will output the entire replicate,\n";
    cout << "\t             otherwise will extract the first haplotype from population -reference, and -ndiploids diploid individuals from population -sample (starting\n";
    cout << "\t             in diploid individual 2, as the first individual of each population is 'reserved' for reference).\n";
    
    //
    //
    
    cout << "\n  *summarisePipeline\n";
    cout << "   ~~~~~~~~~~~~~~~~~\n";
    cout << "\tusage: Pipeliner summarisePipeline -in_vcf|vcf infile1.vcf,infile2.vcf -in_sites|sites infile.ind1.sites,infile.ind2.sites (...)\n";
    cout << "\t                            (...) -in_fasta|fasta infile.fas -tag string -out_sum|out outfile.txt [ -sites_col|col 1 ] (...)\n";
    cout << "\t                            (...) [ -append 0 -errors file.txt -ignoreMA 1 ] \n";
    
    cout << "\targuments: -in_vcf|vcf: input vcf file (s) (single or multiple individuals allowed within each file)\n";
    cout << "\t           -in_sites|sites: sites file, one per individual. Positions not included in this list are treated as missing\n";
    cout << "\t           -sites_col|col: which column in infile.sites contain the site numbers (def: column 1)\n";
    cout << "\t           -in_fasta|fasta: fasta file containing original sequence data. First sequence should be reference genome, \n";
    cout << "\t                            followed by 2 sequences per diploid individual\n";
    cout << "\t           -tag: identifier to include in output\n";
    cout << "\t           -out_sum|out: file to write results\n";
    cout << "\t           -append: whether to append or overwrite outfile (def: overwrite)\n";
    cout << "\t           -errors: write detailed information on errors into file (def: do not write detailed info)\n";
    cout << "\t           -ignoreMA: If set to 1, will only consider genotypes RR, RA1 and A1A1, treating remaining genotype calls as ambiguous (default: 0, i.e. all 10 possible genotypes are considered).\n";
    
    cout << "\tdescription: Takes as input a fasta file with original sequence data, one or several vcf files, and one sites file per diploid individual.\n";
    cout << "\t             Reconstructs a new data matrix with rules: sites not present in the sites file, or considered ambiguous in the vcf file are coded as missing.\n";
    cout << "\t             Ambiguous sites in vcf file are those where more than one, or none of the genotypes, has 0 Phred-scaled likelihood. \n";
    cout << "\t             Remaining sites are coded according to most likely genotype in vcf file. If no genotype call in vcf file, site is coded\n";
    cout << "\t             homozygous for refrence. This new data matrix is then compared to the original fasta file. Output includes the number of \n";
    cout << "\t             homozygous and heterozygous SNPs, and the differnt outcomes (missed, correctly identified, and different error types).\n";
    cout << "\t             Results are reported overall and as function of Alternative Allele Count. If using -errors, reports to file the positions\n";
    cout << "\t             where errors occur, together with original and resulting genotypes, and AAC.\n";
    
    
    
    cout << "\n######\nNotes:\n######\n";
    cout << "   .'-option1|option2 value' means either option1 or option2 can be used to specify same argument\n";
    cout << "   .'[-option value]' means argument is optional\n";
    cout << "   .'[-option1 value || -option2 value ]' means one of the arguments must be present\n";
    
    cout << "   .most arguments' options can be shortened (e.g. -in instead of -infile)\n";
    cout << "   .running the different commands with '-verbose 1' will increase the information printed on screen\n\n";
    
}


vector <float> calc_dist( vector <int> in ){
    // vector <float> toreturn(2,0);  //mean var
    float mean = 0, var = 0;
    int count = 0;
    // starts at 1 to exclude 0 class (whre there is no missing data)
    for (unsigned int i = 1; i < in.size(); i++) {
        mean += ( in.at(i)*i );
        count += in.at(i);
    }
    mean = mean/count;
    
    for (unsigned int i = 1; i < in.size(); i++) {
        var += (i - mean) * (i - mean) * in.at(i);
        
    }
    
    var = var / (count - 1);
    vector <float> toreturn;
    toreturn.push_back(mean);
    toreturn.push_back(var);
    return toreturn;
}


void check_regions ( string regions, vector < unsigned int > &starts, vector < unsigned int > &ends, vector <string> &names ) {
    
    // case 1: single region defined in command line
    if( ( count(regions.begin(), regions.end(), ':')) ){
        // single region
        vector <string> fields;
        msplit(regions, ":", &fields );
        starts.push_back(atoi(fields.at(0).c_str()));
        ends.push_back(atoi(fields.at(1).c_str()));
        names.push_back("NA");
        if( starts.at(0) == 0 ||  ends.at(0) == 0){
            cout << "ERROR (regions) : error in definition of region to output (received " << regions << ")\n";
            exit(1);
        }
        
        
    }
    else{
        string line;
        ifstream infile;
        
        infile.open(regions.c_str());
        
        
        if( !infile.is_open() ) {
            cout << "ERROR (regions): Unable to open for output regions file " << regions << endl;
            exit(1);
        }
        while (getline(infile,line)) {
            if (line.length() == 0) {
                continue;
            }
            
            vector <string> fields;
            msplit( line, "\t", &fields );
            
            if ( fields.size() != 3 && fields.size() != 2 ){
                
                cout << "ERROR (regions): Malformatted region in file " << regions << ": " << line <<  endl;
                exit(1);
                
            }
            
            if( fields.size() == 3 ){
                starts.push_back(atoi(fields.at(0).c_str()));
                ends.push_back(atoi(fields.at(1).c_str()));
                names.push_back(fields.at(2));
                
            }else{
                starts.push_back(atoi(fields.at(0).c_str()));
                ends.push_back(atoi(fields.at(1).c_str()));
                names.push_back("NA");
                
            }
            
            
            
            
        }
        
        infile.close();
        if (starts.size() == 0 ) {
            cout << "ERROR (regions): no regions defined in input file " << regions << "?\n";
            exit(1);
        }
        
        
        for (unsigned int k = 0; k < starts.size(); k++) {
            if( starts.at(k) == 0 ||  ends.at(k) == 0 || ends.at(k) <= starts.at(k) ){
                cout << "ERROR (regions) : error in definition of region number " << k + 1 << " in input file " << regions << endl;
                exit(1);
            }
        }
        
    }
    
}
