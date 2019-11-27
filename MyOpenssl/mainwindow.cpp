#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <stdio.h>      /* puts, printf */
#include <time.h>       /* time_t, struct tm, time, localtime */
void    timer()
{
  time_t rawtime;
  struct tm * timeinfo;

  time (&rawtime);
  timeinfo = localtime (&rawtime);

  char  *tim = asctime(timeinfo);
  tim[strlen(tim) - 5] = 0;
  printf ("%s\t", tim + 11);
//  free(tim);
}


void    MainWindow::init_dir_button() {
    this->dir_button = new QPushButton("Change directory");
    this->dir_button->setFont(QFont("Verdana", 8));
    this->dir_button->setCursor(Qt::PointingHandCursor);
    this->dir_button->setIcon(QIcon(QDir::currentPath() + DIR_ICON));

    QString style("");
    style.append("QPushButton{ background: qradialgradient(cx: 0.3, cy: -0.4,");
    style.append("fx: 0.3, fy: -0.4,");
    style.append("radius: 2.35, stop: 0 "+QString(BUTTON_BG_COLORT)+", stop: 1 "+QString(BUTTON_BG_COLORB)+"); }");
    style.append("QPushButton:focus:pressed{ background: qradialgradient(cx: 0.3, cy: -0.4,");
    style.append("fx: 0.3, fy: -0.4,");
    style.append("radius: 2.35, stop: 0 "+QString(BUTTON_BG_COLORB)+", stop: 1 "+QString(BUTTON_BG_COLORT)+"); }");
    style.append("QPushButton{ font-size: 12pt; }");
    this->dir_button->setStyleSheet(style);

    //this->dir_button->setIconSize(QSize(64, 64));
}
void    MainWindow::init_rootDirectory() {
    this->rootDirectory = new QLineEdit(QDir::currentPath().append(ROOT_DIR));
    this->rootDirectory->setStyleSheet(CONTENT_COLOR);
    this->rootDirectory->setReadOnly(true);
    this->rootDirectory->setMaximumHeight(25);
    if (QDir(this->rootDirectory->text()).exists() == false)
        QDir().mkdir(this->rootDirectory->text());
}
void    MainWindow::init_fileContent() {
    this->fileContent = new QTextEdit();
    this->fileContent->setReadOnly(true);
  //  this->fileContent->setStyleSheet("background-image: url(release/images/grey.jpg)");
    this->fileContent->setStyleSheet(CONTENT_COLOR);
    this->fileContent->setMaximumWidth(450);
}
void    MainWindow::init_model1() {
    this->model1 = new ModelColumnRO(0);
    this->model2 = new ModelColumnRO(0);
    this->loadContent(QDir(this->rootDirectory->text()));
    if (this->model1->getContent()->count() != 0)
    {
        this->loadDomainContent(QDir(this->rootDirectory->text() + "/" + this->model1->getContent()->at(0)));
        if (this->model2->getContent()->count() != 0)
            this->loadFileContent(QString(this->rootDirectory->text() + "/" + this->model1->getContent()->at(0) + "/" + this->model2->getContent()->at(0)));
    }
}
void    MainWindow::init_domainsList() {
    this->domainsList = new QListView();
    this->domainsList->setMovement(QListView::Static);
    this->domainsList->setResizeMode(QListView::Adjust);
    this->domainsList->setMaximumWidth(MODEL_W);
    this->domainsList->setModel(this->model1);
    this->domainsList->setStyleSheet(CONTENT_COLOR);
    QModelIndex index = this->model1->index(0,0);
    this->domainsList->selectionModel()->select(index, QItemSelectionModel::SelectCurrent);
}
void    MainWindow::init_domainContent() {
    this->domainContent = new QListView();
    this->domainContent->setMovement(QListView::Static);
    this->domainContent->setResizeMode(QListView::Adjust);
    this->domainContent->setMaximumWidth(MODEL_W);
    this->domainContent->setSelectionMode(QAbstractItemView::ExtendedSelection);
    this->domainContent->setModel(this->model2);
    this->domainContent->setStyleSheet(CONTENT_COLOR);
    QModelIndex index = this->model2->index(0,0);
    this->domainContent->selectionModel()->select(index, QItemSelectionModel::SelectCurrent);
}
void    MainWindow::init_opensslconf() {
    this->opensslconf = new Sslconf();
}
void    MainWindow::init_actionsMenu() {
    this->actionsMenu = new Actions(this);
    this->actionsMenu->setOpensslConf(this->opensslconf);
    this->actionsMenu->setRootDirectory(this->rootDirectory);
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    this->setWindowTitle(APP_TITLE);
    this->setMinimumSize(QSize(800, 600));
    this->showMaximized();

    this->init_dir_button();
    QWidget::connect(this->dir_button, SIGNAL(clicked()),
                     this, SLOT(choose_dir()));
    this->init_rootDirectory();
    this->init_fileContent();
    this->init_model1();
    this->init_domainsList();
    this->init_domainContent();
    connect(this->domainsList, SIGNAL(clicked(const QModelIndex &)),
            this, SLOT(handleDomainChanged( const QModelIndex &)));
    connect(this->domainContent, SIGNAL(clicked(const QModelIndex &)),
            this, SLOT(handleFileChanged( const QModelIndex &)));
    this->init_opensslconf();
    this->init_actionsMenu();
    connect(this->actionsMenu, SIGNAL(fileCreated(QDir)),
            this, SLOT(loadContent(QDir)));
    connect(this->actionsMenu, SIGNAL(fileCreated(QDir)),
            this, SLOT(reloadContents()));

    /*Widgets*/
    QWidget     *cont1 = new QWidget();
    this->setStyleSheet(MAIN_BG_COLOR);

    QVBoxLayout *global_layout = new QVBoxLayout();
    QHBoxLayout *dir_layout = new QHBoxLayout();
    QHBoxLayout *main_layout = new QHBoxLayout();

    //creation layouts
    this->setCentralWidget(cont1);
    cont1->setLayout(global_layout);
    global_layout->addLayout(dir_layout);
    dir_layout->addWidget(this->dir_button);
    dir_layout->addWidget(this->rootDirectory);
    global_layout->addLayout(main_layout);

    main_layout->addWidget(this->domainsList);
    main_layout->addWidget(this->domainContent);
    main_layout->addWidget(this->fileContent);

    QVBoxLayout     *menuLayout = new QVBoxLayout();
    menuLayout->setAlignment(Qt::AlignTop);
    QTextEdit   *logs = new QTextEdit();
    logs->setStyleSheet(CONTENT_COLOR);
    this->actionsMenu->setLogsConsole(logs);

    menuLayout->addWidget(qobject_cast<QWidget*>(this->opensslconf));
    menuLayout->addWidget(qobject_cast<QWidget*>(this->actionsMenu));
    menuLayout->setAlignment(this->opensslconf, Qt::AlignTop);
    menuLayout->setAlignment(this->actionsMenu, Qt::AlignTop);
    menuLayout->addWidget(logs);
    main_layout->addLayout(menuLayout);

    cout << "mainwindow end constructor" << endl;
}

