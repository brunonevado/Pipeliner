//
//  summarise.cpp
//  Pipeliner
//
//  Copyright (c) 2013 Bruno Nevado. All rights reserved.
//

#include "summarise.h"

summarise::summarise(unsigned int nhaplos, int length){
    len = length;
    num_inds = nhaplos / 2;
    tags.resize(num_inds);
    input_vcfs.resize(num_inds);
    input_sites.resize(num_inds);
    
    vector <int> empty ( nhaplos , 0 )  ;
    for (unsigned int ind = 0; ind < nhaplos/2; ind++) {
        total_screened.push_back(0);
        total_calls.push_back(0);
        
        total_aa.push_back(empty);
        aa_aa.push_back(empty);
        aa_seen.push_back(empty);
        aa_missed.push_back(empty);
        aa_aa.push_back(empty);
        aa_rr.push_back(empty);
        aa_ra.push_back(empty);
        aa_ao.push_back(empty);
        aa_ro.push_back(empty);
        aa_oo.push_back(empty);
        aa_oo2.push_back(empty);
        
        total_ra.push_back(empty);
        ra_seen.push_back(empty);
        ra_missed.push_back(empty);
        ra_ra.push_back(empty);
        ra_rr.push_back(empty);
        ra_aa.push_back(empty);
        ra_ao.push_back(empty);
        ra_ro.push_back(empty);
        ra_oo.push_back(empty);
        ra_oo2.push_back(empty);
        
        rr_oo.push_back(0);
        rr_ro.push_back(0);
        rr_oo2.push_back(0);
        
        aa_amb.push_back(empty);
        aa_indel.push_back(empty);
        ra_amb.push_back(empty);
        ra_indel.push_back(empty);
        
        invar_amb.push_back(0);
        invar_indel.push_back(0);
        total_amb.push_back(0);
        
    }
    
}

