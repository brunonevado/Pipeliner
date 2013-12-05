//
//  pGUI
//
//  Copyright (c) 2013 Bruno Nevado. GNU license
//
#ifndef __pGUI_run_settings__
#define __pGUI_run_settings__

#include <iostream>
#include <vector>
#include <string>
#include <sstream>

class run_settings {

private:
  std::string runprefix, dirdata, runfolder, pipeline_name;
  int num_inds, num_reps;
  bool clean;

public:
  run_settings();
  void write ( std::vector <std::string> & cmds, std::vector <int> & indent, std::vector <std::string> & errors );
  void set_dirdata ( std::string in ) { dirdata = in; }
  std::string get_dirdata ( ) { return dirdata; }
  void set_runfolder ( std::string in ) { runfolder = in; }
  std::string get_runfolder ( ) { return runfolder; }
  void set_runprefix ( std::string in ) { runprefix = in; }
  std::string get_runprefix ( ) { return runprefix; }
  void set_numreps ( int n ) { num_reps = n; }
  int get_numreps ( ) { return num_reps; }
  void set_numinds ( int n ) { num_inds = n; }
  int get_numinds ( ) { return num_inds; }
  void set_clean ( bool t ) { clean = t; }
  bool is_clean () { return clean; }
  bool clean_up () { return clean; };
};




#endif /* defined(__pGUI_run_settings__) */
