#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QLineEdit>
#include <QDir>
#include <QFileSystemModel>
#include <QStringListModel>
#include <QFileDialog>
#include <QMessageBox>
#include <dirent.h>

#include <QAbstractItemView>
#include <QModelIndex>
#include <QColumnView>
#include <QTableView>
#include <QListView>
#include <QPalette>
#include <QBrush>
#include <QPixmap>
#include <QPushButton>

#include <fstream>

#include "myopenssl.h"
#include "sslconf.h"
#include "ModelColumnRO.h"
#include "actions.h"

#include <iostream>
using namespace std;

namespace Ui {
class MainWindow;
}
class Actions;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    ModelColumnRO       *model1;
    ModelColumnRO       *model2;
    QTextEdit*          getFileContent();
public slots:
    void    handleDomainChanged(const QModelIndex & index);
    void    handleFileChanged(const QModelIndex & index);
    void    loadContent(QDir directory);
    void    loadDomainContent(QDir directory);
    void    loadFileContent(QString path);
    void    choose_dir();
    void    reloadContents();
private:
    Ui::MainWindow      *ui;
    QPushButton         *dir_button;
    QLineEdit           *rootDirectory;
    QListView           *domainsList;
    QListView           *domainContent;
    QTextEdit           *fileContent;
    Sslconf             *opensslconf;
    Actions             *actionsMenu;

    void    init_model1();
    void    init_model2();
    void    init_dir_button();
    void    init_rootDirectory();
    void    init_fileContent();
    void    init_domainsList();
    void    init_domainContent();
    void    init_opensslconf();
    void    init_actionsMenu();
};

#endif // MAINWINDOW_H
