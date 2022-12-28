QT = core multimedia gui widgets network

CONFIG += c++22

QMAKE_CXXFLAGS += -Werror=all
release: QMAKE_CXXFLAGS_RELEASE -= -O1
release: QMAKE_CXXFLAGS_RELEASE -= -O2
release: QMAKE_CXXFLAGS_RELEASE += -O3 -ffast-math -march=native -mtune=native

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
    drumpadpresettagsmodel.cpp \
    graphrenderer.cpp \
    jsonconverters.cpp \
    loopstationjsonconverters.cpp \
    loopstationpresets.cpp \
    loopstationpresetsmodel.cpp \
    loopstationpresettagsmodel.cpp \
    main.cpp \
    midicontainers.cpp \
    midiinwrapper.cpp \
    midilearnsetting.cpp \
    midioutwrapper.cpp \
    synthisizer.cpp \
    treetotableproxymodel.cpp \
    widgets/djwidget.cpp \
    widgets/drumpadpresetdetailwidget.cpp \
    widgets/drumpadsampleswidget.cpp \
    widgets/drumpadsamplewidget.cpp \
    widgets/drumpadwidget.cpp \
    widgets/loopstationpresetdetailwidget.cpp \
    widgets/loopstationwidget.cpp \
    widgets/mainwindow.cpp \
    widgets/midibutton.cpp \
    widgets/miditabwidget.cpp \
    widgets/previewwidget.cpp \
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
    drumpadpresettagsmodel.h \
    futurecpp.h \
    graphrenderer.h \
    jsonconverters.h \
    loopstationjsonconverters.h \
    loopstationpresets.h \
    loopstationpresetsmodel.h \
    loopstationpresettagsmodel.h \
    midicontainers.h \
    midiinwrapper.h \
    midilearnsetting.h \
    midioutwrapper.h \
    synthisizer.h \
    treetotableproxymodel.h \
    widgets/djwidget.h \
    widgets/drumpadpresetdetailwidget.h \
    widgets/drumpadsampleswidget.h \
    widgets/drumpadsamplewidget.h \
    widgets/drumpadwidget.h \
    widgets/loopstationpresetdetailwidget.h \
    widgets/loopstationwidget.h \
    widgets/mainwindow.h \
    widgets/midibutton.h \
    widgets/miditabwidget.h \
    widgets/previewwidget.h \
    widgets/scratchwidget.h \
    widgets/sequencerwidget.h \
    widgets/settingsdialog.h \
    widgets/synthisizerwidget.h \
    widgets/trackdeck.h

FORMS += \
    widgets/djwidget.ui \
    widgets/drumpadpresetdetailwidget.ui \
    widgets/drumpadsampleswidget.ui \
    widgets/drumpadsamplewidget.ui \
    widgets/drumpadwidget.ui \
    widgets/loopstationpresetdetailwidget.ui \
    widgets/loopstationwidget.ui \
    widgets/mainwindow.ui \
    widgets/sequencerwidget.ui \
    widgets/settingsdialog.ui \
    widgets/synthisizerwidget.ui \
    widgets/trackdeck.ui
