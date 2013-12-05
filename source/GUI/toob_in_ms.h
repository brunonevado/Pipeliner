//
//  pGUI
//
//  Copyright (c) 2013 Bruno Nevado. GNU license
//
#ifndef __pGUI_toob_in_ms__
#define __pGUI_toob_in_ms__

#include <iostream>
#include <vector>
#include <string>
#include <sstream>

#include "paths.h"

class toob_in_ms {

private:
  std::string toob_type, ancestral_file;
  float split_time, theta, rho;
  bool random_ancestral;
  int seq_len;

public:
  toob_in_ms();
  void write ( std::vector <std::string> & cmds, std::vector <int> & indent, paths path_bucket, std::vector <std::string> & err );
  int get_seqlen ( ) { return seq_len; };
  void set_seqlen ( int n ) { seq_len = n; };
  bool get_rand_ancestral ( ) { return random_ancestral; };
  void set_rand_ancestral ( bool i ) { random_ancestral = i; };
  float get_split_time ( ) { return split_time; };
  void set_split_time ( float n ) { split_time = n; };
  float get_theta ( ) { return theta; };
  void set_theta ( float n ) { theta = n; };
  float get_rho ( ) { return rho; };
  void set_rho ( float n ) { rho = n; };
  std::string get_ancestral_file () { return ancestral_file; }
  void set_ancestral_file ( std::string in ) { ancestral_file = in; };

};


#endif /* defined(__pGUI_toob_in_ms__) */
