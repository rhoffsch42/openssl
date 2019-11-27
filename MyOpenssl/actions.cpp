#include "actions.h"

QPushButton     *create_button(QString name)
{
    QPushButton *button = new QPushButton(name);
    button->setMinimumSize(100,50);
    QString style("");

    style.append("QPushButton{ background: qradialgradient(cx: 0.3, cy: -0.4,");
    style.append("fx: 0.3, fy: -0.4,");
    style.append("radius: 2.35, stop: 0 "+QString(BUTTON_BG_COLORT)+", stop: 1 "+QString(BUTTON_BG_COLORB)+"); }");

    //style.append("QPushButton:disabled{ background-color: yellow; }");
    //style.append("QPushButton:pressed{ background-color: orange; }");

    style.append("QPushButton:focus:pressed{ background: qradialgradient(cx: 0.3, cy: -0.4,");
    style.append("fx: 0.3, fy: -0.4,");
    style.append("radius: 2.35, stop: 0 "+QString(BUTTON_BG_COLORB)+", stop: 1 "+QString(BUTTON_BG_COLORT)+"); }");

    //style.append("QPushButton:focus{ background-color: green; }");
    //style.append("QPushButton:hover{ background-color: cyan; }");

    style.append("QPushButton{ font-size: 12pt; }");

    button->setStyleSheet(style);
    return (button);
}

std::string exec(const char* cmd) {
    char buffer[128];
    std::string result = "";
    FILE* pipe = popen(cmd, "r");
    if (!pipe) throw std::runtime_error("popen() failed!");
    try {
        while (!feof(pipe)) {
            if (fgets(buffer, 128, pipe) != NULL)
                result += buffer;
        }
    } catch (...) {
        pclose(pipe);
        throw;
    }
    pclose(pipe);
    return result;
}
bool        fileExists(QString path) {
    QFileInfo check_file(path);
    return (check_file.exists() && check_file.isFile());
}
QString     Actions::execute_process(QString program, QStringList args)
{
    QProcess    process;
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    cout << "Bimbadaboum:\t" << qPrintable(QDir::currentPath()) << endl;
    env.insert("OPENSSL_CONF", QDir::currentPath() + "\\openssl\\openssl.conf"); // Add an environment variable (ce sera le dossier courrant une fois le programme terminé)
    process.setProcessEnvironment(env);
    //process.setProcessChannelMode(QProcess::MergedChannels);
    process.start(program, args);
    process.waitForFinished(-1);
    QString output(process.readAllStandardOutput());

    if (this->logs)
    {
        for (int i = 0; i < args.size(); ++i)
            program.append(" ").append(args.at(i));
        this->logs->setTextColor("black");
        this->logs->append(program);
        this->logs->setTextColor("gray");
        this->logs->append(output);
        this->logs->setTextColor("black");
        this->logs->append(QString(SEPARATOR));
    }
    return output;
}

Actions::Actions(MainWindow *window) : ui(window)
{
    QGridLayout *lay = new QGridLayout(this);

    this->create_CA_crt = create_button("Create CA certificate");
    this->create_csr = create_button("Create CSR");
    this->delete_crt = create_button("Delete");
    this->create_key_nopass = create_button("Remove key passphrase");
    this->pfx_extract_crt = create_button("pfx: Extract certificate");
    this->pfx_extract_key = create_button("pfx: Extract key");
    this->CA_sign_crt = create_button("CA sign certificate");
    this->autosign_crt = create_button("Autosign certificate");
    this->create_bundle = create_button("Create bundles:\nWindows:yes Nginx:no HAProxy:no");

    this->CA_sign_crt->setEnabled(false);
  //  this->pfx_extract_crt->setEnabled(false);
  //  this->pfx_extract_key->setEnabled(false);

    this->create_csr->setIcon(QIcon(QDir::currentPath() + CREATE_ICON));
    this->delete_crt->setIcon(QIcon(QDir::currentPath() + DEL_ICON));
    this->create_CA_crt->setIcon(QIcon(QDir::currentPath() + CA_ICON));
    this->pfx_extract_crt->setIcon(QIcon(QDir::currentPath() + WINDOWS_ICON));
    this->pfx_extract_key->setIcon(QIcon(QDir::currentPath() + WINDOWS_ICON));

    lay->addWidget(this->create_csr, 0, 0);
    lay->addWidget(this->autosign_crt, 1, 0);
    lay->addWidget(this->create_key_nopass, 2, 0);
    lay->addWidget(this->create_bundle, 0, 1);
    lay->addWidget(this->delete_crt, 2, 2);
    lay->addWidget(this->pfx_extract_crt, 1, 1);
    lay->addWidget(this->pfx_extract_key, 2, 1);
    lay->addWidget(this->create_CA_crt, 0, 2);
    lay->addWidget(this->CA_sign_crt, 1, 2);

    connect(this->create_CA_crt, SIGNAL (released()), this, SLOT (handle_create_CA_crt()));
    connect(this->create_csr, SIGNAL (released()), this, SLOT (handle_create_csr()));
    connect(this->delete_crt, SIGNAL (released()), this, SLOT (handle_delete_crt()));
    connect(this->create_key_nopass, SIGNAL (released()), this, SLOT (handle_create_key_nopass()));
    connect(this->pfx_extract_crt, SIGNAL (released()), this, SLOT (handle_pfx_extract_crt()));
    connect(this->pfx_extract_key, SIGNAL (released()), this, SLOT (handle_pfx_extract_key()));
    connect(this->CA_sign_crt, SIGNAL (released()), this, SLOT (handle_CA_sign_crt()));
    connect(this->autosign_crt, SIGNAL (released()), this, SLOT (handle_autosign_crt()));
    connect(this->create_bundle, SIGNAL (released()), this, SLOT (handle_create_bundle()));

 //   system("set OPENSSL_CONF=C:\\Users\\rhoffschir-cubieres\\Desktop\\openssl\\openssl.conf");
}

