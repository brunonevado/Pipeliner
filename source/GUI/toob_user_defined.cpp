//
//  toob_user_defined.cpp
//  pGUI
//
//  Copyright (c) 2013 Bruno Nevado. All rights reserved.
//

#include "toob_user_defined.h"
#include <iostream>
#include <vector>
#include <string>
#include <sstream>


std::string ReplaceString(std::string subject, const std::string& search, const std::string& replace, int & counter) {
  // http://stackoverflow.com/questions/4643512/replace-substring-with-another-substring-c

  if( subject.find_first_not_of(" \t") != -1 && subject.at( subject.find_first_not_of(" \t") ) == '#' ){
      return subject;
}
  else{
      size_t pos = 0;
      while((pos = subject.find(search, pos)) != std::string::npos) {
          subject.replace(pos, search.length(), replace);
          counter++;
          pos += replace.length();
        }
      return subject;
    }
}




toob_in_user::toob_in_user () {

    toob_type = "input.user";
    ancestral_file = "";
    seq_len = 100000;
    random_ancestral = true;
    ms_format = true;
    find_fas = "infile.fas";
    replace_fas = "$RUNFOLDER/$RUNPREFIX.replicate$REP.fas";
    find_ms = "infile.ms" ;
    replace_ms = "$RUNFOLDER/$RUNPREFIX.replicate$REP.ms";
    find_temp = "tempfile";
    replace_temp = "$RUNFOLDER/$RUNPREFIX.replicate$REP";
    keywords.push_back("infile\\.fas");
    keywords.push_back("infile\\.ms");
    keywords.push_back("tempfile");
}

toob_ngs_user::toob_ngs_user() {

  toob_type = "ngs.user";
  num_ends = 2;
  find_temp = "tempfile";
  replace_temp = "$RUNFOLDER/$RUNPREFIX.replicate$REP";
  find_infas = "infile.fas";
  replace_infas = "$RUNFOLDER/$RUNPREFIX.replicate$REP.Ind$IND.fas";
  find_outfasq = "outfile.fq";
  replace_outfasq = "$RUNFOLDER/$RUNPREFIX.replicate$REP.Ind$IND.fas..fq";
  find_outfasq1 = "outfile.1.fq";
  replace_outfasq1 = "$RUNFOLDER/$RUNPREFIX.replicate$REP.Ind$IND.fas.1.fq";
  find_outfasq2 = "outfile.2.fq";
  replace_outfasq2 = "$RUNFOLDER/$RUNPREFIX.replicate$REP.Ind$IND.fas.2.fq";
  find_number = "ind_number";
  replace_number = "$IND";

  keywords.push_back("infile\\.fas");
  keywords.push_back("outfile\\.fq");
  keywords.push_back("outfile\\.1\\.fq");
  keywords.push_back("outfile\\.2\\.fq");
  keywords.push_back("ind_number");
  keywords.push_back("tempfile");

  }

toob_align_user::toob_align_user() {

  toob_type = "align.user";
  num_ends = 2;
  addReadGroups = true;
  createDict = false;
  find_temp       = "tempfile";
  replace_temp    = "$RUNFOLDER/$RUNPREFIX.replicate$REP";
  find_number     = "ind_number";
  replace_number  = "$IND";
  find_ref        = "reference.fa";
  replace_ref     = "$RUNFOLDER/$RUNPREFIX.replicate$REP.reference.fa";
  find_bam        = "outfile.bam";
  replace_bam     = "$RUNFOLDER/$RUNPREFIX.replicate$REP.Ind$IND.bam";
  find_infasq     = "infile.fq";
  replace_infasq  = "$RUNFOLDER/$RUNPREFIX.replicate$REP.Ind$IND.fas..fq";
  find_infasq1     = "infile.1.fq";
  replace_infasq1  = "$RUNFOLDER/$RUNPREFIX.replicate$REP.Ind$IND.fas.1.fq";
  find_infasq2     = "infile.2.fq";
  replace_infasq2  = "$RUNFOLDER/$RUNPREFIX.replicate$REP.Ind$IND.fas.2.fq";

  keywords.push_back("reference\\.fa");
  keywords.push_back("outfile\\.bam");
  keywords.push_back("infile\\.1\\.fq");
  keywords.push_back("infile\\.2\\.fq");
  keywords.push_back("infile\\.fq");
  keywords.push_back("ind_number");
  keywords.push_back("tempfile");

  }

