//
//  toob_in_sfscode.cpp
//  pGUI
//
//  Copyright (c) 2013 Bruno Nevado. All rights reserved.
//

#include "toob_in_sfscode.h"
#include <sstream>

toob_in_sfscode::toob_in_sfscode( ){

    num_pops = 2;
    num_diploids_ppop = 20;
    study_pop = 2;
    ref_pop = 1;
    num_diploids_to_sample = 10;
    toob_type = "input.sfs", infile_sfs = "";

}

void toob_in_sfscode::write (std::vector <std::string> & cmds, std::vector <int> & indent, paths bucket_paths, std::vector <std::string> & err  ){
    std::stringstream ss;

    cmds.push_back(""); indent.push_back(0);
    cmds.push_back("# STARTING INPUT STEP (FROM SFS_CODE FILE)"); indent.push_back(0);

    if( ref_pop > num_pops || study_pop > num_pops )
        err.push_back("input sfs_code: number of populations in infile sfs_code smaller than index of population of interest / reference");
    if( num_diploids_to_sample >= num_diploids_ppop  )
        err.push_back("input sfs_code: number of diploid individuals to sample (from population of interest) must be smaller than number of individuals simulated per population");
    if( infile_sfs == ""  )
        err.push_back("input sfs_code: input file undefined");

    ss << "INPUT_SFS=" << infile_sfs ;
    cmds.push_back(ss.str()); indent.push_back(0);
    ss.str("");
    ss << "NUM_POPS=" << num_pops ;
    cmds.push_back(ss.str()); indent.push_back(0);
    ss.str("");
    ss << "NUM_INDS_PER_POP=" << num_diploids_ppop ;
    cmds.push_back(ss.str()); indent.push_back(0);
    ss.str("");
    ss << "STUDY_POP=" << study_pop ;
    cmds.push_back(ss.str()); indent.push_back(0);
    ss.str("");
    ss << "REF_POP=" << ref_pop ;
    cmds.push_back(ss.str()); indent.push_back(0);
    ss.str("");
    ss << "SAMPLE_SIZE=" << num_diploids_to_sample ;
    cmds.push_back(ss.str()); indent.push_back(0);
    ss.str("");
    ss << bucket_paths.get_path_pipeliner();
    ss << " sfs2fas -infile $INPUT_SFS \\";
    cmds.push_back(ss.str()); indent.push_back(0); ss.str("");

    cmds.push_back( "  -outfile $RUNFOLDER/$RUNPREFIX.replicate$REP.fas \\" );indent.push_back(0);
    cmds.push_back( "  -replicate $REP -force chr1 \\");indent.push_back(0);
    cmds.push_back( "  -npops $NUM_POPS -pop_size $NUM_INDS_PER_POP \\" );indent.push_back(0);
    cmds.push_back( "  -pop_ref $REF_POP -pop_sample $STUDY_POP -ndiploids $SAMPLE_SIZE");indent.push_back(0);
    cmds.push_back("# FINISHED INPUT STEP #"); indent.push_back(0);
    cmds.push_back(""); indent.push_back(0);


}
