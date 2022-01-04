#include "StyledLabel.h"

#include <QApplication>

StyledLabel::StyledLabel(QString text, QWidget *parent) : QLabel(parent) {
  QPalette palette = QApplication::palette();

  int mode = palette.color(QPalette::WindowText).lightness();
  if (mode == 0) {
    // "Light mode"
    palette.setColor(QPalette::WindowText, QColor(103, 103, 103));
  } else {
    // "Dark mode"
    palette.setColor(QPalette::WindowText, QColor(154, 154, 154));
  }

  this->setText(text);
  this->setWordWrap(true);
  this->setPalette(palette);
  this->setTextFormat(Qt::RichText);
  this->setTextInteractionFlags(Qt::TextBrowserInteraction);
  this->setOpenExternalLinks(true);
  this->setStyleSheet("QLabel {margin-left:15px;}");

  QFont temp = this->font();
#if defined(Q_OS_OSX)
  temp.setPointSize(temp.pointSize() - 1);
#elif defined(Q_OS_WIN)
  temp.setPointSize(8);
#else
  temp.setPointSize(temp.pointSize() - 1);
#endif
  this->setFont(temp);
}
