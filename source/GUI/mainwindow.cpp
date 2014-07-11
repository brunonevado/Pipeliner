//
//  pGUI
//
//  Copyright (c) 2013 Bruno Nevado. GNU license
//

#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  setWindowTitle("Pipeliner GUI");
  setupHelpMenu();
  QApplication::setStyle(QStyleFactory::create("Fusion"));
  man_index = 0;
  man_len = 11;
  version_info = "<p align=\"center\">Pipeliner GUI v 0.2.0 : 11072014";

}

MainWindow::~MainWindow()
{
  delete ui;
}


void MainWindow::about()
{

  QString atext = QString::fromStdString(version_info);

  atext.append(
        "<p align=\"center\"><a href=\"https://github.com/brunonevado/pipeliner\"><span style=\" text-decoration: underline; color:#0000ff;\">website</span></a></p><p><a href=\"https://github.com/brunonevado/pipeliner\"><span style=\" text-decoration: underline; color:#0000ff;\"></p>"
        "Contact: brunonevado@gmail.com</p>"
        "<p align=\"center\">This program uses <a href=\"http://qt-project.org/\"><span style=\" text-decoration: underline; color:#0000ff;\">Qt version 5.1.0</span></a></p><p><a href=\"http://qt-project.org/\"><span style=\" text-decoration: underline; color:#0000ff;\"></p>"
        );
  QMessageBox::about(this, tr("About Pipeliner GUI"), atext);

}

void MainWindow::setupHelpMenu()
{
  QMenu *helpMenu = new QMenu(tr("&Help"), this);
  menuBar()->addMenu(helpMenu);

  helpMenu->addAction(tr("&About PipelinerGUI"), this, SLOT(about()));
}


void MainWindow::on_pushButton_WriteBash_clicked()
{
  pipeline_output aReadyPipeline;

  // Update Paths
  std::vector < std::string > lpaths;
  for( int i = 0; i < ui->tableWidget_paths->rowCount(); i++ )
    lpaths.push_back(ui->tableWidget_paths->item(i,0)->text().toStdString().c_str());

  // check picard tools does not have a dash at end
  if( lpaths.at(7).at( lpaths.at(7).size() - 1 ) == '/'){
      lpaths.at(7).erase( lpaths.at(7).size() - 1 );
    }
  pointer_to_pipeline->daPaths.set_paths(lpaths);

  highlighter_bash->set_bash( pointer_to_pipeline->daPaths );

  // Update User-defined toobs if needed
  // here check that BigScreen is not selected - if it is, update smallScreens
  if(ui->stackedWidget_snpcall->currentIndex() == 2){
      ui->plainTextEdit_user_iSNP->clear();
      ui->plainTextEdit_user_iSNP->appendPlainText( ui->plainTextEdit_user_iSNPcall_bigScreen->toPlainText() );
    }
  if(ui->stackedWidget_snpcall->currentIndex() == 3){
      ui->plainTextEdit_user_mSNP->clear();
      ui->plainTextEdit_user_mSNP->appendPlainText( ui->plainTextEdit_user_mSNPcall_bigScreen->toPlainText() );
    }
  if(ui->stackedWidget_snpcall->currentIndex() == 4){
      ui->plainTextEdit_user_snp_sites->clear();
      ui->plainTextEdit_user_snp_sites->appendPlainText( ui->plainTextEdit_user_sites_bigScreen->toPlainText() );
    }

  if( pointer_to_pipeline->get_active_input() == 2 ){
      std::vector < std::string > user_in;
      QString user_input_contents = ui->plainTextEdit_user_input->toPlainText();
      QStringList in_lines = user_input_contents.split("\n");
      for ( int i = 0; i < in_lines.size(); i++ ){
          user_in.push_back( in_lines.at(i).toStdString().c_str()  );
        }
      pointer_to_pipeline->daInputUser.set_user_comds( user_in );
    }
  if( pointer_to_pipeline->get_active_ngs() == 1 ){
      std::vector < std::string > user_ngs;
      QString user_ngs_contents = ui->plainTextEdit_user_ngs->toPlainText();
      QStringList in_lines = user_ngs_contents.split("\n");
      for ( int i = 0; i < in_lines.size(); i++ ){
          user_ngs.push_back( in_lines.at(i).toStdString().c_str()  );
        }
      pointer_to_pipeline->daNgsUser.set_user_comds( user_ngs );
    }
  if( pointer_to_pipeline->get_active_aligner() == 1 ){
      std::vector < std::string > user_align;
      QString user_align_contents = ui->plainTextEdit_user_align->toPlainText();
      QStringList in_lines = user_align_contents.split("\n");
      for ( int i = 0; i < in_lines.size(); i++ ){
          user_align.push_back( in_lines.at(i).toStdString().c_str()  );
        }
      pointer_to_pipeline->daAlignUser.set_user_comds( user_align );
    }
  if( pointer_to_pipeline->get_active_snpcall() == 1 ){
      std::vector < std::string > vector_cmds;

      QString contents = ui->plainTextEdit_user_iSNP->toPlainText();
      QStringList in_lines = contents.split("\n");
      for ( int i = 0; i < in_lines.size(); i++ ){
          vector_cmds.push_back( in_lines.at(i).toStdString().c_str()  );
        }
      pointer_to_pipeline->daSnpcallUser.set_user_snp_iSNP( vector_cmds );
      vector_cmds.clear(); contents.clear(); in_lines.clear();

      contents = ui->plainTextEdit_user_mSNP->toPlainText();
      in_lines = contents.split("\n");
      for ( int i = 0; i < in_lines.size(); i++ ){
          vector_cmds.push_back( in_lines.at(i).toStdString().c_str()  );
        }
      pointer_to_pipeline->daSnpcallUser.set_user_snp_mSNP( vector_cmds );
      vector_cmds.clear(); contents.clear(); in_lines.clear();

      contents = ui->plainTextEdit_user_snp_sites->toPlainText();
      in_lines = contents.split("\n");
      for ( int i = 0; i < in_lines.size(); i++ ){
          vector_cmds.push_back( in_lines.at(i).toStdString().c_str()  );
        }
      pointer_to_pipeline->daSnpcallUser.set_user_snp_sites( vector_cmds );
    }

  aReadyPipeline =  pointer_to_pipeline->get_cmds();
  ui->plainTextEdit_bash->clear();
  for ( unsigned int i = 0; i < aReadyPipeline.cmds.size(); i++){
      QString x = aReadyPipeline.cmds.at(i).c_str();
      ui->plainTextEdit_bash->appendPlainText(x);

    }
  if( aReadyPipeline.errors.size() > 0){
      std::string errtext;
      for( unsigned int i = 0; i < aReadyPipeline.errors.size(); i++ ){
          errtext.append(aReadyPipeline.errors.at(i));
          errtext.append("\n");
        }

      QMessageBox msgBox;

      msgBox.setText("Errors in defined pipeline.");
      msgBox.setWindowTitle("Errors in defined pipeline.");
      msgBox.setIcon(QMessageBox::Critical);
      msgBox.setInformativeText(errtext.c_str());
      msgBox.setWindowModality(Qt::WindowModal);
      msgBox.exec();

    }
}

void MainWindow::on_lineEdit_bashHeader_textEdited(const QString &arg1)
{
  pointer_to_pipeline->set_bash_header( arg1.toStdString().c_str() );
}

void MainWindow::on_pushButton_clearBashHeader_clicked()
{
  pointer_to_pipeline->set_bash_header( "" );
  ui->lineEdit_bashHeader->setText("");
}

