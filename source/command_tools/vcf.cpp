//
//  vcf.cpp
//  Pipeliner
//
//  Copyright (c) 2013 Bruno Nevado. All rights reserved.
//


#include <fstream>
#include <cstring>

#include "vcf.h"


vcf::vcf ( string in, bool ignore_multihits ){
    infile = in;
    ignoremh = ignore_multihits;
    ifstream filehandle;
    string cline;
    filehandle.open(in.c_str());
    vector<string> tempnames;
    if(!filehandle.is_open()){
        cerr << "ERROR (reading vcf): Unable to open infile " << in << "\n" ;
        exit(1);
    }
    while( filehandle.is_open() ){
        getline(filehandle, cline);
        
        if( strncmp(cline.c_str(), "#CHR", 3) == 0){
            vector<string> fields;
            msplit( cline , "\t",  &fields );
            inds = int ( fields.size() - 9 );
            for( unsigned int f = 9; f < fields.size(); f++ )
                tempnames.push_back(fields.at(f));
        }
        
        else if( strncmp(cline.c_str(), "#", 1) == 0 ){
            continue;
            
        }
        break;
    }
    getline(filehandle, cline);
    vector<string> fields;
    msplit( cline , "\t",  &fields );
    chromosome = fields.at(0);
    indel_sites.resize( inds );
    ambiguous_sites.resize(inds );
    
    filehandle.close();
    
    if( (unsigned) inds == tempnames.size() ){
        names = tempnames;
    }
    else{
        names.resize(inds);
        for( int k = 0; k < inds; k++ )
            names.at(k) = infile + "_" + to_string ( static_cast < long long >( k + 1 ));
    }
}

// vcf CLASS SPEAKING FUNCTION

void vcf::info_to_stdout () {
    
    cout << "Infile: " << infile << " (" << inds << " individuals, " << matrix.size() << " sites from chromosome " << chromosome
    << ", names: " << names.at(0);
    
    for( int i = 1; i < num_inds(); i++)
        cout << ", " << names.at(i);
    
    cout << ")\n";
    /*
     for(int i = 0; i < positions.size(); i++)
     cout << positions.at(i) << " " << matrix.at(i)
     << " " << matrix[i][0] << " " << matrix[i][1] << " " << matrix[i][2] << "\n";
     exit(1);
     */
}


// VCF SLURP FILE

void vcf::slurp_file (){
    lines_processed=0;
    ifstream filehandle;
    string cline;
    filehandle.open(infile.c_str());
    if(!filehandle.is_open()){
        cerr << "ERROR (reading vcf): Unable to open infile " << infile << "\n" ;
        exit(1);
    }
    
    while( !filehandle.eof() ){
        getline(filehandle, cline);
        if( strncmp(cline.c_str(), "#", 1) == 0 || cline.size() == 0 ){
            continue;
        }
        else{
            lines_processed++;
            vector <string> genos;
            genos =     this->get_genotypes(cline.c_str());

            char *iupac_codes;
            iupac_codes = new char [this->num_inds()];
            
            
            for( unsigned int i = 2; i < genos.size(); i++){
                if (genos[i] == "ambiguous" || genos[i] == "indel" ){
                    iupac_codes[i-2] = 'n';
                }
                else{
                    iupac_codes[i-2] = toIUPAC(genos[i]);
                }
            }
            
            matrix.push_back(iupac_codes);
            positions.push_back(atoi(genos.at(0).c_str()));
        }
        
    }
}


// next genotypes


