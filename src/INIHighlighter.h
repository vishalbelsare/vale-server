#ifndef INIHIGHLIGHTER_H
#define INIHIGHLIGHTER_H

#include <QRegularExpression>
#include <QSyntaxHighlighter>
#include <QTextCharFormat>

class QTextDocument;

class INIHighlighter : public QSyntaxHighlighter {
  Q_OBJECT

 public:
  INIHighlighter(QTextDocument *parent = 0);

 protected:
  void highlightBlock(const QString &text) override;

 private:
  struct HighlightingRule {
    QRegularExpression pattern;
    QTextCharFormat format;
  };
  QVector<HighlightingRule> highlightingRules;

  QTextCharFormat keywordFormat;
  QTextCharFormat classFormat;
  QTextCharFormat singleLineCommentFormat;
  QTextCharFormat stringFormat;
};

#endif
