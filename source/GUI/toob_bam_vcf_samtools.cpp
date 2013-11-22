//
//  toob_bam_vcf_samtools.cpp
//  pGUI
//
//  Copyright (c) 2013 Bruno Nevado. All rights reserved.
//

#include "toob_bam_vcf_samtools.h"
#include <sstream>


toob_bam_vcf_samtools::toob_bam_vcf_samtools( ){
    toob_type = "bam.vcf.samtools";
    min_cov = 3;
    max_cov = 100;
    min_rms = 10;
    //min_alt = 2;
    //flt_gap_around = 3;
    //flt_gap_win = 10;
    min_baseq = 13;
    baq = true;
    iSNPcall = true;
    mSNPcall = true;

}

void toob_bam_vcf_samtools::write (std::vector <std::string> & cmds, std::vector <int> & indent, paths bucket_paths, std::vector <std::string> & err  ){
    std::stringstream ss;

    cmds.push_back(""); indent.push_back(0);
    cmds.push_back("# STARTING SNP CALLING AND FILTERING STEP (USING SAMTOOLS)"); indent.push_back(0);

    ss << "MINDEP=" << min_cov;
    cmds.push_back(ss.str()); indent.push_back(0); ss.str("");
    ss << "MAXDEP=" << max_cov;
    if( max_cov < min_cov )
      err.push_back("samtools snp call: max depth < min depth");

    cmds.push_back(ss.str()); indent.push_back(0); ss.str("");
    ss << "MINRMS=" << min_rms;
    cmds.push_back(ss.str()); indent.push_back(0); ss.str("");
    ss << "BASEQ=" << min_baseq;
    cmds.push_back(ss.str()); indent.push_back(0); ss.str("");
    cmds.push_back("unset ALLBAMFILES"); indent.push_back(0);
    cmds.push_back("unset ALL_IND_VCF_FILES"); indent.push_back(0);
    cmds.push_back("unset ALL_MS_VCF_FILES"); indent.push_back(0);
    cmds.push_back("unset ALL_SITES_FILES"); indent.push_back(0);
    cmds.push_back("");indent.push_back(0);
    cmds.push_back("for((IND=1; IND<=$NUMINDS; IND++))");indent.push_back(0);
    cmds.push_back("do");indent.push_back(1);
    cmds.push_back("# sort and index bam files");indent.push_back(1);
    cmds.push_back("mv $RUNFOLDER/$RUNPREFIX.replicate$REP.Ind$IND.bam \\");indent.push_back(1);
    cmds.push_back("  $RUNFOLDER/$RUNPREFIX.replicate$REP.Ind$IND.bam.unsorted");indent.push_back(1);


    ss << bucket_paths.get_path_samtools() << " sort $RUNFOLDER/$RUNPREFIX.replicate$REP.Ind$IND.bam.unsorted \\";
    cmds.push_back(ss.str()); indent.push_back(1); ss.str("");
    cmds.push_back("  $RUNFOLDER/$RUNPREFIX.replicate$REP.Ind$IND");indent.push_back(1);
    ss << bucket_paths.get_path_samtools() << " index $RUNFOLDER/$RUNPREFIX.replicate$REP.Ind$IND.bam";
    cmds.push_back(ss.str()); indent.push_back(1); ss.str("");
    //cmds.push_back("ALLBAMFILES[${IND}]=$RUNFOLDER/$RUNPREFIX.replicate$REP.Ind$IND.bam");indent.push_back(1);
    cmds.push_back("ALLBAMFILES+=(\"$RUNFOLDER/$RUNPREFIX.replicate$REP.Ind$IND.bam\")"); indent.push_back(1);


    ////////////   INDIVIDUAL SNP CALLING  /////////////
    cmds.push_back("");indent.push_back(1);
    cmds.push_back("# Individual snp calling (using samtools)");indent.push_back(1);
    ss << bucket_paths.get_path_samtools() << " mpileup";
    if(baq)
        ss << " -B -Q $BASEQ -uf $RUNFOLDER/$RUNPREFIX.replicate$REP.reference.fa \\";
    else
        ss << " -Q $BASEQ -uf $RUNFOLDER/$RUNPREFIX.replicate$REP.reference.fa \\";

    std::stringstream snpcall_cmd;
    snpcall_cmd << ss.rdbuf();

    cmds.push_back(ss.str()); indent.push_back(1); ss.str("");

    cmds.push_back("  $RUNFOLDER/$RUNPREFIX.replicate$REP.Ind$IND.bam \\"); indent.push_back(1);
    ss << "  | " << bucket_paths.get_path_bcftools() << " view -vcg - \\" ;
    cmds.push_back(ss.str()); indent.push_back(1); ss.str("");
    ss << "  | " << bucket_paths.get_path_vcfutils() << " varFilter -d $MINDEP -D $MAXDEP -Q $MINRMS \\";
    cmds.push_back(ss.str()); indent.push_back(1); ss.str("");
    cmds.push_back("  > $RUNFOLDER/$RUNPREFIX.iSNPcall.replicate$REP.Ind$IND.flt.vcf");indent.push_back(1);


    // SITES LIST
    cmds.push_back("");indent.push_back(1);
    cmds.push_back("# create confident sites list"); indent.push_back(1);


    /*

    cmds.push_back(snpcall_cmd.str()); indent.push_back(1); ss.str("");
    //cmds.push_back("  $RUNFOLDER/$RUNPREFIX.replicate$REP.Ind$IND.bam \\"); indent.push_back(1);
    ss << "  $RUNFOLDER/$RUNPREFIX.replicate$REP.Ind$IND.bam | " << bucket_paths.get_path_bcftools() << " view -vcg - \\" ;


    cmds.push_back(ss.str()); indent.push_back(1); ss.str("");
    cmds.push_back( "  | grep -v \\# - | grep -v INDEL - | grep '[0-9]' - | cut -f 2 > $RUNFOLDER/$RUNPREFIX.replicate$REP.Ind$IND.unflt_var.sites" ); indent.push_back(1);
    cmds.push_back( "cut -f 2 $RUNFOLDER/$RUNPREFIX.iSNPcall.replicate$REP.Ind$IND.flt.vcf \\"); indent.push_back(1);
    cmds.push_back( "  | grep -v \\# - | grep -v INDEL - | grep '[0-9]'  > $RUNFOLDER/$RUNPREFIX.replicate$REP.Ind$IND.flt_var.sites" ); indent.push_back(1);
    cmds.push_back( "grep -Fxv -f $RUNFOLDER/$RUNPREFIX.replicate$REP.Ind$IND.flt_var.sites \\" ); indent.push_back(1);
    cmds.push_back( "  $RUNFOLDER/$RUNPREFIX.replicate$REP.Ind$IND.unflt_var.sites > $RUNFOLDER/$RUNPREFIX.replicate$REP.Ind$IND.bad_var.sites" ); indent.push_back(1);


    */

    ss << bucket_paths.get_path_samtools() << " depth -q $BASEQ -Q $MINRMS $RUNFOLDER/$RUNPREFIX.replicate$REP.Ind$IND.bam \\";
    cmds.push_back(ss.str()); indent.push_back(1); ss.str("");
    cmds.push_back( "  | grep -v \\# - | grep '[0-9]' | awk  '{ if( $3 >= '$MINDEP' && $3 <= '$MAXDEP') print $2}' \\"); indent.push_back(1);
    cmds.push_back("  > $RUNFOLDER/$RUNPREFIX.replicate$REP.Ind$IND.depth.sites" ); indent.push_back(1);

     cmds.push_back(snpcall_cmd.str()); indent.push_back(1); ss.str("");
     ss << "  $RUNFOLDER/$RUNPREFIX.replicate$REP.Ind$IND.bam | " << bucket_paths.get_path_bcftools() << " view -c - \\" ;
     cmds.push_back(ss.str()); indent.push_back(1); ss.str("");
     cmds.push_back( "  | awk  '{ if( $NF == 0) print $2}' > $RUNFOLDER/$RUNPREFIX.replicate$REP.Ind$IND.invar.sites"); indent.push_back(1);

     cmds.push_back( "awk 'NR==FNR{a[$0];next} $0 in a' \\" );indent.push_back(1);
     cmds.push_back( "  $RUNFOLDER/$RUNPREFIX.replicate$REP.Ind$IND.depth.sites \\" );indent.push_back(1);
     cmds.push_back( "  $RUNFOLDER/$RUNPREFIX.replicate$REP.Ind$IND.invar.sites \\" );indent.push_back(1);
     cmds.push_back( "  > $RUNFOLDER/$RUNPREFIX.replicate$REP.Ind$IND.sites" );indent.push_back(1);


     cmds.push_back( "cut -f 2 $RUNFOLDER/$RUNPREFIX.iSNPcall.replicate$REP.Ind$IND.flt.vcf \\" );indent.push_back(1);
     cmds.push_back( "  | grep -v \\# - | grep '[0-9]' - >> $RUNFOLDER/$RUNPREFIX.replicate$REP.Ind$IND.sites" );indent.push_back(1);

    /*
    cmds.push_back( "grep -Fxv -f $RUNFOLDER/$RUNPREFIX.replicate$REP.Ind$IND.bad_var.sites \\" ); indent.push_back(1);
    cmds.push_back( "  $RUNFOLDER/$RUNPREFIX.replicate$REP.Ind$IND.all_unflt.sites > $RUNFOLDER/$RUNPREFIX.replicate$REP.Ind$IND.sites" ); indent.push_back(1);
*/


    cmds.push_back("sort -n -u $RUNFOLDER/$RUNPREFIX.replicate$REP.Ind$IND.sites \\"); indent.push_back(1);
    cmds.push_back("  -o $RUNFOLDER/$RUNPREFIX.replicate$REP.Ind$IND.sites"); indent.push_back(1);


    cmds.push_back(""); indent.push_back(1);
    cmds.push_back("ALL_IND_VCF_FILES+=(\"$RUNFOLDER/$RUNPREFIX.iSNPcall.replicate$REP.Ind$IND.flt.vcf\")"); indent.push_back(1);
    cmds.push_back("ALL_SITES_FILES+=(\"$RUNFOLDER/$RUNPREFIX.replicate$REP.Ind$IND.sites\")"); indent.push_back(1);
    cmds.push_back("done");indent.push_back(1);

    ////////////   MULTIPLE-SAMPLE SNP CALLING  /////////////
    if(mSNPcall){
        cmds.push_back("");indent.push_back(0); ss.str("");
        cmds.push_back("# Multiple-sample snp calling (using samtools)");indent.push_back(0);
        ss << bucket_paths.get_path_samtools() << " mpileup";
        if(baq)
            ss << " -B -Q $BASEQ -uf $RUNFOLDER/$RUNPREFIX.replicate$REP.reference.fa \\";
        else
            ss << " -Q $BASEQ -uf $RUNFOLDER/$RUNPREFIX.replicate$REP.reference.fa \\";

        cmds.push_back(ss.str()); indent.push_back(0); ss.str("");
        cmds.push_back("  ${ALLBAMFILES[*]} \\"); indent.push_back(0);
        ss << "  | " << bucket_paths.get_path_bcftools() << " view -vcg - > $RUNFOLDER/$RUNPREFIX.mSNPcall.replicate$REP.allInds.vcf";
        cmds.push_back(ss.str()); indent.push_back(0); ss.str("");
        cmds.push_back("");indent.push_back(0);
        cmds.push_back("for((IND=1; IND<=$NUMINDS; IND++))");indent.push_back(0);
        cmds.push_back("do");indent.push_back(1);
        ss << bucket_paths.get_path_vcfutils() << " subsam $RUNFOLDER/$RUNPREFIX.mSNPcall.replicate$REP.allInds.vcf Ind$IND \\";
        cmds.push_back(ss.str()); indent.push_back(1); ss.str("");
        cmds.push_back("  > $RUNFOLDER/$RUNPREFIX.mSNPcall.replicate$REP.Ind$IND.unflt.vcf");indent.push_back(1);
        cmds.push_back("ALL_MS_VCF_FILES+=(\"$RUNFOLDER/$RUNPREFIX.mSNPcall.replicate$REP.Ind$IND.unflt.vcf\")"); indent.push_back(1);
        cmds.push_back("done");indent.push_back(1);



    }

    cmds.push_back("# FINISHED SNP CALLING STEP"); indent.push_back(0);
    cmds.push_back(""); indent.push_back(0);

}
