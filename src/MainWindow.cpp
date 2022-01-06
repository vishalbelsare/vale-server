#include "MainWindow.h"

#include "StyledLabel.h"

#ifndef QT_NO_SYSTEMTRAYICON

#include <QAction>
#include <QActionGroup>
#include <QApplication>
#include <QCheckBox>
#include <QClipboard>
#include <QCloseEvent>
#include <QCoreApplication>
#include <QDesktopServices>
#include <QDir>
#include <QFileDialog>
#include <QFontDatabase>
#include <QGroupBox>
#include <QIntValidator>
#include <QJsonDocument>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QListWidgetItem>
#include <QMenu>
#include <QMessageBox>
#include <QPalette>
#include <QPlainTextEdit>
#include <QProgressBar>
#include <QPushButton>
#include <QRegularExpressionValidator>
#include <QResource>
#include <QSettings>
#include <QStackedWidget>
#include <QStandardPaths>
#include <QStatusBar>
#include <QTextEdit>
#include <QToolButton>
#include <QUrl>
#include <QVBoxLayout>
#include <QtConcurrent/QtConcurrent>

#ifdef Q_OS_OSX
#include "darwin/CocoaInitializer.h"
#include "darwin/SparkleAutoUpdater.h"
#include "darwin/mac.h"
#endif

#ifdef Q_OS_WIN
#include "winsparkle.h"
#endif

static const QString kDocsURL =
    QStringLiteral("https://docs.errata.ai/vale-server/install");
static const QString kRepoURL =
    QStringLiteral("https://github.com/errata-ai/vale-server/issues");

/**
 * Find the platform-specific location of a packaged resource.
 */
QString getResource(QString name) {
#if defined(Q_OS_WIN)
  // On Windows, resources are bundled in the same directory as the executable.
  //
  // e.g., `C:\Program Files (x86)\Vale Server`.
  QString parent = QApplication::applicationDirPath() + "/";
#elif defined(Q_OS_OSX)
  // On macOS, resources are packaged into the `.app` under 'Resources'.
  QString parent = QApplication::applicationDirPath() + "/../Resources/";
#else
  QString parent = QApplication::applicationDirPath() + "/build/";
#endif

  return QFileInfo(parent + name).absoluteFilePath();
}

/**
 * Open the user's StylesPath using the default file explorer.
 */
void browseStyles(QString stylesPath, QString style) {
#if defined(Q_OS_OSX)
  QStringList args;
  args << "-e";
  args << "tell application \"Finder\"";
  args << "-e";
  args << "activate";
  args << "-e";
  args << "open POSIX file \"" + stylesPath + "/" + style + "\"";
  args << "-e";
  args << "end tell";
  QProcess::startDetached("osascript", args);
#elif defined(Q_OS_WIN)
  QStringList args;
  args << "/select," << QDir::toNativeSeparators(stylesPath + "/" + style);
  QProcess::startDetached("explorer", args);
#else
  QProcess::startDetached("xdg-open", {stylesPath + "/" + style});
#endif
}

/**
 * [checkForAppUpdate description]
 */
bool checkForAppUpdate(bool unattended) {
#if defined(Q_OS_OSX)
  CocoaInitializer initializer;

  AutoUpdater *updater = new SparkleAutoUpdater(
      "https://raw.githubusercontent.com/errata-ai/vale-server/master/pkg/oss/"
      "appcast.xml");

  if (updater) {
    if (unattended) {
      updater->checkForUpdates();
    } else {
      updater->checkForUpdatesWithUI();
    }
  }

  delete updater;

  // NOTE: We're returning 'false' here because the `checkForUpdates`
  // function should instantiate a UI if there's an update.
  return false;
#elif defined(Q_OS_WIN)
  win_sparkle_set_appcast_url(
      "https://raw.githubusercontent.com/errata-ai/vale-server/master/pkg/oss/"
      "win.xml");
  win_sparkle_set_app_details(
      L"errata.ai", L"Vale Server",
      qApp->applicationVersion().toStdWString().c_str());

  win_sparkle_set_dsa_pub_pem(
      reinterpret_cast<const char *>(QResource(":/dsa_pub.pem").data()));
  win_sparkle_set_automatic_check_for_updates(0);

  win_sparkle_init();

  if (unattended) {
    win_sparkle_check_update_without_ui();
  } else {
    win_sparkle_check_update_with_ui();
  }

  return false;
#else
  return false;
#endif
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  this->awesome = new QtAwesome(qApp);
  this->awesome->initFontAwesome();

  this->server = new HTTPServer(QApplication::applicationDirPath(),
                                qApp->applicationVersion(), this);
  this->init();

  QVersionNumber v = this->server->vale->getVersion();
  if (v.toString().isEmpty()) {
    QMessageBox::warning(
        this, tr("Vale Server"),
        tr("Unable to find a Vale executable on the system "
           "$PATH; please install Vale to continue using Vale Server."),
        QMessageBox::Ok);
  } else {
    this->setUpAPI();
    this->checkForUpdates();
  }
}

