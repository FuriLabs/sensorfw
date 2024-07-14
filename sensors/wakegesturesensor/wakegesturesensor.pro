TARGET       = wakegesturesensor

HEADERS += wakegesturesensor.h   \
           wakegesturesensor_a.h \
           wakegestureplugin.h

SOURCES += wakegesturesensor.cpp   \
           wakegesturesensor_a.cpp \
           wakegestureplugin.cpp

include( ../sensor-config.pri )