void    Actions::handle_create_CA_crt()
{
    cout << qPrintable(this->rootDirectory->text()) << endl;
    QString dirname = QInputDialog::getText(this, "Create new Certificate Signing Request", "Choose a name");
    if (dirname.isEmpty())
        QMessageBox::critical(this, "Directory: error", "Nothing selected");
    else if (QDir(this->rootDirectory->text() + "/" + dirname).exists()) {
        QMessageBox::critical(this, "Directory: error", "\""+ dirname +"\" already exists");
    }
    else {
        QString     file = this->rootDirectory->text() + "\\" + dirname ;
        file.replace('/', '\\');
        QDir().mkdir(file);
        file.append("\\" + dirname);
        QString     root_key = KEY(file);
        QString     root_crt = ROOT_CRT(file);
        QString     intermediate_csr = INTERMEDIATE_CSR(file);
        QString     intermediate_key = INTERMEDIATE_KEY(file);
        QString     intermediate_crt = INTERMEDIATE_CRT(file);

        QStringList arguments;
        arguments << "genrsa" << "-aes256" << "-passout" << "pass:tototo" << "-out" << root_key << "4096";
        QString output = execute_process(OPENSSL_EXE, arguments);
        arguments.clear();
        arguments << "req" << "-key" << root_key << "-new" << "-x509" << "-days" << "7300" << "-sha256" << "-extensions" << "v3_ca" << "-passin" << "pass:tototo" << "-out" << root_crt << "-subj" << "/C=FR/ST=FRANCE/O=ECRITEL_Root_CA/OU=sectionWeb/CN=ecritel.net";
        output = execute_process(OPENSSL_EXE, arguments);

        arguments.clear();
        arguments << "genrsa" << "-des3" << "-passout" << "pass:tototo" << "-out" << intermediate_key;
        output = execute_process(OPENSSL_EXE, arguments);
        arguments.clear();
        arguments << "req" << "-new" << "-key" << intermediate_key << "-passin" << "pass:tototo" << "-out" << intermediate_csr << "-subj" << "/C=FR/CN=FRANCE/O=ECRITELO/OU=Web/CN=ecri.net";
        output = execute_process(OPENSSL_EXE, arguments);
        arguments.clear();
        arguments << "x509" << "-req" << "-days" << "365" << "-passin" << "pass:tototo" << "-in" << intermediate_csr << "-CA" << root_crt << "-CAkey" << root_key << "-set_serial" << "01" << "-out" << intermediate_crt;
        output = execute_process(OPENSSL_EXE, arguments);
        emit this->fileCreated(QDir(this->rootDirectory->text()));
    }
}
void    Actions::handle_create_csr()
{
    QString dirname = QInputDialog::getText(0, "Create new Certificate Signing Request", "Choose a name");

 /*   QMessageBox box;
    box.setStyleSheet(BOX_BG_STYLE);
    box.setText("Nothing selected");
    box.setWindowTitle("Directory: error");
  */
    if (dirname.isEmpty())
        QMessageBox::critical(0, "Directory: error", "Nothing selected");
    else if (QDir(this->rootDirectory->text() + "/" + dirname).exists())
        QMessageBox::critical(0, "Directory: error", "\""+ dirname +"\" already exists");
    else {
        QString     file = this->rootDirectory->text() + "\\" + dirname ;
        file.replace('/', '\\');
        QDir().mkdir(file);
        file.append("\\" + dirname);
        QString     key = KEY(file);
        QString     csr = CSR(file);
        Sslconf     *p = this->opensslconf;

        QString subj = QString("/C=").append(VAL(p->c)).append("/ST=").append(VAL(p->st)).append("/L=").append(VAL(p->l)).append("/O=").append(VAL(p->o)).append("/OU=").append(VAL(p->ou)).append("/CN=").append(VAL(p->cn)).append("/emailAddress=").append(VAL(p->mail)).append("/");
        QStringList arguments;
        arguments << "req" << "-newkey" << "rsa:2048" << "-nodes" << "-sha256" << "-keyout" << key << "-subj" << subj;

        QString output = execute_process(OPENSSL_EXE, arguments);

        QFile ouputfile(csr);
        if ( ouputfile.open(QIODevice::Truncate | QIODevice::ReadWrite) )
        {
            QTextStream stream(&ouputfile);
            stream << output;
        }
        else
            QMessageBox::critical(0, "Error", QString("Error while creating the file."), QMessageBox::Ok);
        emit this->fileCreated(QDir(this->rootDirectory->text()));
    }
}
void    Actions::handle_delete_crt()
{
    QString     cert(this->ui->model1->getContent()->value(this->ui->model1->getN()));
    QMessageBox::StandardButton reply = QMessageBox::critical(0, "Critical operation", QString("Delete \"").append(cert).append("\" and its content ?"),
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        QString original = this->rootDirectory->text() + "/" + cert;
        QString dest = QDir::currentPath() + "/Recycle/";
        if (QDir(dest).exists() == false)
            QDir().mkdir(dest);
        QString  time = QDateTime::currentDateTime().toString("yyyy.MM.dd.hh.mm.ss");
        dest.append(cert).append("_").append(time);
        cout << "original: " << qPrintable(original) << endl;
        cout << "dest: " << qPrintable(dest) << endl;
        QDir dir;
        if(!dir.rename(original, dest))
            cout << "move failed" << endl;

  //      QDir(this->rootDirectory->text() + "/" + cert).removeRecursively();
        this->ui->model2->clear();
        this->ui->getFileContent()->clear();
        emit this->fileCreated(QDir(this->rootDirectory->text()));
    }
    return ;

}
void    Actions::handle_create_key_nopass()
{
    QString     cert(this->ui->model1->getContent()->value(this->ui->model1->getN()));
    QString     file = this->rootDirectory->text() + "\\" + cert;
    file.replace('/', '\\');
    file.append("\\" + cert);
    QString     key = KEY(file);
    QString     oldkey = OLDKEY(file);

    if (!fileExists(key))
        QMessageBox::critical(0, "Directory: error", "\"" + QString(cert).append(".key") + "\" is missing.");
    else {
        QStringList arguments;
        arguments << "rsa" << "-in" << key;
        QString output = execute_process(OPENSSL_EXE, arguments);

        QDir dir;
        if(!dir.rename(key, oldkey))
            cout << "handle_create_key_nopass : move failed" << endl;

        QFile ouputfile(key);
        if ( ouputfile.open(QIODevice::Truncate | QIODevice::ReadWrite) )
        {
            QTextStream stream(&ouputfile);
            stream << output;
        }
        else
            QMessageBox::critical(0, "Error", QString("Error while creating the file."), QMessageBox::Ok);
        emit this->fileCreated(QDir(this->rootDirectory->text()));
    }
}
void    Actions::handle_pfx_extract_crt()
{
    QString     cert(this->ui->model1->getContent()->value(this->ui->model1->getN()));
    QString     file = this->rootDirectory->text() + "\\" + cert;
    file.replace('/', '\\');
    file.append("\\" + cert);
    QString     pfx = PFX(file);
    QString     extract_pem = EXTRACT_PEM(file);

    if (!fileExists(pfx))
    {
        QMessageBox::critical(0, "Error", QString("Error: \"" + pfx + "\" is missing."), QMessageBox::Ok);
        return ;
    }
    QString     password = QString("pass:").append(QInputDialog::getText(0, "Password required", "Enter the pfx's password :"));

    QStringList arguments;
    arguments << "pkcs12" << "-clcerts" << "-nokeys" << "-passin" << password << "-in" << pfx;
    QString output = execute_process(OPENSSL_EXE, arguments);

    QFile ouputfile(extract_pem);
    if ( ouputfile.open(QIODevice::Truncate | QIODevice::ReadWrite) )
    {
        QTextStream stream(&ouputfile);
        stream << output;
    }
    else
        QMessageBox::critical(0, "Error", QString("Error while creating the file."), QMessageBox::Ok);
    emit this->fileCreated(QDir(this->rootDirectory->text()));
}
void    Actions::handle_pfx_extract_key()
{
    QString     cmd("openssl.exe pkcs12 -in test.pfx -nocerts -out test.key2 -nodes");

    QString     cert(this->ui->model1->getContent()->value(this->ui->model1->getN()));
    QString     file = this->rootDirectory->text() + "\\" + cert;
    file.replace('/', '\\');
    file.append("\\" + cert);
    QString     pfx = PFX(file);
    QString     extract_key = EXTRACT_KEY(file);

    if (!fileExists(pfx))
    {
        QMessageBox::critical(0, "Error", QString("Error: \"" + pfx + "\" is missing."), QMessageBox::Ok);
        return ;
    }
    QString     password = QString("pass:").append(QInputDialog::getText(0, "Password required", "Enter the pfx's password :"));

    QStringList arguments;
    arguments << "pkcs12" << "-nocerts" << "-nodes" << "-passin" << password << "-in" << pfx;
    QString output = execute_process(OPENSSL_EXE, arguments);

    QFile ouputfile(extract_key);
    if ( ouputfile.open(QIODevice::Truncate | QIODevice::ReadWrite) )
    {
        QTextStream stream(&ouputfile);
        stream << output;
    }
    else
        QMessageBox::critical(0, "Error", QString("Error while creating the file."), QMessageBox::Ok);
    emit this->fileCreated(QDir(this->rootDirectory->text()));
}
void    Actions::handle_CA_sign_crt()
{
    QString     cmd("openssl.exe x509 -req -passin pass:tototo -in test2.csr -out test2CA.crt -CA CA.crt -CAkey CA.key -CAcreateserial -days 365");

    this->logs->setTextColor("black");
    this->logs->append(cmd);

    QString     result(exec(qPrintable(cmd)).c_str());
    this->logs->setTextColor("gray");
    this->logs->append(result);

    this->logs->append(QString("CAsign"));

    this->logs->setTextColor("black");
    this->logs->append(QString(SEPARATOR));
}
void    Actions::handle_autosign_crt()
{
    QString     cmd("openssl.exe x509 -req -days 365 -in test2.csr -signkey test2.key -out test2.pem");
    QString     cert(this->ui->model1->getContent()->value(this->ui->model1->getN()));

    QString     file = this->rootDirectory->text() + "\\" + cert;
    file.replace('/', '\\');
    file.append("\\" + cert);
    QString     csr = CSR(file);
    QString     key = KEY(file);
    QString     pem = PEM(file);

    if (!fileExists(csr))
    {
        QMessageBox::critical(0, "Error", QString("Error: \"" + csr + "\" is missing."), QMessageBox::Ok);
        return ;
    }
    if (!fileExists(key))
    {
        QMessageBox::critical(0, "Error", QString("Error: \"" + key + "\" is missing."), QMessageBox::Ok);
        return ;
    }
    QStringList arguments;
    arguments << "x509" << "-req" << "-days" << "365" << "-in" << csr << "-signkey" << key;
    QString output = execute_process(OPENSSL_EXE, arguments);

    QFile ouputfile(pem);
    if ( ouputfile.open(QIODevice::Truncate | QIODevice::ReadWrite) )
    {
        QTextStream stream(&ouputfile);
        stream << output;
    }
    else
        QMessageBox::critical(0, "Error", QString("Error while creating the file."), QMessageBox::Ok);
    emit this->fileCreated(QDir(this->rootDirectory->text()));
}
void    Actions::handle_create_bundle()
{
    QString     cmd_windows("openssl pkcs12 -export -inkey $key -in $pem -out $pfx -passout pass:");
    QString     cmd_nginx("cat $site_cert $CA_inter > $site_path/$site_name$suffix");
    QString     cmd2_haproxy("cat $site_cert $CA_inter $CA_root $site_key > $site_path/$site_name$suffix");

    QString     cert(this->ui->model1->getContent()->value(this->ui->model1->getN()));

    QString     file = this->rootDirectory->text() + "\\" +  cert;
    file.replace('/', '\\');
    file.append("\\" + cert);
    QString     key = KEY(file);
    QString     pem = PEM(file);
    QString     nginx = NGINX(file);
    QString     haproxy = HAPROXY(file);
    QString     pfx = PFX(file);

    cout << qPrintable(pfx) << endl;
    QStringList arguments;
    arguments << "pkcs12" << "-export" << "-inkey" << key << "-in" << pem << "-out" << pfx << "-passout" << "pass:";
    QString output = execute_process(OPENSSL_EXE, arguments);

    emit this->fileCreated(QDir(this->rootDirectory->text()));
}

void    Actions::setLogsConsole(QTextEdit *logs)
{
    this->logs = logs;
    this->logs->setReadOnly(true);
}

void    Actions::setOpensslConf(Sslconf *conf)
{
    this->opensslconf = conf;
}

void    Actions::setRootDirectory(QLineEdit *dir)
{
    this->rootDirectory = dir;
}

Actions::~Actions()
{
}
