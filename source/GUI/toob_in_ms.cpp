//
//  pGUI
//
//  Copyright (c) 2013 Bruno Nevado. GNU license
//

#include "toob_in_ms.h"

toob_in_ms::toob_in_ms( ){
  toob_type = "input.ms", ancestral_file = "";
  split_time = 0, theta = 40, rho = 40;
  seq_len = 100000;
  random_ancestral = true;
}


void toob_in_ms::write (std::vector <std::string> & cmds, std::vector <int> & indent, paths bucket_paths, std::vector <std::string> & errors ){
  std::stringstream ss;

  cmds.push_back(""); indent.push_back(0);
  cmds.push_back("# STARTING INPUT STEP (DEFAULT MS COALESCENT SIMULATION)"); indent.push_back(0);

  ss << "THETA=" << theta ;
  cmds.push_back(ss.str()); indent.push_back(0);
  ss.str("");
  ss << "RHO=" << rho ;
  cmds.push_back(ss.str()); indent.push_back(0);
  ss.str("");
  ss << "SEQLEN=" << seq_len;
  cmds.push_back( ss.str() );indent.push_back(0);
  ss.str("");


  if (split_time > 0){
      ss << "STIME=" << split_time ;
      cmds.push_back(ss.str()); indent.push_back(0);
      ss.str("");
    }
  if(random_ancestral){
      cmds.push_back( bucket_paths.get_path_pipeliner() + " writeRandomSeq -outfile "
                      + "$RUNFOLDER/$RUNPREFIX.ancestral.fa -len $SEQLEN"); indent.push_back(0);
    }
  else{
      if( ancestral_file == "" ) {
          errors.push_back("ms input: ancestral file not set");

        }
      cmds.push_back( bucket_paths.get_path_pipeliner() + " desinterleave -infile " + ancestral_file + " \\");indent.push_back(0);
      cmds.push_back( "  -outfile $RUNFOLDER/$RUNPREFIX.ancestral.fa"); indent.push_back(0);
    }
  cmds.push_back( bucket_paths.get_path_pipeliner() + " seedMS -folder $RUNFOLDER"); indent.push_back(0);
  std::string mscmd = bucket_paths.get_path_ms() + " $(($NUMINDS*2+1)) 1 -t $THETA";
  mscmd += ( rho > 0 ) ? " -r $RHO $SEQLEN " : "";
  mscmd += ( split_time > 0 ) ? " -I 2 1 $(($NUMINDS*2)) -ej $STIME  2 1" : "";
  cmds.push_back( mscmd + "\\" ); indent.push_back(0);
  cmds.push_back("  > $RUNFOLDER/$RUNPREFIX.replicate$REP.ms"); indent.push_back(0);

  cmds.push_back( bucket_paths.get_path_pipeliner() + " ms2fas -in_ms $RUNFOLDER/$RUNPREFIX.replicate$REP.ms \\");indent.push_back(0);
  cmds.push_back("  -in_anc $RUNFOLDER/$RUNPREFIX.ancestral.fa \\"); indent.push_back(0);
  cmds.push_back("  -out_fas $RUNFOLDER/$RUNPREFIX.replicate$REP.fas -force chr1"); indent.push_back(0);

  cmds.push_back("# FINISHED INPUT STEP #"); indent.push_back(0);
  cmds.push_back(""); indent.push_back(0);

}
