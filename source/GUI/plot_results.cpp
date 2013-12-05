//
//  pGUI
//
//  Copyright (c) 2013 Bruno Nevado. GNU license
//

#include "plot_results.h"

int get_token ( std::string s, int n ){
  int count = 0;
  unsigned long start = 0U;
  unsigned long end = s.find(" ");
  while (end != std::string::npos)
    {
      count++;
      if( count == n)
        return atoi(s.substr(start, end - start).c_str());
      start = end + 1;
      end = s.find( " ", start);
    }
  count++;
  if( count ==  n ){
      return atoi( s.substr(start, end - start).c_str() );
    }

  return -1;
}


plot_results::plot_results()
{
  // This is the R function to plot summary files from Pipeliner
  filename = "";
  tempfile = "";

  acc_type = 0;
  plot_fun.push_back("plot_summary_pipeliner <-function( summary_files, mstats_files, length, outfile,");
  plot_fun.push_back("  tags_summary, tags_mstats, homozygous_lower_limits=NULL, homozygous_upper_limits=NULL,");
  plot_fun.push_back("  heterozygous_lower_limits=NULL, heterozygous_upper_limits=NULL ){");
  plot_fun.push_back("");
  plot_fun.push_back("    if( !missing(summary_files) ){");
  plot_fun.push_back("        if( missing(length) ){");
  plot_fun.push_back("            stop (\"ERROR: length undefined\")");
  plot_fun.push_back("        }");
  plot_fun.push_back("    if( !is.null(homozygous_lower_limits) && !is.null(homozygous_upper_limits) ");
  plot_fun.push_back("      && !is.null(heterozygous_lower_limits) && !is.null(heterozygous_upper_limits) ){");
  plot_fun.push_back("        if( is.null(homozygous_lower_limits) || is.null(homozygous_upper_limits) ){");
  plot_fun.push_back("            stop (\"Homozygous SNP classes undefined\")");
  plot_fun.push_back("        }");
  plot_fun.push_back("        if( is.null(heterozygous_lower_limits) || is.null(heterozygous_upper_limits) ){");
  plot_fun.push_back("            stop (\"Heterozygous SNP classes undefined\")");
  plot_fun.push_back("        }");
  plot_fun.push_back("    }");
  plot_fun.push_back("    if( missing(outfile) ){");
  plot_fun.push_back("        stop (\"outfile undefined\")");
  plot_fun.push_back("    }");
  plot_fun.push_back("  ");
  plot_fun.push_back("    ## args OK");
  plot_fun.push_back("    data_list <- lapply(summary_files,read.table) # read files");
  plot_fun.push_back("    homo_min_lims=homozygous_lower_limits");
  plot_fun.push_back("    homo_max_lims=homozygous_upper_limits");
  plot_fun.push_back("    hetero_min_lims=heterozygous_lower_limits");
  plot_fun.push_back("    hetero_max_lims=heterozygous_upper_limits");
  plot_fun.push_back("");
  plot_fun.push_back("    namess<-vector(len=length(summary_files))");
  plot_fun.push_back("    if(missing(tags_summary)){");
  plot_fun.push_back("        for(i in 1:length(summary_files)){");
  plot_fun.push_back("            namess[i]<-i");
  plot_fun.push_back("        }");
  plot_fun.push_back("    }");
  plot_fun.push_back("    else{");
  plot_fun.push_back("        namess<-tags_summary");
  plot_fun.push_back("    }");
  plot_fun.push_back("    ");
  plot_fun.push_back("    mlist<-list()   # main list, will contain results from all input files");
  plot_fun.push_back("");
  plot_fun.push_back("    for( file in 1:length(data_list)){");
  plot_fun.push_back("        data<- data_list[[file]]");
  plot_fun.push_back("        x <- list(names=c(");
  plot_fun.push_back("          \"coverage\",");
  plot_fun.push_back("          \"Recovery RR\",");
  plot_fun.push_back("          \"Accuracy RR\",");
  plot_fun.push_back("          \"Recovery AA\",");
  plot_fun.push_back("          \"Accuracy AA\",");
  plot_fun.push_back("          \"AA -> missed\",");
  plot_fun.push_back("          \"AA -> indel\",");
  plot_fun.push_back("          \"AA -> RA\",");
  plot_fun.push_back("          \"AA -> RR\",");
  plot_fun.push_back("          \"AA -> AO\",");
  plot_fun.push_back("          \"AA -> RO\",");
  plot_fun.push_back("          \"AA -> OO\",");
  plot_fun.push_back("          \"AA -> OO2\",");
  plot_fun.push_back("          \"FDR AA\",");
  plot_fun.push_back("          \"Recovery RA\",");
  plot_fun.push_back("          \"Accuracy RA\",");
  plot_fun.push_back("          \"RA -> missed\",");
  plot_fun.push_back("          \"RA -> indel\",");
  plot_fun.push_back("          \"RA -> RR\",");
  plot_fun.push_back("          \"RA -> AA\",");
  plot_fun.push_back("          \"RA -> AO\",");
  plot_fun.push_back("          \"RA -> RO\",");
  plot_fun.push_back("          \"RA -> OO\",");
  plot_fun.push_back("          \"RA -> OO2\",");
  plot_fun.push_back("          \"FDR RA\"");
  plot_fun.push_back("        ))");
  plot_fun.push_back("        x_paac<-list(names=c(\"Homozyogus PAAC\", \"Heterozygous PAAC\"))");
  plot_fun.push_back("");
  plot_fun.push_back("        ######### COVERAGE ############");
  plot_fun.push_back("");
  plot_fun.push_back("        x$coverage <-     ( data$V3[ which(data$V2 == \"T_seen\") ]  ) * 100 /length");
  plot_fun.push_back("    ");
  plot_fun.push_back("        ######### INVARIABLE POSITIONS ############");
  plot_fun.push_back("    ");
  plot_fun.push_back("        invar_len <- length -");
  plot_fun.push_back("          data$V3[ which( data$V2 == \"T_aa\" ) ] -");
  plot_fun.push_back("          data$V3[ which( data$V2 == \"T_ra\" ) ] -");
  plot_fun.push_back("          data$V3[ which( data$V2 == \"rr_amb\" ) ]");
  plot_fun.push_back("        invar_wrong <-");
  plot_fun.push_back("          data$V3[ which( data$V2 == \"rr_oo\" ) ] +");
  plot_fun.push_back("          data$V3[ which( data$V2 == \"rr_ro\" ) ] +");
  plot_fun.push_back("          data$V3[ which( data$V2 == \"rr_oo2\" ) ] +");
  plot_fun.push_back("          data$V3[ which( data$V2 == \"rr_ind\" ) ]");
  plot_fun.push_back("        invar_screened <-");
  plot_fun.push_back("          data$V3[ which( data$V2 == \"T_seen\" ) ] -");
  plot_fun.push_back("          data$V3[ which( data$V2 == \"aa_see\" ) ] -");
  plot_fun.push_back("          data$V3[ which( data$V2 == \"ra_see\" ) ]");
  plot_fun.push_back("    ");
  plot_fun.push_back("        x$\"Recovery RR\" <- (invar_screened - invar_wrong )* 100 /  invar_len");
  plot_fun.push_back("        x$\"Accuracy RR\" <- (invar_screened - invar_wrong )* 100 /  invar_screened");
  plot_fun.push_back("    ");
  plot_fun.push_back("        ######### HOMOZYGOUS POSITIONS ############");
  plot_fun.push_back("    ");
  plot_fun.push_back("        x$\"Recovery AA\"  <- data$V3[ which( data$V2 == \"aa_aa\" ) ] * 100 /  data$V3[ which( data$V2 == \"T_aa\" ) ]");
  plot_fun.push_back("        x$\"Accuracy AA\"  <- data$V3[ which( data$V2 == \"aa_aa\" ) ] * 100 /  data$V3[ which( data$V2 == \"aa_see\" ) ]");
  plot_fun.push_back("        x$\"AA -> missed\" <- data$V3[ which( data$V2 == \"aa_mis\" ) ] * 100 /  data$V3[ which( data$V2 == \"T_aa\" ) ]");
  plot_fun.push_back("        x$\"AA -> indel\"  <- data$V3[ which( data$V2 == \"aa_ind\" ) ] * 100 /  data$V3[ which( data$V2 == \"T_aa\" ) ]");
  plot_fun.push_back("        x$\"AA -> RA\"     <- data$V3[ which( data$V2 == \"aa_ra\" ) ] * 100 /  data$V3[ which( data$V2 == \"T_aa\" ) ]");
  plot_fun.push_back("        x$\"AA -> RR\"     <- data$V3[ which( data$V2 == \"aa_rr\" ) ] * 100 /  data$V3[ which( data$V2 == \"T_aa\" ) ]");
  plot_fun.push_back("        x$\"AA -> AO\"     <- data$V3[ which( data$V2 == \"aa_ao\" ) ] * 100 /  data$V3[ which( data$V2 == \"T_aa\" ) ]");
  plot_fun.push_back("        x$\"AA -> RO\"     <- data$V3[ which( data$V2 == \"aa_ro\" ) ] * 100 /  data$V3[ which( data$V2 == \"T_aa\" ) ]");
  plot_fun.push_back("        x$\"AA -> OO\"     <- data$V3[ which( data$V2 == \"aa_oo\" ) ] * 100 /  data$V3[ which( data$V2 == \"T_aa\" ) ]");
  plot_fun.push_back("        x$\"AA -> OO2\"    <- data$V3[ which( data$V2 == \"aa_oo2\" ) ] * 100 /  data$V3[ which( data$V2 == \"T_aa\" ) ]");
  plot_fun.push_back("    ");
  plot_fun.push_back("        #FDR");
  plot_fun.push_back("        total_fixed_calls <-");
  plot_fun.push_back("          data$V3[ which( data$V2 == \"aa_aa\" ) ] +");
  plot_fun.push_back("          data$V3[ which( data$V2 == \"rr_oo\" ) ] +");
  plot_fun.push_back("          data$V3[ which( data$V2 == \"aa_oo\" ) ] +");
  plot_fun.push_back("          data$V3[ which( data$V2 == \"ra_aa\" ) ] +");
  plot_fun.push_back("          data$V3[ which( data$V2 == \"ra_oo\" ) ]");
  plot_fun.push_back("        x$\"FDR AA\" <- ( total_fixed_calls - data$V3[ which( data$V2 == \"aa_aa\" ) ] ) * 100 /  total_fixed_calls");
  plot_fun.push_back("    ");
  plot_fun.push_back("        ######### HETEROZYGOUS POSITIONS ############");
  plot_fun.push_back("    ");
  plot_fun.push_back("        x$\"Recovery RA\"  <- data$V3[ which( data$V2 == \"ra_ra\" ) ] * 100 /  data$V3[ which( data$V2 == \"T_ra\" ) ]");
  plot_fun.push_back("        x$\"Accuracy RA\"  <- data$V3[ which( data$V2 == \"ra_ra\" ) ] * 100 /  data$V3[ which( data$V2 == \"ra_see\" ) ]");
  plot_fun.push_back("        x$\"RA -> missed\" <- data$V3[ which( data$V2 == \"ra_mis\" ) ] * 100 /  data$V3[ which( data$V2 == \"T_ra\" ) ]");
  plot_fun.push_back("        x$\"RA -> indel\"  <- data$V3[ which( data$V2 == \"ra_ind\" ) ] * 100 /  data$V3[ which( data$V2 == \"T_ra\" ) ]");
  plot_fun.push_back("        x$\"RA -> AA\"     <- data$V3[ which( data$V2 == \"ra_aa\" ) ] * 100 /  data$V3[ which( data$V2 == \"T_ra\" ) ]");
  plot_fun.push_back("        x$\"RA -> RR\"   <- data$V3[ which( data$V2 == \"ra_rr\" ) ] * 100 /  data$V3[ which( data$V2 == \"T_ra\" ) ]");
  plot_fun.push_back("        x$\"RA -> AO\"   <- data$V3[ which( data$V2 == \"ra_ao\" ) ] * 100 /  data$V3[ which( data$V2 == \"T_ra\" ) ]");
  plot_fun.push_back("        x$\"RA -> RO\"   <- data$V3[ which( data$V2 == \"ra_ro\" ) ] * 100 /  data$V3[ which( data$V2 == \"T_ra\" ) ]");
  plot_fun.push_back("        x$\"RA -> OO\"  <- data$V3[ which( data$V2 == \"ra_oo\" ) ] * 100 /  data$V3[ which( data$V2 == \"T_ra\" ) ]");
  plot_fun.push_back("        x$\"RA -> OO2\"  <- data$V3[ which( data$V2 == \"ra_oo2\" ) ] * 100 /  data$V3[ which( data$V2 == \"T_ra\" ) ]");
  plot_fun.push_back("        ");
  plot_fun.push_back("        #FDR");
  plot_fun.push_back("        total_snp_calls <-");
  plot_fun.push_back("          data$V3[ which( data$V2 == \"ra_ra\" ) ] +");
  plot_fun.push_back("          data$V3[ which( data$V2 == \"rr_oo2\" ) ] +");
  plot_fun.push_back("          data$V3[ which( data$V2 == \"rr_ro\" ) ] +");
  plot_fun.push_back("          data$V3[ which( data$V2 == \"aa_oo2\" ) ] +");
  plot_fun.push_back("          data$V3[ which( data$V2 == \"aa_ra\" ) ] +");
  plot_fun.push_back("          data$V3[ which( data$V2 == \"aa_ro\" ) ] +");
  plot_fun.push_back("          data$V3[ which( data$V2 == \"aa_ao\" ) ] +");
  plot_fun.push_back("          data$V3[ which( data$V2 == \"ra_ao\" ) ] +");
  plot_fun.push_back("          data$V3[ which( data$V2 == \"ra_ro\" ) ] +");
  plot_fun.push_back("          data$V3[ which( data$V2 == \"ra_oo2\" ) ]");
  plot_fun.push_back("        x$\"FDR RA\" <- ( total_snp_calls - data$V3[ which( data$V2 == \"ra_ra\" ) ] ) * 100 /  total_snp_calls");
  plot_fun.push_back("    ");
  plot_fun.push_back("    ");
  plot_fun.push_back("    ");
  plot_fun.push_back("    ");
  plot_fun.push_back("        if( !is.null(homozygous_lower_limits) && !is.null(homozygous_upper_limits) ");
  plot_fun.push_back("          && !is.null(heterozygous_lower_limits) && !is.null(heterozygous_upper_limits) ){");
  plot_fun.push_back("");
  plot_fun.push_back("        ######### AAC ############");
  plot_fun.push_back("        for(i in 1:length(homo_min_lims)){");
  plot_fun.push_back("            mint=homo_min_lims[i]+3");
  plot_fun.push_back("            maxt=homo_max_lims[i]+3");
  plot_fun.push_back("            x_paac$\"Homozygous PAAC\"<-c(x_paac$\"Homozygous PAAC\",");
  plot_fun.push_back("              mean(na.rm=TRUE,");
  plot_fun.push_back("              as.matrix((data[ which( data$V2 == \"aa_aa\" ) , mint:maxt ]))*100/");
  plot_fun.push_back("              as.matrix(data[ which( data$V2 == \"aa_see\" ) , mint:maxt ]");
  plot_fun.push_back("              )))");
  plot_fun.push_back("            }");
  plot_fun.push_back("        for(i in 1:length(hetero_min_lims)){");
  plot_fun.push_back("            mint=hetero_min_lims[i]+3");
  plot_fun.push_back("            maxt=hetero_max_lims[i]+3");
  plot_fun.push_back("            x_paac$\"Heterozygous PAAC\"<-c(x_paac$\"Heterozygous PAAC\",");
  plot_fun.push_back("              mean(na.rm=TRUE,");
  plot_fun.push_back("              as.matrix((data[ which( data$V2 == \"ra_ra\" ) , mint:maxt ]))*100/");
  plot_fun.push_back("              as.matrix(data[ which( data$V2 == \"ra_see\" ) , mint:maxt ]");
  plot_fun.push_back("              )))");
  plot_fun.push_back("            }");
  plot_fun.push_back("        }");
  plot_fun.push_back("    ## add to main list ##");
  plot_fun.push_back("    mlist[[file]] = x");
  plot_fun.push_back("    names(mlist)[file] <- namess[file]");
  plot_fun.push_back("    mlist[[file]]$\"homo_paac\" <- x_paac$\"Homozygous PAAC\"");
  plot_fun.push_back("    mlist[[file]]$\"hetero_paac\" <- x_paac$\"Heterozygous PAAC\"");
  plot_fun.push_back("    }");
  plot_fun.push_back("    # here finishes files' loop");
  plot_fun.push_back("    ");
  plot_fun.push_back("    ## PLOTS");
  plot_fun.push_back("    ###########");
  plot_fun.push_back("  ");
  plot_fun.push_back("    pdf(outfile)");
  plot_fun.push_back("    par(mfrow=c(2,3))");
  plot_fun.push_back("  ");
  plot_fun.push_back("    ## Recovery RR");
  plot_fun.push_back("    maxp = max(mlist[[1]]$\"Recovery RR\",na.rm=TRUE)");
  plot_fun.push_back("    minp = min(mlist[[1]]$\"Recovery RR\",na.rm=TRUE)");
  plot_fun.push_back("    if( length(mlist) > 1 )");
  plot_fun.push_back("      for(i in 2:length(mlist)){");
  plot_fun.push_back("        maxp = max( c(maxp, mlist[[i]]$\"Recovery RR\"),na.rm=TRUE )");
  plot_fun.push_back("        minp = min( c(minp, mlist[[i]]$\"Recovery RR\"),na.rm=TRUE )");
  plot_fun.push_back("    }");
  plot_fun.push_back("    minp = minp*0.9");
  plot_fun.push_back("    maxp = maxp*1.1");
  plot_fun.push_back("    minp=max(c(0,minp))");
  plot_fun.push_back("    maxp=min(c(100,maxp))");
  plot_fun.push_back("    boxplot(mlist[[1]]$\"Recovery RR\",xlim=c(0.5,length(mlist)+0.5 ),ylim=c(minp,maxp),at=1,names=\"\",labels=\"\")");
  plot_fun.push_back("    if( length(mlist) > 1 )");
  plot_fun.push_back("      for (i in 2:length(mlist))");
  plot_fun.push_back("        boxplot(mlist[[i]]$\"Recovery RR\",at=i,add=TRUE)");
  plot_fun.push_back("    axis(1,1:length(mlist),names(mlist),las=2)");
  plot_fun.push_back("    title(main=\"Invariable\npositions\",y=\"Recovery (%)\")");
  plot_fun.push_back("  ");
  plot_fun.push_back("    ## Recovery RA");
  plot_fun.push_back("    maxp = max(mlist[[1]]$\"Recovery RA\",na.rm=TRUE)");
  plot_fun.push_back("    minp = min(mlist[[1]]$\"Recovery RA\",na.rm=TRUE)");
  plot_fun.push_back("    if( length(mlist) > 1 )");
  plot_fun.push_back("      for(i in 2:length(mlist)){");
  plot_fun.push_back("        maxp = max( c(maxp, mlist[[i]]$\"Recovery RA\") , na.rm=TRUE )");
  plot_fun.push_back("        minp = min( c(minp, mlist[[i]]$\"Recovery RA\"), na.rm=TRUE )");
  plot_fun.push_back("    }");
  plot_fun.push_back("    minp = minp*0.9");
  plot_fun.push_back("    maxp = maxp*1.1");
  plot_fun.push_back("    minp=max(c(0,minp))");
  plot_fun.push_back("    maxp=min(c(100,maxp))");
  plot_fun.push_back("    boxplot(mlist[[1]]$\"Recovery RA\",xlim=c(0.5,length(mlist)+0.5 ),ylim=c(minp,maxp),at=1,names=\"\",labels=\"\")");
  plot_fun.push_back("    if( length(mlist) > 1 )");
  plot_fun.push_back("      for (i in 2:length(mlist))");
  plot_fun.push_back("        boxplot(mlist[[i]]$\"Recovery RA\",at=i,add=TRUE)");
  plot_fun.push_back("    axis(1,1:length(mlist),names(mlist),las=2)");
  plot_fun.push_back("    title(main=\"Heterozygous\nSNPs\",y=\"Recovery (%)\")");
  plot_fun.push_back("");
  plot_fun.push_back("    ## Recovery AA");
  plot_fun.push_back("    maxp = max(mlist[[1]]$\"Recovery AA\",na.rm=TRUE)");
  plot_fun.push_back("    minp = min(mlist[[1]]$\"Recovery AA\",na.rm=TRUE)");
  plot_fun.push_back("    if( length(mlist) > 1 )");
  plot_fun.push_back("      for(i in 2:length(mlist)){");
  plot_fun.push_back("        maxp = max( c(maxp, mlist[[i]]$\"Recovery AA\"),na.rm=TRUE )");
  plot_fun.push_back("        minp = min( c(minp, mlist[[i]]$\"Recovery AA\"),na.rm=TRUE )");
  plot_fun.push_back("    }");
  plot_fun.push_back("    minp = minp*0.9");
  plot_fun.push_back("    maxp = maxp*1.1");
  plot_fun.push_back("    minp=max(c(0,minp))");
  plot_fun.push_back("    maxp=min(c(100,maxp))");
  plot_fun.push_back("    boxplot(mlist[[1]]$\"Recovery AA\",xlim=c(0.5,length(mlist)+0.5 ),ylim=c(minp,maxp),at=1,names=\"\",labels=\"\")");
  plot_fun.push_back("    if( length(mlist) > 1 )");
  plot_fun.push_back("      for (i in 2:length(mlist))");
  plot_fun.push_back("        boxplot(mlist[[i]]$\"Recovery AA\",at=i,add=TRUE)");
  plot_fun.push_back("    axis(1,1:length(mlist),names(mlist),las=2)");
  plot_fun.push_back("    title(main=\"Homozygous\nSNPs\",y=\"Recovery (%)\")");
  plot_fun.push_back("");
  plot_fun.push_back("    ## ACCURACY RR");
  plot_fun.push_back("    maxp = max(mlist[[1]]$\"Accuracy RR\",na.rm=TRUE)");
  plot_fun.push_back("    minp = min(mlist[[1]]$\"Accuracy RR\",na.rm=TRUE)");
  plot_fun.push_back("    if( length(mlist) > 1 )");
  plot_fun.push_back("      for(i in 2:length(mlist)){");
  plot_fun.push_back("        maxp = max( c(maxp, mlist[[i]]$\"Accuracy RR\"),na.rm=TRUE )");
  plot_fun.push_back("        minp = min( c(minp, mlist[[i]]$\"Accuracy RR\"),na.rm=TRUE )");
  plot_fun.push_back("    }");
  plot_fun.push_back("    minp = minp*0.9");
  plot_fun.push_back("    maxp = maxp*1.1");
  plot_fun.push_back("    minp=max(c(0,minp))");
  plot_fun.push_back("    maxp=min(c(100,maxp))");
  plot_fun.push_back("    boxplot(mlist[[1]]$\"Accuracy RR\",xlim=c(0.5,length(mlist)+0.5 ),ylim=c(minp,maxp),at=1,names=\"\",labels=\"\")");
  plot_fun.push_back("    if( length(mlist) > 1 )");
  plot_fun.push_back("      for (i in 2:length(mlist))");
  plot_fun.push_back("        boxplot(mlist[[i]]$\"Accuracy RR\",at=i,add=TRUE)");
  plot_fun.push_back("    axis(1,1:length(mlist),names(mlist),las=2)");
  plot_fun.push_back("    title(main=\"Invariable\npositions\",y=\"Accuracy (%)\")");
  plot_fun.push_back("");
  plot_fun.push_back("    ## Accuracy RA");
  plot_fun.push_back("    maxp = max(mlist[[1]]$\"Accuracy RA\",na.rm=TRUE)");
  plot_fun.push_back("    minp = min(mlist[[1]]$\"Accuracy RA\",na.rm=TRUE)");
  plot_fun.push_back("    if( length(mlist) > 1 )");
  plot_fun.push_back("      for(i in 2:length(mlist)){");
  plot_fun.push_back("        maxp = max( c(maxp, mlist[[i]]$\"Accuracy RA\"),na.rm=TRUE )");
  plot_fun.push_back("        minp = min( c(minp, mlist[[i]]$\"Accuracy RA\"),na.rm=TRUE )");
  plot_fun.push_back("    }");
  plot_fun.push_back("    minp = minp*0.9");
  plot_fun.push_back("    maxp = maxp*1.1");
  plot_fun.push_back("    minp=max(c(0,minp))");
  plot_fun.push_back("    maxp=min(c(100,maxp))");
  plot_fun.push_back("    boxplot(mlist[[1]]$\"Accuracy RA\",xlim=c(0.5,length(mlist)+0.5 ),ylim=c(minp,maxp),at=1,names=\"\",labels=\"\")");
  plot_fun.push_back("    if( length(mlist) > 1 )");
  plot_fun.push_back("      for (i in 2:length(mlist))");
  plot_fun.push_back("        boxplot(mlist[[i]]$\"Accuracy RA\",at=i,add=TRUE)");
  plot_fun.push_back("    axis(1,1:length(mlist),names(mlist),las=2)");
  plot_fun.push_back("    title(main=\"Heterozygous\nSNPs\",y=\"Accuracy (%)\")");
  plot_fun.push_back("");
  plot_fun.push_back("    ## Accuracy AA");
  plot_fun.push_back("    maxp = max(mlist[[1]]$\"Accuracy AA\",na.rm=TRUE)");
  plot_fun.push_back("    minp = min(mlist[[1]]$\"Accuracy AA\",na.rm=TRUE)");
  plot_fun.push_back("    if( length(mlist) > 1 )");
  plot_fun.push_back("      for(i in 2:length(mlist)){");
  plot_fun.push_back("        maxp = max( c(maxp, mlist[[i]]$\"Accuracy AA\"),na.rm=TRUE )");
  plot_fun.push_back("        minp = min( c(minp, mlist[[i]]$\"Accuracy AA\"),na.rm=TRUE )");
  plot_fun.push_back("    }");
  plot_fun.push_back("    minp = minp*0.9");
  plot_fun.push_back("    maxp = maxp*1.1");
  plot_fun.push_back("    minp=max(c(0,minp))");
  plot_fun.push_back("    maxp=min(c(100,maxp))");
  plot_fun.push_back("    boxplot(mlist[[1]]$\"Accuracy AA\",xlim=c(0.5,length(mlist)+0.5 ),ylim=c(minp,maxp),at=1,names=\"\",labels=\"\")");
  plot_fun.push_back("    if( length(mlist) > 1 )");
  plot_fun.push_back("      for (i in 2:length(mlist))");
  plot_fun.push_back("        boxplot(mlist[[i]]$\"Accuracy AA\",at=i,add=TRUE)");
  plot_fun.push_back("    axis(1,1:length(mlist),names(mlist),las=2)");
  plot_fun.push_back("    title(main=\"Homozygous\nSNPs\",y=\"Accuracy (%)\")");
  plot_fun.push_back("");
  plot_fun.push_back("    ## Outcomes Heterozygous positions");
  plot_fun.push_back("  ");
  plot_fun.push_back("    outcomes_hetero <- c( mean(mlist[[1]]$\"Recovery RA\",na.rm=TRUE), mean(mlist[[1]]$\"RA -> missed\",na.rm=TRUE),  mean(mlist[[1]]$\"RA -> RR\",na.rm=TRUE), mean(mlist[[1]]$\"RA -> AA\",na.rm=TRUE), mean(mlist[[1]]$\"RA -> RO\",na.rm=TRUE), mean(mlist[[1]]$\"RA -> AO\",na.rm=TRUE), mean(mlist[[1]]$\"RA -> OO\",na.rm=TRUE), mean(mlist[[1]]$\"RA -> OO2\",na.rm=TRUE), mean(mlist[[1]]$\"RA -> indel\",na.rm=TRUE) )");
  plot_fun.push_back("    if( length(mlist) > 1 )");
  plot_fun.push_back("      for(i in 2:length(mlist)){");
  plot_fun.push_back("        temp <- c( mean(mlist[[i]]$\"Recovery RA\",na.rm=TRUE), mean(mlist[[i]]$\"RA -> missed\",na.rm=TRUE),  mean(mlist[[i]]$\"RA -> RR\",na.rm=TRUE), mean(mlist[[i]]$\"RA -> AA\",na.rm=TRUE), mean(mlist[[i]]$\"RA -> RO\",na.rm=TRUE), mean(mlist[[i]]$\"RA -> AO\",na.rm=TRUE), mean(mlist[[i]]$\"RA -> OO\",na.rm=TRUE), mean(mlist[[i]]$\"RA -> OO2\",na.rm=TRUE), mean(mlist[[i]]$\"RA -> indel\",na.rm=TRUE) )");
  plot_fun.push_back("        outcomes_hetero <- rbind( outcomes_hetero, temp)");
  plot_fun.push_back("    }");
  plot_fun.push_back("    layout(matrix(c(1,1,2,3,3,4),2,3,by=TRUE))");
  plot_fun.push_back("    ymax= min( 100, max(outcomes_hetero,na.rm=TRUE)*1.1 )");
  plot_fun.push_back("    barplot(outcomes_hetero,beside=TRUE, names.arg=c(\"correct\", \"missed\", \"RR\", \"AA\", \"RO\", \"AO\", \"OO\" , \"OO2\", \"indel\"),las=2,legend.text=names(mlist),args.legend=list(x=\"topright\",bty='n',cex=.9),ylim=c(0,ymax))");
  plot_fun.push_back("    title(main=\"Heterozygous SNPs\",y=\"Outcomes (%)\")");
  plot_fun.push_back("");
  plot_fun.push_back("    ## FDR heterozygous positions");
  plot_fun.push_back("    maxp = max(mlist[[1]]$\"FDR RA\",na.rm=TRUE)");
  plot_fun.push_back("    minp = min(mlist[[1]]$\"FDR RA\",na.rm=TRUE)");
  plot_fun.push_back("    if( length(mlist) > 1 )");
  plot_fun.push_back("      for(i in 2:length(mlist)){");
  plot_fun.push_back("        maxp = max( c(maxp, mlist[[i]]$\"FDR RA\"),na.rm=TRUE )");
  plot_fun.push_back("        minp = min( c(minp, mlist[[i]]$\"FDR RA\"),na.rm=TRUE )");
  plot_fun.push_back("    }");
  plot_fun.push_back("    minp = minp*0.9");
  plot_fun.push_back("    maxp = maxp*1.1");
  plot_fun.push_back("    minp=max(c(0,minp))");
  plot_fun.push_back("    maxp=min(c(100,maxp))");
  plot_fun.push_back("    boxplot(box=FALSE,mlist[[1]]$\"FDR RA\",xlim=c(0.5,length(mlist)+0.5 ),ylim=c(minp,maxp),at=1,names=\"\",labels=\"\")");
  plot_fun.push_back("    if( length(mlist) > 1 )");
  plot_fun.push_back("      for (i in 2:length(mlist))");
  plot_fun.push_back("        boxplot(mlist[[i]]$\"FDR RA\",at=i,add=TRUE)");
  plot_fun.push_back("    axis(1,1:length(mlist),names(mlist),las=2)");
  plot_fun.push_back("    title(main=\"Heterozygous\nSNPs\",y=\"FDR (%)\")");
  plot_fun.push_back("    ");
  plot_fun.push_back("    ## Outcomes Homozygous positions");
  plot_fun.push_back("");
  plot_fun.push_back("    outcomes_homo <- c( mean(mlist[[1]]$\"Recovery AA\",na.rm=TRUE), mean(mlist[[1]]$\"AA -> missed\",na.rm=TRUE),  mean(mlist[[1]]$\"AA -> RR\",na.rm=TRUE), mean(mlist[[1]]$\"AA -> RA\",na.rm=TRUE), mean(mlist[[1]]$\"AA -> RO\",na.rm=TRUE), mean(mlist[[1]]$\"AA -> AO\",na.rm=TRUE), mean(mlist[[1]]$\"AA -> OO\",na.rm=TRUE), mean(mlist[[1]]$\"AA -> OO2\",na.rm=TRUE), mean(mlist[[1]]$\"AA -> indel\",na.rm=TRUE) )");
  plot_fun.push_back("    if( length(mlist) > 1 )");
  plot_fun.push_back("      for(i in 2:length(mlist)){");
  plot_fun.push_back("        temp <- c( mean(mlist[[i]]$\"Recovery AA\",na.rm=TRUE), mean(mlist[[i]]$\"AA -> missed\",na.rm=TRUE),  mean(mlist[[i]]$\"AA -> RR\",na.rm=TRUE), mean(mlist[[i]]$\"AA -> RA\",na.rm=TRUE), mean(mlist[[i]]$\"AA -> RO\",na.rm=TRUE), mean(mlist[[i]]$\"AA -> AO\",na.rm=TRUE), mean(mlist[[i]]$\"AA -> OO\",na.rm=TRUE), mean(mlist[[i]]$\"AA -> OO2\",na.rm=TRUE), mean(mlist[[i]]$\"AA -> indel\",na.rm=TRUE) )");
  plot_fun.push_back("        outcomes_homo <- rbind( outcomes_homo, temp)");
  plot_fun.push_back("    }");
  plot_fun.push_back("    ymax= min( 100, max(outcomes_homo)*1.1 )");
  plot_fun.push_back("    barplot(outcomes_homo,beside=TRUE, names.arg=c(\"correct\", \"missed\", \"RR\", \"RA\", \"RO\", \"AO\", \"OO\" , \"OO2\", \"indel\"),las=2,legend.text=names(mlist),args.legend=list(x=\"topright\",bty='n',cex=.9),ylim=c(0,ymax))");
  plot_fun.push_back("    title(main=\"Homozygous SNPs\",y=\"Outcomes (%)\")");
  plot_fun.push_back("");
  plot_fun.push_back("    ## FDR Homozygous positions");
  plot_fun.push_back("    maxp = max(mlist[[1]]$\"FDR AA\",na.rm=TRUE)");
  plot_fun.push_back("    minp = min(mlist[[1]]$\"FDR AA\",na.rm=TRUE)");
  plot_fun.push_back("    if( length(mlist) > 1 )");
  plot_fun.push_back("      for(i in 2:length(mlist)){");
  plot_fun.push_back("        maxp = max( c(maxp, mlist[[i]]$\"FDR AA\"),na.rm=TRUE )");
  plot_fun.push_back("        minp = min( c(minp, mlist[[i]]$\"FDR AA\"),na.rm=TRUE )");
  plot_fun.push_back("    }");
  plot_fun.push_back("    minp = minp*0.9");
  plot_fun.push_back("    maxp = maxp*1.1");
  plot_fun.push_back("    minp=max(c(0,minp))");
  plot_fun.push_back("    maxp=min(c(100,maxp))");
  plot_fun.push_back("    boxplot(box=FALSE,mlist[[1]]$\"FDR AA\",xlim=c(0.5,length(mlist)+0.5 ),ylim=c(minp,maxp),at=1,names=\"\",labels=\"\")");
  plot_fun.push_back("    if( length(mlist) > 1 )");
  plot_fun.push_back("      for (i in 2:length(mlist))");
  plot_fun.push_back("        boxplot(mlist[[i]]$\"FDR AA\",at=i,add=TRUE)");
  plot_fun.push_back("    axis(1,1:length(mlist),names(mlist),las=2)");
  plot_fun.push_back("    title(main=\"Homozygous\nSNPs\",y=\"FDR (%)\")");
  plot_fun.push_back("  ");
  plot_fun.push_back("    if( !is.null(homozygous_lower_limits) && !is.null(homozygous_upper_limits) ");
  plot_fun.push_back("      && !is.null(heterozygous_lower_limits) && !is.null(heterozygous_upper_limits) ){");
  plot_fun.push_back("");
  plot_fun.push_back("        ## PER AAC, Heterozygous positions");
  plot_fun.push_back("        layout(matrix(c(1,1,4,2,2,3),2,3,by=TRUE))");
  plot_fun.push_back("        peracc_hetero<-matrix(ncol=length(mlist[[1]]$\"hetero_paac\"), nrow=length(mlist))");
  plot_fun.push_back("        for(i in 1:length(mlist)){");
  plot_fun.push_back("            peracc_hetero[i,] <-mlist[[i]]$\"hetero_paac\"");
  plot_fun.push_back("        }");
  plot_fun.push_back("        #prepare labels");
  plot_fun.push_back("        hetero_classes <- vector()");
  plot_fun.push_back("        for(i in 1:length(hetero_min_lims))");
  plot_fun.push_back("            hetero_classes[i]<-paste(hetero_min_lims[i], hetero_max_lims[i] ,sep=\":\")");
  plot_fun.push_back("        ymax= min(  max(peracc_hetero)*1.3 )");
  plot_fun.push_back("        barplot(yaxt='n',peracc_hetero,beside=TRUE, names.arg=hetero_classes,las=2,legend.text=names(mlist),args.legend=list(x=\"topright\",ncol=5,bty='n',cex=.9),ylim=c(0,ymax))");
  plot_fun.push_back("        axis(2,seq(0,min(c(ymax,100)),20),seq(0,min(c(100,ymax)),20))");
  plot_fun.push_back("        title(main=\"Heterozygous SNPs\",y=\"Accuracy per AAC (%)\",x=\"AAC class\" )");
  plot_fun.push_back("");
  plot_fun.push_back("       ## per AAC, Homozygous positions");
  plot_fun.push_back("        peracc_homo<-matrix(ncol=length(mlist[[1]]$\"homo_paac\"), nrow=length(mlist))");
  plot_fun.push_back("        for(i in 1:length(mlist)){");
  plot_fun.push_back("            peracc_homo[i,] <-mlist[[i]]$\"homo_paac\"");
  plot_fun.push_back("        }");
  plot_fun.push_back("        #prepare labels");
  plot_fun.push_back("        homo_classes <- vector()");
  plot_fun.push_back("        for(i in 1:length(homo_min_lims))");
  plot_fun.push_back("            homo_classes[i]<-paste(homo_min_lims[i], homo_max_lims[i] ,sep=\":\")");
  plot_fun.push_back("        ymax= min(  max(peracc_homo,na.rm=TRUE)*1.3 )");
  plot_fun.push_back("        barplot(yaxt='n',peracc_homo,beside=TRUE, names.arg=homo_classes,las=2,legend.text=names(mlist),args.legend=list(x=\"topright\",ncol=5,bty='n',cex=.9),ylim=c(0,ymax))");
  plot_fun.push_back("        axis(2,seq(0,min(c(ymax,100)),20),seq(0,min(c(100,ymax)),20))");
  plot_fun.push_back("        title(main=\"Homozygous SNPs\",y=\"Accuracy per AAC (%)\",x=\"AAC class\" )");
  plot_fun.push_back("        }");
  plot_fun.push_back("    }");
  plot_fun.push_back("");
  plot_fun.push_back("    ## MSTATS_RESULTS");
  plot_fun.push_back("");
  plot_fun.push_back("    if( !missing(mstats_files)){");
  plot_fun.push_back("        mstats_list <- lapply(mstats_files,read.table,header=TRUE) # read files");
  plot_fun.push_back("        ");
  plot_fun.push_back("        par(mfrow=c(3,1),mar=c(1,1,5,1),oma=c(5,5,2,2))");
  plot_fun.push_back("        names_ms<-vector(len=length(mstats_list))");
  plot_fun.push_back("        if(missing(tags_mstats)){");
  plot_fun.push_back("            for(i in 1:length(mstats_list)){");
  plot_fun.push_back("                names_ms[i]<-i");
  plot_fun.push_back("                }");
  plot_fun.push_back("            }");
  plot_fun.push_back("        else{");
  plot_fun.push_back("            names_ms<-tags_mstats");
  plot_fun.push_back("            }");
  plot_fun.push_back("        cols<-grey.colors(length(mstats_files))");
  plot_fun.push_back("");
  plot_fun.push_back("        ## segregating sites");
  plot_fun.push_back("        min_ss<-min(mstats_list[[1]]$'SS',na.rm=TRUE)");
  plot_fun.push_back("        max_ss<-max(mstats_list[[1]]$'SS',na.rm=TRUE)");
  plot_fun.push_back("        if( length(mstats_list) > 1 )");
  plot_fun.push_back("          for(i in 2:length(mstats_list)){");
  plot_fun.push_back("            min_ss<-min(min_ss,mstats_list[[i]]$'SS',na.rm=TRUE)");
  plot_fun.push_back("            max_ss<-max(max_ss,mstats_list[[i]]$'SS',na.rm=TRUE)");
  plot_fun.push_back("            }");
  plot_fun.push_back("        min_ss = min_ss * 0.9");
  plot_fun.push_back("        max_ss = max_ss * 1.1");
  plot_fun.push_back("        par(bty='o',las=1)");
  plot_fun.push_back("        boxplot(mstats_list[[1]]$'SS',xlim=c(0,length(mstats_list)+1),ylim=c(min_ss,max_ss),col=cols[1],ylab=\"\",xlab=\"\")");
  plot_fun.push_back("        par(bty='n')");
  plot_fun.push_back("        if( length(mstats_list) > 1 )");
  plot_fun.push_back("          for(i in 2:length(mstats_list))");
  plot_fun.push_back("            boxplot(mstats_list[[i]]$'SS',at=i,add=TRUE,xaxt='n',yaxt='n',col=cols[i],ylab=\"\",xlab=\"\")");
  plot_fun.push_back("        axis(1,1:length(mstats_list),names_ms,las=2)");
  plot_fun.push_back("        title(main=\"Segregating sites\",line=1)");
  plot_fun.push_back("        par(bty='o')");
  plot_fun.push_back("");
  plot_fun.push_back("        ## theta ");
  plot_fun.push_back("        min_theta<-min(mstats_list[[1]]$'THETA'/mstats_list[[1]]$'EFF_LENGTH2',na.rm=TRUE)");
  plot_fun.push_back("        max_theta<-max(mstats_list[[1]]$'THETA'/mstats_list[[1]]$'EFF_LENGTH2',na.rm=TRUE)");
  plot_fun.push_back("        if( length(mstats_list) > 1 )");
  plot_fun.push_back("          for(i in 2:length(mstats_list)){");
  plot_fun.push_back("            min_theta<-min(min_theta,mstats_list[[i]]$'THETA'/mstats_list[[i]]$'EFF_LENGTH2',na.rm=TRUE)");
  plot_fun.push_back("            max_theta<-max(max_theta,mstats_list[[i]]$'THETA'/mstats_list[[i]]$'EFF_LENGTH2',na.rm=TRUE)");
  plot_fun.push_back("            }");
  plot_fun.push_back("        min_theta = min_theta * 0.9");
  plot_fun.push_back("        max_theta = max_theta * 1.1");
  plot_fun.push_back("        par(bty='o',las=1)");
  plot_fun.push_back("        boxplot(mstats_list[[1]]$'THETA'/mstats_list[[1]]$'EFF_LENGTH2',xlim=c(0,length(mstats_list)+1),ylim=c(min_theta,max_theta),col=cols[1],ylab=\"\",xlab=\"\")");
  plot_fun.push_back("        par(bty='n')");
  plot_fun.push_back("        if( length(mstats_list) > 1 )");
  plot_fun.push_back("          for(i in 2:length(mstats_list))");
  plot_fun.push_back("            boxplot(mstats_list[[i]]$'THETA'/mstats_list[[i]]$'EFF_LENGTH2',at=i,add=TRUE,xaxt='n',yaxt='n',col=cols[i],ylab=\"\",xlab=\"\")");
  plot_fun.push_back("        axis(1,1:length(mstats_list),names_ms,las=2)");
  plot_fun.push_back("        title(main=\"Theta per site\",line=1)");
  plot_fun.push_back("        par(bty='o')");
  plot_fun.push_back("");
  plot_fun.push_back("        ## pi");
  plot_fun.push_back("        min_pi<-min(mstats_list[[1]]$'PI'/mstats_list[[1]]$'EFF_LENGTH2',na.rm=TRUE)");
  plot_fun.push_back("        max_pi<-max(mstats_list[[1]]$'PI'/mstats_list[[1]]$'EFF_LENGTH2',na.rm=TRUE)");
  plot_fun.push_back("        if( length(mstats_list) > 1 )");
  plot_fun.push_back("          for(i in 2:length(mstats_list)){");
  plot_fun.push_back("            min_pi<-min(min_pi,mstats_list[[i]]$'PI'/mstats_list[[i]]$'EFF_LENGTH2',na.rm=TRUE)");
  plot_fun.push_back("            max_pi<-max(max_pi,mstats_list[[i]]$'PI'/mstats_list[[i]]$'EFF_LENGTH2',na.rm=TRUE)");
  plot_fun.push_back("            }");
  plot_fun.push_back("        min_pi = min_pi * 0.9");
  plot_fun.push_back("        max_pi = max_pi * 1.1");
  plot_fun.push_back("        par(bty='o',las=1)");
  plot_fun.push_back("        boxplot(mstats_list[[1]]$'PI'/mstats_list[[1]]$'EFF_LENGTH2',xlim=c(0,length(mstats_list)+1),ylim=c(min_pi,max_pi),col=cols[1],ylab=\"\",xlab=\"\")");
  plot_fun.push_back("        par(bty='n')");
  plot_fun.push_back("        if( length(mstats_list) > 1 )");
  plot_fun.push_back("          for(i in 2:length(mstats_list))");
  plot_fun.push_back("            boxplot(mstats_list[[i]]$'PI'/mstats_list[[i]]$'EFF_LENGTH2',at=i,add=TRUE,xaxt='n',yaxt='n',col=cols[i],ylab=\"\",xlab=\"\")");
  plot_fun.push_back("        axis(1,1:length(mstats_list),names_ms,las=2)");
  plot_fun.push_back("        title(main=\"Pi per site\",line=1)");
  plot_fun.push_back("        par(bty='o')");
  plot_fun.push_back("");
  plot_fun.push_back("        ## Tajima's D");
  plot_fun.push_back("        min_TD<-min(mstats_list[[1]]$'TAJ_D',na.rm=TRUE)");
  plot_fun.push_back("        max_TD<-max(mstats_list[[1]]$'TAJ_D',na.rm=TRUE)");
  plot_fun.push_back("        if( length(mstats_list) > 1 )");
  plot_fun.push_back("          for(i in 2:length(mstats_list)){");
  plot_fun.push_back("            min_TD<-min(min_TD,mstats_list[[i]]$'TAJ_D',na.rm=TRUE)");
  plot_fun.push_back("            max_TD<-max(max_TD,mstats_list[[i]]$'TAJ_D',na.rm=TRUE)");
  plot_fun.push_back("            }");
  plot_fun.push_back("        par(bty='o',las=1)");
  plot_fun.push_back("        boxplot(mstats_list[[1]]$'TAJ_D',xlim=c(0,length(mstats_list)+1),ylim=c(min_TD,max_TD),col=cols[1],ylab=\"\",xlab=\"\")");
  plot_fun.push_back("        par(bty='n')");
  plot_fun.push_back("        if( length(mstats_list) > 1 )");
  plot_fun.push_back("          for(i in 2:length(mstats_list))");
  plot_fun.push_back("            boxplot(mstats_list[[i]]$'TAJ_D',at=i,add=TRUE,xaxt='n',yaxt='n',col=cols[i],ylab=\"\",xlab=\"\")");
  plot_fun.push_back("        axis(1,1:length(mstats_list),names_ms,las=2)");
  plot_fun.push_back("        title(main=\"Tajima's D\",line=1)");
  plot_fun.push_back("        par(bty='o')");
  plot_fun.push_back("");
  plot_fun.push_back("        ## Fu & Li's D");
  plot_fun.push_back("        min_FLD<-min(mstats_list[[1]]$'FULI_D',na.rm=TRUE)");
  plot_fun.push_back("        max_FLD<-max(mstats_list[[1]]$'FULI_D',na.rm=TRUE)");
  plot_fun.push_back("        if( length(mstats_list) > 1 )");
  plot_fun.push_back("          for(i in 2:length(mstats_list)){");
  plot_fun.push_back("            min_FLD<-min(min_FLD,mstats_list[[i]]$'FULI_D',na.rm=TRUE)");
  plot_fun.push_back("            max_FLD<-max(max_FLD,mstats_list[[i]]$'FULI_D',na.rm=TRUE)");
  plot_fun.push_back("        }");
  plot_fun.push_back("        par(bty='o',las=1)");
  plot_fun.push_back("        boxplot(mstats_list[[1]]$'FULI_D',xlim=c(0,length(mstats_list)+1),ylim=c(min_FLD,max_FLD),col=cols[1],ylab=\"\",xlab=\"\")");
  plot_fun.push_back("        par(bty='n')");
  plot_fun.push_back("        if( length(mstats_list) > 1 )");
  plot_fun.push_back("          for(i in 2:length(mstats_list))");
  plot_fun.push_back("            boxplot(mstats_list[[i]]$'FULI_D',at=i,add=TRUE,xaxt='n',yaxt='n',col=cols[i],ylab=\"\",xlab=\"\")");
  plot_fun.push_back("        axis(1,1:length(mstats_list),names_ms,las=2)");
  plot_fun.push_back("        title(main=\"Fu & Li's D\",line=1)");
  plot_fun.push_back("        par(bty='o')");
  plot_fun.push_back("");
  plot_fun.push_back("        ## Fay & Wu's H");
  plot_fun.push_back("        min_FWH<-min(mstats_list[[1]]$'FAYWU_H',na.rm=TRUE)");
  plot_fun.push_back("        max_FWH<-max(mstats_list[[1]]$'FAYWU_H',na.rm=TRUE)");
  plot_fun.push_back("        if( length(mstats_list) > 1 )");
  plot_fun.push_back("          for(i in 2:length(mstats_list)){");
  plot_fun.push_back("            min_FWH<-min(min_FWH,mstats_list[[i]]$'FAYWU_H',na.rm=TRUE)");
  plot_fun.push_back("            max_FWH<-max(max_FWH,mstats_list[[i]]$'FAYWU_H',na.rm=TRUE)");
  plot_fun.push_back("            }");
  plot_fun.push_back("        par(bty='o',las=1)");
  plot_fun.push_back("        boxplot(mstats_list[[1]]$'FAYWU_H',xlim=c(0,length(mstats_list)+1),ylim=c(min_FWH,max_FWH),col=cols[1],ylab=\"\",xlab=\"\")");
  plot_fun.push_back("        par(bty='n')");
  plot_fun.push_back("        if( length(mstats_list) > 1 )");
  plot_fun.push_back("          for(i in 2:length(mstats_list))");
  plot_fun.push_back("            boxplot(mstats_list[[i]]$'FAYWU_H',at=i,add=TRUE,xaxt='n',yaxt='n',col=cols[i],ylab=\"\",xlab=\"\")");
  plot_fun.push_back("        axis(1,1:length(mstats_list),names_ms,las=2)");
  plot_fun.push_back("        title(main=\"Fay & Wu's H\",line=1)");
  plot_fun.push_back("        par(bty='o')");
  plot_fun.push_back("        }");
  plot_fun.push_back("    dev.off()");
  plot_fun.push_back("    mlist");
  plot_fun.push_back("}");

}


