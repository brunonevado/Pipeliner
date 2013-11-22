#ifndef PLOT_RESULTS_H
#define PLOT_RESULTS_H

#include <vector>
#include <iostream>
#include <fstream>
#include "paths.h"

class plot_results
{
private:

  std::string  filename, tempfile;
  std::vector < std::string > files_sum, files_mstats, plot_fun, names_sum, names_mstats;
  int acc_type;

public:
  plot_results();

  void set_filename ( std::string in ) { filename = in; };
  std::string get_filename (  ) { return filename; };

  void set_sum_files ( std::vector < std::string > in ) { files_sum = in; };
  std::vector < std::string > get_sum_files () { return files_sum; };

  void set_mstats_files ( std::vector < std::string > in ) { files_mstats = in; };
  std::vector < std::string > get_mstats_files () { return files_mstats; };

  void set_sum_names ( std::vector < std::string > in ) { names_sum = in; };
  std::vector < std::string > get_sum_names () { return names_sum; };

  void set_ms_names ( std::vector < std::string > in ) { names_mstats = in; };
  std::vector < std::string > get_ms_names () { return names_mstats; };

  void set_acc_type ( int n ) { acc_type = n; };
  int get_acc_type ( ) { return acc_type; };

  void clear_sum_names () { names_sum.clear(); };
  void clear_ms_names () { names_mstats.clear(); };

  std::string work ( paths path_bucket );

};

#endif // PLOT_RESULTS_H
