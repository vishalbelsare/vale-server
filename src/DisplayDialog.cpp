#include "DisplayDialog.h"

ConsoleHighlighter::ConsoleHighlighter(QTextDocument* parent)
    : QSyntaxHighlighter(parent) {
  HighlightingRule rule;

  errorFormat.setForeground(QColor(215, 58, 73));
  rule.pattern = QRegularExpression("\\b(?:errors?)\\b");
  rule.format = errorFormat;
  highlightingRules.append(rule);

  warningFormat.setForeground(QColor(255, 214, 92));
  rule.pattern = QRegularExpression("\\b(?:warnings?)\\b");
  rule.format = warningFormat;
  highlightingRules.append(rule);

  suggestionFormat.setForeground(QColor(0, 92, 197));
  rule.pattern = QRegularExpression("\\b(?:suggestions?)\\b");
  rule.format = suggestionFormat;
  highlightingRules.append(rule);

  classFormat.setForeground(QColor(111, 66, 193));
  rule.pattern = QRegularExpression("\\d+:\\d+");
  rule.format = classFormat;
  highlightingRules.append(rule);
}

void ConsoleHighlighter::highlightBlock(const QString& text) {
  foreach (const HighlightingRule& rule, highlightingRules) {
    QRegularExpressionMatchIterator matchIterator =
        rule.pattern.globalMatch(text);
    while (matchIterator.hasNext()) {
      QRegularExpressionMatch match = matchIterator.next();
      setFormat(match.capturedStart(), match.capturedLength(), rule.format);
    }
  }
  setCurrentBlockState(0);
}

DisplayDialog::DisplayDialog(QWidget* parent) : QDialog(parent) {
  displayEdit = new QPlainTextEdit(this);
  highlighter = new ConsoleHighlighter(displayEdit->document());

  QFont fixed = QFontDatabase::systemFont(QFontDatabase::FixedFont);
  QFont basic = QFontDatabase::systemFont(QFontDatabase::GeneralFont);
  fixed.setPointSize(basic.pointSize());
  displayEdit->setFont(fixed);
  displayEdit->setReadOnly(true);

  QVBoxLayout* mainLayout = new QVBoxLayout;
  mainLayout->setContentsMargins(0, 0, 0, 0);

  mainLayout->addWidget(displayEdit);

  setLayout(mainLayout);
  setWindowTitle("Alerts");
  resize(500, 300);
}

void DisplayDialog::setVisible(const bool visible) {
  QDialog::setVisible(visible);
#ifdef Q_OS_OSX
  showDockIcon(visible);
#endif
  raise();
}

void DisplayDialog::closeEvent(QCloseEvent* /* event */) {
#ifdef Q_OS_OSX
  showDockIcon(false);
#endif
}

void DisplayDialog::setAlerts(QString alerts) {
  if (alerts.isEmpty()) {
    displayEdit->setPlainText("No alerts found.");
  } else {
    displayEdit->setPlainText(alerts);
  }
}