std::string plot_results::work( paths path_bucket ) {
  if(  filename == "" )
    return "file to save plots missing";
  if(  files_sum.size() == 0 )
    return "summary files to plot missing";

  tempfile = filename + ".R";

  // check sum files' format
  int files_ninds, len;
  bool files_inds_ok = true;

  for( unsigned int i = 0; i < files_sum.size(); i++ ){
      std::ifstream inFile;
      inFile.open(files_sum.at(i).c_str());
      if( !inFile.is_open()){
          return "Unable to open for reading infile " + files_sum.at(i);
        }
      else{
          std::string line;
          bool file_done = false;
          while( getline( inFile, line ) ){
              if( strncmp( line.c_str() , "# length" , 6 ) == 0 ){
                  file_done = true;
                  int clen = get_token( line.c_str() , 3 );
                  int cind = get_token( line.c_str() , 6 );
                  if( i == 0 ){
                      files_ninds = cind;
                      len = clen;
                      inFile.close();
                      break;
                    }
                  else{
                      if( clen != len ){
                          return "Summary file " + files_sum.at(i) + " has different length";
                        }
                      if( files_ninds != cind ){
                          files_inds_ok = false;
                          inFile.close();
                          break;
                        }

                    }
                }

            }
          if( !file_done )
            return "Summary file " + files_sum.at(i) + " is not correctly formatted?" ;

        }
    }

  if ( !files_inds_ok && acc_type != 0 ){
      return "Summary files obtained with differing number of individuals, ACC plots not possible";
    }
  if ( files_ninds == 1 && acc_type != 0 ){
      return "Only 1 diploid individual simulated, ACC plots not possible";
    }

  // check mstats sum files if neeeded
  if( files_mstats.size() > 0 ){
      for( unsigned int i = 0; i < files_mstats.size(); i++){
          std::ifstream inFile;
          inFile.open(files_mstats.at(i).c_str());
          if( !inFile.is_open()){
              return "Unable to open for reading infile " + files_mstats.at(i);
            }
          else{
              std::string line;
              getline( inFile, line );
              inFile.close();
              // strncmp( line.c_str() , "# length" , 6 ) == 0
              if( strncmp( line.c_str() , "REP\tEFF_LENGTH2", 10) != 0){
                  return "mstats summary file " + files_mstats.at(i) + " is not correctly formatted?";
                }

            }
        }
    }


  std::vector < std::string > aplot = plot_fun;
  std::stringstream ss;
  ss << "plot_summary_pipeliner(summary_files=c(\"" << files_sum.at(0) << "\"";

  for ( unsigned int i = 1;  i < files_sum.size(); i++ ){
      ss << ",\"" << files_sum.at(i) << "\"";
    }
  ss << "),length=" << len << ",out=\"" << filename << "\"";

  if( names_sum.size() != 0 ){
      ss << ",tags_summary=c(\"" << names_sum.at(0) << "\"";
      for ( unsigned int i = 1;  i < names_sum.size(); i++ ){
          ss << ",\"" << names_sum.at(i) << "\"";
        }
      ss << ")";
    }

  switch( acc_type ){
    case 0:
      break;
    case 1:
      ss << ",homozygous_lower_limits=c(1,2,3), homozygous_upper_limits=c(1,2," << files_ninds * 2 << ")"
         << ",heterozygous_lower_limits=c(1,2,3),"
         << ", heterozygous_upper_limits=c(1,2," << files_ninds * 2 << ")";
      break;

    case 2:
      ss << ",homozygous_lower_limits=1:" << files_ninds * 2 << ", homozygous_upper_limits=1:" << files_ninds * 2
         << ",heterozygous_lower_limits=1:" << ( files_ninds * 2) -1
         << ", heterozygous_upper_limits=1:" << ( files_ninds * 2 ) -1 ;

      break;

    default:
      std::cout << "ACC type out of bounds (plot_results.h)" << std::endl;


    }

  // mstats
  if( files_mstats.size() > 0 ){
      ss << ",mstats_files=c(\"" << files_mstats.at(0) << "\"";
      for ( unsigned int i = 1;  i < files_mstats.size(); i++ ){
          ss << ",\"" << files_mstats.at(i) << "\"";
        }
      ss << ")";

      if( names_mstats.size() > 0 ){
          ss << ",tags_ms=c(\"" << names_mstats.at(0) << "\"";
          for ( unsigned int i = 1;  i < names_mstats.size(); i++ ){
              ss << ",\"" << names_mstats.at(i) << "\"";
            }
          ss << ")";
        }
    }


  ss << ")->x";

  aplot.push_back(ss.str());ss.str("");

  std::ofstream outputFile;
  outputFile.open(tempfile.c_str());
  if( !outputFile.is_open() ){
      return "Unable to write to file " + tempfile;
    }

  for( unsigned int i = 0; i < aplot.size(); i++ ){
      outputFile << aplot.at(i) << std::endl;
    }
  outputFile.close();
  // check that we can write to outfile.pdf
  std::ofstream pdfFile;
  pdfFile.open(filename.c_str());
  if( !pdfFile.is_open() ){
      return "Unable to write to file " + filename;
    }
  pdfFile.close();
  int r = remove(filename.c_str());
  if ( r != 0){
      return "Permissions problem with file " + filename;
    }

  ss << path_bucket.get_path_R() << " --vanilla --slave --file=" << tempfile.c_str();
  system(ss.str().c_str());
  std::ifstream ipdfFile;
  ipdfFile.open(filename.c_str());
  if( !ipdfFile.is_open() ){
      return "Problems with R function writing to " + filename;
    }
  ipdfFile.close();
  QString qfile = QString::fromStdString( filename.c_str() );
  qfile.prepend("file://");
  bool opened = QDesktopServices::openUrl(QUrl(qfile, QUrl::TolerantMode) );
  if(!opened)
    return "Unable to open pdf " + filename;
  return "";
}
