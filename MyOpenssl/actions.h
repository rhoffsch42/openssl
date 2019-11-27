#ifndef ACTIONS_H
#define ACTIONS_H

#include "myopenssl.h"
#include "sslconf.h"
#include "mainwindow.h"

#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QTextEdit>
#include <QDir>
#include <QlineEdit>
#include <QInputDialog>
#include <QMessageBox>
#include <QProcess>
#include <QTextStream>
#include <QDateTime>

#include <iostream>
#include <stdexcept>
#include <stdio.h>
#include <string>
using namespace std;

#define BUTTON_BG_COLORT    "#CDE0F9"
#define BUTTON_BG_COLORB    "#003366"
#define BOX_BG_STYLE        "background-color: #CDE0F9"
#define SEPARATOR           "--------------------------------------"
#define OPENSSL_EXE         QDir::currentPath().append("\\openssl\\openssl.exe")
#define KEY(x)              (QString(x).append(".key"))
#define OLDKEY(x)           (QString(x).append(".key.old"))
#define PEM(x)              (QString(x).append(".pem"))
#define CSR(x)              (QString(x).append(".csr"))
#define ROOT_CRT(x)         (QString(x).append("_root.crt"))
#define INTERMEDIATE_CSR(x) (QString(x).append("_intermediate.csr"))
#define INTERMEDIATE_KEY(x) (QString(x).append("_intermediate.key"))
#define INTERMEDIATE_CRT(x) (QString(x).append("_intermediate.crt"))
#define NGINX(x)            (QString(x).append(".bundle.nginx.pem"))
#define HAPROXY(x)          (QString(x).append(".bundle.nginx.pem"))
#define PFX(x)              (QString(x).append(".pfx"))
#define EXTRACT_KEY(x)      (QString(x).append(".extract.key"))
#define EXTRACT_PEM(x)      (QString(x).append(".extract.pem"))
#define VAL(x)              (x->toPlainText())

class MainWindow;

class Actions : public QWidget
{
    Q_OBJECT

public:
                   // Actions();
                    Actions(MainWindow *window);
                    ~Actions();
    void            setLogsConsole(QTextEdit *logs);
    void            setOpensslConf(Sslconf *conf);
public slots:
    void            setRootDirectory(QLineEdit *dir);
private:
    MainWindow      *ui;
    QPushButton     *create_CA_crt;
    QPushButton     *create_csr;
    QPushButton     *delete_crt;
    QPushButton     *create_key_nopass;
    QPushButton     *pfx_extract_crt;
    QPushButton     *pfx_extract_key;
    QPushButton     *CA_sign_crt;
    QPushButton     *autosign_crt;
    QPushButton     *create_bundle;
    QTextEdit       *logs;
    Sslconf         *opensslconf;
    QLineEdit       *rootDirectory;
    QString         execute_process(QString program, QStringList args);
public slots:
    void            handle_create_CA_crt();
    void            handle_create_csr();
    void            handle_delete_crt();
    void            handle_create_key_nopass();
    void            handle_pfx_extract_crt();
    void            handle_pfx_extract_key();
    void            handle_CA_sign_crt();
    void            handle_autosign_crt();
    void            handle_create_bundle();
signals:
    void            fileCreated(QDir dir);
};

#endif // ACTIONS_H
