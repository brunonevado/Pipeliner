//
//  paths.cpp
//  pGUI
//
//  Copyright (c) 2013 Bruno Nevado. All rights reserved.
//

#include "paths.h"
#include <fstream>
#include <regex.h>


paths::paths(){
  paths_version = "# Pipeliner Paths v1";
  path_ms = "ms";
  path_pipeliner = "Pipeliner";
  path_art = "art_illumina";
  path_bwa = "bwa";
  path_samtools = "samtools";
  path_bcftools = "bcftools";
  path_vcfutils = "vcfutils.pl";
  path_java = "java -jar";
  path_picard = "./picard-tools";
  path_perl = "perl";
  path_bash = "/bin/bash";
  path_awk = "awk";
  path_mstats = "mstatspop";
  path_R = "R";
}

void paths::set_paths ( std::vector < std::string > in ){
  path_ms = in.at(0);
  path_art = in.at(1);
  path_bwa = in.at(2);
  path_samtools = in.at(3);
  path_bcftools = in.at(4);
  path_vcfutils = in.at(5);
  path_java = in.at(6);
  path_picard = in.at(7);
  path_perl = in.at(8);
  path_pipeliner = in.at(9);
  path_bash = in.at(10);
  path_mstats = in.at(11);
  path_R = in.at(12);

}

int paths::write_paths ( std::string filename ){
  std::ofstream oFile;
  oFile.open(filename.c_str());
  oFile << paths_version << std::endl;
  oFile << path_ms << std::endl;
  oFile << path_art << std::endl;
  oFile << path_bwa  << std::endl;
  oFile << path_samtools << std::endl;
  oFile << path_bcftools << std::endl;
  oFile << path_vcfutils << std::endl;
  oFile << path_java << std::endl;
  oFile << path_picard << std::endl;
  oFile << path_perl << std::endl;
  oFile << path_pipeliner << std::endl;
  oFile << path_bash << std::endl;
  oFile << path_mstats << std::endl;
  oFile << path_R << std::endl;
  oFile.close();
  return 0;
}

std::string paths::read_paths ( std::string filename ){
  std::vector < std::string > in;
  std::string cline;
  std::ifstream iFile;
  iFile.open(filename.c_str());
  if (iFile.is_open()){
      while( getline ( iFile, cline ) )
        in.push_back( cline );

      if( in.at(0) != paths_version )
        return "File " + filename + " does not match current version, or is not a paths file. Read aborted!";

      path_ms = in.at(1);
      path_art = in.at(2);
      path_bwa = in.at(3);
      path_samtools = in.at(4);
      path_bcftools = in.at(5);
      path_vcfutils = in.at(6);
      path_java = in.at(7);
      path_picard = in.at(8);
      path_perl = in.at(9);
      path_pipeliner = in.at(10);
      path_bash = in.at(11);
      path_mstats = in.at(12);
      path_R = in.at(13);
    }
  else{
      return "Unable to open file for reading";
    }

  return "";
}

std::vector <std::string > paths::list_paths (){
  std::vector < std::string > out;
  out.push_back( path_ms);
  out.push_back(path_art);
  out.push_back(path_bwa);
  out.push_back(path_samtools);
  out.push_back(path_bcftools);
  out.push_back(path_vcfutils);
  out.push_back(path_java);
  out.push_back(path_picard);
  out.push_back(path_perl);
  out.push_back(path_pipeliner);
  out.push_back(path_bash);
  out.push_back(path_mstats);
  out.push_back(path_R);

  return out;
}
