#include "TextLogger.h"

#include <QDir>
#include <QStandardPaths>
#include <QTextStream>

TextLogger::TextLogger(QObject *parent) : QObject(parent) {
  m_showDate = true;

  QString log =
      QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) +
      QDir::separator() + "vale-server.log";

  file = new QFile;
  file->setFileName(log);
  file->open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text);
}

void TextLogger::write(const QString &value) {
  QString text = value;  // + "";
  if (m_showDate)
    text = QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss ") + text;
  QTextStream out(file);
  if (file != 0) {
    out << text << Qt::endl;
  }
}

void TextLogger::setShowDateTime(bool value) { m_showDate = value; }

TextLogger::~TextLogger() {
  if (file != 0) {
    file->close();
    delete file;
  }
}