void MainWindow::setDefaults(){
  // set default values of widgets, call after setting pointer to pipeline
  // PATHS
  ui->tableWidget_paths->item(0,0)->setText(QString::fromStdString(pointer_to_pipeline->daPaths.get_path_ms().c_str() )      );;
  ui->tableWidget_paths->item(1,0)->setText(QString::fromStdString(pointer_to_pipeline->daPaths.get_path_art().c_str() )      );;
  ui->tableWidget_paths->item(2,0)->setText(QString::fromStdString(pointer_to_pipeline->daPaths.get_path_bwa().c_str() )      );;
  ui->tableWidget_paths->item(3,0)->setText(QString::fromStdString(pointer_to_pipeline->daPaths.get_path_samtools().c_str() )      );;
  ui->tableWidget_paths->item(4,0)->setText(QString::fromStdString(pointer_to_pipeline->daPaths.get_path_bcftools().c_str() )      );;
  ui->tableWidget_paths->item(5,0)->setText(QString::fromStdString(pointer_to_pipeline->daPaths.get_path_vcfutils().c_str() )      );;
  ui->tableWidget_paths->item(6,0)->setText(QString::fromStdString(pointer_to_pipeline->daPaths.get_path_java().c_str() )      );;
  ui->tableWidget_paths->item(7,0)->setText(QString::fromStdString(pointer_to_pipeline->daPaths.get_path_picard().c_str() )      );;
  ui->tableWidget_paths->item(8,0)->setText(QString::fromStdString(pointer_to_pipeline->daPaths.get_path_perl().c_str() )      );;
  ui->tableWidget_paths->item(9,0)->setText(QString::fromStdString(pointer_to_pipeline->daPaths.get_path_pipeliner().c_str() )      );;
  ui->tableWidget_paths->item(10,0)->setText(QString::fromStdString(pointer_to_pipeline->daPaths.get_path_bash().c_str() )      );;
  ui->tableWidget_paths->item(11,0)->setText(QString::fromStdString(pointer_to_pipeline->daPaths.get_path_mstats().c_str() )      );;
  ui->tableWidget_paths->item(12,0)->setText(QString::fromStdString(pointer_to_pipeline->daPaths.get_path_R().c_str() )      );;

  //run settings
  ui->lineEdit_bashHeader->setText( QString::fromStdString(pointer_to_pipeline->get_bash_header().c_str() ) );
  // add bash header
  ui->lineEdit_dirdata->setText( QString::fromStdString(pointer_to_pipeline->daRunSettings.get_dirdata().c_str() ) );
  ui->lineEdit_runfolder->setText( QString::fromStdString(pointer_to_pipeline->daRunSettings.get_runfolder().c_str() ) );
  ui->lineEdit_runprefix->setText( QString::fromStdString(pointer_to_pipeline->daRunSettings.get_runprefix().c_str() ) );

  ui->spinBox_nreps->setValue( pointer_to_pipeline->daRunSettings.get_numreps()  );
  ui->checkBox_dirty->setChecked( ( pointer_to_pipeline->daRunSettings.clean_up() ) ? false : true );

  // input
  switch ( pointer_to_pipeline->get_active_input() ){
    case 0:
      ui->radioButton_input_ms->setChecked(true);
      ui->stackedWidget_input_settings->setCurrentIndex(0);
      break;
    case 1:
      ui->radioButton_input_sfs->setChecked(true);
      ui->stackedWidget_input_settings->setCurrentIndex(1);
      break;
    case 2:
      ui->radioButton_input_user->setChecked(true);
      ui->stackedWidget_input_settings->setCurrentIndex(2);
      break;

    default:
      break;
    }
  // ms input
  ui->spinBox_ms_ninds->setValue( pointer_to_pipeline->daRunSettings.get_numinds() );
  ui->doubleSpinBox_ms_theta->setValue( pointer_to_pipeline->daInputMs.get_theta() );
  ui->doubleSpinBox_ms_rho->setValue( pointer_to_pipeline->daInputMs.get_rho() );
  ui->doubleSpinBox_ms_stime->setValue( pointer_to_pipeline->daInputMs.get_split_time() );
  ui->spinBox_ms_seqlen->setValue( pointer_to_pipeline->daInputMs.get_seqlen() );
  ui->lineEdit_ms_ancestralfile->setText( QString::fromStdString ( pointer_to_pipeline->daInputMs.get_ancestral_file()) );
  if( pointer_to_pipeline->daInputMs.get_rand_ancestral() ){
      ui->spinBox_ms_seqlen->setEnabled( true );
      ui->lineEdit_ms_ancestralfile->setEnabled( false );
      ui->pushButton_ms_findancestral->setEnabled( false );
      ui->radioButton_ms_userandom->setChecked( true );
    }else{
      ui->spinBox_ms_seqlen->setEnabled( false );
      ui->lineEdit_ms_ancestralfile->setEnabled( true );
      ui->radioButton_ms_usefasta->setChecked( true );
      ui->pushButton_ms_findancestral->setEnabled( true );

    }
  // sfs_code input
  ui->lineEdit_sfs_infile->setText( QString::fromStdString ( pointer_to_pipeline->daInputSfscode.get_infile() ) );
  ui->spinBox_sfs_npops->setValue( pointer_to_pipeline->daInputSfscode.get_numpops() );
  ui->spinBox_sfs_popsize->setValue( pointer_to_pipeline->daInputSfscode.get_numindsppop() );
  ui->spinBox_sfs_ninds->setValue( pointer_to_pipeline->daInputSfscode.get_numinds() );
  ui->spinBox_sfs_popsize->setValue( pointer_to_pipeline->daInputSfscode.get_numindsppop() );
  ui->spinBox_sfs_popRef->setValue( pointer_to_pipeline->daInputSfscode.get_popRef() );
  ui->spinBox_sfs_popstudy->setValue( pointer_to_pipeline->daInputSfscode.get_popStudy() );
  // User input
  ui->spinBox_user_ninds->setValue( pointer_to_pipeline->daRunSettings.get_numinds() );
  ui->spinBox_user_seqlen->setValue( pointer_to_pipeline->daInputUser.get_seqlen() );
  ui->lineEdit_user_ancestralfile->setText( QString::fromStdString(pointer_to_pipeline->daInputUser.get_ancestral_file() )  );
  if( pointer_to_pipeline->daInputUser.get_rand_ancestral() == true ) {
      ui->radioButton_user_userandom->setChecked( true );
      ui->spinBox_user_seqlen->setEnabled( true );
      ui->pushButton_user_findancestral->setEnabled( false );
      ui->lineEdit_user_ancestralfile->setEnabled( false );
    }
  else{
      ui->radioButton_user_usefastaancestral->setChecked( true );
      ui->spinBox_user_seqlen->setEnabled( false );
      ui->pushButton_user_findancestral->setEnabled( true );
      ui->lineEdit_user_ancestralfile->setEnabled( true );
    }
  if( pointer_to_pipeline->daInputUser.get_format_ms() ){
      ui->groupBox_input_user_msinoptions->setEnabled( true );
      ui->radioButton_in_user_ms->setChecked( true );
    }
  else{
      ui->groupBox_input_user_msinoptions->setEnabled( false );
      ui->radioButton_in_user_fas->setChecked( true );
    }

  // NGS simulation
  switch ( pointer_to_pipeline->get_active_ngs() ){
    case 0:
      ui->radioButton_ngs_art->setChecked(true);
      ui->stackedWidget_ngs->setCurrentIndex(0);
      break;
    case 1:
      ui->radioButton_ngs_user->setChecked(true);
      ui->stackedWidget_ngs->setCurrentIndex(1);
      break;
    default:
      break;
    }
  if( pointer_to_pipeline->daNgsArt.get_nends() == 1){
      ui->radioButton_art_se->setChecked( true );
    }
  else{
      ui->radioButton_art_pe->setChecked( true );
    }
  ui->doubleSpinBox_art_cov->setValue( pointer_to_pipeline->daNgsArt.get_coverage() );
  ui->spinBox_art_readlen->setValue( pointer_to_pipeline->daNgsArt.get_readlen() );
  ui->spinBox_art_fraglen->setValue( pointer_to_pipeline->daNgsArt.get_fraglen() );
  ui->spinBox_art_fragsd->setValue( pointer_to_pipeline->daNgsArt.get_fragsd() );

  // alignment
  switch ( pointer_to_pipeline->get_active_aligner() ){
    case 0:
      ui->radioButton_align_bwa->setChecked(true);
      ui->stackedWidget_alignment->setCurrentIndex(0);
      break;
    case 1:
      ui->radioButton_align_user->setChecked(true);
      ui->stackedWidget_alignment->setCurrentIndex(1);
      break;
    default:
      break;
    }
  if( pointer_to_pipeline->daAlignmentBwa.get_num_ends() == 1){
      ui->radioButton_bwa_se->setChecked( true );
    }
  else{
      ui->radioButton_bwa_pe->setChecked( true );
    }

  ui->spinBox_bwa_mapq->setValue( pointer_to_pipeline->daAlignmentBwa.get_mapq() );
  ui->doubleSpinBox_bwa_nmp->setValue( pointer_to_pipeline->daAlignmentBwa.get_nmp() );
  ui->spinBox_bwa_np->setValue( pointer_to_pipeline->daAlignmentBwa.get_nt() );
  ui->checkBox_bwa_rmdup->setChecked( pointer_to_pipeline->daAlignmentBwa.use_rmdup() );
  // user-defined alignment
  ui->checkBox_user_align_createDict->setChecked( pointer_to_pipeline->daAlignUser.get_creatDict() );

  // SNP call
  switch ( pointer_to_pipeline->get_active_snpcall() ){
    case 0:
      ui->radioButton_snp_sam->setChecked(true);
      ui->stackedWidget_snpcall->setCurrentIndex(0);
      break;
    case 1:
      ui->radioButton_snp_user->setChecked(true);
      ui->stackedWidget_snpcall->setCurrentIndex(1);
      break;
    default:
      break;
    }
  ui->checkBox_sam_iSNPcall->setChecked( pointer_to_pipeline->daSnpcallSamtools.get_iSNPcall() );
  ui->checkBox_sam_mSNPcall->setChecked( pointer_to_pipeline->daSnpcallSamtools.get_mSNPcall() );
  ui->checkBox_sam_disbaq->setChecked( pointer_to_pipeline->daSnpcallSamtools.get_baq() );
  ui->spinBox_sam_minbaseq->setValue( pointer_to_pipeline->daSnpcallSamtools.get_minbaseq() );
  ui->spinBox_sam_mindep->setValue( pointer_to_pipeline->daSnpcallSamtools.get_mincov() );
  ui->spinBox_sam_minrms->setValue( pointer_to_pipeline->daSnpcallSamtools.get_minrms() );
  ui->spinBox_sam_maxdep->setValue( pointer_to_pipeline->daSnpcallSamtools.get_maxcov() );
  ui->spinBox_sam_minmapq->setValue( pointer_to_pipeline->daSnpcallSamtools.get_mapq() );
  ui->checkBox_snpcall_user_iSNP->setChecked( pointer_to_pipeline->daSnpcallUser.get_iSNPcall() );
  ui->checkBox_snpcall_user_mSNP->setChecked( pointer_to_pipeline->daSnpcallUser.get_mSNPcall() );
  ui->lineEdit_user_snp_mSNPsep->setText( QString::fromStdString( pointer_to_pipeline->daSnpcallUser.get_bam_sep() ) );
  ui->spinBox_user_snp_sitescol->setValue( pointer_to_pipeline->daSnpcallUser.get_sites_col() );

  // SUMMARY
  ui->checkBox_stats_sum_iSNPcall->setChecked( pointer_to_pipeline->get_sum_iSNPcall() );
  ui->checkBox_stats_sum_mSNPcall->setChecked( pointer_to_pipeline->get_sum_mSNPcall() );
  ui->checkBox_stats_err_iSNPcall->setChecked( pointer_to_pipeline->get_inspect_iSNPcall() );
  ui->checkBox_stats_err_mSNPcall->setChecked( pointer_to_pipeline->get_inspect_mSNPcall() );
  if( !pointer_to_pipeline->get_sum_iSNPcall() ){
      pointer_to_pipeline->set_inspect_iSNPcall( false );
      ui->checkBox_stats_err_iSNPcall->setChecked( false );
      ui->checkBox_stats_err_iSNPcall->setEnabled( false );
    }
  if( !pointer_to_pipeline->get_sum_mSNPcall() ){
      pointer_to_pipeline->set_inspect_mSNPcall( false );
      ui->checkBox_stats_err_mSNPcall->setChecked( false );
      ui->checkBox_stats_err_mSNPcall->setEnabled( false );
    }
  ui->spinBox_stats_err_offsetQuals->setValue( pointer_to_pipeline->get_qual_offset() );
  ui->checkBox_stats_mstats_iSNP->setChecked( pointer_to_pipeline->get_mstats_iSNPcall() );
  ui->checkBox_stats_mstats_mSNP->setChecked( pointer_to_pipeline->get_mstats_mSNPcall() );
  ui->checkBox_stats_mstats_pre->setChecked( pointer_to_pipeline->get_mstats_preseq() );

  // Plots
  ui->tableWidget_viewer_sum_2->setCurrentCell(0,0);
  ui->tableWidget_viewer_sum->setCurrentCell(0,0);

  switch ( pointer_to_pipeline->Daplotter.get_acc_type() ){
    case 0:
      ui->radioButton_plots_acc_none->setChecked(true);
      break;
    case 1:
      ui->radioButton_plots_acc_sindoub->setChecked( true );
      break;
    case 2:
      ui->radioButton_plots_acc_full->setChecked( true );
      break;
    default:
      std::cout << "ACC mode out of bounds" << std::endl;

    }

  // highlighting text

  highlighter_user_in = new Highlighter( ui->plainTextEdit_user_input->document());
  std::vector < std::string > k_input = pointer_to_pipeline->daInputUser.get_keywords();
  for( unsigned int i = 0; i < k_input.size(); i++ ){
      highlighter_user_in->add_keyword(k_input.at(i));
    }
  highlighter_user_in->apply();

  highlighter_user_ngs = new Highlighter( ui->plainTextEdit_user_ngs->document());
  std::vector < std::string > k_ngs = pointer_to_pipeline->daNgsUser.get_keywords();
  for( unsigned int i = 0; i < k_ngs.size(); i++ ){
      highlighter_user_ngs->add_keyword(k_ngs.at(i));
    }
  highlighter_user_ngs->apply();

  highlighter_user_align = new Highlighter( ui->plainTextEdit_user_align->document());
  std::vector < std::string > k_align = pointer_to_pipeline->daAlignUser.get_keywords();
  for( unsigned int i = 0; i < k_align.size(); i++ ){
      highlighter_user_align->add_keyword(k_align.at(i));
    }
  highlighter_user_align->apply();

  highlighter_user_snp_iSNP = new Highlighter( ui->plainTextEdit_user_iSNP->document());
  std::vector < std::string > k_iSNP = pointer_to_pipeline->daSnpcallUser.get_keywords_ind();
  for( unsigned int i = 0; i < k_iSNP.size(); i++ ){
      highlighter_user_snp_iSNP->add_keyword(k_iSNP.at(i));
    }
  highlighter_user_snp_iSNP->apply();

  highlighter_user_snp_iSNP_bigScreen = new Highlighter( ui->plainTextEdit_user_iSNPcall_bigScreen->document());
  for( unsigned int i = 0; i < k_iSNP.size(); i++ ){
      highlighter_user_snp_iSNP_bigScreen->add_keyword(k_iSNP.at(i));
    }
  highlighter_user_snp_iSNP_bigScreen->apply();

  highlighter_user_snp_mSNP = new Highlighter( ui->plainTextEdit_user_mSNP->document());
  std::vector < std::string > k_mSNP = pointer_to_pipeline->daSnpcallUser.get_keywords_mult();
  for( unsigned int i = 0; i < k_mSNP.size(); i++ ){
      highlighter_user_snp_mSNP->add_keyword(k_mSNP.at(i));
    }
  highlighter_user_snp_mSNP->apply();

  highlighter_user_snp_mSNP_bigScreen = new Highlighter( ui->plainTextEdit_user_mSNPcall_bigScreen->document());
  for( unsigned int i = 0; i < k_mSNP.size(); i++ ){
      highlighter_user_snp_mSNP_bigScreen->add_keyword(k_mSNP.at(i));
    }
  highlighter_user_snp_mSNP_bigScreen->apply();

  highlighter_user_snp_sites = new Highlighter( ui->plainTextEdit_user_snp_sites->document());
  std::vector < std::string > k_snp_sites = pointer_to_pipeline->daSnpcallUser.get_keywords_sites();
  for( unsigned int i = 0; i < k_snp_sites.size(); i++ ){
      highlighter_user_snp_sites->add_keyword(k_snp_sites.at(i));
    }
  highlighter_user_snp_sites->apply();

  highlighter_user_snp_sites_bigScreen = new Highlighter( ui->plainTextEdit_user_sites_bigScreen->document());
  for( unsigned int i = 0; i < k_snp_sites.size(); i++ ){
      highlighter_user_snp_sites_bigScreen->add_keyword(k_snp_sites.at(i));
    }
  highlighter_user_snp_sites_bigScreen->apply();

  highlighter_bash = new Highlighter( ui->plainTextEdit_bash->document());
  highlighter_bash->set_bash( pointer_to_pipeline->daPaths );

  // fonts

  QFont f;
  f.setStyleHint(QFont::Courier);
  f.setPointSize(11);
  ui->spinBox_fontsize->setValue(f.pointSize());
  ui->fontComboBox->setFont(f);
  ui->plainTextEdit_bash->setFont(f);
  ui->plainTextEdit_user_align->setFont(f);
  ui->plainTextEdit_user_input->setFont(f);
  ui->plainTextEdit_user_iSNP->setFont(f);
  ui->plainTextEdit_user_mSNP->setFont(f);
  ui->plainTextEdit_user_ngs->setFont(f);
  ui->plainTextEdit_user_snp_sites->setFont(f);
  ui->plainTextEdit_user_iSNPcall_bigScreen->setFont(f);
  ui->plainTextEdit_user_mSNPcall_bigScreen->setFont(f);
  ui->plainTextEdit_user_sites_bigScreen->setFont(f);
  ui->textBrowser_help->setFont(f);

  // help
  ui->horizontalSlider_tutorial->setMaximum(man_len);
  ui->horizontalSlider_tutorial->setValue( man_index );
  ui->dockWidget_help->hide();
  tutorial_widgets.push_back( ui->tabWidget_main );
  tutorial_widgets.push_back( ui->tabWidget_pipeline );
  // default tabs
  ui->tabWidget_main->setCurrentIndex(0);
  ui->tabWidget_pipeline->setCurrentIndex(0);

  // icons
  iAlert = QIcon(":/icons/icons/alert.ico");
  iSaveFile = QIcon(":/icons/icons/save.ico");
  iBrowse = QIcon(":/icons/icons/ffile.ico");
  iOpenFolder = QIcon(":/icons/icons/ffolder.png");
  iInfo = QIcon(":/icons/icons/info.ico");
  iFullScreen =  QIcon(":/icons/icons/fullscreen.ico");
  iRun = QIcon(":/icons/icons/gnome_run.ico");
  iWrite = QIcon(":/icons/icons/write.ico");
  iTrash = QIcon(":/icons/icons/delete.png");
  iUp = QIcon(":/icons/icons/up.ico");
  iDown = QIcon(":/icons/icons/down.ico");
  iBack = QIcon(":/icons/icons/back.ico");
  iForw = QIcon(":/icons/icons/forward.ico");
  iPlot = QIcon(":/icons/icons/plots.png");
  iAddFile = QIcon(":/icons/icons/addFile.ico");
  iDelFile = QIcon(":/icons/icons/delFile.png");
  iOpenFile = QIcon(":/icons/icons/browse.ico");
  ui->pushButton_runLocally->setIcon(iRun);
  ui->pushButton_user_iSNPcall_bigScreen->setIcon(iFullScreen);
  ui->pushButton_user_mSNPcall_bigScreen->setIcon(iFullScreen);
  ui->pushButton_user_sites_bigScreen->setIcon(iFullScreen);
  ui->pushButton_user_iSNPcall_BigScreen_close->setIcon(iDown);
  ui->pushButton_user_mSNPcall_BigScreen_close->setIcon(iDown);
  ui->pushButton_user_sites_BigScreen_close->setIcon(iDown);
  ui->pushButtonSaveBash->setIcon(iSaveFile);
  ui->pushButton_clearBashHeader->setIcon(iTrash);
  ui->pushButton_dirdata->setIcon(iOpenFolder);
  ui->pushButton_help_art->setIcon(iInfo);
  ui->pushButton_help_bash->setIcon(iInfo);
  ui->pushButton_help_bcftools->setIcon(iInfo);
  ui->pushButton_help_bwa->setIcon(iInfo);
  ui->pushButton_help_java->setIcon(iInfo);
  ui->pushButton_help_ms->setIcon(iInfo);
  ui->pushButton_help_mstats->setIcon(iInfo);
  ui->pushButton_help_perl->setIcon(iInfo);
  ui->pushButton_help_picard->setIcon(iInfo);
  ui->pushButton_help_pipeliner->setIcon(iInfo);
  ui->pushButton_help_R->setIcon(iInfo);
  ui->pushButton_help_samtools->setIcon(iInfo);
  ui->pushButton_help_vcfutils->setIcon(iInfo);
  ui->pushButton_loadHeader->setIcon(iOpenFile);
  ui->pushButton_loadPaths->setIcon(iBrowse);
  ui->pushButton_load_user_align->setIcon(iOpenFile);
  ui->pushButton_load_user_input->setIcon(iOpenFile);
  ui->pushButton_load_user_input->setIcon(iOpenFile);
  ui->pushButton_load_user_iSNP->setIcon(iOpenFile);
  ui->pushButton_load_user_mSNP->setIcon(iOpenFile);
  ui->pushButton_load_user_ngs->setIcon(iOpenFile);
  ui->pushButton_save_user_align->setIcon(iSaveFile);
  ui->pushButton_save_user_input->setIcon(iSaveFile);
  ui->pushButton_load_user_snp_sites->setIcon(iOpenFile);
  ui->pushButton_save_user_input->setIcon(iSaveFile);
  ui->pushButton_save_user_iSNP->setIcon(iSaveFile);
  ui->pushButton_save_user_mSNP->setIcon(iSaveFile);
  ui->pushButton_save_user_ngs->setIcon(iSaveFile);
  ui->pushButton_ms_findancestral->setIcon(iOpenFile);
  ui->pushButton_plots_chooseFile->setIcon(iOpenFile);
  ui->pushButton_runfolder->setIcon(iOpenFolder);
  ui->pushButton_savePaths->setIcon(iSaveFile);
  ui->pushButton_sfs_infile->setIcon(iOpenFile);
  ui->pushButton_tutorial_back->setIcon(iBack);
  ui->pushButton_tutorial_forward->setIcon(iForw);
  ui->pushButton_tutorial_reset->setIcon(iTrash);
  ui->pushButton_user_findancestral->setIcon(iOpenFile);
  ui->pushButton_viewer_add_sum->setIcon(iAddFile);
  ui->pushButton_viewer_add_sum_2->setIcon(iAddFile);
  ui->pushButton_viewer_del_sum->setIcon(iDelFile);
  ui->pushButton_viewer_del_sum_2->setIcon(iDelFile);
  ui->pushButton_viewer_down_sum->setIcon(iDown);
  ui->pushButton_viewer_down_sum_2->setIcon(iDown);
  ui->pushButton_viewer_up_sum->setIcon(iUp);
  ui->pushButton_viewer_up_sum_2->setIcon(iUp);
  ui->pushButton_WriteBash->setIcon(iWrite);
  ui->pushButton_plots_work->setIcon(iPlot);
  ui->horizontalSlider_tutorial->setEnabled(false);
  ui->pushButton_help_uinput->setIcon(iInfo);
  ui->pushButton_help_ungs->setIcon(iInfo);
  ui->pushButton_help_ualign->setIcon(iInfo);
  ui->pushButton_help_uiSNPcall->setIcon(iInfo);
  ui->pushButton_help_umSNPcall->setIcon(iInfo);
  ui->pushButton_help_usites->setIcon(iInfo);
}

