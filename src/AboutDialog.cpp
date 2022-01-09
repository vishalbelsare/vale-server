#include "AboutDialog.h"

#include "ui_AboutDialog.h"

AboutDialog::AboutDialog(QWidget *parent, QString status)
    : QDialog(parent), ui(new Ui::AboutDialog) {
  ui->setupUi(this);

  const QString description =
      QString("<p>%1</p><p>%2</p>")
          .arg(tr("Vale Server is a native, cross-platform desktop application "
                  "for the Vale command-line tool."))
          .arg(tr(
              "It's designed to "
              "allow for seamless integration with third-party apps "
              "(including those that run in a sandboxed environment) "
              "without sacrificing performance, privacy, or extensibility."));

  const QString appInfo = QString(ui->messageLabel->text())
                              .arg(qApp->applicationDisplayName())
                              .arg(qApp->applicationVersion())
                              .arg(status);

  ui->messageLabel->setText(appInfo);
  ui->descriptionLabel->setText(description);
}

AboutDialog::~AboutDialog() { delete ui; }

void AboutDialog::setVisible(const bool visible) {
  QDialog::setVisible(visible);

#ifdef Q_OS_OSX
  showDockIcon(visible);
#endif

  raise();
}

void AboutDialog::closeEvent(QCloseEvent * /* event */) {
#ifdef Q_OS_OSX
  showDockIcon(false);
#endif
}