MainWindow::~MainWindow() {
  saveSettings();
#if defined(Q_OS_WIN)
  win_sparkle_cleanup();
#endif
  delete awesome;
}

void MainWindow::closeEvent(QCloseEvent *event) {
  if (trayIcon->isVisible()) {
    hide();
    event->ignore();
  }

#ifdef Q_OS_OSX
  showDockIcon(false);
#endif

  configs.insert(activeProject, configEdit->toPlainText());

  saveSettings();
  createProjects();
}

void MainWindow::init() {
  this->toolBar = addToolBar(tr("Tabs"));

  pages = new QStackedWidget(this);
  for (int i = 0; i < 3; ++i) {
    pages->addWidget(new QWidget(this));
  }
  setCentralWidget(pages);

  createTab("General", ":/img/PrefsGeneral.png", 0);
  createTab("Projects", ":/img/PrefsVale.png", 1);
  createTab("Advanced", ":/img/PrefsAdvanced.png", 2);

  configEdit = new QTextEdit(this);
  configEdit->setAcceptRichText(false);
  connect(configEdit, &QTextEdit::textChanged,
          [this]() { server->config(configEdit->toPlainText()); });

  consoleEdit = new QPlainTextEdit(this);
  apiPortEdit = new QLineEdit(this);

  sysUpdates = new QCheckBox("Notify me about style updates", this);
  autoUpdates = new QCheckBox("Automatically check for updates", this);

  loadSettings();

  createActions();
  createTrayIcon();
  createProjects();

  updateStatusBar(false);

  QFont base = QApplication::font();
#if defined(Q_OS_OSX)
  setUnifiedTitleAndToolBarOnMac(true);

  showDockIcon(true);
  showDockIcon(false);

  QIcon icon = QIcon(":/img/logo.png");
  icon.setIsMask(true);

  fontSize = base.pointSize();
#elif defined(Q_OS_WIN)
  QIcon icon = QIcon(":/img/logo-64.png");
  base.setPointSize(8);
  setFont(base);

  fontSize = 8;
#else
  QIcon icon = QIcon(":/img/logo-alt.png");

  QString name = QIcon::themeName();
  if (QIcon::hasThemeIcon("vale-server-tray")) {
    icon = QIcon::fromTheme("vale-server-tray");
  } else if (name == "ubuntu-mono-light") {
    icon = QIcon(":/img/icon-theme-light.png");
  } else if (name == "ubuntu-mono-dark") {
    icon = QIcon(":/img/icon-theme-dark.png");
  }
#endif

  trayIcon->setIcon(icon);
  trayIcon->setToolTip("Vale Server");
  trayIcon->show();

  QIcon logo = QIcon(":/img/icon.png");
  setWindowIcon(logo);

  createGeneralPage();
  createProjectsPage();
  createAdvancedPage();
}

void MainWindow::createTab(QString label, QString icon, int index) {
  const QIcon qIcon = QIcon(icon);

  QToolButton *addonButton = new QToolButton(this);
  addonButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
  addonButton->setIcon(qIcon);
  addonButton->setText(label);

  toolBar->addWidget(addonButton);
  connect(addonButton, &QPushButton::clicked, [this, label, index]() {
    setWindowTitle(label);
    pages->setCurrentIndex(index);
  });
}

void MainWindow::changeProject(QString project) {
  activeProject = project;
  server->project(activeProject);
}

void MainWindow::changeMode(QString mode) {
  activeMode = mode;
  foreach (QAction *action, modeAction->menu()->actions()) {
    action->setCheckable(true);
    if (action->text() == mode) {
      action->setChecked(true);
    }
  }
  server->mode(activeMode);
}