void    MainWindow::reloadContents() {
    timer();
    cout << "MainWindow::reloadContents - " << endl;
    if (this->model1->getContent()->count() != 0)
    {
        QModelIndex index = this->model1->index(0,0);
        this->domainsList->selectionModel()->select(index, QItemSelectionModel::SelectCurrent);
        this->handleDomainChanged(index);
    }
}

void    MainWindow::handleDomainChanged( const QModelIndex & index) {
    timer();
    cout << "MainWindow::handleDomainChanged - " << qPrintable(this->rootDirectory->text() + "/" + this->model1->getContent()->at(index.row())) << endl;
    this->model1->setN(index.row());
    this->loadDomainContent(QDir(this->rootDirectory->text() + "/" + this->model1->getContent()->at(index.row())));
    this->fileContent->clear();
    QModelIndex index2 = this->model2->index(0,0);
    this->domainContent->selectionModel()->select(index2, QItemSelectionModel::SelectCurrent);
    this->handleFileChanged(index2);
}

void    MainWindow::handleFileChanged( const QModelIndex & index) {
    timer();
    cout << "MainWindow::handleFileChanged( const QModelIndex & index) - " << endl;
    cout << this->model2->getContent()->count() << " - " << index.row() << endl;
    if (this->model2->getContent()->count() > index.row() && index.row() != -1)
    {
        this->model2->setN(index.row());
        QString     filename = QString(this->rootDirectory->text() + "/");
        filename.append(this->model1->getContent()->at(this->model1->getN()));
        filename.append("/");
        filename.append(this->model2->getContent()->at(index.row()));
        this->loadFileContent(filename);
    }
}

void    MainWindow::loadContent(QDir directory) {
    timer();
    cout << "MainWindow::loadContent(QDir directory) - " << qPrintable(directory.absolutePath()) << endl;
    this->model1->setN(0);
    this->model1->setStringList(new QStringList(directory.entryList(QDir::NoDotAndDotDot | QDir::AllDirs, QDir::DirsFirst)));
}

void    MainWindow::loadDomainContent(QDir directory) {
    timer();
    cout << "MainWindow::loadDomainContent(QDir directory) - " << qPrintable(directory.absolutePath()) << endl;
    this->model2->setN(0);
    QStringList filters;
    filters << "*.csr" << "*.pem" << "*.org" << "*.pfx" << "*.key" << "*.crt" << "*.old";
    this->model2->setStringList(new QStringList(directory.entryList(filters, QDir::Files)));
}

void    MainWindow::loadFileContent(QString path) {
    timer();
    cout << "MainWindow::loadFileContent(QString path) - " << qPrintable(path) << endl;
    this->fileContent->clear();
    string getcontent;
    ifstream    openfile(path.toStdString().c_str());
    if(openfile.is_open())
    {
        QString content = "<font color=\"#002500\">";
        while(!openfile.eof())
        {
            getline(openfile, getcontent);
            content.append(QString(getcontent.c_str()));
            content.append("<br>");
        }
        content.append("</font><br>");
        this->fileContent->insertHtml(content);
    }
    else
        cout << "fuck" << endl;
}

void    MainWindow::choose_dir() {
    QString dir = QFileDialog::getExistingDirectory(this);
    if (dir.isEmpty())
        QMessageBox::critical(this, "Directory: error", "Nothing selected");
    else {
        this->rootDirectory->setText(dir);
        QPalette palette;
        palette.setColor(QPalette::Text, Qt::red);
        DIR*    rep = NULL;
        if ((rep = opendir(qPrintable(this->rootDirectory->text()))) == NULL) {
            this->rootDirectory->setPalette(palette);
        } else {
            palette.setColor(QPalette::Text, Qt::black);
            this->rootDirectory->setPalette(palette);
            closedir(rep);
        }
        this->loadContent(QDir(this->rootDirectory->text()));
        this->fileContent->clear();
        this->model2->clear();
        this->reloadContents();
        //QMessageBox::information(this, "Directory", "You selected :\n" + dir_description->text());
    }
}

QTextEdit*          MainWindow::getFileContent() {  return this->fileContent;   }

MainWindow::~MainWindow(){
    delete ui;
}
