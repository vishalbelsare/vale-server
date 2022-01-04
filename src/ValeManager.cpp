#include "ValeManager.h"

#include <QDir>
#include <QFileInfo>
#include <QDebug>
#include <QProcess>
#include <QProcessEnvironment>
#include <QRegularExpression>
#include <QStandardPaths>
#include <QTemporaryFile>
#include <QTextStream>

ValeManager::ValeManager(const QString& path, TextLogger* logger, ManagedProcess *mp) {
  this->logger = logger;
  this->mp = mp;

  QFile f(":/config/.vale.ini");
  if (f.open(QIODevice::ReadOnly)) {
    this->defaultConfig = QString::fromStdString(f.readAll().toStdString());
  } else {
    this->logger->write("Failed to load default config");
  }

#if defined(Q_OS_WIN)
  QString lib = path + "/valelib.exe";
#elif defined(Q_OS_OSX)
  QString lib = path + "/valelib";
#else
  QString lib = path + "/build/valelib";
#endif

  libPath = QFileInfo(lib).absoluteFilePath();

  this->enginePath = QStandardPaths::findExecutable("vale");
  this->stylesPath =
      QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) +
      QDir::separator() + "styles";

  QDir dir;
  if (!dir.exists(stylesPath)) {
    dir.mkpath(stylesPath);
  }

  this->configPath =
      QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) +
      QDir::separator() + "_vale.ini";
}

std::string ValeManager::lint(QStringList args, QString cwd, bool local) {
  std::string ret = "{\"error\": \"timeout\"}";
  if (!local) {
    args.insert(0, "--config=" + configPath);
  }

  QProcess* myProcess = this->mp->buildProcess(cwd);
  myProcess->start(enginePath, args);
  if (!myProcess->waitForFinished(60000)) {
    myProcess->kill();
    myProcess->waitForFinished(-1);
  } else {
    ret = myProcess->readAllStandardOutput().toStdString();
    if (QString::fromStdString(ret).isEmpty()) {
      ret = myProcess->readAllStandardError().toStdString();
    }
  }

  delete myProcess;
  return ret;
}

std::string ValeManager::command(QStringList args) {
  QProcess* myProcess = this->mp->buildProcess("");

  myProcess->start(enginePath, args);
  myProcess->waitForFinished(-1);

  std::string ret = myProcess->readAllStandardOutput().toStdString();
  if (QString::fromStdString(ret).isEmpty()) {
    ret = myProcess->readAllStandardError().toStdString();
  }

  delete myProcess;
  return ret;
}

/**
 * Run the library command with the given arguments.
 */
std::string ValeManager::lib(QStringList args) {
  QProcess* myProcess = this->mp->buildProcess("");

  myProcess->start(libPath, args);
  myProcess->waitForFinished(-1);

  std::string ret = myProcess->readAllStandardOutput().toStdString();
  if (QString::fromStdString(ret).isEmpty()) {
    ret = myProcess->readAllStandardError().toStdString();
  }

  delete myProcess;
  return ret;
}

QVersionNumber ValeManager::getVersion() {
  std::string vs = this->command({"-v"});
  QString vq = QString::fromStdString(vs);

  QStringList parts = vq.split(" ");
  if (parts.size() != 3) {
    return QVersionNumber::fromString("");
  }

  QString final = parts[2];
  if (final.startsWith("v")) {
      final.remove(0, 1);
  }

  return QVersionNumber::fromString(final);
}
