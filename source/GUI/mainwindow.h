//
//  pGUI
//
//  Copyright (c) 2013 Bruno Nevado. GNU license
//

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QRegExpValidator>
#include <QStringList>
#include <QList>
#include <QApplication>
#include <QStyleFactory>
#include <stdlib.h>

#include "ui_mainwindow.h"
#include "pipeline.h"
#include "highlighter.h"
#include "bashrunner.h"

namespace Ui {

  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);

  ~MainWindow();

  pipeline * pointer_to_pipeline;

  void setDefaults() ;

  void set_input_constrains ();

private slots:

  void on_pushButton_WriteBash_clicked();

  void on_lineEdit_bashHeader_textEdited(const QString &arg1);

  void on_pushButton_clearBashHeader_clicked();

  void on_pushButton_loadHeader_clicked();

  void on_lineEdit_dirdata_textChanged(const QString &arg1);

  void on_pushButton_dirdata_clicked();

  void on_lineEdit_runfolder_textChanged(const QString &arg1);

  void on_pushButton_runfolder_clicked();

  void on_lineEdit_runprefix_textChanged(const QString &arg1);

  void on_checkBox_dirty_stateChanged();

  void on_actionPreview_Bash_triggered();

  void on_radioButton_input_ms_clicked();

  void on_radioButton_input_sfs_clicked();

  void on_spinBox_ms_ninds_valueChanged(int arg1);

  void on_doubleSpinBox_ms_theta_valueChanged(double arg1);

  void on_doubleSpinBox_ms_rho_valueChanged(double arg1);

  void on_doubleSpinBox_ms_stime_valueChanged(double arg1);

  void on_radioButton_ms_userandom_clicked();

  void on_radioButton_ms_usefasta_clicked();

  void on_spinBox_ms_seqlen_valueChanged(int arg1);

  void on_lineEdit_ms_ancestralfile_textChanged(const QString &arg1);

  void on_lineEdit_sfs_infile_textChanged(const QString &arg1);

  void on_spinBox_sfs_npops_valueChanged(int arg1);

  void on_spinBox_sfs_popsize_valueChanged(int arg1);

  void on_spinBox_sfs_ninds_valueChanged(int arg1);

  void on_spinBox_sfs_popstudy_valueChanged(int arg1);

  void on_spinBox_sfs_popRef_valueChanged(int arg1);

  void on_radioButton_ngs_art_clicked();

  void on_radioButton_art_se_clicked();

  void on_radioButton_ngs_user_clicked();

  void on_doubleSpinBox_art_cov_valueChanged(double arg1);

  void on_spinBox_art_readlen_valueChanged(int arg1);

  void on_spinBox_art_fraglen_valueChanged(int arg1);

  void on_spinBox_art_fragsd_valueChanged(int arg1);

  void on_radioButton_art_pe_clicked();

  void on_radioButton_bwa_se_clicked();

  void on_radioButton_bwa_pe_clicked();

  void on_spinBox_bwa_mapq_valueChanged(int arg1);

  void on_doubleSpinBox_bwa_nmp_valueChanged(double arg1);

  void on_spinBox_bwa_np_valueChanged(int arg1);

  void on_checkBox_bwa_rmdup_stateChanged();

  void on_radioButton_snp_sam_clicked();

  void on_radioButton_snp_user_clicked();

  void on_checkBox_sam_mSNPcall_stateChanged();

  void on_checkBox_sam_iSNPcall_stateChanged();

  void on_checkBox_sam_disbaq_stateChanged();

  void on_spinBox_sam_minbaseq_valueChanged(int arg1);

  void on_spinBox_sam_mindep_valueChanged(int arg1);

  void on_spinBox_sam_maxdep_valueChanged(int arg1);

  void on_spinBox_sam_minrms_valueChanged(int arg1);

  void on_actionSave_Paths_triggered();

  void on_actionLoad_paths_triggered();

  void SavePaths ();

  void LoadPaths ();

  QString SaveBash ();

  void on_pushButton_loadPaths_clicked();

  void on_pushButton_savePaths_clicked();

  void on_spinBox_nreps_valueChanged(int arg1);

  void on_radioButton_input_user_clicked();

  void on_radioButton_align_bwa_clicked();

  void on_radioButton_align_user_clicked();

  void on_pushButton_ms_findancestral_clicked();

  void on_pushButton_sfs_infile_clicked();

  void on_pushButtonSaveBash_clicked();

  void on_actionSave_Bash_file_triggered();

  void on_spinBox_user_ninds_valueChanged(int arg1);

  void on_pushButton_load_user_input_clicked();

  void on_pushButton_save_user_input_clicked();

  void on_radioButton_in_user_fas_clicked();

  void on_radioButton_in_user_ms_clicked();

  void on_radioButton_user_userandom_clicked();

  void on_radioButton_user_usefastaancestral_clicked();

  void on_spinBox_user_seqlen_valueChanged(int arg1);

  void on_lineEdit_user_ancestralfile_textChanged(const QString &arg1);

  void on_pushButton_user_findancestral_clicked();

  void on_pushButton_load_user_ngs_clicked();

  void on_pushButton_save_user_ngs_clicked();

  void on_pushButton_load_user_align_clicked();

  void on_pushButton_save_user_align_clicked();

  void on_pushButton_load_user_iSNP_clicked();

  void on_pushButton_save_user_iSNP_clicked();

  void on_pushButton_load_user_mSNP_clicked();

  void on_pushButton_save_user_mSNP_clicked();

  void on_pushButton_load_user_snp_sites_clicked();

  void on_pushButton_save_user_snp_sites_clicked();

  void on_checkBox_snpcall_user_iSNP_clicked(bool checked);

  void on_checkBox_snpcall_user_mSNP_clicked(bool checked);

  void on_lineEdit_user_snp_mSNPsep_textChanged(const QString &arg1);

  void on_spinBox_user_snp_sitescol_valueChanged(int arg1);

  void on_checkBox_stats_sum_iSNPcall_clicked(bool checked);

  void on_checkBox_stats_sum_mSNPcall_clicked(bool checked);

  void on_checkBox_stats_err_iSNPcall_clicked(bool checked);

  void on_checkBox_stats_err_mSNPcall_clicked(bool checked);

  void on_spinBox_stats_err_offsetQuals_valueChanged(int arg1);

  void on_checkBox_stats_mstats_iSNP_clicked(bool checked);

  void on_checkBox_stats_mstats_mSNP_clicked(bool checked);

  void on_checkBox_stats_mstats_pre_clicked(bool checked);

  void on_pushButton_viewer_add_sum_clicked();

  void on_pushButton_viewer_del_sum_clicked();

  void on_pushButton_viewer_up_sum_clicked();

  void on_pushButton_viewer_down_sum_clicked();

  void on_pushButton_viewer_add_sum_2_clicked();

  void on_pushButton_viewer_del_sum_2_clicked();

  void on_pushButton_viewer_up_sum_2_clicked();

  void on_pushButton_viewer_down_sum_2_clicked();

  void on_pushButton_plots_chooseFile_clicked();

  void on_pushButton_plots_work_clicked();

  void on_radioButton_plots_acc_none_clicked();

  void on_radioButton_plots_acc_sindoub_clicked();

  void on_radioButton_plots_acc_full_clicked();

  void on_fontComboBox_currentFontChanged(const QFont &f);

  void on_spinBox_fontsize_valueChanged(int arg1);

  void on_pushButton_help_ms_clicked();

  void on_pushButton_help_art_clicked();

  void on_pushButton_help_bwa_clicked();

  void on_pushButton_help_samtools_clicked();

  void on_pushButton_help_bcftools_clicked();

  void on_pushButton_help_vcfutils_clicked();

  void on_pushButton_help_java_clicked();

  void on_pushButton_help_picard_clicked();

  void on_pushButton_help_perl_clicked();

  void on_pushButton_help_pipeliner_clicked();

  void on_pushButton_help_bash_clicked();

  void on_pushButton_help_mstats_clicked();

  void on_pushButton_help_R_clicked();

  void on_actionHelp_triggered();

  void on_pushButton_tutorial_back_clicked();

  void on_pushButton_tutorial_forward_clicked();

  void on_pushButton_tutorial_reset_clicked();

  void on_checkBox_user_align_createDict_clicked(bool checked);

  void on_spinBox_sam_minmapq_valueChanged(int arg1);

  void on_pushButton_user_iSNPcall_bigScreen_clicked();

  void on_pushButton_user_iSNPcall_BigScreen_close_clicked();

  void on_pushButton_user_mSNPcall_bigScreen_clicked();

  void on_pushButton_user_mSNPcall_BigScreen_close_clicked();

  void on_pushButton_user_sites_bigScreen_clicked();

  void on_pushButton_user_sites_BigScreen_close_clicked();

  void on_pushButton_runLocally_clicked();

  void on_actionRun_triggered();

private:
  Ui::MainWindow *ui;

  Highlighter *highlighter_user_in,
  *highlighter_user_ngs,
  *highlighter_user_align,
  *highlighter_user_snp_iSNP,
  *highlighter_user_snp_iSNP_bigScreen,
  *highlighter_user_snp_mSNP_bigScreen,
  *highlighter_user_snp_sites_bigScreen,
  *highlighter_user_snp_sites,
  *highlighter_user_snp_mSNP,
  *highlighter_bash;

  std::string version_info;

  void setupHelpMenu();

  int man_index, man_len;

  void pipeliner_manual( );

  QIcon iAlert, iSaveFile, iOpenFile, iOpenFolder, iFullScreen, iRun,
  iInfo, iWrite, iTrash, iUp, iDown, iBack, iForw, iPlot, iAddFile, iDelFile, iBrowse;

  std::vector < QTabWidget * > tutorial_widgets;


public slots:

  void about();

};

#endif // MAINWINDOW_H
