//
//  pGUI
//
//  Copyright (c) 2013 Bruno Nevado. GNU license
//

#ifndef __pGUI_bashrunner__
#define __pGUI_bashrunner__

// code from http://msturm.p7.de/programming_qprocess.php

#include <QMainWindow>
#include <QProcess>
#include <QMessageBox>
#include <stdlib.h>
#include <iostream>
#include <QObject>
#include <QTextEdit>
#include <QTimer>
#include <QFile>

#include "highlighter.h"

namespace Ui {
  class BashRunner;
}

class BashRunner : public QMainWindow
{
  Q_OBJECT

public:
  explicit BashRunner(QWidget *parent = 0, QString title = "", QString folder = "", QString run_file = "", QString bash_bin = "");
  ~BashRunner();
  void start_run();

private slots:
  void printOutput();
  void executeFinished();
  void executeError(QProcess::ProcessError);
  void runFinished();

private:
  Ui::BashRunner *ui;
  QTextEdit* text_edit;
  QProcess process;
  QTimer process_timer;
  QString process_file;
  qint64 process_file_pos;
  QString run_title, dirdata, bashfile, bashbin;
  Highlighter *highlighter_log;

};

#endif // __pGUI_bashrunner__
