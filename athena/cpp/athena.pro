TEMPLATE = lib

HEADERS += autocorrelation_function.hpp         \
           predominant_frequency_descriptor.hpp \

SOURCES += autocorrelation_function.cpp         \
           predominant_frequency_descriptor.cpp \

LIBS += -lopencv_core       \
        -lopencv_highgui    \
        -lopencv_imgproc    \
