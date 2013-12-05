//
//  pGUI
//
//  Copyright (c) 2013 Bruno Nevado. GNU license
//


#include "bashrunner.h"
#include "ui_bashrunner.h"

BashRunner::BashRunner(QWidget *parent, QString title, QString folder, QString run_file, QString bash_bin ) :
  QMainWindow(parent),
  ui(new Ui::BashRunner)
{
  ui->setupUi(this);
  text_edit = ui->textEdit;
  setWindowTitle("Pipeliner, running: "+ title);
  dirdata= folder;
  bashfile = run_file;
  bashbin = bash_bin;
  process_file = dirdata + "/log." + title + ".temp";
  process.setProcessChannelMode(QProcess::MergedChannels);
  process.setStandardOutputFile(process_file);

  connect(&process, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(runFinished()));
  connect(&process, SIGNAL(error(QProcess::ProcessError)), this, SLOT(executeError(QProcess::ProcessError)));

  process_timer.setInterval(100);
  process_timer.setSingleShot(false);
  connect(&process_timer, SIGNAL(timeout()), this, SLOT(printOutput()));

  highlighter_log = new Highlighter( ui->textEdit->document());
  highlighter_log->apply_to_log();
}

BashRunner::~BashRunner()
{
  delete ui;
}

void BashRunner::printOutput()
{
  QFile file(process_file);
  if (!file.open(QIODevice::ReadOnly)) return;

  if (file.size()>process_file_pos)
    {
      file.seek(process_file_pos);
      text_edit->moveCursor(QTextCursor::End);
      text_edit->insertPlainText(file.readAll());
      process_file_pos = file.pos();
    }

  file.close();
}

void BashRunner::executeFinished()
{
  process_timer.stop();
  printOutput();
}

void BashRunner::runFinished()
{
  text_edit->append("# # PIPELINE FINISHED # #\n");
  text_edit->moveCursor(QTextCursor::End);
}


void BashRunner::executeError(QProcess::ProcessError)
{
  process_timer.stop();
  printOutput();
}



void BashRunner::start_run()
{

  QStringList args;
  args << "-c" << "-l";
  QFile file(bashfile);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
      QMessageBox msgBox;
      msgBox.setText("Error reading bash file.");
      msgBox.exec();
    }
  else{
      args << file.readAll();
      QFile::remove(process_file);
      process_file_pos = 0;
      process.start(bashbin,args);
      process_timer.start();
    }
}