void MainWindow::saveSettings() {
  QSettings core;

  core.beginGroup("Server");
  core.setValue("port", apiPortEdit->text());
  core.endGroup();

  core.beginGroup("Options");
  core.setValue("styleUpdates", sysUpdates->isChecked());
  core.setValue("appUpdates", autoUpdates->isChecked());
  core.endGroup();

  QString stylesPath =
      QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) +
      QDir::separator() + "vale-server.ini";

  QSettings valeSettings(stylesPath, QSettings::IniFormat);

  valeSettings.beginGroup("Config");
  valeSettings.remove("");
  QMap<QString, QString>::const_iterator i = configs.constBegin();
  while (i != configs.constEnd()) {
    valeSettings.setValue(i.key(), i.value());
    ++i;
  }
  valeSettings.endGroup();

  valeSettings.beginGroup("Vale");
  valeSettings.setValue("active", activeProject);
  valeSettings.setValue("mode", activeMode);
  valeSettings.endGroup();
}

void MainWindow::loadSettings() {
  QSettings core;

  core.beginGroup("Server");
  apiPortEdit->setText(core.value("port", 7777).toString());

  core.endGroup();

  core.beginGroup("Options");
  sysUpdates->setChecked(core.value("styleUpdates", false).toBool());
  autoUpdates->setChecked(core.value("appUpdates", false).toBool());
  core.endGroup();

  QString stylesPath =
      QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) +
      QDir::separator() + "vale-server.ini";

  QSettings valeSettings(stylesPath, QSettings::IniFormat);

  valeSettings.beginGroup("Config");

  QStringList keys = valeSettings.childKeys();
  foreach (QString key, keys) {
    QString value = valeSettings.value(key).toString();
    if (!value.contains("StylesPath")) {
      // NOTE: StylesPath conversion:
      value = "StylesPath = \"" +
              QDir::fromNativeSeparators(server->vale->stylesPath) + "\"\n\n" +
              value;
    }
    configs.insert(key, value);
  }

  valeSettings.endGroup();

  valeSettings.beginGroup("Vale");

  changeProject(valeSettings.value("active", "N/A").toString());
  if (activeProject != "N/A") {
    configEdit->setPlainText(configs.value(activeProject));
  }
  activeMode = valeSettings.value("mode", "Server").toString();

  valeSettings.endGroup();
}

void MainWindow::checkForUpdates() {
  if (sysUpdates->isChecked()) {
    QString library = QString::fromStdString(
        server->vale->command({"ls-library", server->stylesPath}));

    QJsonDocument doc = QJsonDocument::fromJson(library.toUtf8());
    if (!doc.isNull()) {
      if (doc.isArray()) {
        QJsonArray array = doc.array();
        foreach (const QJsonValue &v, array) {
          bool update = v.toObject().value("has_update").toBool();
          bool installed = v.toObject().value("installed").toBool();
          if (update && installed) {
            QString name = v.toObject().value("name").toString();
            trayIcon->showMessage(
                "[Vale Server] Style updates available!",
                "The '" + name + "' style has an update available.",
                QSystemTrayIcon::Information, 500);
          }
        }
      }
    }
  }
  if (autoUpdates->isChecked() && checkForAppUpdate(true)) {
    trayIcon->showMessage("[Vale Server] Checking for updates...",
                          "A new version is available!",
                          QSystemTrayIcon::Information, 500);
  }
}

