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
    djwidget.cpp \
    drummachinesettings.cpp \
    drumpadwidget.cpp \
    filesmodel.cpp \
    graphrenderer.cpp \
    jsonconverters.cpp \
    main.cpp \
    mainwindow.cpp \
    midicontainers.cpp \
    midiinwrapper.cpp \
    midioutwrapper.cpp \
    presetdetailwidget.cpp \
    presets.cpp \
    presetsmodel.cpp \
    previewwidget.cpp \
    sampleswidget.cpp \
    samplewidget.cpp \
    scratchwidget.cpp \
    sequencerwidget.cpp \
    synthisizer.cpp \
    synthisizerwidget.cpp \
    trackdeck.cpp \
    treetotableproxymodel.cpp

HEADERS += \
    audiodecoder.h \
    audioformat.h \
    audioplayer.h \
    djwidget.h \
    drummachinesettings.h \
    drumpadwidget.h \
    filesmodel.h \
    graphrenderer.h \
    jsonconverters.h \
    mainwindow.h \
    midicontainers.h \
    midiinwrapper.h \
    midioutwrapper.h \
    presetdetailwidget.h \
    presets.h \
    presetsmodel.h \
    previewwidget.h \
    sampleswidget.h \
    samplewidget.h \
    scratchwidget.h \
    sequencerwidget.h \
    synthisizer.h \
    synthisizerwidget.h \
    trackdeck.h \
    treetotableproxymodel.h

FORMS += \
    djwidget.ui \
    drumpadwidget.ui \
    mainwindow.ui \
    presetdetailwidget.ui \
    sampleswidget.ui \
    samplewidget.ui \
    sequencerwidget.ui \
    synthisizerwidget.ui \
    trackdeck.ui

RESOURCES += \
    resources.qrc
