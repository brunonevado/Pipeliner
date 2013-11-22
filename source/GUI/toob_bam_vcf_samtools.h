//
//  toob_bam_vcf_samtools.h
//  xcode-GUI-backbone
//
//  Created by Bruno Nevado on 07/08/2013.
//  Copyright (c) 2013 Bruno Nevado. All rights reserved.
//

#ifndef __xcode_GUI_backbone__toob_bam_vcf_samtools__
#define __xcode_GUI_backbone__toob_bam_vcf_samtools__

#include <iostream>
#include <vector>
#include "paths.h"


class toob_bam_vcf_samtools {

private:
    std::string toob_type;
    int min_cov, max_cov, min_rms, min_baseq; //, min_alt, flt_gap_around, flt_gap_win;
    bool iSNPcall, mSNPcall, baq;
public:
    toob_bam_vcf_samtools();
    void write ( std::vector <std::string> & cmds, std::vector <int> & indent, paths path_bucket, std::vector <std::string> & err  );
    bool summarise_iSNPcall () { return iSNPcall; };
    bool summarise_mSNPcall () { return mSNPcall; };

    int get_mincov () { return min_cov; };
    void set_mincov ( int n ) { min_cov = n; };
    int get_maxcov () { return max_cov; };
    void set_maxcov ( int n ) { max_cov = n; };
    int get_minrms () { return min_rms; };
    void set_minrms ( int n ) { min_rms = n; };
    int get_minbaseq () { return min_baseq; };
    void set_minbaseq ( int n ) { min_baseq = n; };
    bool get_iSNPcall () { return iSNPcall; };
    void set_iSNPcall ( bool i ) { iSNPcall = i; };
    bool get_mSNPcall () { return mSNPcall; };
    void set_mSNPcall ( bool i ) { mSNPcall = i; };
    bool get_baq () { return baq; };
    void set_baq ( bool i ) { baq = i; };

};

#endif /* defined(__xcode_GUI_backbone__toob_bam_vcf_samtools__) */
