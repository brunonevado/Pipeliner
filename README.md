README FOR PIPELINER VERSION 0.2.0

PREREQUISITES:

Pipeliner's consists of two executables:
  Pipeliner's command line tools requires a C++11 compiler.
  Pipeliner's GUI requires QT v5.1 (http://qt-project.org/).

AVAILABLE OS:

Pipeliner is available for Linux and MacOS.


INSTALLATION INSTRUCTIONS:

Pipeliner's command line tools:
  1. Open Terminal and navigate to ./source/command_tools.
  2. In Linux: Type "g++ -Wall -g -std=c++0x -pthread -O3  *.cpp -o Pipeliner".
     In Mac:  Type "clang++ -std=c++11 -stdlib=libc++ -O3 -Wall *.cpp -o Pipeliner".
  3. A file named Pipeliner will be created in ./source/command_tools. Type "./Pipeliner help"
     to see the available commands.
  Note: For Mac, clang++ needs to be version 4+, which is part of Xcode tools 4.2+.
    These tools are available for free for MacOS 10.7+ from  https://developer.apple.com/technologies/tools/.
  
Pipeliner's GUI:
  1. Download and install QT v5.1 for your OS from http://qt-project.org/downloads.
     QT will install an executable called qmake into the computer's PATH or installation folder.
  2. Open Terminal, navigate to ./source/GUI, and type "qmake PipelinerGUI.pro". In case qmake is not 
     in the computer's PATH, use the full path to qmake instead.
  3. Still in Terminal (same folder) type "make".
  4. A file named PipelinerGUI (or PipelinerGUI.app in MacOS) will be created in folder ./source/GUI.
     Double-clicking it or typing ./PipelinerGUI in Terminal should start the GUI.
  Note: in Linux, openGL libraries might need to be installed with "sudo apt-get install libglu1-mesa-dev".

THIS DISTRIBUTION INCLUDES:

./R/plot_summary_pipeliner.R => R function to plot results manually
./doc/PipeManual.pdf         => PDF manual
./examples                   => example pdf output + user-defined steps
./source/command_tools       => source code for Pipeliner's command line tools
./source/GUI                 => source code for Pipeliner's GUI
