//
//  pGUI
//
//  Copyright (c) 2013 Bruno Nevado. GNU license
//


#include <QApplication>

#include "pipeline.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
  if ( argc > 1 && strcmp (argv[1] , "runner") == 0 ){
      QApplication a(argc, argv);
      if (argc != 6){
          QMessageBox msgBox;
          msgBox.setText("Pipeliner runner called with wrong number of args (need 6).");
          msgBox.exec();
          exit (1);
        }
      BashRunner runner( 0, argv[2], argv[3], argv[4], argv[5] );
      runner.show();
      runner.start_run();
      return a.exec();

    }
  else{

      pipeline dapipeline(argv[0]);

      QApplication a(argc, argv);

      MainWindow w;

      w.pointer_to_pipeline = & dapipeline ;

      w.setDefaults();

      w.set_input_constrains();

      w.show();

      return a.exec();
    }
}
