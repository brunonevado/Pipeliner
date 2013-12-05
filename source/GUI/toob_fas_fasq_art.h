//
//  pGUI
//
//  Copyright (c) 2013 Bruno Nevado. GNU license
//

#ifndef __pGUI_toob_fas_fasq_art__
#define __pGUI_toob_fas_fasq_art__

#include <iostream>
#include <vector>
#include <sstream>

#include "paths.h"

class toob_fas_fasq_art {

private:
  std::string toob_type;
  int read_len, frag_len, frag_len_sd, num_ends;
  float av_cov;

public:
  toob_fas_fasq_art();
  void write ( std::vector <std::string> & cmds, std::vector <int> & indent, paths path_bucket, std::vector <std::string> & err  );
  void set_readlen ( int n ){ read_len = n; };
  int get_readlen () { return read_len; };
  void set_fraglen ( int n ) { frag_len = n; };
  int get_fraglen () { return frag_len; };
  void set_fragsd ( int n ) { frag_len_sd = n; };
  int get_fragsd () { return frag_len_sd; };
  void set_nends ( int n ) { num_ends = n; };
  int get_nends () { return num_ends; };
  void set_coverage ( float n ) { av_cov = n; }
  float get_coverage () { return av_cov; };
};


#endif /* defined(__pGUI_toob_fas_fasq_art__) */