void MainWindow::createActions() {
  statusAction = new QAction(tr("Vale Server is stopped"), this);
  statusAction->setDisabled(true);

  QVariantMap options;
  options.insert("color", QColor(255, 0, 0));
  statusAction->setIcon(awesome->icon(fa::circle, options));

  projectAction = new QAction("Select Project");
  projectAction->setMenu(new QMenu(this));
  projectAction->setActionGroup(new QActionGroup(this));
  projectAction->actionGroup()->setExclusive(true);
  connect(projectAction->actionGroup(), &QActionGroup::triggered,
          [this](QAction *action) {
            auto items = projects->findItems(action->text(), Qt::MatchExactly);
            projects->setCurrentItem(items.front());
          });

  modeAction = new QAction("Select Mode");
  modeAction->setMenu(new QMenu(this));
  modeAction->setActionGroup(new QActionGroup(this));
  modeAction->actionGroup()->setExclusive(true);

  const QStringList list{"Server", "Compatibility", "Command Line"};
  for (const auto &i : list) {
    QAction *a = new QAction(i, this);
    modeAction->menu()->addAction(a);
    modeAction->actionGroup()->addAction(a);
  }

  changeMode(activeMode);
  connect(modeAction->actionGroup(), &QActionGroup::triggered,
          [this](QAction *action) { changeMode(action->text()); });

  checkAction = new QAction("Check Clipboard as");
  checkAction->setMenu(new QMenu(this));
  checkAction->menu()->addAction(new QAction("Plain Text", this));
  checkAction->menu()->addAction(new QAction("Markdown", this));
  checkAction->menu()->addAction(new QAction("AsciiDoc", this));
  checkAction->menu()->addAction(new QAction("reStructuredText", this));
  checkAction->menu()->addAction(new QAction("HTML", this));
  connect(checkAction->menu(), &QMenu::triggered,
          [this](QAction *action) { checkClipboard(action->text()); });

  dashAction = new QAction(tr("Open Dashboard..."), this);
  connect(dashAction, &QAction::triggered, [this]() {
    QString port = apiPortEdit->text();
    QDesktopServices::openUrl(QUrl("http://localhost:" + port));
  });

  logAction = new QAction(tr("Server Log"), this);
  connect(logAction, &QAction::triggered, []() {
    QString log =
        QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) +
        +"/vale-server.log";
    QDesktopServices::openUrl(QUrl::fromLocalFile(log));
  });

  aboutAction = new QAction(tr("About Vale Server"), this);
  connect(aboutAction, &QAction::triggered, [this]() {
    QDialog *aboutDialog = new AboutDialog(this, "Open Source");
    aboutDialog->show();
  });

  appUpdateAction = new QAction(tr("Check for Updates..."), this);
  connect(appUpdateAction, &QAction::triggered,
          []() { checkForAppUpdate(false); });

  prefsAction = new QAction(tr("Preferences..."), this);
  prefsAction->setShortcut(Qt::Key_Comma | Qt::CTRL);
  connect(prefsAction, &QAction::triggered, [this]() {
    showNormal();
#ifdef Q_OS_OSX
    showDockIcon(true);
#endif
    raise();
    resize(500, 450);
  });

  styleAction = new QAction(tr("Browse Styles..."), this);
  connect(styleAction, &QAction::triggered,
          [this]() { browseStyles(server->stylesPath, ""); });

  docsAction = new QAction(tr("Documentation"), this);
  connect(docsAction, &QAction::triggered,
          []() { QDesktopServices::openUrl(QUrl(kDocsURL)); });

  supportAction = new QAction(tr("Support"), this);
  connect(supportAction, &QAction::triggered,
          []() { QDesktopServices::openUrl(QUrl(kRepoURL)); });

  quitAction = new QAction(tr("&Quit Vale Server"), this);
  quitAction->setShortcut(Qt::Key_Q | Qt::CTRL);
  connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);
}

void MainWindow::createTrayIcon() {
  trayIconMenu = new QMenu(this);

  // App info & status
  trayIconMenu->addAction(statusAction);
  trayIconMenu->addSeparator();
  trayIconMenu->addAction(aboutAction);
  trayIconMenu->addAction(appUpdateAction);
  trayIconMenu->addSeparator();

#ifdef Q_OS_LINUX
  appUpdateAction->setEnabled(false);
#endif

  // Navigation
  trayIconMenu->addAction(prefsAction);
  trayIconMenu->addAction(dashAction);
  trayIconMenu->addAction(styleAction);
  trayIconMenu->addSeparator();

  // Functions
  trayIconMenu->addAction(modeAction);
  trayIconMenu->addAction(projectAction);
  trayIconMenu->addAction(checkAction);
  trayIconMenu->addSeparator();

  // Support
  trayIconMenu->addAction(docsAction);
  trayIconMenu->addAction(supportAction);
  trayIconMenu->addAction(logAction);

  trayIconMenu->addSeparator();
  trayIconMenu->addAction(quitAction);

  trayIcon = new QSystemTrayIcon(this);
  trayIcon->setContextMenu(trayIconMenu);
}

void MainWindow::createProjects() {
  projectAction->menu()->clear();
  for (QString k : configs.keys()) {
    if (k == "N/A") {
      continue;
    }
    QAction *p = new QAction(k, this);

    p->setCheckable(true);
    if (k == activeProject) {
      p->setChecked(true);
    }

    projectAction->menu()->addAction(p);
    projectAction->actionGroup()->addAction(p);
  }
  projectAction->setDisabled(projectAction->menu()->isEmpty());

  changeMode(activeMode);
}

