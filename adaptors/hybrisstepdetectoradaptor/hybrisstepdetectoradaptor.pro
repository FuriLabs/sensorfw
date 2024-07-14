TARGET       = hybrisstepdetectoradaptor

HEADERS += hybrisstepdetectoradaptor.h \
           hybrisstepdetectoradaptorplugin.h

SOURCES += hybrisstepdetectoradaptor.cpp \
           hybrisstepdetectoradaptorplugin.cpp

LIBS+= -L../../core -lhybrissensorfw-qt$${QT_MAJOR_VERSION}

include( ../adaptor-config.pri )
config_hybris {
    PKGCONFIG += android-headers
}
