//
//  pGUI
//
//  Copyright (c) 2013 Bruno Nevado. GNU license
//

#ifndef __pGUI_toob_in_sfscode__
#define __pGUI_toob_in_sfscode__

#include <iostream>
#include <vector>
#include <sstream>

#include "paths.h"


class toob_in_sfscode {

private:
  std::string toob_type, infile_sfs;
  int num_pops, num_diploids_ppop, study_pop, ref_pop, num_diploids_to_sample;

public:
  toob_in_sfscode();
  void write ( std::vector <std::string> & cmds, std::vector <int> & indent, paths path_bucket, std::vector <std::string> & err  );
  void set_numpops ( int n ) { num_pops = n; }
  int get_numpops ( ) { return num_pops; }
  void set_numindsppop ( int n ) { num_diploids_ppop = n; }
  int get_numindsppop ( ) { return num_diploids_ppop; }
  void set_popStudy ( int n ) { study_pop = n; }
  int get_popStudy ( ) { return study_pop; }
  void set_popRef ( int n ) { ref_pop = n; }
  int get_popRef ( ) { return ref_pop; }
  void set_infile ( std::string in ) { infile_sfs = in; }
  std::string get_infile ( ) { return infile_sfs; }
  void set_numinds ( int n ) { num_diploids_to_sample = n; }
  int get_numinds ( ) { return num_diploids_to_sample; }

};


#endif /* defined(__pGUI_toob_in_sfscode__) */
