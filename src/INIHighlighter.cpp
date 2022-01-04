#include "INIHighlighter.h"

INIHighlighter::INIHighlighter(QTextDocument *parent) : QSyntaxHighlighter(parent) {
  HighlightingRule rule;

  keywordFormat.setForeground(QColor(215, 58, 73));
  rule.pattern = QRegularExpression("[A-Za-z.]+(?=\\s+=)");
  rule.format = keywordFormat;
  highlightingRules.append(rule);

  classFormat.setForeground(QColor(111, 66, 193));
  rule.pattern = QRegularExpression("^\\[.+\\]$");
  rule.format = classFormat;
  highlightingRules.append(rule);

  singleLineCommentFormat.setForeground(QColor(106, 115, 125));
  rule.pattern = QRegularExpression("#[^\n\"]*");
  rule.format = singleLineCommentFormat;
  highlightingRules.append(rule);

  stringFormat.setForeground(QColor(3, 101, 214));
  rule.pattern = QRegularExpression("[\"'].+['\"]");
  rule.format = stringFormat;
  highlightingRules.append(rule);
}

void INIHighlighter::highlightBlock(const QString &text) {
  foreach (const HighlightingRule &rule, highlightingRules) {
    QRegularExpressionMatchIterator matchIterator =
        rule.pattern.globalMatch(text);
    while (matchIterator.hasNext()) {
      QRegularExpressionMatch match = matchIterator.next();
      setFormat(match.capturedStart(), match.capturedLength(), rule.format);
    }
  }
  setCurrentBlockState(0);
}
