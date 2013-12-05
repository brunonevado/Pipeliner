//
//  pGUI
//
//  Copyright (c) 2013 Bruno Nevado. GNU license
//

#include "toob_fasq_bam_bwa.h"

toob_fasq_bam_bwa::toob_fasq_bam_bwa( ){
  toob_type = "fasq.bam.bwa";
  num_ends = 2;
  mmapq = 20;
  nmp = 6;
  nt = 1;
  rmdup = true;
}

void toob_fasq_bam_bwa::write (std::vector <std::string> & cmds, std::vector <int> & indent, paths bucket_paths, std::vector <std::string> & err  ){
  std::stringstream ss;

  cmds.push_back(""); indent.push_back(0);
  cmds.push_back("# STARTING ALIGNMENT STEP (USING BWA)"); indent.push_back(0);

  ss << "NMP=" << nmp;
  cmds.push_back(ss.str()); indent.push_back(0);
  ss.str("");
  ss << "NT=" << nt;
  cmds.push_back(ss.str()); indent.push_back(0);
  ss.str("");
  ss << "MAPQ=" << mmapq;
  cmds.push_back(ss.str()); indent.push_back(0);
  ss.str("");
  cmds.push_back("sed -n \"1,2p\" $RUNFOLDER/$RUNPREFIX.replicate$REP.fas \\"); indent.push_back(0);
  cmds.push_back( "  > $RUNFOLDER/$RUNPREFIX.replicate$REP.reference.fa"); indent.push_back(0);

  ss << bucket_paths.get_path_bwa() << " index -a is $RUNFOLDER/$RUNPREFIX.replicate$REP.reference.fa";
  cmds.push_back(ss.str()); indent.push_back(0);
  ss.str("");
  ss << bucket_paths.get_path_samtools() << " faidx $RUNFOLDER/$RUNPREFIX.replicate$REP.reference.fa";
  cmds.push_back(ss.str()); indent.push_back(0);
  ss.str("");

  ss << bucket_paths.get_path_java() << " " << bucket_paths.get_path_picard() << "/CreateSequenceDictionary.jar \\";
  cmds.push_back(ss.str()); indent.push_back(0); ss.str("");
  cmds.push_back("  R=$RUNFOLDER/$RUNPREFIX.replicate$REP.reference.fa \\"); indent.push_back(0);
  cmds.push_back("  O=$RUNFOLDER/$RUNPREFIX.replicate$REP.reference.dict"); indent.push_back(0);

  cmds.push_back(""); indent.push_back(0);
  cmds.push_back("for((IND=1; IND<=$NUMINDS; IND++))");indent.push_back(0);
  cmds.push_back("do");indent.push_back(1);

  if( num_ends == 2 ){
      ss << bucket_paths.get_path_bwa() << " aln -n $NMP -t $NT $RUNFOLDER/$RUNPREFIX.replicate$REP.reference.fa \\";
      cmds.push_back(ss.str()); indent.push_back(1); ss.str("");
      ss  << "  $RUNFOLDER/$RUNPREFIX.replicate$REP.Ind$IND.fas.1.fq \\";
      cmds.push_back(ss.str()); indent.push_back(1);
      ss.str("");
      cmds.push_back("  > $RUNFOLDER/$RUNPREFIX.replicate$REP.Ind$IND.1.sai"); indent.push_back(1);
      ss << bucket_paths.get_path_bwa() << " aln -n $NMP -t $NT $RUNFOLDER/$RUNPREFIX.replicate$REP.reference.fa \\";
      cmds.push_back(ss.str()); indent.push_back(1); ss.str("");
      ss  << "  $RUNFOLDER/$RUNPREFIX.replicate$REP.Ind$IND.fas.2.fq \\";
      cmds.push_back(ss.str()); indent.push_back(1); ss.str("");
      cmds.push_back("  > $RUNFOLDER/$RUNPREFIX.replicate$REP.Ind$IND.2.sai"); indent.push_back(1);
      ss << bucket_paths.get_path_bwa() << " sampe $RUNFOLDER/$RUNPREFIX.replicate$REP.reference.fa \\";
      cmds.push_back( ss.str() ); indent.push_back(1); ss.str("");
      cmds.push_back( "  $RUNFOLDER/$RUNPREFIX.replicate$REP.Ind$IND.1.sai \\" ); indent.push_back(1);
      cmds.push_back( "  $RUNFOLDER/$RUNPREFIX.replicate$REP.Ind$IND.2.sai \\" ); indent.push_back(1);
      cmds.push_back( "  $RUNFOLDER/$RUNPREFIX.replicate$REP.Ind$IND.fas.1.fq \\" ); indent.push_back(1);
      cmds.push_back( "  $RUNFOLDER/$RUNPREFIX.replicate$REP.Ind$IND.fas.2.fq \\" ); indent.push_back(1);
      ss << "  | " << bucket_paths.get_path_samtools() << " view -q $MAPQ -Suh - \\";
      cmds.push_back(ss.str()); indent.push_back(1); ss.str("");
      ss << "  | " << bucket_paths.get_path_samtools() << " sort - $RUNFOLDER/$RUNPREFIX.replicate$REP.Ind$IND.temp";
      cmds.push_back(ss.str()); indent.push_back(1); ss.str("");
    }
  else if ( num_ends == 1){
      ss << bucket_paths.get_path_bwa() << " aln -n $NMP -t $NT > $RUNFOLDER/$RUNPREFIX.replicate$REP.Ind$IND.sai \\";
      cmds.push_back(ss.str()); indent.push_back(1); ss.str("");
      cmds.push_back( "  $RUNFOLDER/$RUNPREFIX.replicate$REP.reference.fa \\" ); indent.push_back(1);
      cmds.push_back( "  $RUNFOLDER/$RUNPREFIX.replicate$REP.Ind$IND.fas..fq" ); indent.push_back(1);
      ss << bucket_paths.get_path_bwa() << " samse $RUNFOLDER/$RUNPREFIX.replicate$REP.reference.fa \\";
      cmds.push_back(ss.str()); indent.push_back(1); ss.str("");
      cmds.push_back( "  $RUNFOLDER/$RUNPREFIX.replicate$REP.Ind$IND.sai \\" ); indent.push_back(1);
      cmds.push_back( "  $RUNFOLDER/$RUNPREFIX.replicate$REP.Ind$IND.fas..fq \\" ); indent.push_back(1);
      ss << "  | " << bucket_paths.get_path_samtools() << " view -q $MAPQ -Suh - \\";
      cmds.push_back(ss.str()); indent.push_back(1); ss.str("");

      ss << "  | " << bucket_paths.get_path_samtools() << " sort - $RUNFOLDER/$RUNPREFIX.replicate$REP.Ind$IND.temp";
      cmds.push_back(ss.str()); indent.push_back(1); ss.str("");
    }
  else{
      err.push_back("alignment bwa: number of ends should be 1 or 2");
    }

  ss << bucket_paths.get_path_java() << " " << bucket_paths.get_path_picard() << "/AddOrReplaceReadGroups.jar \\";
  cmds.push_back(ss.str()); indent.push_back(1); ss.str("");
  cmds.push_back("  I=$RUNFOLDER/$RUNPREFIX.replicate$REP.Ind$IND.temp.bam \\"); indent.push_back(1);
  cmds.push_back("  O=$RUNFOLDER/$RUNPREFIX.replicate$REP.Ind$IND.RGOK.bam \\"); indent.push_back(1);
  cmds.push_back("  ID=1 LB=1 PL=illumina PU=1 SM=\"Ind$IND\""); indent.push_back(1);

  if(rmdup){
      ss << bucket_paths.get_path_samtools() << " rmdup $RUNFOLDER/$RUNPREFIX.replicate$REP.Ind$IND.RGOK.bam \\";
      cmds.push_back(ss.str()); indent.push_back(1); ss.str("");
      cmds.push_back("  $RUNFOLDER/$RUNPREFIX.replicate$REP.Ind$IND.bam"); indent.push_back(1);
    }else{
      cmds.push_back("mv $RUNFOLDER/$RUNPREFIX.replicate$REP.Ind$IND.RGOK.bam \\");indent.push_back(1); ss.str("");
      cmds.push_back("  $RUNFOLDER/$RUNPREFIX.replicate$REP.Ind$IND.bam");indent.push_back(1); ss.str("");
    }

  cmds.push_back("done");indent.push_back(1);
  cmds.push_back("# FINISHED ALIGNMENT STEP"); indent.push_back(0);
  cmds.push_back(""); indent.push_back(0);
}