void MainWindow::on_pushButton_loadHeader_clicked()
{
  ui->plainTextEdit_bash->showMaximized();
  QString fileName = QFileDialog::getOpenFileName(this, tr("Choose header file"), QString());
  if (!fileName.isEmpty()) {
      pointer_to_pipeline->set_bash_header( fileName.toStdString().c_str() );
      ui->lineEdit_bashHeader->setText(fileName);
      if( !ui->lineEdit_bashHeader->hasAcceptableInput() ){
          ui->lineEdit_bashHeader->clear();
          ui->lineEdit_bashHeader->setPlaceholderText("Path to file should not contain empty spaces");
          pointer_to_pipeline->set_bash_header( "" );
        }
    }
}

void MainWindow::on_lineEdit_dirdata_textChanged(const QString &arg1)
{
  pointer_to_pipeline->daRunSettings.set_dirdata(arg1.toStdString().c_str());
}

void MainWindow::on_pushButton_dirdata_clicked()
{
  QString dirName =  QFileDialog::getExistingDirectory(this, tr("Choose directory to store results"), QString());
  if (!dirName.isEmpty()) {
      pointer_to_pipeline->daRunSettings.set_dirdata(dirName.toStdString().c_str() );
      ui->lineEdit_dirdata->setText(dirName);
      if( !ui->lineEdit_dirdata->hasAcceptableInput() ){
          ui->lineEdit_dirdata->clear();
          ui->lineEdit_dirdata->setPlaceholderText("Path to folder should not contain empty spaces");
        }
    }
}

void MainWindow::on_lineEdit_runfolder_textChanged(const QString &arg1)
{
  pointer_to_pipeline->daRunSettings.set_runfolder(arg1.toStdString().c_str());
}

void MainWindow::on_pushButton_runfolder_clicked()
{
  QString dirName =  QFileDialog::getExistingDirectory(this, tr("Choose directory to run analysis"), QString());
  if (!dirName.isEmpty()) {
      pointer_to_pipeline->daRunSettings.set_runfolder(dirName.toStdString().c_str() );
      ui->lineEdit_runfolder->setText(dirName);
      if( !ui->lineEdit_runfolder->hasAcceptableInput() ){
          ui->lineEdit_runfolder->clear();
          ui->lineEdit_runfolder->setPlaceholderText("Path to folder should not contain empty spaces");
        }
    }
}

void MainWindow::on_lineEdit_runprefix_textChanged(const QString &arg1)
{
  pointer_to_pipeline->daRunSettings.set_runprefix(arg1.toStdString().c_str());
}

void  MainWindow::on_spinBox_nreps_valueChanged(int arg1)
{
  pointer_to_pipeline->daRunSettings.set_numreps( arg1 );
}



void MainWindow::on_checkBox_dirty_stateChanged()
{
  bool dirty = ui->checkBox_dirty->isChecked();
  if(dirty)
    pointer_to_pipeline->daRunSettings.set_clean(false);
  else
    pointer_to_pipeline->daRunSettings.set_clean(true);
}

void MainWindow::on_actionPreview_Bash_triggered()
{
  ui->tabWidget_main->setCurrentWidget( ui->tab_writebash );
  ui->pushButton_WriteBash->click();
}

void MainWindow::on_radioButton_input_ms_clicked()
{
  pointer_to_pipeline->set_active_input(0);
  ui->stackedWidget_input_settings->setCurrentIndex(0);
  pointer_to_pipeline->daRunSettings.set_numinds( ui->spinBox_ms_ninds->value() );
}


void MainWindow::on_radioButton_input_sfs_clicked()
{
  pointer_to_pipeline->set_active_input(1);
  ui->stackedWidget_input_settings->setCurrentIndex(1);
  pointer_to_pipeline->daRunSettings.set_numinds( ui->spinBox_sfs_ninds->value() );
}

void MainWindow::on_radioButton_input_user_clicked()
{
  pointer_to_pipeline->set_active_input(2);
  ui->stackedWidget_input_settings->setCurrentIndex(2);
  pointer_to_pipeline->daRunSettings.set_numinds( ui->spinBox_user_ninds->value() );
}

void MainWindow::on_spinBox_ms_ninds_valueChanged(int arg1)
{
  pointer_to_pipeline->daRunSettings.set_numinds( arg1 );
  pointer_to_pipeline->daInputSfscode.set_numinds( arg1 );
}

void MainWindow::on_doubleSpinBox_ms_theta_valueChanged(double arg1)
{
  pointer_to_pipeline->daInputMs.set_theta( float ( arg1) );
}

void MainWindow::on_doubleSpinBox_ms_rho_valueChanged(double arg1)
{
  pointer_to_pipeline->daInputMs.set_rho( float( arg1 ) );
}

void MainWindow::on_doubleSpinBox_ms_stime_valueChanged(double arg1)
{
  pointer_to_pipeline->daInputMs.set_split_time( float( arg1 ) );
}

void MainWindow::on_radioButton_ms_userandom_clicked()
{
  pointer_to_pipeline->daInputMs.set_rand_ancestral( true );
  ui->spinBox_ms_seqlen->setEnabled( true );
  ui->lineEdit_ms_ancestralfile->setEnabled( false );
  ui->pushButton_ms_findancestral->setEnabled( false );
  ui->spinBox_ms_seqlen->setFocus();
}

void MainWindow::on_radioButton_ms_usefasta_clicked()
{
  pointer_to_pipeline->daInputMs.set_rand_ancestral( false );
  ui->spinBox_ms_seqlen->setEnabled( false );
  ui->lineEdit_ms_ancestralfile->setEnabled( true );
  ui->pushButton_ms_findancestral->setEnabled( true );
  ui->lineEdit_ms_ancestralfile->setFocus();
}

void MainWindow::on_spinBox_ms_seqlen_valueChanged(int arg1)
{
  pointer_to_pipeline->daInputMs.set_seqlen( arg1 );
}

void MainWindow::on_lineEdit_ms_ancestralfile_textChanged(const QString &arg1)
{
  pointer_to_pipeline->daInputMs.set_ancestral_file( arg1.toStdString().c_str());
}

void MainWindow::on_lineEdit_sfs_infile_textChanged(const QString &arg1)
{
  pointer_to_pipeline->daInputSfscode.set_infile( arg1.toStdString().c_str() );
}

void MainWindow::on_spinBox_sfs_npops_valueChanged(int arg1)
{
  pointer_to_pipeline->daInputSfscode.set_numpops( arg1 );
}

void MainWindow::on_spinBox_sfs_popsize_valueChanged(int arg1)
{
  pointer_to_pipeline->daInputSfscode.set_numindsppop( arg1 );
}

void MainWindow::on_spinBox_sfs_ninds_valueChanged(int arg1)
{
  pointer_to_pipeline->daInputSfscode.set_numinds( arg1 );
  pointer_to_pipeline->daRunSettings.set_numinds( arg1 );
}

void MainWindow::on_spinBox_user_ninds_valueChanged(int arg1)
{
  pointer_to_pipeline->daRunSettings.set_numinds( arg1 );
}


void MainWindow::on_spinBox_sfs_popstudy_valueChanged(int arg1)
{
  pointer_to_pipeline->daInputSfscode.set_popStudy( arg1 );
}

void MainWindow::on_spinBox_sfs_popRef_valueChanged(int arg1)
{
  pointer_to_pipeline->daInputSfscode.set_popRef( arg1 );
}

void MainWindow::on_radioButton_ngs_art_clicked()
{
  pointer_to_pipeline->set_active_ngs(0);
  ui->stackedWidget_ngs->setCurrentIndex(0);
}
void MainWindow::on_radioButton_ngs_user_clicked()
{
  pointer_to_pipeline->set_active_ngs(1);
  ui->stackedWidget_ngs->setCurrentIndex(1);
}

void MainWindow::on_radioButton_art_se_clicked()
{
  pointer_to_pipeline->daNgsArt.set_nends(1);
}
void MainWindow::on_radioButton_art_pe_clicked()
{
  pointer_to_pipeline->daNgsArt.set_nends(2);
}

void MainWindow::on_doubleSpinBox_art_cov_valueChanged(double arg1)
{
  pointer_to_pipeline->daNgsArt.set_coverage( float(arg1));
}

void MainWindow::on_spinBox_art_readlen_valueChanged(int arg1)
{
  pointer_to_pipeline->daNgsArt.set_readlen( arg1 );
}

void MainWindow::on_spinBox_art_fraglen_valueChanged(int arg1)
{
  pointer_to_pipeline->daNgsArt.set_fraglen( arg1 );
}

void MainWindow::on_spinBox_art_fragsd_valueChanged(int arg1)
{
  pointer_to_pipeline->daNgsArt.set_fragsd( arg1 );
}

void MainWindow::on_radioButton_bwa_se_clicked()
{
  pointer_to_pipeline->daAlignmentBwa.set_num_ends(1);
}

