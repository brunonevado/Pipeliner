//
//  pipeline.h
//  xcode-GUI-backbone
//
//  Created by Bruno Nevado on 02/08/2013.
//  Copyright (c) 2013 Bruno Nevado. All rights reserved.
//

#ifndef __xcode_GUI_backbone__pipeline__
#define __xcode_GUI_backbone__pipeline__

#include <iostream>
#include "paths.h"
#include "run_settings.h"
#include "toob_in_ms.h"
#include "toob_in_sfscode.h"
#include "toob_fas_fasq_art.h"
#include "toob_fasq_bam_bwa.h"
#include "toob_bam_vcf_samtools.h"
#include "toob_user_defined.h"
#include "plot_results.h"

struct pipeline_output {
    std::vector <std::string> cmds;
    std::vector <std::string> errors;
};

class pipeline{
private:
    std::string pipeline_name, bash_header;
    int active_input, active_ngs_sim, active_alignment, active_snpcall;
    int qual_offset;
    bool sum_iSNPcall, sum_mSNPcall, inspect_iSNPcall, inspect_mSNPcall;
    bool mstats_iSNPcall, mstats_mSNPcall, mstats_pre;
public:
    pipeline(std::string name);
    //std::vector <std::string> get_cmds();
    pipeline_output get_cmds ();
    //
    paths daPaths;
    run_settings daRunSettings;
    // active toobs
    void set_active_input ( int n ) { active_input = n; };
    int get_active_input ( ) { return active_input; };
    void set_active_ngs ( int n ) { active_ngs_sim = n; };
    int get_active_ngs ( ) { return active_ngs_sim; };
    void set_active_aligner ( int n ) { active_alignment = n; };
    int get_active_aligner ( ) { return active_alignment; };
    void set_active_snpcall ( int n ) { active_snpcall = n; };
    int get_active_snpcall ( ) { return active_snpcall; };

    void set_sum_iSNPcall ( bool o ) { sum_iSNPcall = o; };
    bool get_sum_iSNPcall ( ) { return sum_iSNPcall; };
    void set_inspect_iSNPcall ( bool o ) { inspect_iSNPcall = o; };
    bool get_inspect_iSNPcall ( ) { return inspect_iSNPcall; };
    void set_mstats_iSNPcall ( bool o ) { mstats_iSNPcall = o; };
    bool get_mstats_iSNPcall ( ) { return mstats_iSNPcall; };

    void set_mstats_preseq ( bool o ) { mstats_pre = o; };
    bool get_mstats_preseq ( ) { return mstats_pre; };

    void set_sum_mSNPcall ( bool o ) { sum_mSNPcall = o; };
    bool get_sum_mSNPcall ( ) { return sum_mSNPcall; };
    void set_inspect_mSNPcall ( bool o ) { inspect_mSNPcall = o; };
    bool get_inspect_mSNPcall ( ) { return inspect_mSNPcall; };
    void set_mstats_mSNPcall ( bool o ) { mstats_mSNPcall = o; };
    bool get_mstats_mSNPcall ( ) { return mstats_mSNPcall; };

    void set_qual_offset ( int n ) { qual_offset = n; };
    int get_qual_offset ( ) { return qual_offset; };

    // toobs
    toob_in_ms daInputMs;
    toob_in_sfscode daInputSfscode;
    toob_in_user daInputUser;
    toob_fas_fasq_art daNgsArt;
    toob_ngs_user daNgsUser;
    toob_fasq_bam_bwa daAlignmentBwa;
    toob_align_user daAlignUser;
    toob_bam_vcf_samtools daSnpcallSamtools;
    toob_snpcall_user daSnpcallUser;
    //
    void set_bash_header( std::string in ) { bash_header = in ; };
    std::string get_bash_header(  ) { return bash_header ; };

    plot_results Daplotter;
};




#endif /* defined(__xcode_GUI_backbone__pipeline__) */
