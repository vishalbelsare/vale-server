#ifndef MANAGEDPROCESS_H
#define MANAGEDPROCESS_H

#include <QProcess>
#include <QObject>

class ManagedProcess : public QObject
{

public:
    ManagedProcess(QObject *parent);

    QString runProc(QString exe, QString path, QStringList args);
    QProcess *buildProcess(QString cwd, QObject *parent = nullptr);
};

#endif // MANAGEDPROCESS_H
