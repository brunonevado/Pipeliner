#-------------------------------------------------
#
# Project created by QtCreator
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PipelinerGUI
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    toob_in_sfscode.cpp \
    paths.cpp \
    toob_in_ms.cpp \
    toob_fasq_bam_bwa.cpp \
    toob_fas_fasq_art.cpp \
    toob_bam_vcf_samtools.cpp \
    run_settings.cpp \
    pipeline.cpp \
    toob_user_defined.cpp \
    plot_results.cpp \
    highlighter.cpp \
    bashrunner.cpp

HEADERS  += mainwindow.h \
    toob_in_sfscode.h \
    paths.h \
    toob_in_ms.h \
    toob_fasq_bam_bwa.h \
    toob_fas_fasq_art.h \
    toob_bam_vcf_samtools.h \
    run_settings.h \
    pipeline.h \
    toob_user_defined.h \
    plot_results.h \
    highlighter.h \
    bashrunner.h

FORMS    += mainwindow.ui \
    bashrunner.ui

OTHER_FILES +=

RESOURCES += \
    resources.qrc

