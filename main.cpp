#include <QApplication>
#include <QSslSocket>
#include <QDebug>
#include <QMessageBox>

#include "portaudio.h"

#include "widgets/mainwindow.h"

namespace {
template<typename T>
class CleanupHelper
{
public:
    CleanupHelper(T &&callback) :
        m_callback{std::move(callback)}
    {}

    ~CleanupHelper()
    {
        m_callback();
    }

private:
    T m_callback;
};

template<typename T>
auto makeCleanupHelper(T &&callback)
{
    return CleanupHelper<T>{std::move(callback)};
}
}

int main(int argc, char *argv[])
{
    qSetMessagePattern("%{time dd.MM.yyyy HH:mm:ss.zzz} "
                       "["
                       "%{if-debug}D%{endif}"
                       "%{if-info}I%{endif}"
                       "%{if-warning}W%{endif}"
                       "%{if-critical}C%{endif}"
                       "%{if-fatal}F%{endif}"
                       "] "
                       "%{function}(): "
                       "%{message}");

    QApplication app{argc, argv};
    QCoreApplication::setOrganizationDomain("brunner.ninja");
    QCoreApplication::setOrganizationName("brunner.ninja");
    QCoreApplication::setApplicationName("DrumMachine");
    QCoreApplication::setApplicationVersion("1.0");

    qDebug() << "supportsSsl:" << QSslSocket::supportsSsl()
             << "sslLibraryVersionString:" << QSslSocket::sslLibraryVersionString()
             << "sslLibraryBuildVersionString:" << QSslSocket::sslLibraryBuildVersionString();

    if (PaError err = Pa_Initialize(); err != paNoError)
    {
        QMessageBox::warning({}, QApplication::translate("main", "Error initializing PortAudio!"), QApplication::translate("main", "Error initializing PortAudio!") + "\n\n" + Pa_GetErrorText(err));
        return 1;
    }

    auto helper0 = makeCleanupHelper([](){
        if (PaError err = Pa_Terminate(); err != paNoError)
            fprintf(stderr, "Could not terminate PortAudio!\n");
    });

#if !defined(Q_OS_WIN)
    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, QColor(53,53,53));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, QColor(25,25,25));
    darkPalette.setColor(QPalette::AlternateBase, QColor(53,53,53));
    darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Button, QColor(53,53,53));
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::BrightText, Qt::red);
    darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::HighlightedText, Qt::black);
    app.setPalette(darkPalette);

    app.setStyleSheet("QToolTip { color: #ffffff; background-color: #2a82da; border: 1px solid white; }");
#endif

    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}
