//
//  paths.cpp
//  pGUI
//
//  Copyright (c) 2013 Bruno Nevado. All rights reserved.
//

#include "mainwindow.h"
#include <QApplication>

#include "pipeline.h"

int main(int argc, char *argv[])
{
  pipeline dapipeline("def");

  QApplication a(argc, argv);

  MainWindow w;

  w.pointer_to_pipeline = & dapipeline ;

  w.setDefaults();

  w.set_input_constrains();

  w.show();
  
  return a.exec();
}
