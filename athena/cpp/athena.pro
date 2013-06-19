TEMPLATE = lib

HEADERS += autocorrelation_function.hpp         \
           predominant_frequency_descriptor.hpp \
           utility.hpp                          \
           probability_mass_function.hpp        \
           text_height_estimation.hpp           \
           rose_of_directions.hpp               \

SOURCES += autocorrelation_function.cpp         \
           predominant_frequency_descriptor.cpp \
           utility.cpp                          \
           probability_mass_function.cpp        \
           text_height_estimation.cpp           \
           rose_of_directions.cpp               \

LIBS += -lopencv_core       \
        -lopencv_highgui    \
        -lopencv_imgproc    \
