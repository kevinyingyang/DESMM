######################################
## Local config

CONFIG += release
CONFIG -= debug

######################################

include(../../cpp/athena.pri)

TEMPLATE = app

CONFIG += 
LIBS += -lopencv_core -lopencv_imgproc -lopencv_highgui -lathena

HEADERS += 

SOURCES += main.cpp 

TARGET = predominant_frequency_descriptor_app
