QT = core multimedia gui widgets network

CONFIG += c++20

#release: QMAKE_CXXFLAGS_RELEASE -= -O1
#release: QMAKE_CXXFLAGS_RELEASE -= -O2
#release: QMAKE_CXXFLAGS_RELEASE += -O3 -ffast-math -march=native -mtune=native

LIBS += -lrtmidi -lportaudio

DEFINES += QT_DEPRECATED_WARNINGS QT_DISABLE_DEPRECATED_BEFORE=0x060000

SOURCES += \
    audiodecoder.cpp \
    audioformat.cpp \
    audioplayer.cpp \
    drummachinesettings.cpp \
    filesmodel.cpp \
    graphrenderer.cpp \
    jsonconverters.cpp \
    main.cpp \
    midicontainers.cpp \
    midiinwrapper.cpp \
    midioutwrapper.cpp \
    presets.cpp \
    presetsmodel.cpp \
    synthisizer.cpp \
    treetotableproxymodel.cpp \
    widgets/djwidget.cpp \
    widgets/drumpadwidget.cpp \
    widgets/mainwindow.cpp \
    widgets/presetdetailwidget.cpp \
    widgets/previewwidget.cpp \
    widgets/sampleswidget.cpp \
    widgets/samplewidget.cpp \
    widgets/scratchwidget.cpp \
    widgets/sequencerwidget.cpp \
    widgets/synthisizerwidget.cpp \
    widgets/trackdeck.cpp

HEADERS += \
    audiodecoder.h \
    audioformat.h \
    audioplayer.h \
    drummachinesettings.h \
    filesmodel.h \
    graphrenderer.h \
    jsonconverters.h \
    midicontainers.h \
    midiinwrapper.h \
    midioutwrapper.h \
    presets.h \
    presetsmodel.h \
    synthisizer.h \
    treetotableproxymodel.h \
    widgets/djwidget.h \
    widgets/drumpadwidget.h \
    widgets/mainwindow.h \
    widgets/presetdetailwidget.h \
    widgets/previewwidget.h \
    widgets/sampleswidget.h \
    widgets/samplewidget.h \
    widgets/scratchwidget.h \
    widgets/sequencerwidget.h \
    widgets/synthisizerwidget.h \
    widgets/trackdeck.h

FORMS += \
    widgets/djwidget.ui \
    widgets/drumpadwidget.ui \
    widgets/mainwindow.ui \
    widgets/presetdetailwidget.ui \
    widgets/sampleswidget.ui \
    widgets/samplewidget.ui \
    widgets/sequencerwidget.ui \
    widgets/synthisizerwidget.ui \
    widgets/trackdeck.ui
