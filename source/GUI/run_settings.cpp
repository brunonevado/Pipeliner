//
//  pGUI
//
//  Copyright (c) 2013 Bruno Nevado. GNU license
//


#include "run_settings.h"

run_settings::run_settings(  ) {
  runprefix = "test";
  dirdata = "";
  runfolder = "";
  num_inds = 3;
  num_reps = 2;
  clean = true;
}


void run_settings::write ( std::vector <std::string> & command_list, std::vector <int> & indent, std::vector <std::string> & errors) {

  command_list.push_back( "#BASH file created by Pipeliner, run " +  runprefix );indent.push_back(0);
  command_list.push_back( "" );indent.push_back(0);
  command_list.push_back( "DIRDATA=" +  dirdata );indent.push_back(0);
  if(dirdata == "")
    errors.push_back("run settings: data folder undefined");

  std::stringstream ss;
  ss << "RUNFOLDER=";
  if(runfolder == "" )
    ss << dirdata;
  else
    ss << runfolder;
  command_list.push_back( ss.str() );indent.push_back(0); ss.str("");
  command_list.push_back( "RUNPREFIX=" +  runprefix );indent.push_back(0);
  if(runprefix == "")
    errors.push_back("run settings: run prefix undefined");
  ss << "NUMREPS=" << num_reps;
  command_list.push_back( ss.str() ) ;indent.push_back(0);
  ss.str("");
  if( num_reps < 1 )
    errors.push_back("run settings: number of reps must be 1+");

  ss << "NUMINDS=" << num_inds;
  command_list.push_back( ss.str() );indent.push_back(0);
  ss.str("");
  command_list.push_back( "" );indent.push_back(0);

}

