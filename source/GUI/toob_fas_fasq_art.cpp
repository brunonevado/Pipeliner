//
//  pGUI
//
//  Copyright (c) 2013 Bruno Nevado. GNU license
//

#include "toob_fas_fasq_art.h"

toob_fas_fasq_art::toob_fas_fasq_art( ){
  toob_type = "fas.fasq.art";
  read_len = 75;
  frag_len = 500;
  frag_len_sd = 10;
  num_ends = 2;
  av_cov = 10;
}

void toob_fas_fasq_art::write (std::vector <std::string> & cmds, std::vector <int> & indent, paths bucket_paths, std::vector <std::string> & err  ){
  std::stringstream ss;

  cmds.push_back(""); indent.push_back(0);
  cmds.push_back("# STARTING NGS SIMULATION STEP (USING ART_ILLUMINA)"); indent.push_back(0);

  if( num_ends != 1 && num_ends != 2 )
    err.push_back("ngs_simulation art: number of ends should be 1 or 2");

  ss << "AVCOV=" << av_cov/2 << "  # half the desired coverage per diploid individual";
  cmds.push_back(ss.str()); indent.push_back(0);
  ss.str("");
  cmds.push_back(""); indent.push_back(0);
  cmds.push_back("for((IND=1; IND<=$NUMINDS; IND++))");indent.push_back(0);
  cmds.push_back("do");indent.push_back(1);

  ss << "SEQ_FROM=$(((($IND-1)*4)+3))";
  cmds.push_back(ss.str()); indent.push_back(1);
  ss.str("");
  ss << "SEQ_TO=$(((($IND-1)*4)+6))";
  cmds.push_back(ss.str()); indent.push_back(1);
  ss.str("");
  cmds.push_back( "sed -n \"${SEQ_FROM},${SEQ_TO}p\" $RUNFOLDER/$RUNPREFIX.replicate$REP.fas \\");indent.push_back(1);
  cmds.push_back( "  > $RUNFOLDER/$RUNPREFIX.replicate$REP.Ind$IND.fas");indent.push_back(1);

  ss << bucket_paths.get_path_art() << " -i $RUNFOLDER/$RUNPREFIX.replicate$REP.Ind$IND.fas \\";
  cmds.push_back(ss.str()); indent.push_back(1); ss.str("");

  if( num_ends == 2 )
    ss << "  -p -m " << frag_len <<  " -s " << frag_len_sd << " -l " << read_len << " -f $AVCOV \\";
  else
    ss << "  -l " << read_len << " -f $AVCOV \\";
  cmds.push_back(ss.str()); indent.push_back(1); ss.str("");
  cmds.push_back( "  -o $RUNFOLDER/$RUNPREFIX.replicate$REP.Ind$IND.fas. -na -q"); indent.push_back(1); ss.str("");
  cmds.push_back("sleep 1 # to get new random seed for art"); indent.push_back(1);
  cmds.push_back("done");indent.push_back(1);
  cmds.push_back("# FINISHED NGS SIMULATION STEP"); indent.push_back(0);
  cmds.push_back(""); indent.push_back(0);

}