void summarise::summarise_ind( fasta &preseq, fasta &posseq, int index0_ind,
                              vector < unsigned int  > ambiguous,
                              vector < unsigned int  > indels,
                              string tag, string vcffile, string sitesfile, string err_file  ){
    
    vector < string > err_types, err_genotypes;
    vector < int > err_positions;
    
    
    input_vcfs.at(index0_ind) = vcffile;
    input_sites.at(index0_ind) = sitesfile;
    int index0_haplo1 = ( (index0_ind + 1) * 2 ) - 1;  // lines of current ind in preseq fasta file (2 lines per ind)
    int index0_haplo2 = (index0_ind + 1)* 2 ;        //   note that posseq fasta file has a single line per ind
    tags.at(index0_ind) = tag;
    for (unsigned int site0 = 0; site0 < preseq.num_bases(); site0++ ) {
        string original_conf;
        original_conf.push_back(preseq.show_base(index0_haplo1, site0));
        original_conf.push_back(preseq.show_base(index0_haplo2, site0));
        char ref = preseq.show_base(0, site0);
        
        if( original_conf.at(0) == original_conf.at(1) && original_conf.at(0) != ref ){
            
            ////////////////////
            // homozygous SNP //
            ////////////////////
            
            string out_ar;

            
            out_ar.push_back(original_conf.at(0));
            out_ar.push_back(ref);
            int dac = preseq.calc_dac(original_conf.at(0), site0 + 1 );
            
            string genotypes;
            genotypes.push_back(ref);
            genotypes.append(original_conf);
            genotypes.push_back('_');
            genotypes.push_back(fromIUPAC(posseq.show_base(index0_ind  , site0))[0] );
            genotypes.push_back(fromIUPAC(posseq.show_base(index0_ind  , site0))[1] );
      
            total_aa.at( index0_ind  ).at(dac - 1)++;
                        
            if (posseq.show_base( index0_ind , site0) == original_conf.at(0)){
                // correct, and screened, and called
                aa_aa.at( index0_ind  ).at(dac - 1)++;
                total_screened.at( index0_ind  )++;
                aa_seen.at( index0_ind ).at(dac - 1)++;
                total_calls.at( index0_ind  )++;
                
            }
            else if ( posseq.show_base( index0_ind  , site0) == 'n' ){
                // missed
                aa_missed.at( index0_ind  ).at(dac - 1)++;
            }
            else if ( posseq.show_base( index0_ind  , site0) == ref ){
                // aa_called_ref
                aa_rr.at( index0_ind  ).at(dac - 1)++;
                aa_seen.at( index0_ind ).at(dac - 1)++;
                total_screened.at( index0_ind  )++;
                err_types.push_back("aa_rr");
                err_positions.push_back(site0+1);
                err_genotypes.push_back( genotypes  );
            }
            else if ( posseq.show_base( index0_ind  , site0) == toIUPAC(out_ar) ){
                // aa_called_ar
                aa_ra.at( index0_ind  ).at(dac - 1)++;
                aa_seen.at( index0_ind ).at(dac - 1)++;
                total_screened.at( index0_ind  )++;
                total_calls.at( index0_ind  )++;
                err_types.push_back("aa_ra");
                err_positions.push_back(site0+1);
                err_genotypes.push_back( genotypes  );

                
            }
            else if (   posseq.show_base( index0_ind  , site0) == 'a'
                     || posseq.show_base( index0_ind  , site0) == 'c'
                     || posseq.show_base( index0_ind  , site0) == 'g'
                     || posseq.show_base( index0_ind  , site0) == 't'
                     ){
                // aa_called_oo
                aa_oo.at( index0_ind  ).at(dac - 1)++;
                aa_seen.at( index0_ind ).at(dac - 1)++;
                total_screened.at( index0_ind  )++;
                total_calls.at( index0_ind  )++;
                err_types.push_back("aa_oo");
                err_positions.push_back(site0+1);
                err_genotypes.push_back( genotypes  );

                
            }
            else if (  fromIUPAC(posseq.show_base(index0_ind  , site0))[0] == ref
                     || fromIUPAC(posseq.show_base(index0_ind  , site0))[1] == ref
                     ){
                // aa_called_ro
                aa_ro.at( index0_ind  ).at(dac - 1)++;
                aa_seen.at( index0_ind ).at(dac - 1)++;
                total_screened.at( index0_ind  )++;
                total_calls.at( index0_ind  )++;
                err_types.push_back("aa_ro");
                err_positions.push_back(site0+1);
                err_genotypes.push_back( genotypes  );

                
            }
            else if (  fromIUPAC(posseq.show_base(index0_ind  , site0))[0] == original_conf.at(0)
                     || fromIUPAC(posseq.show_base(index0_ind  , site0))[1] == original_conf.at(0)
                     ){
                // aa_called_ao
                aa_ao.at( index0_ind  ).at(dac - 1)++;
                aa_seen.at( index0_ind ).at(dac - 1)++;
                total_screened.at( index0_ind  )++;
                total_calls.at( index0_ind  )++;
                err_types.push_back("aa_ao");
                err_positions.push_back(site0+1);
                err_genotypes.push_back( genotypes  );

                
            }
            else if (  fromIUPAC(posseq.show_base(index0_ind  , site0) ).size() == 2
                     ){
                // aa_called_oo2. the condition is just to check for the spanish inquisition on the next else
                // fromIUPAC will crash with an error if the bases are wrong
                aa_oo2.at( index0_ind  ).at(dac - 1)++;
                aa_seen.at( index0_ind ).at(dac - 1)++;
                total_screened.at( index0_ind  )++;
                total_calls.at( index0_ind  )++;
                err_types.push_back("aa_oo2");
                err_positions.push_back(site0+1);
                err_genotypes.push_back( genotypes  );

                
            }
            else{
                cerr << "ERROR(summarise): site " << site0 + 1 << " slipped through the cracks!\n";
                exit(1);
            }
            continue;
            // cout << "fixed: " << site0+1 << endl;
        }
        
        
        else if( original_conf.at(0) != original_conf.at(1) && ( original_conf.at(0) == ref || original_conf.at(1) == ref ) ){
            
            ///////////////////////
            // heterozygous SNP  //
            ///////////////////////
            
            char alt = ( original_conf.at(0) == ref ) ? original_conf.at(1) : original_conf.at(0) ;
            int dac = preseq.calc_dac(alt, site0 + 1 );
            total_ra.at( index0_ind  ).at(dac - 1 )++;
          
            string genotypes;
            genotypes.push_back(ref);
            genotypes.append(original_conf);
            genotypes.push_back('_');
            genotypes.push_back(fromIUPAC(posseq.show_base(index0_ind  , site0))[0] );
            genotypes.push_back(fromIUPAC(posseq.show_base(index0_ind  , site0))[1] );
        
            string ra;
            ra.push_back(ref);
            ra.push_back(alt);
            
            if (posseq.show_base( index0_ind , site0) ==  toIUPAC( ra )  ){
                // correct, and screened, and called
                ra_ra.at( index0_ind  ).at(dac - 1)++;
                total_screened.at( index0_ind  )++;
                ra_seen.at( index0_ind ).at(dac - 1)++;
                total_calls.at( index0_ind  )++;
                
            }
            else if ( posseq.show_base( index0_ind  , site0) == 'n' ){
                // missed
                ra_missed.at( index0_ind  ).at(dac - 1)++;
            }
            else if ( posseq.show_base( index0_ind  , site0) == ref ){
                // ra_called_ref_ref
                ra_rr.at( index0_ind  ).at(dac - 1)++;
                ra_seen.at( index0_ind ).at(dac - 1)++;
                total_screened.at( index0_ind  )++;
                err_types.push_back("ra_rr");
                err_positions.push_back(site0+1);
                err_genotypes.push_back( genotypes  );

            }
            else if ( posseq.show_base( index0_ind  , site0) == alt ){
                // ra_called_alt_alt
                ra_aa.at( index0_ind  ).at(dac - 1)++;
                ra_seen.at( index0_ind ).at(dac - 1)++;
                total_screened.at( index0_ind  )++;
                total_calls.at( index0_ind  )++;
                err_types.push_back("ra_aa");
                err_positions.push_back(site0+1);
                err_genotypes.push_back( genotypes  );

            }
            
            else if (   posseq.show_base( index0_ind  , site0) == 'a'
                     || posseq.show_base( index0_ind  , site0) == 'c'
                     || posseq.show_base( index0_ind  , site0) == 'g'
                     || posseq.show_base( index0_ind  , site0) == 't'
                     ){
                // ra_called_oo
                ra_oo.at( index0_ind  ).at(dac - 1)++;
                ra_seen.at( index0_ind ).at(dac - 1)++;
                total_screened.at( index0_ind  )++;
                total_calls.at( index0_ind  )++;
                err_types.push_back("ra_oo");
                err_positions.push_back(site0+1);
                err_genotypes.push_back( genotypes  );

            }
            else if (  fromIUPAC(posseq.show_base(index0_ind  , site0))[0] == ref
                     || fromIUPAC(posseq.show_base(index0_ind  , site0))[1] == ref
                     ){
                // ra_called_ro
                ra_ro.at( index0_ind  ).at(dac - 1)++;
                ra_seen.at( index0_ind ).at(dac - 1)++;
                total_screened.at( index0_ind  )++;
                total_calls.at( index0_ind  )++;
                err_types.push_back("ra_ro");
                err_positions.push_back(site0+1);
                err_genotypes.push_back( genotypes  );

            }
            else if (  fromIUPAC(posseq.show_base(index0_ind  , site0))[0] == alt
                     || fromIUPAC(posseq.show_base(index0_ind  , site0))[1] == alt
                     ){
                // ra_called_ao
                ra_ao.at( index0_ind  ).at(dac - 1)++;
                ra_seen.at( index0_ind ).at(dac - 1)++;
                total_screened.at( index0_ind  )++;
                total_calls.at( index0_ind  )++;
                err_types.push_back("ra_ao");
                err_positions.push_back(site0+1);
                err_genotypes.push_back( genotypes  );

            }
            else if (  fromIUPAC(posseq.show_base(index0_ind  , site0) ).size() == 2
                     ){
                // ra_called_oo2. the condition is just to check for the spanish inquisition on the next else
                // fromIUPAC will crash with an error if the bases are wrong
                ra_oo2.at( index0_ind  ).at(dac - 1)++;
                ra_seen.at( index0_ind ).at(dac - 1)++;
                total_screened.at( index0_ind  )++;
                total_calls.at( index0_ind  )++;
                err_types.push_back("ra_oo2");
                err_positions.push_back(site0+1);
                err_genotypes.push_back( genotypes  );

            }
            else{
                cerr << "ERROR(summarise): site " << site0 + 1 << " slipped through the cracks!\n";
                exit(1);
            }
            
            continue;
        }
        else if ( original_conf.at(0) == original_conf.at(1) && original_conf.at(0) == ref ){
            
            /////////////////////
            // invariable site //
            /////////////////////
            
            
            if ( posseq.show_base( index0_ind  , site0) == ref ){
                // screened
                total_screened.at( index0_ind  )++;
                
            }
            else if (  posseq.show_base( index0_ind  , site0) != 'n' ){
                // screened, but wrong call
                
                string genotypes;
                genotypes.push_back(ref);
                genotypes.append(original_conf);
                genotypes.push_back('_');
                genotypes.push_back(fromIUPAC(posseq.show_base(index0_ind  , site0))[0] );
                genotypes.push_back(fromIUPAC(posseq.show_base(index0_ind  , site0))[1] );

                
                
                total_calls.at( index0_ind  )++;
                total_screened.at( index0_ind  )++;
                if (   posseq.show_base( index0_ind  , site0) == 'a'
                    || posseq.show_base( index0_ind  , site0) == 'c'
                    || posseq.show_base( index0_ind  , site0) == 'g'
                    || posseq.show_base( index0_ind  , site0) == 't'
                    ){
                    rr_oo.at( index0_ind  )++;
                    err_types.push_back("rr_oo");
                    err_positions.push_back(site0+1);
                    err_genotypes.push_back( genotypes  );

                }
                else if ( fromIUPAC(posseq.show_base(index0_ind  , site0) ).at(0) == ref
                         || fromIUPAC(posseq.show_base(index0_ind  , site0) ).at(1) == ref
                         ){
                    rr_ro.at( index0_ind  )++;
                    err_types.push_back("rr_ro");
                    err_positions.push_back(site0+1);
                    err_genotypes.push_back( genotypes  );
                
                }
                
                else if ( fromIUPAC(posseq.show_base(index0_ind  , site0) ).size() == 2 ){
                    rr_oo2.at( index0_ind  )++;
                    err_types.push_back("rr_oo2");
                    err_positions.push_back(site0+1);
                    err_genotypes.push_back( genotypes  );

                }
                
                
            }
            
            
            continue;
        }
        else{
            cout << "WARNING, tri allelic site found and ignored (positions: " << site0 + 1 << ", ref: "
            << ref << " original: " << original_conf << " res: " << fromIUPAC(posseq.show_base(index0_ind  , site0))[0]
            << fromIUPAC(posseq.show_base(index0_ind  , site0))[1] << ")\n";
            
        }
        
        
        /*
         if (posseq.show_base(index1_ind - 1, site0) != 'n') {
         total_screened.at( index1_ind - 1 )++;
         }
         */
        
        
    }
    
    // now account for ambiguous and indel calls
    total_amb.at(index0_ind) += ambiguous.size();
    
    for (unsigned int iamb = 0; iamb < ambiguous.size(); iamb++) {
        if(   ( preseq.show_base(index0_haplo1, (ambiguous.at(iamb) - 1)  ) == preseq.show_base(index0_haplo2, (ambiguous.at(iamb) - 1)  ) )
           && ( preseq.show_base(index0_haplo1, (ambiguous.at(iamb) - 1)  ) == preseq.show_base(0, (ambiguous.at(iamb) - 1)  )   )
           ){
            // invar called ambiguous
            invar_amb.at(index0_ind)++;
        }
        else if ( ( preseq.show_base(index0_haplo1, (ambiguous.at(iamb) - 1)  ) == preseq.show_base(index0_haplo2, (ambiguous.at(iamb) - 1)  ) )
                 && ( ( preseq.show_base(index0_haplo1, (ambiguous.at(iamb) - 1)  ) != preseq.show_base(0, (ambiguous.at(iamb) - 1)  )   )
                     || ( preseq.show_base(index0_haplo2, (ambiguous.at(iamb) - 1)  ) != preseq.show_base(0, (ambiguous.at(iamb) - 1)  )   )
                     )
                 ){
            // homozygous SNP called ambiguous
            int dac = preseq.calc_dac( ( preseq.show_base(index0_haplo1, (ambiguous.at(iamb) - 1)  ) ) , ambiguous.at(iamb) );
            aa_amb.at(index0_ind).at( dac - 1 )++;
            
            
        }
        else if ( ( preseq.show_base(index0_haplo1, (ambiguous.at(iamb) - 1)  ) != preseq.show_base(index0_haplo2, (ambiguous.at(iamb) - 1)  ) )
                 && ( ( preseq.show_base(index0_haplo1, (ambiguous.at(iamb) - 1)  ) == preseq.show_base(0, (ambiguous.at(iamb) - 1)  )   )
                     || ( preseq.show_base(index0_haplo2, (ambiguous.at(iamb) - 1)  ) == preseq.show_base(0, (ambiguous.at(iamb) - 1)  )   )
                     )
                 ){
            // heterozygous SNP called ambiguous
            int dac = preseq.calc_dac( ( preseq.show_base(index0_haplo1, (ambiguous.at(iamb) - 1)  ) ) , ambiguous.at(iamb) );
            ra_amb.at(index0_ind).at( dac - 1 )++;
        }
        else{
            cout << "WARNING, tri allelic site found and ignored\n";
        }
        
    }
    // and indels
    
    for (unsigned int iindel = 0; iindel < indels.size(); iindel++) {
        if(   ( preseq.show_base(index0_haplo1, (indels.at(iindel) - 1)  ) == preseq.show_base(index0_haplo2, (indels.at(iindel) - 1)  ) )
           && ( preseq.show_base(index0_haplo1, (indels.at(iindel) - 1)  ) == preseq.show_base(0, (indels.at(iindel) - 1)  )   )
           ){
            // invar called indels
            invar_indel.at(index0_ind)++;
            total_calls.at(index0_ind)++;
            total_screened.at(index0_ind)++;
            
        }
        else if ( ( preseq.show_base(index0_haplo1, (indels.at(iindel) - 1)  ) == preseq.show_base(index0_haplo2, (indels.at(iindel) - 1)  ) )
                 && ( ( preseq.show_base(index0_haplo1, (indels.at(iindel) - 1)  ) != preseq.show_base(0, (indels.at(iindel) - 1)  )   )
                     || ( preseq.show_base(index0_haplo2, (indels.at(iindel) - 1)  ) != preseq.show_base(0, (indels.at(iindel) - 1)  )   )
                     )
                 ){
            // homozygous SNP called indels
            int dac = preseq.calc_dac( ( preseq.show_base(index0_haplo1, (indels.at(iindel) - 1)  ) ) , indels.at(iindel) );
            aa_indel.at(index0_ind).at( dac - 1 )++;
            total_calls.at(index0_ind)++;
            total_screened.at(index0_ind)++;
            aa_missed.at(index0_ind).at( dac - 1 )--;
            aa_seen.at(index0_ind).at( dac - 1 )++;
            
            
        }
        else if ( ( preseq.show_base(index0_haplo1, (indels.at(iindel) - 1)  ) != preseq.show_base(index0_haplo2, (indels.at(iindel) - 1)  ) )
                 && ( ( preseq.show_base(index0_haplo1, (indels.at(iindel) - 1)  ) == preseq.show_base(0, (indels.at(iindel) - 1)  )   )
                     || ( preseq.show_base(index0_haplo2, (indels.at(iindel) - 1)  ) == preseq.show_base(0, (indels.at(iindel) - 1)  )   )
                     )
                 ){
            // heterozygous SNP called indels
            int dac = preseq.calc_dac( ( preseq.show_base(index0_haplo1, (indels.at(iindel) - 1)  ) ) , indels.at(iindel) );
            ra_indel.at(index0_ind).at( dac - 1 )++;
            total_calls.at(index0_ind)++;
            total_screened.at(index0_ind)++;
            ra_missed.at(index0_ind).at( dac - 1 )--;
            ra_seen.at(index0_ind).at( dac - 1 )++;
            
        }
        else{
            cout << "WARNING, tri allelic site found and ignored\n";
        }
        
    }
    
    if( err_file != "NA" ){
        ofstream errFile;

        errFile.open(err_file.c_str(), ios::app );
        if( !errFile.is_open() ){
            cerr << "ERROR (summarise): unable to open for output error file " << err_file << "\n";
        }
        for ( unsigned int i = 0; i < err_positions.size() ; i++) {
            errFile << index0_ind + 1 << "\t" << err_positions.at(i) << "\t" << err_types.at(i) << "\t" << err_genotypes.at(i) << endl;
        }
        errFile.close();
    }
    
}

