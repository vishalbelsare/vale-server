#include <singleapplication.h>

#include <QApplication>
#include <QMessageBox>
#include <QSettings>
#include <QStyleFactory>

#ifndef QT_NO_SYSTEMTRAYICON

#include "MainWindow.h"

int main(int argc, char* argv[]) {
  SingleApplication app(argc, argv, false,
                        SingleApplication::Mode::User |
                            SingleApplication::Mode::ExcludeAppPath |
                            SingleApplication::Mode::ExcludeAppVersion);

  QApplication::setQuitOnLastWindowClosed(false);

  app.setOrganizationName("errata-ai");
  app.setApplicationName("Vale Server");
  app.setApplicationVersion(APP_VERSION);

#ifdef Q_OS_WIN
  QSettings theme(
      "HKEY_CURRENT_"
      "USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize",
      QSettings::NativeFormat);

  if (theme.value("AppsUseLightTheme") == 0) {
    app.setStyle(QStyleFactory::create("Fusion"));

    QColor darkGray(53, 53, 53);
    QColor gray(128, 128, 128);
    QColor black(25, 25, 25);
    QColor blue(42, 130, 218);

    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, darkGray);
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, black);
    darkPalette.setColor(QPalette::AlternateBase, darkGray);
    darkPalette.setColor(QPalette::ToolTipBase, blue);
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Button, darkGray);
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::Link, blue);
    darkPalette.setColor(QPalette::Highlight, blue);
    darkPalette.setColor(QPalette::HighlightedText, Qt::black);

    darkPalette.setColor(QPalette::Active, QPalette::Button, gray.darker());
    darkPalette.setColor(QPalette::Disabled, QPalette::ButtonText, gray);
    darkPalette.setColor(QPalette::Disabled, QPalette::WindowText, gray);
    darkPalette.setColor(QPalette::Disabled, QPalette::Text, gray);
    darkPalette.setColor(QPalette::Disabled, QPalette::Light, darkGray);

    app.setPalette(darkPalette);
    app.setStyleSheet(
        "QToolTip { color: #ffffff; background-color: #2a82da; border: 1px "
        "solid white; }");
  }
#endif

  if (!QSystemTrayIcon::isSystemTrayAvailable()) {
    QMessageBox::critical(nullptr, QObject::tr("Vale Server"),
                          QObject::tr("I couldn't detect any system tray "
                                      "on this system."));
    return 1;
  }

  MainWindow window;
  return app.exec();
}

#else

#include <QDebug>
#include <QLabel>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  QString text("QSystemTrayIcon is not supported on this platform.");

  QLabel *label = new QLabel(text);
  label->setWordWrap(true);

  label->show();
  qDebug() << text;

  app.exec();
}

#endif
