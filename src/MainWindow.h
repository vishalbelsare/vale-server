#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#ifndef QT_NO_SYSTEMTRAYICON

#include <QtAwesome.h>

#include <QMainWindow>
#include <QMap>
#include <QSystemTrayIcon>
#include <QToolBar>

#include "AboutDialog.h"
#include "DisplayDialog.h"
#include "HTTPServer.h"
#include "INIHighlighter.h"

QT_BEGIN_NAMESPACE
class QAction;
class QCheckBox;
class QComboBox;
class QDialogButtonBox;
class QGroupBox;
class QLabel;
class QLineEdit;
class QListWidget;
class QMenu;
class QPlainTextEdit;
class QPushButton;
class QStackedWidget;
class QTextEdit;
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow() Q_DECL_OVERRIDE;

 protected:
  void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

 private:
  void setUpAPI();
  void stopAPI();

  void init();

  void loadSettings();
  void saveSettings();

  void createActions();
  void createTrayIcon();
  void createProjects();
  void createTab(QString label, QString icon, int index);

  void createGeneralPage();
  void createProjectsPage();
  void createAdvancedPage();

  void updateStatusBar(bool running);
  void checkClipboard(QString format);
  void checkForUpdates();
  void changeProject(QString project);
  void changeMode(QString mode);

  // UI: The list that displays the available projects.
  QListWidget *projects;
  // UI: The list that displays the installed styles.
  QListWidget *styles;

  // Setting: Try to use Vale-style local configurations (as determined
  // by system paths) when possible.
  // QCheckBox *compatMode;

  // Setting: Send system-level notification whenever a style becomes
  // outdated.
  QCheckBox *sysUpdates;

  // Setting: App updates.
  QCheckBox *autoUpdates;

  // Data: A map of <project_name, ini_content>.
  QMap<QString, QString> configs;

  // Data: Active <project_name>.
  QString activeProject;
  QString activeMode;

  QStackedWidget *pages;
  QToolBar *toolBar;

  // Server config
  QLineEdit *apiPortEdit;
  QPlainTextEdit *consoleEdit;

  // Vale config
  QTextEdit *configEdit;
  INIHighlighter *highlighter;

  QAction *statusAction;

  // Functions
  QAction *checkAction;    // Check clipboard
  QAction *projectAction;  // Select project
  QAction *styleAction;    // Browse styles
  QAction *modeAction;     // Select mode

  // Window state
  QAction *quitAction;   // Quit Vale Server
  QAction *aboutAction;  // About dialog
  QAction *prefsAction;  // Preferences
  QAction *appUpdateAction;

  // URLs
  QAction *dashAction;     // Dashboard
  QAction *supportAction;  // GitHub
  QAction *docsAction;     // Docs
  QAction *logAction;      // Log file

  QSystemTrayIcon *trayIcon;
  QMenu *trayIconMenu;

  HTTPServer *server;
  QtAwesome *awesome;

  DisplayDialog *displayDialog;

  int fontSize;
};

#endif

#endif