toob_snpcall_user::toob_snpcall_user() {

  toob_type = "snpcall.user";
  iSNPcall = true, mSNPcall = true;
  bam_sep = " ";
  sites_col = 1;
  find_temp       = "tempfile";
  replace_temp    = "$RUNFOLDER/$RUNPREFIX.replicate$REP";
  find_number     = "ind_number";
  replace_number  = "$IND";
  find_ref        = "reference.fa";
  replace_ref     = "$RUNFOLDER/$RUNPREFIX.replicate$REP.reference.fa";
  find_ibam       = "infile.bam";
  replace_ibam    = "$RUNFOLDER/$RUNPREFIX.replicate$REP.Ind$IND.bam";
  find_mbam       = "all.infiles.bam";
  replace_mbam    = "$bamlist";
  find_ivcf       = "outfile.ind.vcf";
  replace_ivcf    = "$RUNFOLDER/$RUNPREFIX.iSNPcall.replicate$REP.Ind$IND.flt.vcf";
  find_mvcf       = "outfile.allinds.vcf";
  replace_mvcf    = "$RUNFOLDER/$RUNPREFIX.mSNPcall.replicate$REP.allInds.vcf";
  find_sites      = "outfile.sites";
  replace_sites   = "$RUNFOLDER/$RUNPREFIX.replicate$REP.Ind$IND.sites";

  keywords_ind.push_back("reference\\.fa");
  keywords_ind.push_back("tempfile");
  keywords_ind.push_back("ind_number");
  keywords_ind.push_back("infile\\.bam");
  keywords_ind.push_back("outfile\\.ind\\.vcf");

  keywords_mult.push_back("reference\\.fa");
  keywords_mult.push_back("tempfile");
  keywords_mult.push_back("ind_number");
  keywords_mult.push_back("all\\.infiles\\.bam");
  keywords_mult.push_back("outfile\\.allinds\\.vcf");

  keywords_sites.push_back("reference\\.fa");
  keywords_sites.push_back("tempfile");
  keywords_sites.push_back("ind_number");
  keywords_sites.push_back("infile\\.bam");
  keywords_sites.push_back("outfile\\.sites");

}