void MainWindow::on_radioButton_bwa_pe_clicked()
{
  pointer_to_pipeline->daAlignmentBwa.set_num_ends(2);
}

void MainWindow::on_spinBox_bwa_mapq_valueChanged(int arg1)
{
  pointer_to_pipeline->daAlignmentBwa.set_mapq( arg1 );
}

void MainWindow::on_doubleSpinBox_bwa_nmp_valueChanged(double arg1)
{
  pointer_to_pipeline->daAlignmentBwa.set_nmp( float (arg1) );
}

void MainWindow::on_spinBox_bwa_np_valueChanged(int arg1)
{
  pointer_to_pipeline->daAlignmentBwa.set_nt( arg1 );
}

void MainWindow::on_checkBox_bwa_rmdup_stateChanged()
{
  bool rm = ui->checkBox_bwa_rmdup->isChecked();
  if(rm)
    pointer_to_pipeline->daAlignmentBwa.set_rmdup( true );
  else
    pointer_to_pipeline->daAlignmentBwa.set_rmdup( false );
}


void MainWindow::on_radioButton_snp_sam_clicked()
{
  pointer_to_pipeline->set_active_snpcall( 0 );
  ui->stackedWidget_snpcall->setCurrentIndex( 0 );
}


void MainWindow::on_radioButton_snp_user_clicked()
{
  pointer_to_pipeline->set_active_snpcall( 1 );
  ui->stackedWidget_snpcall->setCurrentIndex( 1 );
}

void MainWindow::on_checkBox_sam_mSNPcall_stateChanged()
{
  pointer_to_pipeline->daSnpcallSamtools.set_mSNPcall( ui->checkBox_sam_mSNPcall->isChecked() );
}

void MainWindow::on_checkBox_sam_iSNPcall_stateChanged()
{
  pointer_to_pipeline->daSnpcallSamtools.set_iSNPcall( ui->checkBox_sam_iSNPcall->isChecked() );
}

void MainWindow::on_checkBox_sam_disbaq_stateChanged()
{
  pointer_to_pipeline->daSnpcallSamtools.set_baq( ui->checkBox_sam_disbaq->isChecked() );
}

void MainWindow::on_spinBox_sam_minbaseq_valueChanged(int arg1)
{
  pointer_to_pipeline->daSnpcallSamtools.set_minbaseq( arg1 );
}

void MainWindow::on_spinBox_sam_mindep_valueChanged(int arg1)
{
  pointer_to_pipeline->daSnpcallSamtools.set_mincov( arg1 );
}

void MainWindow::on_spinBox_sam_maxdep_valueChanged(int arg1)
{
  pointer_to_pipeline->daSnpcallSamtools.set_maxcov( arg1 );
}

void MainWindow::on_spinBox_sam_minrms_valueChanged(int arg1)
{
  pointer_to_pipeline->daSnpcallSamtools.set_minrms( arg1 );
}

void MainWindow::SavePaths() {
  std::vector < std::string > lpaths;
  for( int i = 0; i < ui->tableWidget_paths->rowCount(); i++ )
    lpaths.push_back(ui->tableWidget_paths->item(i,0)->text().toStdString().c_str());
  pointer_to_pipeline->daPaths.set_paths(lpaths);
  QString fileName = QFileDialog::getSaveFileName(this, "Choose file to save paths", "pipeliner.paths", "Path files (*.paths);;All files (*)");
  if (!fileName.isEmpty()) {
      QFile file(fileName);
      if (!file.open(QIODevice::WriteOnly)) {
          QMessageBox msgBox;
          msgBox.setText("Unable to write to selected file.");
          msgBox.setIcon(QMessageBox::Critical);
          msgBox.exec();
        } else {
          file.close();
          int written = pointer_to_pipeline->daPaths.write_paths( fileName.toStdString() );
          if( written != 0 ){
              QMessageBox msgBox;
              msgBox.setText("Unable to write to selected file.");
              msgBox.setIcon(QMessageBox::Critical);
              msgBox.exec();
            }
        }
    }
}

void MainWindow::LoadPaths(){
  QString fileName = QFileDialog::getOpenFileName(this, "Choose paths file to read", "pipeliner.paths", "Path files (*.paths);;All files (*)" );
  if (!fileName.isEmpty()) {
      QFile file(fileName);
      if (!file.open(QIODevice::ReadOnly)) {
          QMessageBox msgBox;
          msgBox.setText("Unable to read from selected file.");
          msgBox.setIcon(QMessageBox::Critical);
          msgBox.exec();
        } else {
          file.close();
          std::string msg = pointer_to_pipeline->daPaths.read_paths( fileName.toStdString() );
          if(msg != ""){
              QMessageBox msgBox;
              msgBox.setText(QString( msg.c_str()) );
              msgBox.setIcon(QMessageBox::Critical);
              msgBox.exec();
            }
          else{
              ui->tableWidget_paths->item(0,0)->setText(QString::fromStdString(pointer_to_pipeline->daPaths.get_path_ms().c_str() )      );;
              ui->tableWidget_paths->item(1,0)->setText(QString::fromStdString(pointer_to_pipeline->daPaths.get_path_art().c_str() )      );;
              ui->tableWidget_paths->item(2,0)->setText(QString::fromStdString(pointer_to_pipeline->daPaths.get_path_bwa().c_str() )      );;
              ui->tableWidget_paths->item(3,0)->setText(QString::fromStdString(pointer_to_pipeline->daPaths.get_path_samtools().c_str() )      );;
              ui->tableWidget_paths->item(4,0)->setText(QString::fromStdString(pointer_to_pipeline->daPaths.get_path_bcftools().c_str() )      );;
              ui->tableWidget_paths->item(5,0)->setText(QString::fromStdString(pointer_to_pipeline->daPaths.get_path_vcfutils().c_str() )      );;
              ui->tableWidget_paths->item(6,0)->setText(QString::fromStdString(pointer_to_pipeline->daPaths.get_path_java().c_str() )      );;
              ui->tableWidget_paths->item(7,0)->setText(QString::fromStdString(pointer_to_pipeline->daPaths.get_path_picard().c_str() )      );;
              ui->tableWidget_paths->item(8,0)->setText(QString::fromStdString(pointer_to_pipeline->daPaths.get_path_perl().c_str() )      );;
              ui->tableWidget_paths->item(9,0)->setText(QString::fromStdString(pointer_to_pipeline->daPaths.get_path_pipeliner().c_str() )      );;
              ui->tableWidget_paths->item(10,0)->setText(QString::fromStdString(pointer_to_pipeline->daPaths.get_path_bash().c_str() )      );;
              ui->tableWidget_paths->item(11,0)->setText(QString::fromStdString(pointer_to_pipeline->daPaths.get_path_mstats().c_str() )      );;
              ui->tableWidget_paths->item(12,0)->setText(QString::fromStdString(pointer_to_pipeline->daPaths.get_path_R().c_str() )      );;
            }
        }
      ui->tabWidget_main->setCurrentWidget( ui->tab_run_settings );
    }
}

void MainWindow::on_actionSave_Paths_triggered()
{
  MainWindow::SavePaths();
}

void MainWindow::on_actionLoad_paths_triggered()
{
  MainWindow::LoadPaths();
}

void MainWindow::on_pushButton_loadPaths_clicked()
{
  MainWindow::LoadPaths();
}

void MainWindow::on_pushButton_savePaths_clicked()
{
  MainWindow::SavePaths();
}

void MainWindow::on_radioButton_align_bwa_clicked()
{
  pointer_to_pipeline->set_active_aligner(0);
  ui->stackedWidget_alignment->setCurrentIndex(0);
}

void MainWindow::on_radioButton_align_user_clicked()
{
  pointer_to_pipeline->set_active_aligner(1);
  ui->stackedWidget_alignment->setCurrentIndex(1);
}

void MainWindow::on_pushButton_ms_findancestral_clicked()
{
  QString fileName = QFileDialog::getOpenFileName(this, tr("Choose ancestral file (fasta format)"), QString(), "Fasta files (*.fas *.fa);;All files (*)");
  if (!fileName.isEmpty()) {
      pointer_to_pipeline->daInputMs.set_ancestral_file( fileName.toStdString().c_str() );
      ui->lineEdit_ms_ancestralfile->setText(fileName);
    }
  if( !ui->lineEdit_ms_ancestralfile->hasAcceptableInput() ){
      ui->lineEdit_ms_ancestralfile->clear();
      ui->lineEdit_ms_ancestralfile->setPlaceholderText("No empty spaces allowed in path to file");
      pointer_to_pipeline->daInputMs.set_ancestral_file( "" );
    }
}

void MainWindow::on_pushButton_sfs_infile_clicked()
{
  QString fileName = QFileDialog::getOpenFileName(this, tr("Choose sfs_code infile"), QString());
  if (!fileName.isEmpty()) {
      pointer_to_pipeline->daInputSfscode.set_infile( fileName.toStdString().c_str() );
      ui->lineEdit_sfs_infile->setText(fileName);
    }
  if( !ui->lineEdit_sfs_infile->hasAcceptableInput() ){
      ui->lineEdit_sfs_infile->clear();
      ui->lineEdit_sfs_infile->setPlaceholderText("No empty spaces allowed in path to file");
      pointer_to_pipeline->daInputSfscode.set_infile( "" );
    }
}


QString MainWindow::SaveBash () {
  QString str = ui->plainTextEdit_bash->toPlainText();
  if( str.length() == 0 ){
      QMessageBox msgBox;
      msgBox.setText( "Bash text is empty, use 'Write Bash commands' before saving " );
      msgBox.setIcon(QMessageBox::Critical);
      msgBox.exec();
    }
  else{

      QString default_bash = QString::fromStdString(pointer_to_pipeline->daRunSettings.get_dirdata());
      default_bash.append("/run_");
      default_bash.append( QString::fromStdString(pointer_to_pipeline->daRunSettings.get_runprefix()) );
      default_bash.append(".sh");
      QString fileName = QFileDialog::getSaveFileName(this, tr("Save Bash file"), default_bash,
                                                      "Bash files (*.sh);;All files (*)");
      if (!fileName.isEmpty()) {
          QFile file(fileName);
          if (!file.open(QIODevice::WriteOnly)) {
              QMessageBox msgBox;
              msgBox.setText( "Unable to write to file" );
              msgBox.setIcon(QMessageBox::Critical);
              msgBox.exec();
            }
          else{
              QTextStream stream(&file);
              stream << ui->plainTextEdit_bash->toPlainText();
              stream.flush();
              file.close();
              return fileName;
            }
        }
    }
  return "err";
}


void MainWindow::on_pushButtonSaveBash_clicked()
{
  QString bfile = MainWindow::SaveBash();
}



void MainWindow::set_input_constrains(){
  QRegExp rx_nospaces("^\\S+$");
  QValidator *validator = new QRegExpValidator(rx_nospaces, this);
  ui->lineEdit_dirdata->setValidator(validator);
  ui->lineEdit_runfolder->setValidator(validator);
  ui->lineEdit_bashHeader->setValidator(validator);
  ui->lineEdit_ms_ancestralfile->setValidator(validator);
  ui->lineEdit_sfs_infile->setValidator(validator);
  ui->lineEdit_runprefix->setValidator(validator);
  ui->lineEdit_user_ancestralfile->setValidator(validator);
  ui->lineEdit_plot_saveTofile->setValidator(validator);
}

void MainWindow::on_pushButton_load_user_input_clicked()
{
  QString fileName = QFileDialog::getOpenFileName(this, tr("Choose user-defined input file"), QString(), "Toob files (*.toob);;All files (*)");
  if (!fileName.isEmpty()) {
      QFile file(fileName);
      if (!file.open(QIODevice::ReadOnly)) {
          QMessageBox msgBox;
          msgBox.setText("Unable to read from selected file.");
          msgBox.setIcon(QMessageBox::Critical);
          msgBox.exec();
        }
      else {
          ui->plainTextEdit_user_input->clear();
          QTextStream in(&file);
          ui->plainTextEdit_user_input->appendPlainText(in.readAll());
          file.close();
        }
    }
}

void MainWindow::on_pushButton_save_user_input_clicked()
{
  QString str = ui->plainTextEdit_user_input->toPlainText();
  if( str.length() == 0 ){
      QMessageBox msgBox;
      msgBox.setText( "User-defined input text is empty" );
      msgBox.setIcon(QMessageBox::Critical);
      msgBox.exec();
    }
  else{
      QString fileName = QFileDialog::getSaveFileName(this, tr("Save user-defined input file"), "input",
                                                      "Toob files (*.toob);;All files (*)");
      if (!fileName.isEmpty()) {
          QFile file(fileName);
          if (!file.open(QIODevice::WriteOnly)) {
              QMessageBox msgBox;
              msgBox.setText( "Unable to write to file" );
              msgBox.setIcon(QMessageBox::Critical);
              msgBox.exec();
            }
          else{
              QTextStream stream(&file);
              stream << ui->plainTextEdit_user_input->toPlainText();
              stream.flush();
              file.close();
            }
        }
    }
}

void MainWindow::on_radioButton_in_user_fas_clicked()
{
  pointer_to_pipeline->daInputUser.set_format_ms( false );
  ui->groupBox_input_user_msinoptions->setEnabled( false );
}

void MainWindow::on_radioButton_in_user_ms_clicked()
{
  pointer_to_pipeline->daInputUser.set_format_ms( true );
  ui->groupBox_input_user_msinoptions->setEnabled( true );
}

