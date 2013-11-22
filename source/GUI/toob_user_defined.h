//
//  toob_user_defined.h
//  xcode-GUI-backbone
//
//  Created by Bruno Nevado on 20/08/2013.
//  Copyright (c) 2013 Bruno Nevado. All rights reserved.
//

#ifndef __xcode_GUI_backbone__toob_user_defined__
#define __xcode_GUI_backbone__toob_user_defined__

#include <iostream>
#include <vector>
#include "paths.h"


class toob_in_user {

private:
    std::string toob_type, ancestral_file;
    int seq_len;
    bool random_ancestral, ms_format;
    std::vector <std::string> user_cmds, keywords;
    std::string find_fas, replace_fas, find_ms, replace_ms, find_temp, replace_temp;
public:
    toob_in_user();
    void write ( std::vector <std::string> & cmds, std::vector <int> & indent, paths bucket_paths, std::vector <std::string> & errors );
    int get_seqlen ( ) { return seq_len; };
    void set_seqlen ( int n ) { seq_len = n; };
    bool get_rand_ancestral ( ) { return random_ancestral; };
    void set_rand_ancestral ( bool i ) { random_ancestral = i; };
    std::string get_ancestral_file () { return ancestral_file; }
    void set_ancestral_file ( std::string in ) { ancestral_file = in; };
    void set_user_comds ( std::vector < std::string> in) { user_cmds = in; };
    bool get_format_ms ( ) { return ms_format; };
    void set_format_ms ( bool i ) { ms_format = i; };

    std::vector < std::string > get_keywords () { return keywords; };

};

class toob_ngs_user {

private:
    std::string toob_type;
    int num_ends;
    std::vector <std::string> user_cmds, keywords;
    std::string find_infas, replace_infas, find_temp, replace_temp;
    std::string find_outfasq, replace_outfasq;
    std::string find_outfasq1, replace_outfasq1;
    std::string find_outfasq2, replace_outfasq2;
    std::string find_number, replace_number;

public:
    toob_ngs_user();
    void write ( std::vector <std::string> & cmds, std::vector <int> & indent, paths bucket_paths, std::vector <std::string> & errors );
    int get_nends () { return num_ends; };
    void set_user_comds ( std::vector < std::string> in) { user_cmds = in; };

    std::vector < std::string > get_keywords () { return keywords; };

};

class toob_align_user {

private:
    std::string toob_type;
    bool addReadGroups, createDict;
    int num_ends;
    std::vector <std::string> user_cmds, keywords;

    std::string find_temp, replace_temp;
    std::string find_bam, replace_bam;
    std::string find_ref, replace_ref;
    std::string find_infasq, replace_infasq;
    std::string find_infasq1, replace_infasq1;
    std::string find_infasq2, replace_infasq2;
    std::string find_number, replace_number;

public:
    toob_align_user();
    void write ( std::vector <std::string> & cmds, std::vector <int> & indent, paths bucket_paths, std::vector <std::string> & errors );
    bool get_addGroups ( ) { return addReadGroups; };
    void set_addGroups ( bool i ) { addReadGroups = i; };
    bool get_creatDict ( ) { return createDict; };
    void set_creatDict ( bool i ) { createDict = i; };
    int get_nends () { return num_ends; };
    void set_user_comds ( std::vector < std::string> in) { user_cmds = in; };
    std::vector < std::string > get_keywords () { return keywords; };

};

class toob_snpcall_user {

private:
    std::string toob_type;
    bool iSNPcall, mSNPcall;
    std::string bam_sep;
    int sites_col;
    std::vector <std::string> user_cmds_iSNP, user_cmds_sites, user_cmds_mSNP;
    std::vector <std::string> keywords_ind, keywords_mult, keywords_sites;

    std::string find_temp, replace_temp;
    std::string find_number, replace_number;
    std::string find_ibam, replace_ibam;
    std::string find_mbam, replace_mbam;
    std::string find_sites, replace_sites;
    std::string find_ref, replace_ref;
    std::string find_ivcf, replace_ivcf;
    std::string find_mvcf, replace_mvcf;

public:
    toob_snpcall_user();
    void write ( std::vector <std::string> & cmds, std::vector <int> & indent, paths bucket_paths, std::vector <std::string> & errors );
    bool get_iSNPcall ( ) { return iSNPcall; };
    void set_iSNPcall ( bool i ) { iSNPcall = i; };
    bool get_mSNPcall ( ) { return mSNPcall; };
    void set_mSNPcall ( bool i ) { mSNPcall = i; };
    void set_sites_col ( int n ) { sites_col = n; };
    int get_sites_col ( ) { return sites_col; };
    void set_bam_sep ( std::string in ) { bam_sep = in; };
    std::string get_bam_sep ( ) { return bam_sep; };
    void set_user_snp_iSNP ( std::vector < std::string> in) { user_cmds_iSNP = in; };
    void set_user_snp_mSNP ( std::vector < std::string> in) { user_cmds_mSNP = in; };
    void set_user_snp_sites ( std::vector < std::string> in) { user_cmds_sites = in; };
    std::vector < std::string > get_keywords_ind () { return keywords_ind; };
    std::vector < std::string > get_keywords_sites () { return keywords_sites; };
    std::vector < std::string > get_keywords_mult () { return keywords_mult; };

};


#endif /* defined(__xcode_GUI_backbone__toob_user_defined__) */