void MainWindow::stopAPI() {
  server->stop();

  QVariantMap options;
  options.insert("color", QColor(255, 0, 0));

  statusAction->setText("Vale Server is stopped");
  statusAction->setIcon(awesome->icon(fa::circle, options));

  consoleEdit->setPlainText("Vale Server is stopped.");
  updateStatusBar(false);

  dashAction->setEnabled(false);
  appUpdateAction->setEnabled(false);
}

void MainWindow::setUpAPI() {
  server->set_base_dir(getResource("dashboard"));

  QString port = apiPortEdit->text();
  QFuture<bool> future =
      QtConcurrent::run([=]() { return server->start(port.toInt()); });

  if (future.isRunning()) {
    QVariantMap options;
    options.insert("color", QColor(0, 255, 0));

    statusAction->setText("Vale Server is running");
    statusAction->setIcon(awesome->icon(fa::circle, options));

    updateStatusBar(true);

    consoleEdit->setPlainText(
        "Running.\n\nServer is available at http://localhost:" + port +
        " (bind address 127.0.0.1).\n\nUsing Vale v" +
        this->server->vale->getVersion().toString() + ".");
  } else {
    consoleEdit->setPlainText(
        "Stopped.\n\nUnable to start server; maybe port " + port +
        " is unavailable?");
  }

  dashAction->setEnabled(true);
  appUpdateAction->setEnabled(true);
}

void MainWindow::createGeneralPage() {
  QWidget *page = pages->widget(0);

  // General UI ...
  QLabel *autoupdateLabel = new StyledLabel(
      "Send a notification whenever a new version of Vale Server "
      "becomes available.",
      this);

  QLabel *startLabel = new StyledLabel(
      "Send a notification whenever a new version of an installed "
      "style becomes available.",
      this);

  // Page layout ...
  QVBoxLayout *layout = new QVBoxLayout;

#ifdef Q_OS_LINUX
  autoUpdates->setEnabled(false);
#endif

  layout->addWidget(autoUpdates);
  layout->addWidget(autoupdateLabel);
  layout->addItem(
      new QSpacerItem(0, 8, QSizePolicy::Fixed, QSizePolicy::Fixed));

  layout->addWidget(sysUpdates);
  layout->addWidget(startLabel);
  layout->addItem(
      new QSpacerItem(0, 8, QSizePolicy::Fixed, QSizePolicy::Fixed));

  layout->addStretch();
  layout->setContentsMargins(20, 20, 20, 20);

  page->setLayout(layout);
}

void MainWindow::createProjectsPage() {
  QWidget *page = pages->widget(1);

  // List-level layout:
  QVBoxLayout *listLayout = new QVBoxLayout;

  projects = new QListWidget(this);
  // NOTE: This breaks editing, as the current row isn't necessarily the most
  // recent item.
  //
  // projects->setSortingEnabled(true);
  projects->setAlternatingRowColors(true);

  for (QString k : configs.keys()) {
    QListWidgetItem *item = new QListWidgetItem(k, projects);
    if (k == activeProject) {
      projects->setCurrentItem(item);
    }
    item->setFlags(item->flags() | Qt::ItemIsEditable);
  }

  // A project was re-named.
  connect(projects->itemDelegate(), &QAbstractItemDelegate::commitData,
          [this](QWidget *pLineEdit) {
            QString update = reinterpret_cast<QLineEdit *>(pLineEdit)->text();
            // Remove the old name:
            configs.remove(activeProject);

            // Update the active project:
            changeProject(update);

            configs.insert(activeProject, configEdit->toPlainText());
          });

  // Current project changed:
  connect(projects, &QListWidget::currentItemChanged,
          [this](QListWidgetItem *current, QListWidgetItem *previous) {
            if (previous) {
              configs.insert(activeProject, configEdit->toPlainText());
              previous->setSelected(false);
            }
            if (current) {
              changeProject(current->text());
              QString ini = configs.value(activeProject);

              configEdit->setPlainText(ini);
              current->setSelected(true);
            } else {
              changeProject("N/A");
              configEdit->clear();
            }
            saveSettings();
          });

  QPushButton *addBtn = new QPushButton("Add", this);
  connect(addBtn, &QPushButton::clicked, [this]() {
    int idx = projects->count();
    changeProject("Untitled " + QString::number(idx));

    projects->addItem(activeProject);
    QListWidgetItem *item = projects->item(idx);
    projects->setCurrentItem(item);

    item->setFlags(item->flags() | Qt::ItemIsEditable);
    item->setSelected(true);

    QString temp = server->vale->defaultConfig.arg(
        QDir::fromNativeSeparators(server->vale->stylesPath));

    configs.insert(activeProject, temp);
    configEdit->setPlainText(temp);
  });

  QPushButton *rmvBtn = new QPushButton("Remove", this);
  connect(rmvBtn, &QPushButton::clicked, [this]() {
    QListWidgetItem *it = projects->takeItem(projects->currentRow());
    configs.remove(it->text());
    delete it;
    if (projects->count() > 0) {
      QListWidgetItem *item = projects->currentItem();
      configEdit->setPlainText(configs.value(item->text()));
      item->setSelected(true);
    }
  });

  QHBoxLayout *listBtnLayout = new QHBoxLayout;
  listBtnLayout->addWidget(addBtn);
  listBtnLayout->addWidget(rmvBtn);

  listLayout->addWidget(projects);
  listLayout->addLayout(listBtnLayout);

  highlighter = new INIHighlighter(configEdit->document());

#ifndef Q_OS_LINUX
  QFont fixed = QFontDatabase::systemFont(QFontDatabase::FixedFont);
  fixed.setPointSize(fontSize);
  consoleEdit->setFont(fixed);
#endif

  // Page-level layout:
  QHBoxLayout *inputLayout = new QHBoxLayout;
  inputLayout->addLayout(listLayout);
  inputLayout->addWidget(configEdit);
  inputLayout->setStretch(0, 1);
  inputLayout->setStretch(1, 9);

  page->setLayout(inputLayout);
}

