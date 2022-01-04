#ifndef STYLEDLABEL_H
#define STYLEDLABEL_H

#include <QLabel>
#include <QString>

class StyledLabel: public QLabel
{
    Q_OBJECT
public:
    StyledLabel(QString text, QWidget *parent = nullptr);
};

#endif // STYLEDLABEL_H
