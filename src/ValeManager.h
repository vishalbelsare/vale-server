#ifndef VALEMANAGER_H
#define VALEMANAGER_H

#include <QMap>
#include <QString>
#include <QStringList>
#include <QVersionNumber>

#include "ManagedProcess.h"
#include "TextLogger.h"

class ValeManager {
 public:
  ValeManager(const QString &path, TextLogger *logger, ManagedProcess *mp);

  std::string lint(QStringList args, QString cwd = "", bool local = false);
  std::string command(QStringList args);
  std::string lib(QStringList args);

  QVersionNumber getVersion();

  QString stylesPath;
  QString defaultConfig;

 private:
  QString configPath;
  QString enginePath;
  QString libPath;

  TextLogger *logger;
  ManagedProcess *mp;
};

#endif