void toob_in_user::write ( std::vector <std::string> & cmds, std::vector <int> & indent, paths bucket_paths, std::vector <std::string> & errors  ){
    std::stringstream ss;

    cmds.push_back(""); indent.push_back(0);
    cmds.push_back("# STARTING INPUT STEP (USER-DEFINED)"); indent.push_back(0);

    //
    if( ms_format == true ){
        cmds.push_back("# USING MS FILE"); indent.push_back(0);
        if( random_ancestral ){
            ss << "SEQLEN=" << seq_len;
            cmds.push_back( ss.str() );indent.push_back(0);
            ss.str("");
            cmds.push_back( bucket_paths.get_path_pipeliner() + " writeRandomSeq -outfile "
                           + "$RUNFOLDER/$RUNPREFIX.ancestral.fa -len $SEQLEN"); indent.push_back(0);
        }
        else{
            if( ancestral_file == "" ) {
                errors.push_back("user input: ancestral file not set");

            }
            cmds.push_back( bucket_paths.get_path_pipeliner() + " desinterleave -infile " + ancestral_file + " \\");indent.push_back(0);
            cmds.push_back( "  -outfile $RUNFOLDER/$RUNPREFIX.ancestral.fa"); indent.push_back(0);

        }
        int fas_counter = 0, ms_counter = 0, temp_counter = 0;
        for ( unsigned int i = 0; i < user_cmds.size(); i++) {
            std::string res = ReplaceString( user_cmds.at(i), find_fas, replace_fas, fas_counter );
            std::string res2 = ReplaceString( res, find_ms, replace_ms, ms_counter );
            std::string res3 = ReplaceString( res2, find_temp, replace_temp, temp_counter );
            cmds.push_back( res3 ); indent.push_back(0);
        }
        cmds.push_back( bucket_paths.get_path_pipeliner() + " ms2fas -in_ms $RUNFOLDER/$RUNPREFIX.replicate$REP.ms \\");indent.push_back(0);
        cmds.push_back("  -in_anc $RUNFOLDER/$RUNPREFIX.ancestral.fa \\"); indent.push_back(0);
        cmds.push_back("  -out_fas $RUNFOLDER/$RUNPREFIX.replicate$REP.fas -force chr1"); indent.push_back(0);


        if( fas_counter > 0 ){
            errors.push_back("input user: fasta input file found (expected ms)");
        }
        if( ms_counter == 0 ){
            errors.push_back("input user: ms input file not found");
        }
        if( ms_counter > 1 ){
            errors.push_back("input user: ms input file found more than once");
        }
    }
    else{
    // fasta
        cmds.push_back("# USING FASTA FILE"); indent.push_back(0);
        int fas_counter = 0, ms_counter = 0, temp_counter = 0;
        for ( unsigned int i = 0; i < user_cmds.size(); i++) {
            std::string res = ReplaceString( user_cmds.at(i), find_fas, replace_fas, fas_counter );
            res = ReplaceString( res, find_ms, replace_ms, ms_counter );
            res = ReplaceString( res, find_temp, replace_temp, temp_counter );
            cmds.push_back( res ); indent.push_back(0);
        }
        cmds.push_back( bucket_paths.get_path_pipeliner() + " desinterleave -infile " + replace_fas + " \\");indent.push_back(0);
        cmds.push_back( "  -force chr1 -outfile " + replace_fas + " \\");indent.push_back(0);

        if( ms_counter > 0 ){
        errors.push_back("input user: ms input file found (expected fasta)");
        }
        if( fas_counter == 0 ){
            errors.push_back("input user: fasta input file not found");
        }
        if( fas_counter > 1 ){
            errors.push_back("input user: fasta input file found more than once");
        }
    }


    cmds.push_back("# FINISHED INPUT STEP"); indent.push_back(0);
    cmds.push_back(""); indent.push_back(0);

}


void toob_ngs_user::write ( std::vector <std::string> & cmds, std::vector <int> & indent, paths bucket_paths, std::vector <std::string> & errors  ){
    std::stringstream ss;

    cmds.push_back(""); indent.push_back(0);
    cmds.push_back("# STARTING NGS SIMULATION STEP (USER-DEFINED)"); indent.push_back(0);
    cmds.push_back("for((IND=1; IND<=$NUMINDS; IND++))");indent.push_back(0);
    cmds.push_back("do");indent.push_back(1);
    cmds.push_back("SEQ_FROM=$(((($IND-1)*4)+3))");indent.push_back(1);
    cmds.push_back("SEQ_TO=$(((($IND-1)*4)+6))");indent.push_back(1);
    cmds.push_back("sed -n \"$SEQ_FROM,${SEQ_TO}p\" $RUNFOLDER/$RUNPREFIX.replicate$REP.fas \\");indent.push_back(1);
    cmds.push_back("  > $RUNFOLDER/$RUNPREFIX.replicate$REP.Ind$IND.fas");indent.push_back(1);

   int c_infas = 0, c_useless = 0, c_outfas = 0, c_outfas1 = 0, c_outfas2 = 0;
    for ( unsigned int i = 0; i < user_cmds.size(); i++) {
            std::string res = ReplaceString( user_cmds.at(i), find_infas, replace_infas, c_infas );
            res = ReplaceString( res, find_number, replace_number, c_useless );
            res = ReplaceString( res, find_temp, replace_temp, c_useless );
            res = ReplaceString( res, find_outfasq, replace_outfasq, c_outfas );
            res = ReplaceString( res, find_outfasq1, replace_outfasq1, c_outfas1 );
            res = ReplaceString( res, find_outfasq2, replace_outfasq2, c_outfas2 );
            cmds.push_back( res ); indent.push_back(1);
        }
    cmds.push_back("done");indent.push_back(1);
    cmds.push_back("# FINISHED NGS SIMULATION STEP"); indent.push_back(0);
    cmds.push_back(""); indent.push_back(0);

    // error checking
    if( c_outfas > 0 && c_outfas1 == 0 && c_outfas2 == 0  ){
        num_ends = 1;
        if( c_outfas != 1 ){
            errors.push_back("NGS user: outfile.fq seen more than once (warning)");
          }
      }
    else if ( c_outfas == 0 && c_outfas1 > 0 && c_outfas2 > 0 ) {
        num_ends = 2;
        if( c_outfas1 != 1 || c_outfas2 != 1 ){
            errors.push_back("NGS user: outfile.n.fq seen more than once (warning)");
          }
      }
    else if ( c_outfas == 0 ) {
        errors.push_back("NGS user: outfile.n.fq missing");
      }
    else {
        errors.push_back("NGS user: output undefined?");
      }
    if( c_infas == 0 ){
        errors.push_back("NGS user: input file not used");
      }

}

