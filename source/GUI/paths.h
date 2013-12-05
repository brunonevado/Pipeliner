//
//  pGUI
//
//  Copyright (c) 2013 Bruno Nevado. GNU license
//

#ifndef __pGUI_paths__
#define __pGUI_paths__

#include <iostream>
#include <vector>
#include <fstream>
#include <regex.h>

class paths {

private:
  std::string path_ms, path_pipeliner, path_art, path_bwa;
  std::string path_samtools, path_bcftools, path_vcfutils;
  std::string path_java, path_picard, path_perl, path_bash, path_awk;
  std::string paths_version, path_mstats, path_R;

public:
  paths();
  std::string get_path_ms () { return path_ms; };
  std::string get_path_pipeliner () { return path_pipeliner; };
  std::string get_path_art () { return path_art; };
  std::string get_path_bwa () { return path_bwa; };
  std::string get_path_samtools () { return path_samtools; };
  std::string get_path_bcftools () { return path_bcftools; };
  std::string get_path_vcfutils () { return path_vcfutils; };
  std::string get_path_java () { return path_java; };
  std::string get_path_picard () { return path_picard; };
  std::string get_path_perl () { return path_perl; };
  std::string get_path_bash () { return path_bash; };
  std::string get_path_mstats () { return path_mstats; };
  std::string get_path_R () { return path_R; };
  void set_paths ( std::vector < std::string > in );
  int write_paths ( std::string filename );
  std::vector < std::string > list_paths ();
  std::string read_paths ( std::string filename );
};


#endif /* defined(__pGUI_paths__) */
