#ifndef DISPLAYDIALOG_H
#define DISPLAYDIALOG_H

#include <QDialog>
#include <QFontDatabase>
#include <QPlainTextEdit>
#include <QRegularExpression>
#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QVBoxLayout>

#ifdef Q_OS_OSX
#include "darwin/mac.h"
#endif

class QTextDocument;

class ConsoleHighlighter : public QSyntaxHighlighter {
  Q_OBJECT

 public:
  ConsoleHighlighter(QTextDocument *parent = 0);

 protected:
  void highlightBlock(const QString &text) override;

 private:
  struct HighlightingRule {
    QRegularExpression pattern;
    QTextCharFormat format;
  };
  QVector<HighlightingRule> highlightingRules;

  QTextCharFormat errorFormat;
  QTextCharFormat warningFormat;
  QTextCharFormat suggestionFormat;
  QTextCharFormat classFormat;
  QTextCharFormat singleLineCommentFormat;
};

class DisplayDialog : public QDialog {
  Q_OBJECT

 public:
  explicit DisplayDialog(QWidget *parent = 0);

  void setVisible(bool visible) Q_DECL_OVERRIDE;
  void setAlerts(QString alerts);

 private:
  QPlainTextEdit *displayEdit;
  ConsoleHighlighter *highlighter;

 protected:
  void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;
};

#endif