void toob_align_user::write ( std::vector <std::string> & cmds, std::vector <int> & indent, paths bucket_paths, std::vector <std::string> & errors  ){
    std::stringstream ss;

    cmds.push_back(""); indent.push_back(0);
    cmds.push_back("# STARTING ALIGNMENT STEP (USER-DEFINED)"); indent.push_back(0);
    cmds.push_back("sed -n \"1,2p\" $RUNFOLDER/$RUNPREFIX.replicate$REP.fas \\");indent.push_back(0);
    cmds.push_back("  > $RUNFOLDER/$RUNPREFIX.replicate$REP.reference.fa");indent.push_back(0);
    if( createDict ){
        ss << bucket_paths.get_path_java() << " " << bucket_paths.get_path_picard() << "/CreateSequenceDictionary.jar \\";
        cmds.push_back(ss.str()); indent.push_back(0); ss.str("");
        cmds.push_back("  R=$RUNFOLDER/$RUNPREFIX.replicate$REP.reference.fa \\"); indent.push_back(0);
        cmds.push_back("  O=$RUNFOLDER/$RUNPREFIX.replicate$REP.reference.dict"); indent.push_back(0);
      }
    cmds.push_back(""); indent.push_back(0);
    cmds.push_back("for((IND=1; IND<=$NUMINDS; IND++))");indent.push_back(0);
    cmds.push_back("do");indent.push_back(1);

    int c_infasq = 0, c_useless = 0, c_infasq1 = 0, c_infasq2 = 0, c_ref = 0, c_outbam = 0;
    for ( unsigned int i = 0; i < user_cmds.size(); i++) {
            std::string res = ReplaceString( user_cmds.at(i), find_temp, replace_temp, c_useless );
            res = ReplaceString( res, find_number, replace_number, c_useless );
            res = ReplaceString( res, find_ref, replace_ref, c_ref );
            res = ReplaceString( res, find_bam, replace_bam, c_outbam );
            res = ReplaceString( res, find_infasq, replace_infasq, c_infasq );
            res = ReplaceString( res, find_infasq1, replace_infasq1, c_infasq1 );
            res = ReplaceString( res, find_infasq2, replace_infasq2, c_infasq2 );
            cmds.push_back( res ); indent.push_back(1);
        }
    if( addReadGroups ){
        ss << bucket_paths.get_path_java() << " " << bucket_paths.get_path_picard() << "/AddOrReplaceReadGroups.jar \\";
        cmds.push_back(ss.str()); indent.push_back(1); ss.str("");
        cmds.push_back("  I=$RUNFOLDER/$RUNPREFIX.replicate$REP.Ind$IND.bam \\"); indent.push_back(1);
        cmds.push_back("  O=$RUNFOLDER/$RUNPREFIX.replicate$REP.Ind$IND.RGOK.bam \\"); indent.push_back(1);
        cmds.push_back("  ID=1 LB=1 PL=illumina PU=1 SM=\"Ind$IND\""); indent.push_back(1);
        cmds.push_back("mv $RUNFOLDER/$RUNPREFIX.replicate$REP.Ind$IND.RGOK.bam \\"); indent.push_back(1);
        cmds.push_back("  $RUNFOLDER/$RUNPREFIX.replicate$REP.Ind$IND.bam"); indent.push_back(1);
      }


    cmds.push_back("done");indent.push_back(1);
    cmds.push_back("# FINISHED ALIGNMENT STEP"); indent.push_back(0);
    cmds.push_back(""); indent.push_back(0);

    // error checking
    if( c_infasq > 0 && c_infasq1 == 0 && c_infasq2 == 0  ){
        num_ends = 1;
        if( c_infasq != 1 ){
            errors.push_back("Alignment user: infile.fq seen more than once (warning)");
          }
      }
    else if ( c_infasq == 0 && c_infasq1 > 0 && c_infasq2 > 0 ) {
        num_ends = 2;
        if( c_infasq1 != 1 || c_infasq2 != 1 ){
            errors.push_back("Alignment user: infile.n.fq seen more than once (warning)");
          }
      }
    else if ( c_infasq == 0 ) {
        errors.push_back("Alignment user: infile.n.fq missing");
      }
    else {
        errors.push_back("Alignment user: output undefined?");
      }

    if( c_outbam == 0 ){
        errors.push_back("Alignment user: output bam file not used");
      }
    if( c_ref == 0 ){
        errors.push_back("Alignment user: reference file not used");
      }

}



