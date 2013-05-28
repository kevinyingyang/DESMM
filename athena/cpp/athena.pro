TEMPLATE = lib

HEADERS += autocorrelation_function.hpp         \
           predominant_frequency_descriptor.hpp \
           utility.hpp                          \
           probability_mass_function.hpp        \
           text_height_estimation.hpp           \

SOURCES += autocorrelation_function.cpp         \
           predominant_frequency_descriptor.cpp \
           utility.cpp                          \
           probability_mass_function.cpp        \
           text_height_estimation.cpp           \

LIBS += -lopencv_core       \
        -lopencv_highgui    \
        -lopencv_imgproc    \