void MainWindow::on_radioButton_user_userandom_clicked()
{
  pointer_to_pipeline->daInputUser.set_rand_ancestral( true );
  ui->spinBox_user_seqlen->setEnabled( true );
  ui->lineEdit_user_ancestralfile->setEnabled( false );
  ui->pushButton_user_findancestral->setEnabled( false );
  ui->spinBox_user_seqlen->setFocus();
}

void MainWindow::on_radioButton_user_usefastaancestral_clicked()
{
  pointer_to_pipeline->daInputUser.set_rand_ancestral( false );
  ui->spinBox_user_seqlen->setEnabled( false );
  ui->lineEdit_user_ancestralfile->setEnabled( true );
  ui->pushButton_user_findancestral->setEnabled( true );
  ui->lineEdit_user_ancestralfile->setFocus();
}

void MainWindow::on_spinBox_user_seqlen_valueChanged(int arg1)
{
  pointer_to_pipeline->daInputUser.set_seqlen( arg1 );
}

void MainWindow::on_lineEdit_user_ancestralfile_textChanged(const QString &arg1)
{
  pointer_to_pipeline->daInputUser.set_ancestral_file( arg1.toStdString().c_str());
}

void MainWindow::on_pushButton_user_findancestral_clicked()
{
  QString fileName = QFileDialog::getOpenFileName(this, tr("Choose ancestral file (fasta format)"), QString(), "Fasta files (*.fas *.fa);;All files (*)" );
  if (!fileName.isEmpty()) {
      pointer_to_pipeline->daInputUser.set_ancestral_file( fileName.toStdString().c_str() );
      ui->lineEdit_user_ancestralfile->setText(fileName);
    }
  if( !ui->lineEdit_user_ancestralfile->hasAcceptableInput() ){
      ui->lineEdit_user_ancestralfile->clear();
      ui->lineEdit_user_ancestralfile->setPlaceholderText("No empty spaces allowed in path to file");
      pointer_to_pipeline->daInputUser.set_ancestral_file( "" );
    }
}

void MainWindow::on_pushButton_load_user_ngs_clicked()
{
  QString fileName = QFileDialog::getOpenFileName(this, tr("Choose user-defined ngs file"), QString(), "Toob files (*.toob);;All files (*)");
  if (!fileName.isEmpty()) {
      QFile file(fileName);
      if (!file.open(QIODevice::ReadOnly)) {
          QMessageBox msgBox;
          msgBox.setText("Unable to read from selected file.");
          msgBox.setIcon(QMessageBox::Critical);
          msgBox.exec();
        } else {
          ui->plainTextEdit_user_ngs->clear();
          QTextStream in(&file);
          ui->plainTextEdit_user_ngs->appendPlainText(in.readAll());
          file.close();
        }
    }
}

void MainWindow::on_pushButton_save_user_ngs_clicked()
{
  QString str = ui->plainTextEdit_user_ngs->toPlainText();
  if( str.length() == 0 ){
      QMessageBox msgBox;
      msgBox.setText( "User-defined ngs text is empty" );
      msgBox.setIcon(QMessageBox::Critical);
      msgBox.exec();
    }
  else{
      QString fileName = QFileDialog::getSaveFileName(this, tr("Save user-defined ngs file"), "ngs_sim",
                                                      "Toob files (*.toob);;All files (*)");
      if (!fileName.isEmpty()) {
          QFile file(fileName);
          if (!file.open(QIODevice::WriteOnly)) {
              QMessageBox msgBox;
              msgBox.setText( "Unable to write to file" );
              msgBox.setIcon(QMessageBox::Critical);
              msgBox.exec();
            }
          else{
              QTextStream stream(&file);
              stream << ui->plainTextEdit_user_ngs->toPlainText();
              stream.flush();
              file.close();
            }
        }
    }
}

void MainWindow::on_pushButton_load_user_align_clicked()
{
  QString fileName = QFileDialog::getOpenFileName(this, tr("Choose user-defined alignment file"), QString(), "Toob files (*.toob);;All files (*)");
  if (!fileName.isEmpty()) {
      QFile file(fileName);
      if (!file.open(QIODevice::ReadOnly)) {
          QMessageBox msgBox;
          msgBox.setText("Unable to read from selected file.");
          msgBox.setIcon(QMessageBox::Critical);
          msgBox.exec();
        } else {
          ui->plainTextEdit_user_align->clear();
          QTextStream in(&file);
          ui->plainTextEdit_user_align->appendPlainText(in.readAll());
          file.close();
        }
    }
}

void MainWindow::on_pushButton_save_user_align_clicked()
{
  QString str = ui->plainTextEdit_user_align->toPlainText();
  if( str.length() == 0 ){
      QMessageBox msgBox;
      msgBox.setText( "User-defined alignment text is empty" );
      msgBox.setIcon(QMessageBox::Critical);
      msgBox.exec();
    }
  else{
      QString fileName = QFileDialog::getSaveFileName(this, tr("Save user-defined alignment file"), "alignment",
                                                      "Toob files (*.toob);;All files (*)");
      if (!fileName.isEmpty()) {
          QFile file(fileName);
          if (!file.open(QIODevice::WriteOnly)) {
              QMessageBox msgBox;
              msgBox.setText( "Unable to write to file" );
              msgBox.setIcon(QMessageBox::Critical);
              msgBox.exec();
            }
          else{
              QTextStream stream(&file);
              stream << ui->plainTextEdit_user_align->toPlainText();
              stream.flush();
              file.close();
            }
        }
    }
}


void MainWindow::on_pushButton_load_user_iSNP_clicked()
{
  QString fileName = QFileDialog::getOpenFileName(this, tr("Choose user-defined iSNPcall file"), QString(), "Toob files (*.toob);;All files (*)");
  if (!fileName.isEmpty()) {
      QFile file(fileName);
      if (!file.open(QIODevice::ReadOnly)) {
          QMessageBox msgBox;
          msgBox.setText("Unable to read from selected file.");
          msgBox.setIcon(QMessageBox::Critical);
          msgBox.exec();
        } else {
          ui->plainTextEdit_user_iSNP->clear();
          QTextStream in(&file);
          ui->plainTextEdit_user_iSNP->appendPlainText(in.readAll());
          file.close();
        }
    }
}

void MainWindow::on_pushButton_save_user_iSNP_clicked()
{
  QString str = ui->plainTextEdit_user_iSNP->toPlainText();
  if( str.length() == 0 ){
      QMessageBox msgBox;
      msgBox.setText( "User-defined iSNPcall text is empty" );
      msgBox.setIcon(QMessageBox::Critical);
      msgBox.exec();
    }
  else{
      QString fileName = QFileDialog::getSaveFileName(this, tr(" user-defined iSNPcall file"), "iSNPcall",
                                                      "Toob files (*.toob);;All files (*)");
      if (!fileName.isEmpty()) {
          QFile file(fileName);
          if (!file.open(QIODevice::WriteOnly)) {
              QMessageBox msgBox;
              msgBox.setText( "Unable to write to file" );
              msgBox.setIcon(QMessageBox::Critical);
              msgBox.exec();
            }
          else{
              QTextStream stream(&file);
              stream << ui->plainTextEdit_user_iSNP->toPlainText();
              stream.flush();
              file.close();
            }
        }
    }
}


void MainWindow::on_pushButton_load_user_mSNP_clicked()
{
  QString fileName = QFileDialog::getOpenFileName(this, tr("Choose user-defined mSNPcall file"), QString(), "Toob files (*.toob);;All files (*)");
  if (!fileName.isEmpty()) {
      QFile file(fileName);
      if (!file.open(QIODevice::ReadOnly)) {
          QMessageBox msgBox;
          msgBox.setText("Unable to read from selected file.");
          msgBox.setIcon(QMessageBox::Critical);
          msgBox.exec();
        } else {
          ui->plainTextEdit_user_mSNP->clear();
          QTextStream in(&file);
          ui->plainTextEdit_user_mSNP->appendPlainText(in.readAll());
          file.close();
        }
    }
}

void MainWindow::on_pushButton_save_user_mSNP_clicked()
{
  QString str = ui->plainTextEdit_user_mSNP->toPlainText();
  if( str.length() == 0 ){
      QMessageBox msgBox;
      msgBox.setText( "User-defined mSNPcall text is empty" );
      msgBox.setIcon(QMessageBox::Critical);
      msgBox.exec();
    }
  else{
      QString fileName = QFileDialog::getSaveFileName(this, tr("Save user-defined mSNPcall file"), "mSNPcall",
                                                      "Toob files (*.toob);;All files (*)");
      if (!fileName.isEmpty()) {
          QFile file(fileName);
          if (!file.open(QIODevice::WriteOnly)) {
              QMessageBox msgBox;
              msgBox.setText( "Unable to write to file" );
              msgBox.setIcon(QMessageBox::Critical);
              msgBox.exec();
            }
          else{
              QTextStream stream(&file);
              stream << ui->plainTextEdit_user_mSNP->toPlainText();
              stream.flush();
              file.close();
            }
        }
    }
}

void MainWindow::on_pushButton_load_user_snp_sites_clicked()
{
  QString fileName = QFileDialog::getOpenFileName(this, tr("Choose user-defined sites file"), QString(), "Toob files (*.toob);;All files (*)");
  if (!fileName.isEmpty()) {
      QFile file(fileName);
      if (!file.open(QIODevice::ReadOnly)) {
          QMessageBox msgBox;
          msgBox.setText("Unable to read from selected file.");
          msgBox.setIcon(QMessageBox::Critical);
          msgBox.exec();
        } else {
          ui->plainTextEdit_user_snp_sites->clear();
          QTextStream in(&file);
          ui->plainTextEdit_user_snp_sites->appendPlainText(in.readAll());
          file.close();
        }
    }
}

void MainWindow::on_pushButton_save_user_snp_sites_clicked()
{
  QString str = ui->plainTextEdit_user_snp_sites->toPlainText();
  if( str.length() == 0 ){
      QMessageBox msgBox;
      msgBox.setText( "User-defined sites text is empty" );
      msgBox.setIcon(QMessageBox::Critical);
      msgBox.exec();
    }
  else{
      QString fileName = QFileDialog::getSaveFileName(this, tr("Save user-defined sites file"), "sites",
                                                      "Toob files (*.toob);;All files (*)");
      if (!fileName.isEmpty()) {
          QFile file(fileName);
          if (!file.open(QIODevice::WriteOnly)) {
              QMessageBox msgBox;
              msgBox.setText( "Unable to write to file" );
              msgBox.setIcon(QMessageBox::Critical);
              msgBox.exec();
            }
          else{
              QTextStream stream(&file);
              stream << ui->plainTextEdit_user_snp_sites->toPlainText();
              stream.flush();
              file.close();
            }
        }
    }
}

void MainWindow::on_checkBox_snpcall_user_iSNP_clicked(bool checked)
{
  pointer_to_pipeline->daSnpcallUser.set_iSNPcall( checked );
}

void MainWindow::on_checkBox_snpcall_user_mSNP_clicked(bool checked)
{
  pointer_to_pipeline->daSnpcallUser.set_mSNPcall( checked );
}

void MainWindow::on_lineEdit_user_snp_mSNPsep_textChanged(const QString &arg1)
{
  pointer_to_pipeline->daSnpcallUser.set_bam_sep(arg1.toStdString().c_str());
}

void MainWindow::on_spinBox_user_snp_sitescol_valueChanged(int arg1)
{
  pointer_to_pipeline->daSnpcallUser.set_sites_col( arg1 );
}

void MainWindow::on_checkBox_stats_sum_iSNPcall_clicked(bool checked)
{
  pointer_to_pipeline->set_sum_iSNPcall( checked );
  if( checked ){
      ui->checkBox_stats_err_iSNPcall->setEnabled( checked );
    }
  else{
      pointer_to_pipeline->set_inspect_iSNPcall( checked );
      ui->checkBox_stats_err_iSNPcall->setEnabled( checked );
    }
}

void MainWindow::on_checkBox_stats_sum_mSNPcall_clicked(bool checked)
{
  pointer_to_pipeline->set_sum_mSNPcall( checked );
  if( checked ){
      ui->checkBox_stats_err_mSNPcall->setEnabled( checked );
    }
  else{
      pointer_to_pipeline->set_inspect_mSNPcall( checked );
      ui->checkBox_stats_err_mSNPcall->setEnabled( checked );
    }
}

void MainWindow::on_checkBox_stats_err_iSNPcall_clicked(bool checked)
{
  pointer_to_pipeline->set_inspect_iSNPcall( checked );
}

void MainWindow::on_checkBox_stats_err_mSNPcall_clicked(bool checked)
{
  pointer_to_pipeline->set_inspect_mSNPcall( checked );
}

void MainWindow::on_spinBox_stats_err_offsetQuals_valueChanged(int arg1)
{
  pointer_to_pipeline->set_qual_offset( arg1 );
}

void MainWindow::on_checkBox_stats_mstats_iSNP_clicked(bool checked)
{
  pointer_to_pipeline->set_mstats_iSNPcall( checked );
}

void MainWindow::on_checkBox_stats_mstats_mSNP_clicked(bool checked)
{
  pointer_to_pipeline->set_mstats_mSNPcall( checked );
}

void MainWindow::on_checkBox_stats_mstats_pre_clicked(bool checked)
{
  pointer_to_pipeline->set_mstats_preseq( checked );
}

