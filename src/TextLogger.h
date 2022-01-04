#ifndef TEXTLOGGER_H
#define TEXTLOGGER_H

#include <QDateTime>
#include <QFile>
#include <QObject>
#include <QTextStream>

class TextLogger : public QObject {
  Q_OBJECT
 public:
  explicit TextLogger(QObject *parent);
  ~TextLogger();
  void setShowDateTime(bool value);

 private:
  QFile *file;
  bool m_showDate;

 signals:

 public slots:
  void write(const QString &value);
};

#endif
