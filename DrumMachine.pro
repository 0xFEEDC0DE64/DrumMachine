QT = core multimedia gui widgets network

CONFIG += c++17

release: QMAKE_CXXFLAGS_RELEASE -= -O1
release: QMAKE_CXXFLAGS_RELEASE -= -O2
release: QMAKE_CXXFLAGS_RELEASE += -O3 -ffast-math -march=native -mtune=native

LIBS += -lrtmidi -lportaudio

DEFINES += QT_DEPRECATED_WARNINGS QT_DISABLE_DEPRECATED_BEFORE=0x060000

SOURCES += \
    audiodecoder.cpp \
    audioformat.cpp \
    audioplayer.cpp \
    filesmodel.cpp \
    jsonconverters.cpp \
    main.cpp \
    mainwindow.cpp \
    midicontainers.cpp \
    midiinwrapper.cpp \
    presetdetailwidget.cpp \
    presets.cpp \
    presetsmodel.cpp \
    sampleswidget.cpp \
    samplewidget.cpp \
    sequencerwidget.cpp

HEADERS += \
    audiodecoder.h \
    audioformat.h \
    audioplayer.h \
    filesmodel.h \
    jsonconverters.h \
    mainwindow.h \
    midicontainers.h \
    midiinwrapper.h \
    presetdetailwidget.h \
    presets.h \
    presetsmodel.h \
    sampleswidget.h \
    samplewidget.h \
    sequencerwidget.h

FORMS += \
    mainwindow.ui \
    presetdetailwidget.ui \
    sampleswidget.ui \
    samplewidget.ui \
    sequencerwidget.ui

RESOURCES += \
    resources.qrc