void MainWindow::createAdvancedPage() {
  QWidget *page = pages->widget(2);

  QIntValidator *intValidator = new QIntValidator(1, 65535, this);
  QLabel *fileNameLabel =
      new QLabel(tr("Port (server restart required):"), this);
  apiPortEdit->setValidator(intValidator);

  QPushButton *verifyBtn = new QPushButton("Restart Server", this);
  connect(verifyBtn, &QPushButton::clicked, [this]() {
    stopAPI();
    setUpAPI();
  });

  QHBoxLayout *inputLayout = new QHBoxLayout;
  inputLayout->addWidget(apiPortEdit);
  inputLayout->addWidget(verifyBtn);

  QLabel *consoleLabel = new QLabel(tr("API status:"), this);

#ifndef Q_OS_LINUX
  QFont fixed = QFontDatabase::systemFont(QFontDatabase::FixedFont);
  fixed.setPointSize(fontSize);
  consoleEdit->setFont(fixed);
#endif
  consoleEdit->setReadOnly(true);

  QVBoxLayout *mainLayout = new QVBoxLayout;

  mainLayout->addWidget(fileNameLabel);
  mainLayout->addLayout(inputLayout);
  mainLayout->addWidget(consoleLabel);
  mainLayout->addWidget(consoleEdit);

  page->setLayout(mainLayout);
}

void MainWindow::updateStatusBar(bool running) {
  QStatusBar *statusBar = new QStatusBar(this);
  QHBoxLayout *statusLayout = new QHBoxLayout;

  QLabel *iconLabel = new QLabel(this);
  QLabel *msgLabel = new QLabel(this);

  QVariantMap options;
  if (running) {
    options.insert("color", QColor(0, 240, 0));
    msgLabel->setText("Vale Server is running");

    QString port = apiPortEdit->text();

  } else {
    options.insert("color", QColor(255, 0, 0));
    msgLabel->setText("Vale Server is stopped");
  }
  QIcon sIcon = awesome->icon(fa::circle, options);
  QPixmap pixmap = sIcon.pixmap(QSize(16, 16));
  iconLabel->setPixmap(pixmap);

  statusLayout->addWidget(iconLabel);
  statusLayout->addWidget(msgLabel);

  QWidget *status = new QWidget(this);
  status->setLayout(statusLayout);
  statusBar->addWidget(status);

  setStatusBar(statusBar);
}

void MainWindow::checkClipboard(QString format) {
  DisplayDialog *displayDialog = new DisplayDialog(this);

  QMap<QString, QString> exts{{"Plain Text", ".txt"},
                              {"Markdown", ".md"},
                              {"AsciiDoc", ".adoc"},
                              {"HTML", ".html"},
                              {"reStructuredText", ".rst"}};

  QString alerts = QString::fromStdString(
      server->vale->lint({"--output=line", "--ext=" + exts.value(format),
                          QApplication::clipboard()->text()}));

  displayDialog->setAlerts(alerts);
  displayDialog->showNormal();
}

#endif
