#ifndef SSLCONF_H
#define SSLCONF_H

#include "myopenssl.h"

#include <QWidget>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QLabel>

#define HEIGHT      23
#define CONT_SIZE   7*(HEIGHT+8)
#define TITLE_STYLE "QLabel { color : #CDE0F9; font-weight: bold; }"

class Sslconf : public QWidget
{
    Q_OBJECT

public:
    Sslconf();
    ~Sslconf();

    QGridLayout         *container;
    QTextEdit           *c;
    QTextEdit           *st;
    QTextEdit           *l;
    QTextEdit           *o;
    QTextEdit           *ou;
    QTextEdit           *cn;
    QTextEdit           *mail;
    QTextEdit           *DNScontainer;
};

#endif // SSLCONF_H
