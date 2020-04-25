#include <QApplication>
#include <QSslSocket>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QSplashScreen>
#include <QMessageBox>
#include <QDebug>

#include "jsonconverters.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QCoreApplication::setOrganizationDomain("brunner.ninja");
    QCoreApplication::setOrganizationName("brunner.ninja");
    QCoreApplication::setApplicationName("miditest");
    QCoreApplication::setApplicationVersion("1.0");

    qDebug() << "supportsSsl" << QSslSocket::supportsSsl();
    qDebug() << "sslLibraryVersionString" << QSslSocket::sslLibraryVersionString();
    qDebug() << "sslLibraryBuildVersionString" << QSslSocket::sslLibraryBuildVersionString();

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

    presets::PresetsConfig presetsConfig;

    {
        QSplashScreen splashScreen{QPixmap{":/drummachine/splashscreen.png"}};
        splashScreen.showMessage(QCoreApplication::translate("main", "Loading list of presets..."));
        splashScreen.show();

        QEventLoop eventLoop;

        QNetworkAccessManager manager;
        const auto reply = std::unique_ptr<QNetworkReply>(manager.get(QNetworkRequest{QUrl{"https://brunner.ninja/komposthaufen/dpm/presets_config_v12.json"}}));
        QObject::connect(reply.get(), &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
        eventLoop.exec();

        if (reply->error() != QNetworkReply::NoError)
        {
            QMessageBox::warning(nullptr, QCoreApplication::translate("main", "Could not load presets!"), QCoreApplication::translate("main", "Could not load presets!") + "\n\n" + reply->errorString());
            return 1;
        }

        presetsConfig = json_converters::parsePresetsConfig(json_converters::loadJson(reply->readAll()));
    }

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

    MainWindow mainWindow{presetsConfig};
    mainWindow.showMaximized();
    mainWindow.selectFirstPreset();

    return app.exec();
}
