#include "ManagedProcess.h"

ManagedProcess::ManagedProcess(QObject *parent): QObject(parent) {}

QProcess *ManagedProcess::buildProcess(QString cwd, QObject *parent) {
  QProcess *myProcess = new QProcess(parent);

  if (!cwd.isEmpty()) {
    myProcess->setWorkingDirectory(cwd);
  }

  return myProcess;
}

QString ManagedProcess::runProc(QString exe, QString path, QStringList args) {
  QProcess *proc = this->buildProcess(path, nullptr);
  proc->start(exe, args);

  proc->waitForFinished(-1);
  QString output(proc->readAllStandardOutput());

  delete proc;
  return output;
}
