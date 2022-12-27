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
    drumpadfilesmodel.cpp \
    drumpadjsonconverters.cpp \
    drumpadpresets.cpp \
    drumpadpresetsmodel.cpp \
    graphrenderer.cpp \
    jsonconverters.cpp \
    loopstationjsonconverters.cpp \
    loopstationpresets.cpp \
    loopstationpresetsmodel.cpp \
    main.cpp \
    midicontainers.cpp \
    midiinwrapper.cpp \
    midioutwrapper.cpp \
    synthisizer.cpp \
    treetotableproxymodel.cpp \
    widgets/djwidget.cpp \
    widgets/drumpadwidget.cpp \
    widgets/loopstationwidget.cpp \
    widgets/mainwindow.cpp \
    widgets/midibutton.cpp \
    widgets/presetdetailwidget.cpp \
    widgets/previewwidget.cpp \
    widgets/sampleswidget.cpp \
    widgets/samplewidget.cpp \
    widgets/scratchwidget.cpp \
    widgets/sequencerwidget.cpp \
    widgets/settingsdialog.cpp \
    widgets/synthisizerwidget.cpp \
    widgets/trackdeck.cpp

HEADERS += \
    audiodecoder.h \
    audioformat.h \
    audioplayer.h \
    drummachinesettings.h \
    drumpadfilesmodel.h \
    drumpadjsonconverters.h \
    drumpadpresets.h \
    drumpadpresetsmodel.h \
    graphrenderer.h \
    jsonconverters.h \
    loopstationjsonconverters.h \
    loopstationpresets.h \
    loopstationpresetsmodel.h \
    midicontainers.h \
    midiinwrapper.h \
    midioutwrapper.h \
    synthisizer.h \
    treetotableproxymodel.h \
    widgets/djwidget.h \
    widgets/drumpadwidget.h \
    widgets/loopstationwidget.h \
    widgets/mainwindow.h \
    widgets/midibutton.h \
    widgets/presetdetailwidget.h \
    widgets/previewwidget.h \
    widgets/sampleswidget.h \
    widgets/samplewidget.h \
    widgets/scratchwidget.h \
    widgets/sequencerwidget.h \
    widgets/settingsdialog.h \
    widgets/synthisizerwidget.h \
    widgets/trackdeck.h

FORMS += \
    widgets/djwidget.ui \
    widgets/drumpadwidget.ui \
    widgets/loopstationwidget.ui \
    widgets/mainwindow.ui \
    widgets/presetdetailwidget.ui \
    widgets/sampleswidget.ui \
    widgets/samplewidget.ui \
    widgets/sequencerwidget.ui \
    widgets/settingsdialog.ui \
    widgets/synthisizerwidget.ui \
    widgets/trackdeck.ui