vector <string> vcf::get_genotypes (string line){
    vector <string> toreturn;
    
    if(line.size() > 0){
        vector<string> fields;
        msplit( line , "\t",  &fields );
        // site number
        toreturn.push_back(fields.at(1).c_str());
        // reference allele
        toreturn.push_back(  fields.at(3).c_str());
        // alternative alleles
        vector <string> alt_alleles;
        msplit( fields.at(4), ",", &alt_alleles );
        
        if( strncmp(fields.at(7).c_str(), "INDEL", 4) == 0 ){
            for( unsigned int col = 9; col < fields.size(); col++)
                toreturn.push_back("indel");
            
            for ( int ij = 0; ij < inds; ij++ ){
                if (strncmp(fields.at(9+ij).c_str(), "0/0", 3) != 0 ){
                    indel_sites.at(ij).push_back(  atoi ( fields.at(1).c_str() ) );
                }
            }
            return toreturn;
        }
        
        
        // check where phred-scaled likelihood is stored
        unsigned int pl_field=0;
        bool found_pl = false;
        vector <string> keys;
        msplit( fields.at(8), ":", &keys );
        unsigned int keys_num = keys.size();
        for(pl_field=0; pl_field < keys.size(); pl_field++){
            if( strncmp(keys.at(pl_field).c_str(), "PL", 2) == 0 ){
                found_pl = true;
                break;
            }
        }
        
        if(!found_pl){
            cerr << "ERROR (vcf, next_genotypes): infile " << infile << " does not seem to contain PL fields\n";
            exit(1);
        }
        
        for( unsigned int col = 9; col < fields.size(); col++){
            int cind0 = col - 9;
            vector <string> values;
            msplit( fields.at(col), ":", &values );
            
            if( values.size() != keys_num ){
                cout << "WARNING (summarise): number of keys and values in vcf line does not match, SNP call considered missing (site: "
                << fields.at(1) << " ind: " << cind0+1 << ")" << endl;
                toreturn.push_back("nn");
                ambiguous_sites.at(cind0).push_back(  atoi ( fields.at(1).c_str() ) );
            }
            else{
            
            vector <string> pl_values;
            msplit( values.at(pl_field), ",", &pl_values );
            int min_pl_field = -1 , num_zeros = 0;
            for( unsigned int index_pl = 0; index_pl < pl_values.size(); index_pl++ ){
                if ( atoi(pl_values.at(index_pl).c_str()) == 0 ){
                    min_pl_field = index_pl;
                    num_zeros++;
                }
            }
            
            if( num_zeros != 1 ){
                toreturn.push_back("ambiguous");
                ambiguous_sites.at(cind0).push_back(  atoi ( fields.at(1).c_str() ) );
            }
            else{
                // min_pl_field, alt_alleles
                string geno;
                // check input first
                if(  (alt_alleles.size() == 1 && pl_values.size() != 3 )
                   || (alt_alleles.size() == 2 && pl_values.size() != 6 )
                   || (alt_alleles.size() == 3 && pl_values.size() != 10 )
                   || (alt_alleles.size() == 4 && pl_values.size() != 15 )
                   
                   ){
                    cerr << "ERROR (get_geno_from_vcf): number of PL fields and alt alleles does not match. Offending line: " << line << endl;
                    exit(1);
                }
                
                
                if(ignoremh && min_pl_field > 2){
                    toreturn.push_back("ambiguous");
                    ambiguous_sites.at(cind0).push_back(  atoi ( fields.at(1).c_str() ) );
                }
                else{
                switch(min_pl_field){
                    case 0: // RR
                        geno = fields.at(3) + fields.at(3);
                        toreturn.push_back(geno);
                        break;
                    case 1: //RA1
                        geno = fields.at(3) + alt_alleles.at(0);
                        toreturn.push_back(geno);
                        break;
                    case 2: //A1A1
                        geno = alt_alleles.at(0) + alt_alleles.at(0);
                        toreturn.push_back(geno);
                        break;
                    case 3: //RA2
                        geno = fields.at(3) + alt_alleles.at(1);
                        toreturn.push_back(geno);
                        break;
                    case 4: //A1A2
                        geno = alt_alleles.at(0) + alt_alleles.at(1);
                        toreturn.push_back(geno);
                        break;
                    case 5: // A2A2
                        geno = alt_alleles.at(1) + alt_alleles.at(1);
                        toreturn.push_back(geno);
                        break;
                    case 6: // RA3
                        geno = fields.at(3) + alt_alleles.at(2);
                        toreturn.push_back(geno);
                        break;
                    case 7: // A1A3
                        geno = alt_alleles.at(0) + alt_alleles.at(2);
                        toreturn.push_back(geno);
                        break;
                    case 8:  // A2A3
                        geno = alt_alleles.at(1) + alt_alleles.at(2);
                        toreturn.push_back(geno);
                        break;
                    case 9:  // A3A3
                        geno = alt_alleles.at(2) + alt_alleles.at(2);
                        toreturn.push_back(geno);
                        break;
                    case 10:  
                        geno = fields.at(3) + alt_alleles.at(3);
                        toreturn.push_back(geno);
                        break;
                    case 11:
                        geno = alt_alleles.at(0) + alt_alleles.at(3);
                        toreturn.push_back(geno);
                        break;
                    case 12:
                        geno = alt_alleles.at(1) + alt_alleles.at(3);
                        toreturn.push_back(geno);
                        break;
                    case 13:
                        geno = alt_alleles.at(2) + alt_alleles.at(3);
                        toreturn.push_back(geno);
                        break;
                    case 14:
                        geno = alt_alleles.at(3) + alt_alleles.at(3);
                        toreturn.push_back(geno);
                        break;
                    default:
                        cerr << "ERROR(vcf, next_genotypes): Unable to process line" << line << "\n";
                }
                }
            }
        }
    }
        
    }
    
    for( unsigned int i  = 1; i < toreturn.size(); i++ )
        for( unsigned int j = 0; j < toreturn.at(i).size(); j++ )
            toreturn.at(i).at(j) = tolower(toreturn.at(i).at(j));
    
    return toreturn;
}