void MainWindow::on_pushButton_viewer_add_sum_clicked()
{
  QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Choose summary file(s) to add"), QString(), "Summary files (*.*SNPcall.txt);;All files (*)");
  if (!fileNames.isEmpty()) {
      for( int i = 0; i < ui->tableWidget_viewer_sum->rowCount(); i++ ){
          if( ui->tableWidget_viewer_sum->item(i,1)->text().isEmpty() ){
              ui->tableWidget_viewer_sum->item(i,1)->setText(fileNames.at(0));
              fileNames.pop_front();
              if( fileNames.size() == 0)
                break;
            }
        }
      if(fileNames.size() != 0 ){
          QMessageBox msgBox;
          msgBox.setText("Too many files to plot (maximum is 20 files).");
          msgBox.setIcon(QMessageBox::Critical);
          msgBox.exec();
        }
    }
  ui->tableWidget_viewer_sum->resizeColumnToContents(1);
}

void MainWindow::on_pushButton_viewer_del_sum_clicked()
{
  int x  = ui->tableWidget_viewer_sum->currentRow();
  if( !ui->tableWidget_viewer_sum->item(x,1)->text().isEmpty() ){
      ui->tableWidget_viewer_sum->item(x,1)->setText("");
    }
  for(int i = x; i < ui->tableWidget_viewer_sum->rowCount() -1 ; i ++){
      if(!ui->tableWidget_viewer_sum->item(i+1,1)->text().isEmpty()){
          ui->tableWidget_viewer_sum->item(i,1)->setText(  ui->tableWidget_viewer_sum->item(i+1,1)->text() );
          ui->tableWidget_viewer_sum->item(i+1,1)->setText("");
        }
    }
}

void MainWindow::on_pushButton_viewer_up_sum_clicked()
{
  int x  = ui->tableWidget_viewer_sum->currentRow();
  if ( x > 0 && !ui->tableWidget_viewer_sum->item(x,1)->text().isEmpty() ){
      QString swap = ui->tableWidget_viewer_sum->item(x-1,1)->text();
      ui->tableWidget_viewer_sum->item(x-1,1)->setText(  ui->tableWidget_viewer_sum->item(x,1)->text() );
      ui->tableWidget_viewer_sum->item(x,1)->setText(  swap );
      ui->tableWidget_viewer_sum->setCurrentCell(x-1, 1);
    }
}

void MainWindow::on_pushButton_viewer_down_sum_clicked()
{
  int x  = ui->tableWidget_viewer_sum->currentRow();
  if ( x <  ui->tableWidget_viewer_sum->rowCount() - 1  && !ui->tableWidget_viewer_sum->item(x,1)->text().isEmpty() ){
      QString swap = ui->tableWidget_viewer_sum->item(x+1,1)->text();
      ui->tableWidget_viewer_sum->item(x+1,1)->setText(  ui->tableWidget_viewer_sum->item(x,1)->text() );
      ui->tableWidget_viewer_sum->item(x,1)->setText(  swap );
      ui->tableWidget_viewer_sum->setCurrentCell(x+1, 1);
    }
}






void MainWindow::on_pushButton_viewer_add_sum_2_clicked()
{
  QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Choose mstats summary file(s) to add"), QString(), "mstasts sum files (*.*_mstats.txt);;All files (*)");
  if (!fileNames.isEmpty()) {
      for( int i = 0; i < ui->tableWidget_viewer_sum_2->rowCount(); i++ ){
          if( ui->tableWidget_viewer_sum_2->item(i,1)->text().isEmpty() ){
              ui->tableWidget_viewer_sum_2->item(i,1)->setText(fileNames.at(0));
              fileNames.pop_front();
              if( fileNames.size() == 0)
                break;
            }
        }
      if(fileNames.size() != 0 ){
          QMessageBox msgBox;
          msgBox.setText("Too many files to plot (maximum is 20 files).");
          msgBox.setIcon(QMessageBox::Critical);
          msgBox.exec();
        }
    }
  ui->tableWidget_viewer_sum_2->resizeColumnToContents(1);
}

void MainWindow::on_pushButton_viewer_del_sum_2_clicked()
{
  int x  = ui->tableWidget_viewer_sum_2->currentRow();
  if( !ui->tableWidget_viewer_sum_2->item(x,1)->text().isEmpty() ){
      ui->tableWidget_viewer_sum_2->item(x,1)->setText("");
    }
  for(int i = x; i < ui->tableWidget_viewer_sum_2->rowCount() -1 ; i ++){
      if(!ui->tableWidget_viewer_sum_2->item(i+1,1)->text().isEmpty()){
          ui->tableWidget_viewer_sum_2->item(i,1)->setText(  ui->tableWidget_viewer_sum_2->item(i+1,1)->text() );
          ui->tableWidget_viewer_sum_2->item(i+1,1)->setText("");
        }
    }
}

void MainWindow::on_pushButton_viewer_up_sum_2_clicked()
{
  int x  = ui->tableWidget_viewer_sum_2->currentRow();
  if ( x > 0 && !ui->tableWidget_viewer_sum_2->item(x,1)->text().isEmpty() ){
      QString swap = ui->tableWidget_viewer_sum_2->item(x-1,1)->text();
      ui->tableWidget_viewer_sum_2->item(x-1,1)->setText(  ui->tableWidget_viewer_sum_2->item(x,1)->text() );
      ui->tableWidget_viewer_sum_2->item(x,1)->setText(  swap );
      ui->tableWidget_viewer_sum_2->setCurrentCell(x-1, 1);
    }
}

void MainWindow::on_pushButton_viewer_down_sum_2_clicked()
{
  int x  = -1;
  x = ui->tableWidget_viewer_sum_2->currentRow();
  if ( x >= 0 && x <  (ui->tableWidget_viewer_sum_2->rowCount() - 1)  && !ui->tableWidget_viewer_sum_2->item(x,1)->text().isEmpty() ){
      QString swap = ui->tableWidget_viewer_sum_2->item(x+1,1)->text();
      ui->tableWidget_viewer_sum_2->item(x+1,1)->setText(  ui->tableWidget_viewer_sum_2->item(x,1)->text() );
      ui->tableWidget_viewer_sum_2->item(x,1)->setText(  swap );
      ui->tableWidget_viewer_sum_2->setCurrentCell(x+1, 1);
    }
}

void MainWindow::on_pushButton_plots_chooseFile_clicked()
{
  QString default_pdf = QString::fromStdString(pointer_to_pipeline->daRunSettings.get_dirdata());
  default_pdf.append("/result.pdf");
  QString fileName = QFileDialog::getSaveFileName(this, tr("Choose file to store results"),  default_pdf, "Pdf files (*.pdf);;" );
  if (!fileName.isEmpty()) {
      ui->lineEdit_plot_saveTofile->setText(fileName);
      pointer_to_pipeline->Daplotter.set_filename( fileName.toStdString().c_str() );
      if( !ui->lineEdit_plot_saveTofile->hasAcceptableInput() ){
          ui->lineEdit_plot_saveTofile->clear();
          ui->lineEdit_plot_saveTofile->setPlaceholderText("Path to file should not contain empty spaces");
          pointer_to_pipeline->Daplotter.set_filename( "" );
        }
    }
}

void MainWindow::on_pushButton_plots_work_clicked( )
{
  if( ui->lineEdit_plot_saveTofile->text() == "" )
    ui->pushButton_plots_chooseFile->click();

  std::vector < std::string > sum_files, sum_names;
  // get files
  for(  int i = 0; i < ui->tableWidget_viewer_sum->rowCount(); i++ ){
      if( !ui->tableWidget_viewer_sum->item(i,1)->text().isEmpty() )
        sum_files.push_back( ui->tableWidget_viewer_sum->item(i,1)->text().toStdString().c_str()  );
      if( !ui->tableWidget_viewer_sum->item(i,0)->text().isEmpty() )
        sum_names.push_back( ui->tableWidget_viewer_sum->item(i,0)->text().toStdString().c_str()  );
    }
  pointer_to_pipeline->Daplotter.set_sum_files( sum_files );

  if( sum_files.size() != sum_names.size() )
    pointer_to_pipeline->Daplotter.clear_sum_names();
  else
    pointer_to_pipeline->Daplotter.set_sum_names( sum_names );
  //get mstats files
  std::vector < std::string > ms_files, ms_names;
  for(  int i = 0; i < ui->tableWidget_viewer_sum_2->rowCount(); i++ ){
      if( !ui->tableWidget_viewer_sum_2->item(i,1)->text().isEmpty() )
        ms_files.push_back( ui->tableWidget_viewer_sum_2->item(i,1)->text().toStdString().c_str()  );
      if( !ui->tableWidget_viewer_sum_2->item(i,0)->text().isEmpty() )
        ms_names.push_back( ui->tableWidget_viewer_sum_2->item(i,0)->text().toStdString().c_str()  );
    }
  pointer_to_pipeline->Daplotter.set_mstats_files( ms_files );
  if( ms_files.size() != ms_names.size() )
    pointer_to_pipeline->Daplotter.clear_ms_names();
  else
    pointer_to_pipeline->Daplotter.set_ms_names( ms_names );
  pointer_to_pipeline->Daplotter.set_filename( ui->lineEdit_plot_saveTofile->text().toStdString() );
  std::string res = pointer_to_pipeline->Daplotter.work( pointer_to_pipeline->daPaths );
  if( res != "" ){
      QMessageBox msgBox;
      msgBox.setText("Errors plotting results");
      msgBox.setIcon(QMessageBox::Critical);
      msgBox.setInformativeText(res.c_str());
      msgBox.setWindowModality(Qt::WindowModal);
      msgBox.exec();
    }
}

void MainWindow::on_radioButton_plots_acc_none_clicked()
{
  pointer_to_pipeline->Daplotter.set_acc_type( 0 );
}

void MainWindow::on_radioButton_plots_acc_sindoub_clicked()
{
  pointer_to_pipeline->Daplotter.set_acc_type( 1 );
}

void MainWindow::on_radioButton_plots_acc_full_clicked()
{
  pointer_to_pipeline->Daplotter.set_acc_type( 2 );
}


void MainWindow::on_fontComboBox_currentFontChanged(const QFont &f)
{
  ui->plainTextEdit_bash->setFont(f);
  ui->plainTextEdit_user_align->setFont(f);
  ui->plainTextEdit_user_input->setFont(f);
  ui->plainTextEdit_user_iSNP->setFont(f);
  ui->plainTextEdit_user_mSNP->setFont(f);
  ui->plainTextEdit_user_ngs->setFont(f);
  ui->plainTextEdit_user_snp_sites->setFont(f);
  ui->plainTextEdit_user_iSNPcall_bigScreen->setFont(f);
  ui->plainTextEdit_user_mSNPcall_bigScreen->setFont(f);
  ui->plainTextEdit_user_sites_bigScreen->setFont(f);
}

void MainWindow::on_spinBox_fontsize_valueChanged(int arg1)
{
  QFont f = ui->fontComboBox->font();
  f.setPointSize(arg1);
  ui->plainTextEdit_bash->setFont(f);
  ui->plainTextEdit_user_align->setFont(f);
  ui->plainTextEdit_user_input->setFont(f);
  ui->plainTextEdit_user_iSNP->setFont(f);
  ui->plainTextEdit_user_mSNP->setFont(f);
  ui->plainTextEdit_user_ngs->setFont(f);
  ui->plainTextEdit_user_snp_sites->setFont(f);
  ui->plainTextEdit_user_iSNPcall_bigScreen->setFont(f);
  ui->plainTextEdit_user_mSNPcall_bigScreen->setFont(f);
  ui->plainTextEdit_user_sites_bigScreen->setFont(f);
}


void MainWindow::on_pushButton_help_ms_clicked()
{
  QMessageBox::information(this,"Executables",
                           tr(
                             "<p align=\"center\">ms is a coalescent simulator, used in input step with default options</p>"
                             "<p align=\"center\"><a href=\"http://home.uchicago.edu/rhudson1/source/mksamples.html\"><span style=\" text-decoration: underline; color:#0000ff;\">website</span></a></p>"
                             ));
}

void MainWindow::on_pushButton_help_art_clicked()
{
  QMessageBox::information(this,"Executables",
                           tr(
                             "<p align=\"center\">art_illumina is a NGS short read simulator, used in NGS simulation step with default options</p>"
                             "<p align=\"center\"><a href=\"http://www.niehs.nih.gov/research/resources/software/biostatistics/art\"><span style=\" text-decoration: underline; color:#0000ff;\">website</span></a></p>"
                             ));
}

void MainWindow::on_pushButton_help_bwa_clicked()
{
  QMessageBox::information(this,"Executables",
                           tr(
                             "<p align=\"center\">bwa is an aligner for NGS short reads, used in alignment step with default options</p>"
                             "<p align=\"center\"><a href=\"http://bio-bwa.sourceforge.net/\"><span style=\" text-decoration: underline; color:#0000ff;\">website</span></a></p>"
                             ));
}

void MainWindow::on_pushButton_help_samtools_clicked()
{
  QMessageBox::information(this,"Executables",
                           tr(
                             "<p align=\"center\">samtools is a package for SNP calling and filtering, used in SNP calling step with default options</p>"
                             "<p align=\"center\"><a href=\"http://samtools.sourceforge.net/\"><span style=\" text-decoration: underline; color:#0000ff;\">website</span></a></p>"
                             ));
}

void MainWindow::on_pushButton_help_bcftools_clicked()
{
  QMessageBox::information(this,"Executables",
                           tr(
                             "<p align=\"center\">bacftools is part of samtools package</p>"
                             "<p align=\"center\"><a href=\"http://samtools.sourceforge.net/\"><span style=\" text-decoration: underline; color:#0000ff;\">website</span></a></p>"
                             ));
}

void MainWindow::on_pushButton_help_vcfutils_clicked()
{
  QMessageBox::information(this,"Executables",
                           tr(
                             "<p align=\"center\">vcfutils.pl is part of samtools package</p>"
                             "<p align=\"center\"><a href=\"http://samtools.sourceforge.net\"><span style=\" text-decoration: underline; color:#0000ff;\">website</span></a></p>"
                             ));
}

