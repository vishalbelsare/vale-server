#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>

#ifdef Q_OS_OSX
#include "darwin/mac.h"
#endif

namespace Ui {
class AboutDialog;
}

class AboutDialog : public QDialog {
  Q_OBJECT

 public:
  explicit AboutDialog(QWidget *parent, QString status);
  ~AboutDialog() Q_DECL_OVERRIDE;

  void setVisible(bool visible) Q_DECL_OVERRIDE;

 protected:
  void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

 private:
  Ui::AboutDialog *ui;
};

#endif  // ABOUTDIALOG_H
