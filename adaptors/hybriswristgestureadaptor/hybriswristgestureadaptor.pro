TARGET       = hybriswristgestureadaptor

HEADERS += hybriswristgestureadaptor.h \
           hybriswristgestureadaptorplugin.h

SOURCES += hybriswristgestureadaptor.cpp \
           hybriswristgestureadaptorplugin.cpp

LIBS+= -L../../core -lhybrissensorfw-qt$${QT_MAJOR_VERSION}

include( ../adaptor-config.pri )
config_hybris {
    PKGCONFIG += android-headers
}