void MainWindow::on_pushButton_help_java_clicked()
{
  QMessageBox::information(this,"Executables",
                           tr(
                             "<p align=\"center\">java is used with picard tools</p>"
                             "<p align=\"center\"><a href=\"http://www.java.com\"><span style=\" text-decoration: underline; color:#0000ff;\">website</span></a></p>"
                             ));
}

void MainWindow::on_pushButton_help_picard_clicked()
{
  QMessageBox::information(this,"Executables",
                           tr(
                             "<p align=\"center\"> Picard tools' package. Specifically, Pipeliner uses AddOrReplaceReadGroups.jar and CreateSequenceDictionary.jar</p>"
                             "<p align=\"center\"><a href=\"http://picard.sourceforge.net/command-line-overview.shtml\"><span style=\" text-decoration: underline; color:#0000ff;\">website</span></a></p>"
                             "<p align=\"center\"><i>Must contain full path to folder containing the .jar files!</p>"
                             ));
}

void MainWindow::on_pushButton_help_perl_clicked()
{
  QMessageBox::information(this,"Executables",
                           tr(
                             "<p align=\"center\">The Perl Programming Language</p>"
                             "<p align=\"center\"><a href=\"http://www.perl.org/\"><span style=\" text-decoration: underline; color:#0000ff;\">website</span></a></p>"
                             ));
}

void MainWindow::on_pushButton_help_pipeliner_clicked()
{
  QMessageBox::information(this,"Executables",
                           tr(
                             "<p align=\"center\">Pipeliner's command-line tool</p>"
                             "<p align=\"center\"><a href=\"https://github.com/brunonevado/pipeliner\"><span style=\" text-decoration: underline; color:#0000ff;\">website</span></a></p>"
                             ));
}

void MainWindow::on_pushButton_help_bash_clicked()
{
  QMessageBox::information(this,"Executables",
                           tr(
                             "<p align=\"center\">Bash interpreter</p>"
                             "<p align=\"center\"><a href=\"http://www.gnu.org/software/bash/manual/bashref.html\"><span style=\" text-decoration: underline; color:#0000ff;\">website</span></a></p>"
                             "<p align=\"center\"><i>Must contain full path to executable</p>"
                             ));
}

void MainWindow::on_pushButton_help_mstats_clicked()
{
  QMessageBox::information(this,"Executables",
                           tr(
                             "<p align=\"center\">mstatspop is used to estimate population genetics' statistics</p>"
                             "<p align=\"center\"><a href=\"http://bioinformatics.cragenomica.es/numgenomics/people/sebas/software/software.html\"><span style=\" text-decoration: underline; color:#0000ff;\">website</span></a></p>"
                             ));
}

void MainWindow::on_pushButton_help_R_clicked()
{
  QMessageBox::information(this,"Executables",
                           tr(
                             "<p align=\"center\">The R project. Used for plotting results</p>"
                             "<p align=\"center\"><a href=\"http://www.r-project.org/\"><span style=\" text-decoration: underline; color:#0000ff;\">website</span></a></p>"
                             ));
}


void MainWindow::on_actionHelp_triggered()
{
  if(ui->dockWidget_help->isHidden())
    ui->dockWidget_help->show();
  this->pipeliner_manual();
}


void MainWindow::pipeliner_manual ( ){
  for (unsigned int i = 0; i < tutorial_widgets.size(); i++)
    for( int j = 0; j < tutorial_widgets.at(i)->count(); j++ )
      tutorial_widgets.at(i)->setTabIcon(j, QIcon());
  switch (man_index) {
    case 0:
      ui->textBrowser_help->setText(
            "<p><b>Pipeliner: introdution</b>"
            "<p><b>Pipeliner</b> is is a tool to compare alternative experimental designs and bioinformatics’ pipelines for NGS re-sequencing studies."
            "<p>The analysis in <b>Pipeliner</b> includes: simulation of genetic data; simulation of NGS reads upon this genetic data; bioinformatic analysis of resulting NGS data; and comparison of original and final datasets, to evaluate the performance of the bioinformatic analysis."
            "<p><b>Pipeliner</b> consists of two separate executables: a Graphical User Interface (GUI) used for setting up analysis’ parameters and plotting results - this program - and a command-line tool used during the analysis to convert file formats, compute results, etc."
            "<p>In addition, <b>Pipeliner</b> uses several software packages for simulation of input data, NGS reads, and analysis of NGS data."
            );
      break;
    case 1:
      ui->tabWidget_main->setTabIcon(0,iAlert);
      ui->tabWidget_main->setTabIcon(1,iAlert);
      ui->tabWidget_main->setTabIcon(2,iAlert);
      ui->tabWidget_main->setTabIcon(3,iAlert);
      ui->textBrowser_help->setText(
            "<p><b>Pipeliner: introdution</b>"
            "<p>The <b>Pipeliner GUI</b> contains four main tabs:"
            "<p>The <b>Pipeline</b> tab is used to set up the analysis, i.e. defining the sampling design, study system, and bioinformatics’ analysis."
            "<p>The <b>Run settings</b> tab is used to define paths, number of replicates, and other run-time options."
            "<p>The <b>Bash file</b> allows previewing, editing and saving the pipeline’s commands."
            "<p>The <b>Plots</b> tab can be used for summarising and plotting the results of one or several pipelines."
            "<p>For the following example, we will be using only default options. Pipeliner's manual contains more information about alternative steps."
            );
      break;
    case 2:
      ui->tabWidget_main->setTabIcon(0,iAlert);
      ui->tabWidget_pipeline->setTabIcon(0,iAlert);
      ui->tabWidget_main->setCurrentIndex(0);
      ui->tabWidget_pipeline->setCurrentIndex(0);
      pointer_to_pipeline->set_active_input(0);
      ui->textBrowser_help->setText(
            "<p><b>Pipeliner: a short example</b>"
            "<p><b>Pipeline > Input</b>"
            "<p>In the input step, the user defines how to obtain the DNA sequences for analysis. These include two haplotypes per diploid individual and one haplotype to be used as reference genome. Three input modes are available to achieve this: ms coalescent simulation, sfs_code forward simulation, and User-defined commands."
            "<p>For this example, set the input type to <b>ms</b>, then number of <b>Individuals</b> to 5, <b>theta</b> and <b>rho</b> to 100, and <b>split time</b> to 0 (zero)"
            "<p>In the ancestral sequence box, choose <b>use random sequence</b> and set sequence length to 100 000"
            );
      break;
    case 3:
      ui->tabWidget_main->setTabIcon(0,iAlert);
      ui->tabWidget_pipeline->setTabIcon(1,iAlert);
      ui->tabWidget_main->setCurrentIndex(0);
      ui->tabWidget_pipeline->setCurrentIndex(1);
      pointer_to_pipeline->set_active_ngs(0);
      ui->textBrowser_help->setText(
            "<p><b>Pipeliner: a short example</b>"
            "<p><b>Pipeline > NGS</b>"
            "<p>Used to define how to obtain the NGS short reads for each diploid individual analysed. At the end of this step, there should be one (or two, if simulating paired-end sequencing) fastq files per individual. Two options currently available: default art_illumina and User-defined."
            "<p>For this example, set the NGS type to <b>art</b>, then set <b>Paired-ends</b> run, set <b>coverage</b> to 6x, and <b>Read length</b> to 75."
            );
      break;
    case 4:
      ui->tabWidget_main->setTabIcon(0,iAlert);
      ui->tabWidget_pipeline->setTabIcon(2,iAlert);
      ui->tabWidget_main->setCurrentIndex(0);
      ui->tabWidget_pipeline->setCurrentIndex(2);
      pointer_to_pipeline->set_active_aligner(0);
      ui->textBrowser_help->setText(
            "<p><b>Pipeliner: a short example</b>"
            "<p><b>Pipeline > Alignment</b>"
            "<p>Used to specify how to align the NGS short reads from each individual to the reference sequence. This step should start from the fastq files obtained before, and finish with a single aligned bam file for each individual. Two options available: default using bwa, or User-defined step."
            "<p>For this example, set the alignment type to <b>bwa</b>, input to <b>Paired-end</b>, <b>mapping quality - MAPQ</b> to 20 and <b>number of mismatches permitted - NMP</b> to 6."
            );
      break;
    case 5:
      ui->tabWidget_main->setTabIcon(0,iAlert);
      ui->tabWidget_pipeline->setTabIcon(3,iAlert);
      ui->tabWidget_main->setCurrentIndex(0);
      ui->tabWidget_pipeline->setCurrentIndex(3);
      pointer_to_pipeline->set_active_snpcall(0);
      ui->textBrowser_help->setText(
            "<p><b>Pipeliner: a short example</b>"
            "<p><b>Pipeline > SNP calling</b>"
            "<p>Used to define SNP calling and filtering options. This step starts from the aligned short-reads obtained before (.bam files) and finishes with a .vcf format file. SNP calling can be done separately for each individual (iSNPcall) as well as jointly for all individuals under analysis (multiple-sample SNP calling, mSNPcall). Two options available: default using samtools, or User-defined step."
            "<p>For this example, choose <b>samtools</b> type, select both <b>iSNPcall</b> and <b>mSNPcall</b> options and check the <b>disable BAQ</b> box. In the filtering section set <b>Minimum depth</b> to 3, <b>Maximum depth</b> to 100, <b>Minimum baseQ</b> of 13 and <b>Minimum RMS</b> of 10."
            );
      break;
    case 6:
      ui->tabWidget_main->setTabIcon(0,iAlert);
      ui->tabWidget_pipeline->setTabIcon(4,iAlert);
      ui->tabWidget_main->setCurrentIndex(0);
      ui->tabWidget_pipeline->setCurrentIndex(4);
      ui->textBrowser_help->setText(
            "<p><b>Pipeliner: a short example</b>"
            "<p><b>Pipeline > Statistics</b>"
            "<p>Used to define how to summarise the performance of the pipeline defined. Three main summary options are available: <b>Genotype calls</b>, <b>Inspect errors</b>, and <b>Population genomics</b>."
            "<p>For this example, select both <b>iSNPcall</b> and <b>mSNPcall</b> in the <b>Summarise genotype calls’ results</b> section, and all three boxes in the <b>Population genomics</b> section."
            );
      break;
    case 7:
      ui->tabWidget_main->setTabIcon(1,iAlert);
      ui->tabWidget_main->setCurrentIndex(1);
      ui->textBrowser_help->setText(
            "<p><b>Pipeliner: a short example</b>"
            "<p><b>Run settings</b>"
            "<p>This tab is used to specify several run-time options."
            "<p>For this example, select a <b>Data folder</b> to store the results, by either typing the folder's address or using the browse filesystem button."
            " Leave the <b>Run folder</b> empty, choose a <b>Prefix</b> for the run (e.g., test), set the <b>Number of replicates</b> to 10, and uncheck the <b>keep all files</b> box."
            "<p>In the Paths section, enter the paths to the different executables used in the analysis. Info buttons contain links to the corresponding websites. Once finished,"
            " use the <b>Save Paths</b> button to store this information in a file, which can be loaded in future runs of PipelinerGUI"
            );
      break;
    case 8:
      ui->tabWidget_main->setTabIcon(2,iAlert);
      ui->tabWidget_main->setCurrentIndex(2);
      ui->textBrowser_help->setText(
            "<p><b>Pipeliner: a short example</b>"
            "<p><b>Bash file</b>"
            "<p>This tab is used to preview the Bash commands file."
            "<p>Press the <b>Write Bash</b> button to preview the commands. If any settings are incorrectly set, there will be a warning. Otherwise, the bash file is ready to run."
            "<p>Press the <b>Save Bash and Run</b> button, and save the commands into a file, preferably in the Data folder defined before."
            "<p>The pipeline defined in the PipelinerGUI will now be executed."
            );
      break;
    case 9:
      ui->tabWidget_main->setTabIcon(2,iAlert);
      ui->tabWidget_main->setCurrentIndex(2);
      ui->textBrowser_help->setText(
            "<p><b>Pipeliner: a short example</b>"
            "<p><b>Running the analysis</b>"
            "<p>Wait until the run finishes, and then check the log of the run."
            "<p>In case the run encountered some errors, they should be present in the text, marked e.g. <i>ERROR</i>, <i>abort</i>, <i>WARNING</i>, followed by some information on what went wrong. This can be missing executables, wrong file formats, or any other of a myriad of problems."
            "<p>If no errors were encountered during the analysis, there should now be 5 files in the Data folder named <i>summary.runprefix.(...).txt</i>."
            );
      break;
    case 10:
      ui->tabWidget_main->setTabIcon(3,iAlert);
      ui->tabWidget_main->setCurrentIndex(3);

      ui->textBrowser_help->setText(
            "<p><b>Pipeliner: a short example</b>"
            "<p><b>Plots</b>"
            "<p>The <b>Plots</b> tab is used to summarise the results of the pipeline defined. In the <b>Plot Genotype call's results</b>, press the Add file button, navigate to the Data folder, and select the two appropriate summary files. In the name column, enter either iSNPcall or mSNPcall, according to which summary file is present on the file column. "
            "<p>In the <b>AAC classes</b>, choose the singletons + doubletons option."
            "<p>In the <b>Include population genomics' results</b> section add the summary files (...)preseq_mstats.txt, (...)iSNPcall_mstats.txt and (...)mSNPcall_mstats.txt, and label then accordingly in the name column."
            "<p>Now select the file to save the PDF results to (<b>PDF file to create</b>), and click Plot results. After a brief moment, the resulting pdf file should open automatically with the default PDF viewer of the computer."
            );
      break;
    case 11:
      ui->tabWidget_main->setTabIcon(3,iAlert);
      ui->tabWidget_main->setCurrentIndex(3);

      ui->textBrowser_help->setText(
            "<p><b>Pipeliner: a short example</b>"
            "<p>This short example is complete."
            "<p>It is now possible to go back and define a different pipeline, e.g. using a different number of individuals, average depth, or diffferent software for alignment and SNP calling."
            "<p>The results of different runs can then be loaded into the <b>Plots</b>, so that alternative pipelines can be compared in a straighforward way."
            );
      break;
    default:
      break;
    }
  ui->horizontalSlider_tutorial->setEnabled(false);
}