void toob_snpcall_user::write ( std::vector <std::string> & cmds, std::vector <int> & indent, paths bucket_paths, std::vector <std::string> & errors  ){
    std::stringstream ss;

    cmds.push_back(""); indent.push_back(0);
    cmds.push_back("# STARTING SNP CALLING AND FILTERING STEP (USER-DEFINED)"); indent.push_back(0);
    cmds.push_back("unset ALLBAMFILES"); indent.push_back(0);
    cmds.push_back("unset ALL_IND_VCF_FILES"); indent.push_back(0);
    cmds.push_back("unset ALL_MS_VCF_FILES"); indent.push_back(0);
    cmds.push_back("unset ALL_SITES_FILES"); indent.push_back(0);
    cmds.push_back("");indent.push_back(0);
    cmds.push_back("# iSNPcall"); indent.push_back(0);
    cmds.push_back("for((IND=1; IND<=$NUMINDS; IND++))");indent.push_back(0);
    cmds.push_back("do");indent.push_back(1);

   // cmds.push_back("ALLBAMFILES[${IND}]=$RUNFOLDER/$RUNPREFIX.replicate$REP.Ind$IND.bam");indent.push_back(1);
    cmds.push_back("ALLBAMFILES+=(\"$RUNFOLDER/$RUNPREFIX.replicate$REP.Ind$IND.bam\")"); indent.push_back(1);

    int c_useless = 0, c_ref = 0, c_ivcf = 0, c_ibam = 0;
    for ( unsigned int i = 0; i < user_cmds_iSNP.size(); i++) {
            std::string res = ReplaceString( user_cmds_iSNP.at(i), find_temp, replace_temp, c_useless );
            res = ReplaceString( res, find_number, replace_number, c_useless );
            res = ReplaceString( res, find_ref, replace_ref, c_ref );
            res = ReplaceString( res, find_ibam, replace_ibam, c_ibam );
            res = ReplaceString( res, find_ivcf, replace_ivcf, c_ivcf );
            cmds.push_back( res ); indent.push_back(1);
        }

    if( iSNPcall ){
        cmds.push_back("ALL_IND_VCF_FILES+=(\"$RUNFOLDER/$RUNPREFIX.iSNPcall.replicate$REP.Ind$IND.flt.vcf\")"); indent.push_back(1);
        if( c_ref == 0 ){
            errors.push_back("user defined iSNPcall: reference not used");
          }
        if( c_ivcf == 0 ){
            errors.push_back("user defined iSNPcall: output vcf file undefined");
          }
        if( c_ibam == 0 ){
            errors.push_back("user defined iSNPcall: input bam file not used");
          }
      }

    // SITES
    cmds.push_back("");indent.push_back(0);
    cmds.push_back("# sites file"); indent.push_back(0);
    int c_sites = 0;
    for ( unsigned int i = 0; i < user_cmds_sites.size(); i++) {
            std::string res = ReplaceString( user_cmds_sites.at(i), find_temp, replace_temp, c_useless );
            res = ReplaceString( res, find_number, replace_number, c_useless );
            res = ReplaceString( res, find_ref, replace_ref, c_ref );
            res = ReplaceString( res, find_ibam, replace_ibam, c_ibam );
            res = ReplaceString( res, find_sites, replace_sites, c_sites );
            cmds.push_back( res ); indent.push_back(1);
        }
    if( c_sites == 0){
        errors.push_back("user defined sites: output sites file undefined");
      }
    ss << "cut -f " << sites_col << " $RUNFOLDER/$RUNPREFIX.replicate$REP.Ind$IND.sites > $RUNFOLDER/$RUNPREFIX.replicate$REP.Ind$IND.sites.unsorted";
    cmds.push_back(ss.str());indent.push_back(1);ss.str("");
    cmds.push_back("sort -n -u $RUNFOLDER/$RUNPREFIX.replicate$REP.Ind$IND.sites.unsorted | grep -v \\# > $RUNFOLDER/$RUNPREFIX.replicate$REP.Ind$IND.sites"); indent.push_back(1);
    cmds.push_back("ALL_SITES_FILES+=(\"$RUNFOLDER/$RUNPREFIX.replicate$REP.Ind$IND.sites\")"); indent.push_back(1);

    cmds.push_back("done");indent.push_back(1);
    cmds.push_back("");indent.push_back(0);
    // mSNPcall
    if( mSNPcall ){
        cmds.push_back("# mSNPcall"); indent.push_back(0);
        cmds.push_back("bamlist=${ALLBAMFILES[0]}");indent.push_back(0);
        cmds.push_back("for((IND=1; IND<$NUMINDS; IND++))");indent.push_back(0);
        cmds.push_back("do");indent.push_back(1);
        ss << "bamlist+=\" " << bam_sep << " \"${ALLBAMFILES[${IND}]}";
        cmds.push_back(ss.str());indent.push_back(1);ss.str("");

        cmds.push_back("done");indent.push_back(1);

        int c_mvcf = 0, c_mbam = 0;
        c_useless = 0, c_ref = 0;
        for ( unsigned int i = 0; i < user_cmds_mSNP.size(); i++) {
                std::string res = ReplaceString( user_cmds_mSNP.at(i), find_temp, replace_temp, c_useless );
                res = ReplaceString( res, find_ref, replace_ref, c_ref );
                res = ReplaceString( res, find_mbam, replace_mbam, c_mbam );
                res = ReplaceString( res, find_mvcf, replace_mvcf, c_mvcf );
                cmds.push_back( res ); indent.push_back(0);
            }

        if( c_ref == 0 ){
            errors.push_back("user defined mSNPcall: reference not used");
          }
        if( c_mvcf == 0 ){
            errors.push_back("user defined mSNPcall: output vcf file undefined");
          }
        if( c_mbam == 0 ){
            errors.push_back("user defined mSNPcall: input bam file not used");
          }
      }
    cmds.push_back(ss.str()); indent.push_back(0); ss.str("");
    cmds.push_back("");indent.push_back(0);
    cmds.push_back("for((IND=1; IND<=$NUMINDS; IND++))");indent.push_back(0);
    cmds.push_back("do");indent.push_back(1);
    ss << bucket_paths.get_path_vcfutils() << " subsam $RUNFOLDER/$RUNPREFIX.mSNPcall.replicate$REP.allInds.vcf Ind$IND \\";
    cmds.push_back(ss.str()); indent.push_back(1); ss.str("");
    cmds.push_back("  > $RUNFOLDER/$RUNPREFIX.mSNPcall.replicate$REP.Ind$IND.unflt.vcf");indent.push_back(1);
    cmds.push_back("ALL_MS_VCF_FILES+=(\"$RUNFOLDER/$RUNPREFIX.mSNPcall.replicate$REP.Ind$IND.unflt.vcf\")"); indent.push_back(1);
    cmds.push_back("done");indent.push_back(1);

    cmds.push_back("# FINISHED SNP CALLING AND FILTERING STEP"); indent.push_back(0);
    cmds.push_back(""); indent.push_back(0);


}
