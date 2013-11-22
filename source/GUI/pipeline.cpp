//
//  pipeline.cpp
//  pGUI
//
//  Copyright (c) 2013 Bruno Nevado. All rights reserved.
//

#include "pipeline.h"
#include <sstream>
#include <fstream>

pipeline::pipeline( std::string name ){
  pipeline_name = name;
  bash_header = "";
  active_input = 0;
  active_ngs_sim = 0;
  active_alignment = 0;
  active_snpcall = 0;
  sum_iSNPcall = true;
  sum_mSNPcall = true;
  inspect_iSNPcall = false;
  inspect_mSNPcall = false;
  mstats_iSNPcall = false;
  mstats_mSNPcall = false;
  mstats_pre = false;
  qual_offset = 33; // used for inspect errors
}

pipeline_output pipeline::get_cmds (){

  std::vector <std::string> cmds, toreturn, errors;
  std::vector <int> tabs;

  //
  std::stringstream ss;
  ss << "#!" << daPaths.get_path_bash() << std::endl;
  toreturn.push_back(ss.str());
  ss.str("");
  if( bash_header != "" ){
      std::ifstream ifile;
      ifile.open(bash_header.c_str());
      if( !ifile.is_open() ){
          ss << "bash header: unable to open for reading file " << bash_header;
          errors.push_back(ss.str());
          ss.str("");
        }
      else{
          std::string line;
          while(getline(ifile, line)){
              toreturn.push_back(line);
            }

        }

    }

  daRunSettings.write( cmds, tabs, errors );

  int indent_level = 0;
  for (unsigned int icmd = 0; icmd < cmds.size(); icmd++ ) {
      std::string newcmd;
      for( int tab = 0; tab < ( indent_level + tabs.at(icmd) ) ; tab++)
        newcmd += "\t";
      newcmd += cmds.at(icmd);
      toreturn.push_back(newcmd);
    }

  toreturn.push_back("REMOVE=0");
  toreturn.push_back("if [[ !( -d $RUNFOLDER ) ]]");
  toreturn.push_back("\tthen mkdir $RUNFOLDER; REMOVE=1; fi");
  toreturn.push_back("cd $RUNFOLDER");

  toreturn.push_back("");
  toreturn.push_back("# # REMOVING OLD SUMMARY FILES # #");


  if( sum_iSNPcall ){
     toreturn.push_back("if [[ -f $RUNFOLDER/summary.$RUNPREFIX.iSNPcall.txt ]]");
     toreturn.push_back("\tthen rm $RUNFOLDER/summary.$RUNPREFIX.iSNPcall.txt;");
     toreturn.push_back("\techo \"WARNING: found and deleted old summary file (iSNPcall)\"; fi");
    }
  if( sum_mSNPcall ){
     toreturn.push_back("if [[ -f $RUNFOLDER/summary.$RUNPREFIX.mSNPcall.txt ]]");
     toreturn.push_back("\tthen rm $RUNFOLDER/summary.$RUNPREFIX.mSNPcall.txt;");
     toreturn.push_back("\techo \"WARNING: found and deleted old summary file (mSNPcall)\"; fi");
    }
  if( inspect_iSNPcall ){
     toreturn.push_back("if [[ -f $RUNFOLDER/summary.$RUNPREFIX.iSNPcall_errors.txt ]]");
     toreturn.push_back("\tthen rm $RUNFOLDER/summary.$RUNPREFIX.iSNPcall_errors.txt;");
     toreturn.push_back("\techo \"WARNING: found and deleted old summary file (iSNPcall_errors)\"; fi;");
     toreturn.push_back("printf \"REP\\tIND\\tSITE\\tERR_TYPE\\tCONFIG\\t:\\tMPILEUP LINE\\t(BASE QUALS)\\n\" \\");
     toreturn.push_back("  > $RUNFOLDER/summary.$RUNPREFIX.iSNPcall_errors.txt");
    }
  if( inspect_mSNPcall ){
     toreturn.push_back("if [[ -f $RUNFOLDER/summary.$RUNPREFIX.mSNPcall_errors.txt ]]");
     toreturn.push_back("\tthen rm $RUNFOLDER/summary.$RUNPREFIX.mSNPcall_errors.txt;");
     toreturn.push_back("\techo \"WARNING: found and deleted old summary file mSNPcall_errors)\"; fi;");
     toreturn.push_back("printf \"REP\\tIND\\tSITE\\tERR_TYPE\\tCONFIG\\t:\\tMPILEUP LINE\\t(BASE QUALS)\\n\" \\");
     toreturn.push_back("  > $RUNFOLDER/summary.$RUNPREFIX.mSNPcall_errors.txt");
    }
  if( mstats_pre ){
     toreturn.push_back("if [[ -f $RUNFOLDER/summary.$RUNPREFIX.preseq_mstats.txt ]]");
     toreturn.push_back("\tthen rm $RUNFOLDER/summary.$RUNPREFIX.preseq_mstats.txt;");
     toreturn.push_back("\techo \"WARNING: found and deleted old summary file (preseq_mstats)\"; fi;");
     toreturn.push_back("printf \"REP\\tEFF_LENGTH2\\tSS\\tTHETA\\tPI\\tTAJ_D\\tFULI_D\\tFAYWU_H\\n\" \\");
     toreturn.push_back("  > $RUNFOLDER/summary.$RUNPREFIX.preseq_mstats.txt");
    }
  if( mstats_iSNPcall ){
     toreturn.push_back("if [[ -f $RUNFOLDER/summary.$RUNPREFIX.iSNPcall_mstats.txt ]]");
     toreturn.push_back("\tthen rm $RUNFOLDER/summary.$RUNPREFIX.iSNPcal_mstats.txt;");
     toreturn.push_back("\techo \"WARNING: found and deleted old summary file (iSNPcall_mstats)\"; fi;");
     toreturn.push_back("printf \"REP\\tEFF_LENGTH2\\tSS\\tTHETA\\tPI\\tTAJ_D\\tFULI_D\\tFAYWU_H\\n\" \\");
     toreturn.push_back("  > $RUNFOLDER/summary.$RUNPREFIX.iSNPcall_mstats.txt");
    }
  if( mstats_mSNPcall ){
     toreturn.push_back("if [[ -f $RUNFOLDER/summary.$RUNPREFIX.mSNPcall_mstats.txt ]]");
     toreturn.push_back("\tthen rm $RUNFOLDER/summary.$RUNPREFIX.mSNPcall_mstats.txt;");
     toreturn.push_back("\techo \"WARNING: found and deleted old summary file (mSNPcall_mstats)\"; fi;");
     toreturn.push_back("printf \"REP\\tEFF_LENGTH2\\tSS\\tTHETA\\tPI\\tTAJ_D\\tFULI_D\\tFAYWU_H\\n\" \\");
     toreturn.push_back("  > $RUNFOLDER/summary.$RUNPREFIX.mSNPcall_mstats.txt");
    }

  toreturn.push_back("");
  toreturn.push_back("# # STARTING PIPELINE # #");
  toreturn.push_back("for((REP=1; REP<=$NUMREPS; REP++))");
  toreturn.push_back("\tdo");
  indent_level++;

  //  INPUT TYPE
  cmds.clear(), tabs.clear();
  switch (active_input) {
    case 0:
      daInputMs.write(cmds, tabs, daPaths, errors );
      break;
    case 1:
      daInputSfscode.write(cmds, tabs, daPaths, errors);
      break;
    case 2:
      daInputUser.write(cmds, tabs, daPaths, errors);
      break;
    default:
      std::cout << "Only ms/sfs_code input type available !" << std::endl;
      break;
    }
  for (unsigned int icmd = 0; icmd < cmds.size(); icmd++ ) {
      std::string newcmd;
      for( int tab = 0; tab < ( indent_level + tabs.at(icmd) ) ; tab++)
        newcmd += "\t";
      newcmd += cmds.at(icmd);
      toreturn.push_back(newcmd);
    }

  // NGS SIMULATION
  int nends_sim;
  cmds.clear(), tabs.clear();
  switch (active_ngs_sim) {
    case 0:
      daNgsArt.write(cmds, tabs, daPaths, errors );
      nends_sim = daNgsArt.get_nends();
      break;
    case 1:
      daNgsUser.write(cmds, tabs, daPaths, errors );
      nends_sim = daNgsUser.get_nends();
      break;
    default:
      std::cout << "Only art ngs sim type available !" << std::endl;
      break;
    }
  for (unsigned int icmd = 0; icmd < cmds.size(); icmd++ ) {
      std::string newcmd;
      for( int tab = 0; tab < ( indent_level + tabs.at(icmd) ) ; tab++)
        newcmd += "\t";
      newcmd += cmds.at(icmd);
      toreturn.push_back(newcmd);
    }

  // ALIGNMENT STEP
  cmds.clear(), tabs.clear();
  switch (active_alignment) {
    case 0:
      daAlignmentBwa.write(cmds, tabs, daPaths, errors );
      if( nends_sim != daAlignmentBwa.get_num_ends() ){
          errors.push_back("Paired/Single ends mismatch (NGS simulation vs alignment)");
        }
      break;
    case 1:
      daAlignUser.write(cmds, tabs, daPaths, errors );
      if( nends_sim != daAlignUser.get_nends() ){
          errors.push_back("Paired/Single ends mismatch (NGS simulation vs alignment)");
        }
      break;
    default:
      std::cout << "Only bwa alignment type available !" << std::endl;
      break;
    }
  for (unsigned int icmd = 0; icmd < cmds.size(); icmd++ ) {
      std::string newcmd;
      for( int tab = 0; tab < ( indent_level + tabs.at(icmd) ) ; tab++)
        newcmd += "\t";
      newcmd += cmds.at(icmd);
      toreturn.push_back(newcmd);
    }

  // SNP CALL
  cmds.clear(), tabs.clear();
  switch (active_snpcall) {
    case 0:
      daSnpcallSamtools.write( cmds, tabs, daPaths, errors );
      break;
    case 1:
      daSnpcallUser.write( cmds, tabs, daPaths, errors );
      break;
    default:
      std::cout << "Only samtools and User-defined SNP call type available !" << std::endl;
      break;
    }
  for (unsigned int icmd = 0; icmd < cmds.size(); icmd++ ) {
      std::string newcmd;
      for( int tab = 0; tab < ( indent_level + tabs.at(icmd) ) ; tab++)
        newcmd += "\t";
      newcmd += cmds.at(icmd);
      toreturn.push_back(newcmd);
    }

  // SUMMARISE
  toreturn.push_back("");
  toreturn.push_back("\t# CALCULATING STATS FOR PIPELINE");
  toreturn.push_back("\tSAVE_IFS=$IFS");
  toreturn.push_back("\tIFS=\",\"");
  toreturn.push_back("\tsites_list=\"${ALL_SITES_FILES[*]}\"");
  toreturn.push_back("\tvcf_list=\"${ALL_IND_VCF_FILES[*]}\"");
  toreturn.push_back("\tms_vcf_list=\"${ALL_MS_VCF_FILES[*]}\"");
  toreturn.push_back("\tIFS=$SAVE_IFS");

  if( sum_iSNPcall ){
      std::string part;
      switch ( active_snpcall ) {
        case 0:
          if( !daSnpcallSamtools.summarise_iSNPcall() ){
              errors.push_back( "stats: iSNPcall off in snp call, but on in stats" );
            }
          part = "\t  -tag iSNPcall -out $RUNFOLDER/summary.$RUNPREFIX.iSNPcall.txt";
          break;
        case 1:
          if( !daSnpcallUser.get_iSNPcall() ){
              errors.push_back( "stats: iSNPcall off in snp call, but on in stats" );
            }
          part = "\t  -tag iSNPcall_user -out $RUNFOLDER/summary.$RUNPREFIX.iSNPcall.txt";
          break;
        default:
          break;
        }
      std::stringstream ss;
      toreturn.push_back("");
      toreturn.push_back("\t# SUMMARISING GENOTYPE CALLS (iSNPcall)");
      ss << "\t" << daPaths.get_path_pipeliner() << " summarisePipeline -in_fasta $RUNFOLDER/$RUNPREFIX.replicate$REP.fas \\";
      toreturn.push_back(ss.str()); ss.str("");
      toreturn.push_back("\t  -in_vcf $vcf_list \\");
      toreturn.push_back("\t  -sites $sites_list -append 1 \\");
      if( inspect_iSNPcall ){
          toreturn.push_back( "\t  -error $RUNFOLDER/$RUNPREFIX.replicate$REP.iSNPcall.errors \\" );
        }
      toreturn.push_back( part );
      toreturn.push_back("");

      if( inspect_iSNPcall ){
          toreturn.push_back("\t# INSPECTING ERRORS (iSNPcall)");
          ss << "\t" << daPaths.get_path_perl() << " -e '$cmdo=\"" << daPaths.get_path_samtools()
             << " mpileup -f '$RUNFOLDER/$RUNPREFIX'.replicate'$REP'.reference.fa \".";
          toreturn.push_back(ss.str()); ss.str("");
         toreturn.push_back( "\t  \"'$RUNFOLDER/$RUNPREFIX'.replicate'$REP'.INDIVIDUAL.bam -r chr1:SITE-SITE 2> /dev/null\";");
         toreturn.push_back( "\t  while(<>){" );
         toreturn.push_back( "\t  chomp; $cmd=$cmdo; @t=split(/\\t/,$_);" );
         toreturn.push_back( "\t  $cmd =~ s/INDIVIDUAL/Ind$t[0]/;" );
         toreturn.push_back( "\t  $cmd =~ s/SITE/$t[1]/g;" );
         toreturn.push_back( "\t  $r=`$cmd`; chomp $r;" );
         toreturn.push_back( "\t  print '$REP', \"\\t\", $_, \"\\t:\\t\", $r, \"\\t(\";" );
         toreturn.push_back( "\t  @t=split/\\t/,$r;@qs=split//,$t[$#t];" );
         ss << "\t  @q=map {ord($_)-" <<  qual_offset << "} @qs;print \"@q)\\n\";";
         toreturn.push_back(ss.str()); ss.str("");
         toreturn.push_back( "\t  }' < $RUNFOLDER/$RUNPREFIX.replicate$REP.iSNPcall.errors \\" );
         toreturn.push_back( "\t  >> $RUNFOLDER/summary.$RUNPREFIX.iSNPcall_errors.txt" );

        }

    }

  if( sum_mSNPcall ){
      std::string part;
      switch ( active_snpcall ) {
        case 0:
          if( !daSnpcallSamtools.summarise_mSNPcall() ){
              errors.push_back( "stats: mSNPcall off in snp call, but on in stats" );
            }
          part = "\t  -tag mSNPcall -out $RUNFOLDER/summary.$RUNPREFIX.mSNPcall.txt";
          break;
        case 1:
          if( !daSnpcallUser.get_mSNPcall() ){
              errors.push_back( "stats: mSNPcall off in snp call, but on in stats" );
            }
          part = "\t  -tag mSNPcall_user -out $RUNFOLDER/summary.$RUNPREFIX.mSNPcall.txt";
          break;
        default:
          break;
        }
      std::stringstream ss;
      toreturn.push_back("");
      toreturn.push_back("\t# SUMMARISING GENOTYPE CALLS (mSNPcall)");
      ss << "\t" << daPaths.get_path_pipeliner() << " summarisePipeline -in_fasta $RUNFOLDER/$RUNPREFIX.replicate$REP.fas \\";
      toreturn.push_back(ss.str()); ss.str("");
      toreturn.push_back("\t  -in_vcf $ms_vcf_list \\");
      toreturn.push_back("\t  -sites $sites_list -append 1 \\");
      if( inspect_mSNPcall ){
          toreturn.push_back( "\t  -error $RUNFOLDER/$RUNPREFIX.replicate$REP.mSNPcall.errors \\" );
        }
      toreturn.push_back( part );
      toreturn.push_back("");

      if( inspect_mSNPcall ){
          toreturn.push_back("\t# INSPECTING ERRORS (mSNPcall)");
          ss << "\t" << daPaths.get_path_perl() << " -e '$cmdo=\"" << daPaths.get_path_samtools()
             << " mpileup -f '$RUNFOLDER/$RUNPREFIX'.replicate'$REP'.reference.fa \".";
          toreturn.push_back(ss.str()); ss.str("");
         toreturn.push_back( "\t  \"'$RUNFOLDER/$RUNPREFIX'.replicate'$REP'.INDIVIDUAL.bam -r chr1:SITE-SITE 2> /dev/null\";");
         toreturn.push_back( "\t  while(<>){" );
         toreturn.push_back( "\t  chomp; $cmd=$cmdo; @t=split(/\\t/,$_);" );
         toreturn.push_back( "\t  $cmd =~ s/INDIVIDUAL/Ind$t[0]/;" );
         toreturn.push_back( "\t  $cmd =~ s/SITE/$t[1]/g;" );
         toreturn.push_back( "\t  $r=`$cmd`; chomp $r;" );
         toreturn.push_back( "\t  print '$REP', \"\\t\", $_, \"\\t:\\t\", $r, \"\\t(\";" );
         toreturn.push_back( "\t  @t=split/\\t/,$r;@qs=split//,$t[$#t];" );
         ss << "\t  @q=map {ord($_)-" <<  qual_offset << "} @qs;print \"@q)\\n\";";
         toreturn.push_back(ss.str()); ss.str("");
         toreturn.push_back( "\t  }' < $RUNFOLDER/$RUNPREFIX.replicate$REP.mSNPcall.errors \\" );
         toreturn.push_back( "\t  >> $RUNFOLDER/summary.$RUNPREFIX.mSNPcall_errors.txt" );

        }

    }



    if( mstats_pre ){
        toreturn.push_back("");
        toreturn.push_back("\t# CALCULATING POP GENOMICS STATISTICS (pre-sequencing)");
        std::stringstream ss;
        toreturn.push_back( "\tsed \"1,2d\" $RUNFOLDER/$RUNPREFIX.replicate$REP.fas \\" );
        toreturn.push_back( "\t   > $RUNFOLDER/$RUNPREFIX.replicate$REP.fas.pre_mstats.fas" );
        toreturn.push_back( "\tsed -n \"1,2p\" $RUNFOLDER/$RUNPREFIX.replicate$REP.fas \\" );
        toreturn.push_back( "\t  >> $RUNFOLDER/$RUNPREFIX.replicate$REP.fas.pre_mstats.fas" );
        ss << "\t" << daPaths.get_path_mstats() << " -i $RUNFOLDER/$RUNPREFIX.replicate$REP.fas.pre_mstats.fas -f fasta \\" ;
        toreturn.push_back(ss.str()); ss.str("");
        toreturn.push_back( "\t  -o 1 -N 2 $(($NUMINDS*2)) 1 -G 1 -p 1 -u 1 | \\");
        ss << "\t  " << daPaths.get_path_perl() << " -ne 'print '$REP',\"\\t\",$1,\"\\t\",$2,\"\\t\",$3,\"\\t\",$4,\"\\t\",$5,\"\\t\",$6,\"\\t\",$7,\"\\n\"if";
        toreturn.push_back(ss.str()); ss.str("");
        toreturn.push_back("\t  /");
        toreturn.push_back("\t   Eff_length2_pop\\[0]\\:\\t(.+?)\\t.*");
        toreturn.push_back("\t   S\\[0]\\:\\t(.+?)\\t.*");
        toreturn.push_back("\t   Theta\\(Wat\\)\\[0]\\:\\t(.+?)\\t.*");
        toreturn.push_back("\t   Theta\\(Taj\\)\\[0]\\:\\t(.+?)\\t.*");
        toreturn.push_back("\t   TajimaD\\[0]\\:\\t(.+?)\\t.*");
        toreturn.push_back("\t   Fu&LiD\\[0]\\:\\t(.+?)\\t.*");
        toreturn.push_back("\t   Fay&WunormH\\[0]\\:\\t(.+?)\\t.*");
        toreturn.push_back("\t  /x' >> $RUNFOLDER/summary.$RUNPREFIX.preseq_mstats.txt");




    }


  if( mstats_iSNPcall ){
      switch ( active_snpcall ) {
        case 0:
          if( !daSnpcallSamtools.summarise_iSNPcall() ){
              errors.push_back( "stats: iSNPcall off in snp call, but on in Pop Gen stats" );
            }
          break;
        case 1:
          if( !daSnpcallUser.get_iSNPcall() ){
              errors.push_back( "stats: iSNPcall off in snp call, but on in Pop Gen stats" );
            }
          break;
        default:
          break;
        }

      toreturn.push_back("");
      toreturn.push_back("\t# CALCULATING POP GENOMICS STATISTICS (iSNPcall)");
      toreturn.push_back("\tfor((IND=1; IND<=$NUMINDS; IND++))");
      toreturn.push_back("\t\tdo");
      ss << "\t\t" << daPaths.get_path_pipeliner() << " vcf2fas -vcf $RUNFOLDER/$RUNPREFIX.iSNPcall.replicate$REP.Ind$IND.flt.vcf \\";
      toreturn.push_back(ss.str()); ss.str("");
      toreturn.push_back("\t\t  -sites $RUNFOLDER/$RUNPREFIX.replicate$REP.Ind$IND.sites \\");
      toreturn.push_back("\t\t  -ref $RUNFOLDER/$RUNPREFIX.replicate$REP.reference.fa \\");
      toreturn.push_back("\t\t  -out $RUNFOLDER/$RUNPREFIX.replicate$REP.fas.mstats_iSNPcall.fas \\");
      toreturn.push_back("\t\t  -append 1 -names indi$IND -col 1");
      toreturn.push_back("\t\tdone");
      toreturn.push_back("");
      toreturn.push_back("\tcat $RUNFOLDER/$RUNPREFIX.replicate$REP.reference.fa \\");
      toreturn.push_back("\t  >> $RUNFOLDER/$RUNPREFIX.replicate$REP.fas.mstats_iSNPcall.fas");
      ss << "\t" << daPaths.get_path_mstats() << " -i $RUNFOLDER/$RUNPREFIX.replicate$REP.fas.mstats_iSNPcall.fas -f fasta \\" ;
      toreturn.push_back(ss.str()); ss.str("");
      toreturn.push_back( "\t  -o 1 -N 2 $NUMINDS 1 -G 1 -p 2 -u 1 | \\");
      //ss << "\t  " << daPaths.get_path_perl() << " -ne 'print $1,\"\\t\",$2,\"\\t\",$3,\"\\t\",$4,\"\\t\",$5,\"\\t\",$6,\"\\t\",$7,\"\\n\"if/Eff_length2_pop\\[0]\\:\\t(.+?)\\t.*S\\[0]\\:\\t(.+?)\\t.*Theta\\(Wat\\)\\[0]\\:\\t(.+?)\\t.*Theta\\(Taj\\)\\[0]\\:\\t(.+?)\\t.*TajimaD\\[0]\\:\\t(.+?)\\t.*Fu&LiD\\[0]\\:\\t(.+?)\\t.*Fay&WunormH\\[0]\\:\\t(.+?)\\t.*/' \\";

       ss << "\t  " << daPaths.get_path_perl() << " -ne 'print '$REP',\"\\t\",$1,\"\\t\",$2,\"\\t\",$3,\"\\t\",$4,\"\\t\",$5,\"\\t\",$6,\"\\t\",$7,\"\\n\"if";
      toreturn.push_back(ss.str()); ss.str("");
      toreturn.push_back("\t  /");
      toreturn.push_back("\t   Eff_length2_pop\\[0]\\:\\t(.+?)\\t.*");
      toreturn.push_back("\t   S\\[0]\\:\\t(.+?)\\t.*");
      toreturn.push_back("\t   Theta\\(Wat\\)\\[0]\\:\\t(.+?)\\t.*");
      toreturn.push_back("\t   Theta\\(Taj\\)\\[0]\\:\\t(.+?)\\t.*");
      toreturn.push_back("\t   TajimaD\\[0]\\:\\t(.+?)\\t.*");
      toreturn.push_back("\t   Fu&LiD\\[0]\\:\\t(.+?)\\t.*");
      toreturn.push_back("\t   Fay&WunormH\\[0]\\:\\t(.+?)\\t.*");
      toreturn.push_back("\t  /x' >> $RUNFOLDER/summary.$RUNPREFIX.iSNPcall_mstats.txt");


      toreturn.push_back("");
    }

  if( mstats_mSNPcall ){
      switch ( active_snpcall ) {
        case 0:
          if( !daSnpcallSamtools.summarise_mSNPcall() ){
              errors.push_back( "stats: mSNPcall off in snp call, but on in Pop Gen stats" );
            }
          break;
        case 1:
          if( !daSnpcallUser.get_mSNPcall() ){
              errors.push_back( "stats: mSNPcall off in snp call, but on in Pop Gen stats" );
            }
          break;
        default:
          break;
        }

      toreturn.push_back("");
      toreturn.push_back("\t# CALCULATING POP GENOMICS STATISTICS (mSNPcall)");
      toreturn.push_back("\tfor((IND=1; IND<=$NUMINDS; IND++))");
      toreturn.push_back("\t\tdo");
      ss << "\t\t" << daPaths.get_path_pipeliner() << " vcf2fas -vcf $RUNFOLDER/$RUNPREFIX.mSNPcall.replicate$REP.Ind$IND.unflt.vcf \\";
      toreturn.push_back(ss.str()); ss.str("");
      toreturn.push_back("\t\t  -sites $RUNFOLDER/$RUNPREFIX.replicate$REP.Ind$IND.sites \\");
      toreturn.push_back("\t\t  -ref $RUNFOLDER/$RUNPREFIX.replicate$REP.reference.fa \\");
      toreturn.push_back("\t\t  -out $RUNFOLDER/$RUNPREFIX.replicate$REP.fas.mstats_mSNPcall.fas \\");
      toreturn.push_back("\t\t  -append 1 -names indi$IND -col 1");
      toreturn.push_back("\t\tdone");
      toreturn.push_back("");
      toreturn.push_back("\tcat $RUNFOLDER/$RUNPREFIX.replicate$REP.reference.fa \\");
      toreturn.push_back("\t  >> $RUNFOLDER/$RUNPREFIX.replicate$REP.fas.mstats_mSNPcall.fas");

      ss << "\t" << daPaths.get_path_mstats() << " -i $RUNFOLDER/$RUNPREFIX.replicate$REP.fas.mstats_mSNPcall.fas -f fasta \\" ;
      toreturn.push_back(ss.str()); ss.str("");
      //toreturn.push_back( "\t  -o 1 -N 2 $NUMINDS 1 -G 1 -p 2 -u 1 >> $RUNFOLDER/summary.$RUNPREFIX.mstats_mSNPcall.txt" );
      toreturn.push_back( "\t  -o 1 -N 2 $NUMINDS 1 -G 1 -p 2 -u 1 | \\");
      ss << "\t  " << daPaths.get_path_perl() << " -ne 'print '$REP',\"\\t\",$1,\"\\t\",$2,\"\\t\",$3,\"\\t\",$4,\"\\t\",$5,\"\\t\",$6,\"\\t\",$7,\"\\n\"if";
     toreturn.push_back(ss.str()); ss.str("");
     toreturn.push_back("\t  /");
     toreturn.push_back("\t   Eff_length2_pop\\[0]\\:\\t(.+?)\\t.*");
     toreturn.push_back("\t   S\\[0]\\:\\t(.+?)\\t.*");
     toreturn.push_back("\t   Theta\\(Wat\\)\\[0]\\:\\t(.+?)\\t.*");
     toreturn.push_back("\t   Theta\\(Taj\\)\\[0]\\:\\t(.+?)\\t.*");
     toreturn.push_back("\t   TajimaD\\[0]\\:\\t(.+?)\\t.*");
     toreturn.push_back("\t   Fu&LiD\\[0]\\:\\t(.+?)\\t.*");
     toreturn.push_back("\t   Fay&WunormH\\[0]\\:\\t(.+?)\\t.*");
     toreturn.push_back("\t  /x' >> $RUNFOLDER/summary.$RUNPREFIX.mSNPcall_mstats.txt");
    }

  toreturn.push_back("");


  if( !sum_iSNPcall && !sum_mSNPcall && !inspect_iSNPcall && !inspect_mSNPcall && !mstats_iSNPcall && !mstats_mSNPcall&& ! mstats_pre )
    errors.push_back( "Stats: no statistics to calculate?" );

  // END REPLICATE LOOP
  toreturn.push_back("\t# REPLICATE FINISHED");
  if ( daRunSettings.clean_up() )
    toreturn.push_back("\trm $RUNFOLDER/$RUNPREFIX*");

  toreturn.push_back("\tdone");
  toreturn.push_back("# copy summary files");
  toreturn.push_back("cp $RUNFOLDER/*$RUNPREFIX* $DIRDATA");

  indent_level--;
  if( daRunSettings.clean_up() ){
      toreturn.push_back("if [ $REMOVE == 1 ]");
      toreturn.push_back("\tthen cd $DIRDATA; rm -r $RUNFOLDER; fi");
    }
  toreturn.push_back("");


  toreturn.push_back("# # PIPELINE FINISHED # #");
  toreturn.push_back("exit 0");

  pipeline_output aReadyPipeline;
  aReadyPipeline.cmds = toreturn;
  aReadyPipeline.errors = errors;
  return aReadyPipeline;

}