void summarise::print( string outfile, bool append ) {
    
    ofstream outputFile;
    
    if( !append  )
        outputFile.open(outfile.c_str());
    else
        outputFile.open(outfile.c_str(), ios::app );
    
    if( !outputFile.is_open() ){
        cerr << "ERROR (print_summary): unable to open for output file " << outfile << "\n";
        exit(1);
    }
    string nas = "\tNA";
    for ( int i = 1; i < num_inds * 2; i++ ) {
        nas += "\tNA";
    }
    
    for (int i = 0; i < num_inds; i++) {
        // HEADER
        outputFile <<"# summarised from " << input_vcfs.at(i) << " and " << input_sites.at(i) << endl;
        outputFile <<"# length " << len << " bp and " << num_inds  << " individuals" << endl;
        outputFile <<"# ID\twhat\ttotal\tper_DAC: 1/Nhaplos .. 2/Nhaplos .. N/Nhaplos" << endl;
        
        
        outputFile <<tags.at(i) << "\tT_seen\t" << total_screened.at(i) << nas << endl;
        
        outputFile <<tags.at(i) << "\tT_calls\t" << total_calls.at(i)  << nas << endl;
        
        outputFile <<tags.at(i) << "\tT_amb\t" << total_amb.at(i)  << nas << endl;
        //
        outputFile <<tags.at(i) << "\trr_ro\t" << rr_ro.at(i)  << nas << endl;
  
        outputFile <<tags.at(i) << "\trr_oo\t" << rr_oo.at(i)  << nas << endl;
        
        outputFile <<tags.at(i) << "\trr_oo2\t" << rr_oo2.at(i)  << nas << endl;
        
        outputFile <<tags.at(i) << "\trr_amb\t" << invar_amb.at(i)  << nas << endl;
        
        outputFile <<tags.at(i) << "\trr_ind\t" << invar_indel.at(i)  << nas << endl;
        //
        outputFile <<tags.at(i) <<  "\tT_aa\t" << sum_vec_ints(total_aa.at(i));
        for (unsigned int j = 0; j < total_aa.at(i).size(); j++)
            outputFile <<"\t" << total_aa.at(i).at(j);
        outputFile <<endl;
        //
        outputFile <<tags.at(i) <<  "\taa_mis\t" << sum_vec_ints(aa_missed.at(i));
        for (unsigned int j = 0; j < aa_missed.at(i).size(); j++)
            outputFile <<"\t" << aa_missed.at(i).at(j);
        outputFile <<endl;
        //
        outputFile <<tags.at(i) <<  "\taa_see\t" << sum_vec_ints(aa_seen.at(i));
        
        for (unsigned int j = 0; j < aa_seen.at(i).size(); j++)
            outputFile <<"\t" << aa_seen.at(i).at(j);
        outputFile <<endl;
        //
        outputFile <<tags.at(i) << "\taa_amb\t" << sum_vec_ints(aa_amb.at(i));
        for (unsigned int j = 0; j < aa_amb.at(i).size(); j++)
            outputFile <<"\t" << aa_amb.at(i).at(j);
        outputFile <<endl;
        //
        outputFile <<tags.at(i) << "\taa_ind\t" << sum_vec_ints(aa_indel.at(i));
        for (unsigned int j = 0; j < aa_indel.at(i).size(); j++)
            outputFile <<"\t" << aa_indel.at(i).at(j);
        outputFile <<endl;
        //
        outputFile <<tags.at(i) <<  "\taa_aa\t" << sum_vec_ints(aa_aa.at(i));
        for (unsigned int j = 0; j < aa_aa.at(i).size(); j++)
            outputFile <<"\t" << aa_aa.at(i).at(j);
        outputFile <<endl;
        //
        outputFile <<tags.at(i) <<  "\taa_rr\t"  << sum_vec_ints(aa_rr.at(i));
        for (unsigned int j = 0; j < aa_rr.at(i).size(); j++)
            outputFile <<"\t" << aa_rr.at(i).at(j);
        outputFile <<endl;
        //
        outputFile <<tags.at(i) <<  "\taa_ra\t"  << sum_vec_ints(aa_ra.at(i));
        for (unsigned int j = 0; j < aa_ra.at(i).size(); j++)
            outputFile <<"\t" << aa_ra.at(i).at(j);
        outputFile <<endl;
        //
        outputFile <<tags.at(i) <<  "\taa_oo\t"  << sum_vec_ints(aa_oo.at(i));
        for (unsigned int j = 0; j < aa_oo.at(i).size(); j++)
            outputFile <<"\t" << aa_oo.at(i).at(j);
        outputFile <<endl;
        //
        outputFile <<tags.at(i) <<  "\taa_ro\t"  << sum_vec_ints(aa_ro.at(i));
        for (unsigned int j = 0; j < aa_ro.at(i).size(); j++)
            outputFile <<"\t" << aa_ro.at(i).at(j);
        outputFile <<endl;
        //
        outputFile <<tags.at(i) <<  "\taa_ao\t"  << sum_vec_ints(aa_ao.at(i));
        for (unsigned int j = 0; j < aa_ao.at(i).size(); j++)
            outputFile <<"\t" << aa_ao.at(i).at(j);
        outputFile <<endl;
        //
        outputFile <<tags.at(i) <<  "\taa_oo2\t"  << sum_vec_ints(aa_oo2.at(i));
        for (unsigned int j = 0; j < aa_oo2.at(i).size(); j++)
            outputFile <<"\t" << aa_oo2.at(i).at(j);
        outputFile <<endl;
        
        outputFile <<tags.at(i) <<  "\tT_ra\t"  << sum_vec_ints(total_ra.at(i));
        for (unsigned int j = 0; j < total_ra.at(i).size(); j++)
            outputFile <<"\t" << total_ra.at(i).at(j);
        outputFile <<endl;
        //
        outputFile <<tags.at(i) <<  "\tra_mis\t"  << sum_vec_ints(ra_missed.at(i));
        for (unsigned int j = 0; j < ra_missed.at(i).size(); j++)
            outputFile <<"\t" << ra_missed.at(i).at(j);
        outputFile <<endl;
        //
        outputFile <<tags.at(i) <<  "\tra_see\t"  << sum_vec_ints(ra_seen.at(i));
        for (unsigned int j = 0; j < ra_seen.at(i).size(); j++)
            outputFile <<"\t" << ra_seen.at(i).at(j);
        outputFile <<endl;
        //
        outputFile <<tags.at(i) << "\tra_amb\t" << sum_vec_ints(ra_amb.at(i));
        for (unsigned int j = 0; j < ra_amb.at(i).size(); j++)
            outputFile <<"\t" << ra_amb.at(i).at(j);
        outputFile <<endl;
        //
        outputFile <<tags.at(i) << "\tra_ind\t" << sum_vec_ints(ra_indel.at(i));
        for (unsigned int j = 0; j < ra_indel.at(i).size(); j++)
            outputFile <<"\t" << ra_indel.at(i).at(j);
        outputFile <<endl;
        //
        outputFile <<tags.at(i) <<  "\tra_ra\t"  << sum_vec_ints(ra_ra.at(i));
        for (unsigned int j = 0; j < ra_ra.at(i).size(); j++)
            outputFile <<"\t" << ra_ra.at(i).at(j);
        outputFile <<endl;
        //
        outputFile <<tags.at(i) <<  "\tra_rr\t"  << sum_vec_ints(ra_rr.at(i));
        for (unsigned int j = 0; j < ra_rr.at(i).size(); j++)
            outputFile <<"\t" << ra_rr.at(i).at(j);
        outputFile <<endl;
        //
        outputFile <<tags.at(i) <<  "\tra_aa\t"  << sum_vec_ints(ra_aa.at(i));
        for (unsigned int j = 0; j < ra_aa.at(i).size(); j++)
            outputFile <<"\t" << ra_aa.at(i).at(j);
        outputFile <<endl;
        //
        outputFile <<tags.at(i) <<  "\tra_ao\t"  << sum_vec_ints(ra_ao.at(i));
        for (unsigned int j = 0; j < ra_ao.at(i).size(); j++)
            outputFile <<"\t" << ra_ao.at(i).at(j);
        outputFile <<endl;
        outputFile <<tags.at(i) <<  "\tra_ro\t"  << sum_vec_ints(ra_ro.at(i));
        for (unsigned int j = 0; j < ra_ro.at(i).size(); j++)
            outputFile <<"\t" << ra_ro.at(i).at(j);
        outputFile <<endl;
        //
        outputFile <<tags.at(i) <<  "\tra_oo\t"  << sum_vec_ints(ra_oo.at(i));
        for (unsigned int j = 0; j < ra_oo.at(i).size(); j++)
            outputFile <<"\t" << ra_oo.at(i).at(j);
        outputFile <<endl;
        //
        outputFile <<tags.at(i) <<  "\tra_oo2\t"  << sum_vec_ints(ra_oo2.at(i));
        for (unsigned int j = 0; j < ra_oo2.at(i).size(); j++)
            outputFile <<"\t" << ra_oo2.at(i).at(j);
        outputFile <<endl;
    }
    
    outputFile.close();
}


