//
//  toob_fasq_bam_bwa.h
//  xcode-GUI-backbone
//
//  Created by Bruno Nevado on 07/08/2013.
//  Copyright (c) 2013 Bruno Nevado. All rights reserved.
//

#ifndef __xcode_GUI_backbone__toob_fasq_bam_bwa__
#define __xcode_GUI_backbone__toob_fasq_bam_bwa__

#include <iostream>
#include <vector>
#include "paths.h"


class toob_fasq_bam_bwa {

private:
    std::string toob_type;
    int  num_ends, mmapq, nt;
    float nmp;
    bool rmdup;
public:
    toob_fasq_bam_bwa();
    void write ( std::vector <std::string> & cmds, std::vector <int> & indent, paths path_bucket, std::vector <std::string> & err  );
    int get_num_ends () { return num_ends; };
    void set_num_ends ( int n ) { num_ends = n; };
    int get_mapq () { return mmapq; };
    void set_mapq ( int n ) { mmapq = n; };
    int get_nt () { return nt; };
    void set_nt ( int n ) { nt = n; };
    float get_nmp () { return nmp; };
    void set_nmp ( float n ) { nmp = n; };
    bool use_rmdup () { return rmdup; };
    void set_rmdup ( bool in ) { rmdup = in; };
};


#endif /* defined(__xcode_GUI_backbone__toob_fasq_bam_bwa__) */