void MainWindow::on_pushButton_tutorial_back_clicked()
{
  if( man_index != 0)  {
      man_index--;
      ui->horizontalSlider_tutorial->setValue(man_index);
    }
  this->pipeliner_manual();
}

void MainWindow::on_pushButton_tutorial_forward_clicked()
{
  if( man_index != man_len )  {
      man_index++;
      ui->horizontalSlider_tutorial->setValue(man_index);
    }
  this->pipeliner_manual();
}

void MainWindow::on_pushButton_tutorial_reset_clicked()
{
  man_index = 0;
  ui->horizontalSlider_tutorial->setValue(man_index);
  this->pipeliner_manual();
}

void MainWindow::on_checkBox_user_align_createDict_clicked(bool checked)
{
  pointer_to_pipeline->daAlignUser.set_creatDict( checked );
}

void MainWindow::on_spinBox_sam_minmapq_valueChanged(int arg1)
{
  pointer_to_pipeline->daSnpcallSamtools.set_mapq( arg1 );
}

void MainWindow::on_pushButton_user_iSNPcall_bigScreen_clicked()
{
  ui->stackedWidget_snpcall->setCurrentIndex(2);
  ui->plainTextEdit_user_iSNPcall_bigScreen->clear();
  ui->plainTextEdit_user_iSNPcall_bigScreen->appendPlainText( ui->plainTextEdit_user_iSNP->toPlainText() );
}

void MainWindow::on_pushButton_user_iSNPcall_BigScreen_close_clicked()
{
  ui->stackedWidget_snpcall->setCurrentIndex(1);
  ui->plainTextEdit_user_iSNP->clear();
  ui->plainTextEdit_user_iSNP->appendPlainText( ui->plainTextEdit_user_iSNPcall_bigScreen->toPlainText() );
}

void MainWindow::on_pushButton_user_mSNPcall_bigScreen_clicked()
{
  ui->stackedWidget_snpcall->setCurrentIndex(3);
  ui->plainTextEdit_user_mSNPcall_bigScreen->clear();
  ui->plainTextEdit_user_mSNPcall_bigScreen->appendPlainText( ui->plainTextEdit_user_mSNP->toPlainText() );
}

void MainWindow::on_pushButton_user_mSNPcall_BigScreen_close_clicked()
{
  ui->stackedWidget_snpcall->setCurrentIndex(1);
  ui->plainTextEdit_user_mSNP->clear();
  ui->plainTextEdit_user_mSNP->appendPlainText( ui->plainTextEdit_user_mSNPcall_bigScreen->toPlainText() );
}

void MainWindow::on_pushButton_user_sites_bigScreen_clicked()
{
  ui->stackedWidget_snpcall->setCurrentIndex(4);
  ui->plainTextEdit_user_sites_bigScreen->clear();
  ui->plainTextEdit_user_sites_bigScreen->appendPlainText( ui->plainTextEdit_user_snp_sites->toPlainText() );
}

void MainWindow::on_pushButton_user_sites_BigScreen_close_clicked()
{
  ui->stackedWidget_snpcall->setCurrentIndex(1);
  ui->plainTextEdit_user_snp_sites->clear();
  ui->plainTextEdit_user_snp_sites->appendPlainText( ui->plainTextEdit_user_sites_bigScreen->toPlainText() );
}

void MainWindow::on_pushButton_runLocally_clicked()
{
  // check that data folder exists
  QString dirdata = QString::fromStdString(pointer_to_pipeline->daRunSettings.get_dirdata());
  if ( !QDir(dirdata).exists() ){

      QMessageBox msgBox;

      msgBox.setText("Error: data folder does not exist. Please create it before running the analysis.");
      msgBox.setIcon(QMessageBox::Critical);
      msgBox.setWindowModality(Qt::WindowModal);
      msgBox.exec();
    }
  else{
      QString bfile = MainWindow::SaveBash();
      if ( strcmp ( bfile.toStdString().c_str() , "err" ) != 0){
          QProcess runner;
          QStringList args;
          args << "runner" << QString::fromStdString(pointer_to_pipeline->daRunSettings.get_runprefix())
               << QString::fromStdString(pointer_to_pipeline->daRunSettings.get_dirdata())
               << bfile << QString::fromStdString(pointer_to_pipeline->daPaths.get_path_bash());
          runner.startDetached(QString::fromStdString(pointer_to_pipeline->get_name()),args);
        }
    }
}

void MainWindow::on_actionRun_triggered()
{
  ui->pushButton_runLocally->click();
}

void MainWindow::on_pushButton_help_uinput_clicked()
{
  QMessageBox* msgBox = new QMessageBox( this );;
  msgBox->setAttribute( Qt::WA_DeleteOnClose );
  msgBox->setText(ui->plainTextEdit_user_input->toolTip());
  msgBox->setWindowTitle("User-defined input step");
  msgBox->setWindowModality(Qt::NonModal);
  msgBox->show();
}

void MainWindow::on_pushButton_eg_uinput_clicked()
{
  ui->plainTextEdit_user_input->clear();
  ui->plainTextEdit_user_input->appendPlainText("# Example input step using ms. simulates 1 reference and 3 diploid individuals\n");
  ui->plainTextEdit_user_input->appendPlainText("ms 7 1 -t 40 -r 40 100000 > infile.ms\n");
  ui->plainTextEdit_user_input->appendPlainText("# Must specify input format: ms, and random sequence 100 Kb long !");
  ui->spinBox_user_ninds->setValue(3);
  ui->groupBox_input_user_msinoptions->setEnabled( true );
  ui->radioButton_in_user_ms->setChecked( true );
  ui->radioButton_user_userandom->setChecked( true );
  ui->spinBox_user_seqlen->setValue( 100000 );

}

void MainWindow::on_pushButton_help_ungs_clicked()
{
  QMessageBox* msgBox = new QMessageBox( this );;
  msgBox->setAttribute( Qt::WA_DeleteOnClose );
  msgBox->setText(ui->plainTextEdit_user_ngs->toolTip());
  msgBox->setWindowTitle("User-defined NGS simulation step");
  msgBox->setWindowModality(Qt::NonModal);
  msgBox->show();

}

void MainWindow::on_pushButton_eg_ungs_clicked()
{
  ui->plainTextEdit_user_ngs->clear();
  ui->plainTextEdit_user_ngs->appendPlainText("# Example NGS simulation step with art_illumina, Paired-Ends run, average cov 10x\n");
  ui->plainTextEdit_user_ngs->appendPlainText("art_illumina -i infile.fas -p -m 500 -s 10 -l 75 -f 5 -o tempfile\n");
  ui->plainTextEdit_user_ngs->appendPlainText("mv tempfile1.fq outfile.1.fq");
  ui->plainTextEdit_user_ngs->appendPlainText("mv tempfile2.fq outfile.2.fq");

}

void MainWindow::on_pushButton_help_ualign_clicked()
{
  QMessageBox* msgBox = new QMessageBox( this );;
  msgBox->setAttribute( Qt::WA_DeleteOnClose );
  msgBox->setText(ui->plainTextEdit_user_align->toolTip());
  msgBox->setWindowTitle("User-defined alignment step");
  msgBox->setWindowModality(Qt::NonModal);
  msgBox->show();

}

void MainWindow::on_pushButton_eg_ualign_clicked()
{
  ui->plainTextEdit_user_align->clear();
  ui->plainTextEdit_user_align->appendPlainText("# Example alignment step using bwa, for Paired-Ends run\n");
  ui->plainTextEdit_user_align->appendPlainText("# first index reference");
  ui->plainTextEdit_user_align->appendPlainText("bwa index -a is reference.fa\n");
  ui->plainTextEdit_user_align->appendPlainText("# align fastq reads to reference");
  ui->plainTextEdit_user_align->appendPlainText("bwa aln -n 6 reference.fa infile.1.fq > tempfile.1.sai");
  ui->plainTextEdit_user_align->appendPlainText("bwa aln -n 6 reference.fa infile.2.fq > tempfile.2.sai\n");
  ui->plainTextEdit_user_align->appendPlainText("# create bam file and sort it");
  ui->plainTextEdit_user_align->appendPlainText("bwa sampe reference.fa tempfile.1.sai tempfile.2.sai infile.1.fq infile.2.fq \\");
  ui->plainTextEdit_user_align->appendPlainText("  | samtools view -q 20 -Suh - | samtools sort -f - outfile.bam");
  ui->checkBox_user_align_createDict->setChecked( true );
}

void MainWindow::on_pushButton_help_uiSNPcall_clicked()
{
  QMessageBox* msgBox = new QMessageBox( this );;
  msgBox->setAttribute( Qt::WA_DeleteOnClose );
  msgBox->setText(ui->plainTextEdit_user_iSNP->toolTip());
  msgBox->setWindowTitle("User-defined iSNP calling step");
  msgBox->setWindowModality(Qt::NonModal);
  msgBox->show();

}

void MainWindow::on_pushButton_eg_uiSNPcall_clicked()
{
  ui->plainTextEdit_user_iSNP->clear();
  ui->plainTextEdit_user_iSNP->appendPlainText("# Example individual SNP calling step with samtools\n");
  ui->plainTextEdit_user_iSNP->appendPlainText("# index bam file");
  ui->plainTextEdit_user_iSNP->appendPlainText("samtools index infile.bam\n");
  ui->plainTextEdit_user_iSNP->appendPlainText("# Individual snp calling using samtools");
  ui->plainTextEdit_user_iSNP->appendPlainText("samtools mpileup -Q 20 -q 20 -uf reference.fa infile.bam \\");
  ui->plainTextEdit_user_iSNP->appendPlainText("  | bcftools view -vcg - | vcfutils.pl varFilter > outfile.ind.vcf");
  ui->checkBox_snpcall_user_iSNP->setChecked( true );
}

void MainWindow::on_pushButton_help_usites_clicked()
{
  QMessageBox* msgBox = new QMessageBox( this );;
  msgBox->setAttribute( Qt::WA_DeleteOnClose );
  msgBox->setText(ui->plainTextEdit_user_snp_sites->toolTip());
  msgBox->setWindowTitle("User-defined sites step");
  msgBox->setWindowModality(Qt::NonModal);
  msgBox->show();

}

void MainWindow::on_pushButton_eg_usites_clicked()
{
  ui->plainTextEdit_user_snp_sites->clear();
  ui->plainTextEdit_user_snp_sites->appendPlainText("# create confident sites list with samtools");
  ui->plainTextEdit_user_snp_sites->appendPlainText("# 1. list sites with depth between 5 and 20 (average simulated depth = 10) and baseQ and mapQ >= 20");
  ui->plainTextEdit_user_snp_sites->appendPlainText("samtools depth -q 20 -Q 20 infile.bam | grep -v \\# - | grep '[0-9]' \\");
  ui->plainTextEdit_user_snp_sites->appendPlainText("  | awk  '{ if( $3 >= 5 && $3 <= 20 ) print $2}' > tempfile.depth.sites\n");
  ui->plainTextEdit_user_snp_sites->appendPlainText("# 2. list invariable (homozygous-reference) sites");
  ui->plainTextEdit_user_snp_sites->appendPlainText("samtools mpileup -Q 20 -q 20 -uf reference.fa infile.bam \\");
  ui->plainTextEdit_user_snp_sites->appendPlainText("  | bcftools view -c - | awk  '{ if( $NF == 0) print $2}' > tempfile.invar.sites\n");
  ui->plainTextEdit_user_snp_sites->appendPlainText("# 3. list invariable positions with appropriate depth (i.e. intersect .depth.sites and .invar.sites)");
  ui->plainTextEdit_user_snp_sites->appendPlainText("awk 'NR==FNR{a[$0];next} $0 in a' tempfile.depth.sites tempfile.invar.sites > outfile.sites\n");
  ui->plainTextEdit_user_snp_sites->appendPlainText("# 4. add to sites list the variable positions passing vcfutils varFilter with same depth filters (5-20)");
  ui->plainTextEdit_user_snp_sites->appendPlainText("samtools mpileup -Q 20 -q 20 -uf reference.fa infile.bam | bcftools view -vcg - | vcfutils.pl varFilter -d 5 -D 20 - \\");
  ui->plainTextEdit_user_snp_sites->appendPlainText("  | cut -f 2 | grep -v \\# - |  grep '[0-9]' - >> outfile.sites");
  ui->spinBox_user_snp_sitescol->setValue( 1 );
}

void MainWindow::on_pushButton_help_umSNPcall_clicked()
{
  QMessageBox* msgBox = new QMessageBox( this );;
  msgBox->setAttribute( Qt::WA_DeleteOnClose );
  msgBox->setText(ui->plainTextEdit_user_mSNP->toolTip());
  msgBox->setWindowTitle("User-defined mSNP calling step");
  msgBox->setWindowModality(Qt::NonModal);
  msgBox->show();

}
