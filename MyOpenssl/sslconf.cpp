#include "sslconf.h"

QTextEdit   *create_qtext(QString value)
{
    QTextEdit   *text = new QTextEdit(value);
    text->setStyleSheet(CONTENT_COLOR);
    text->setMaximumHeight(HEIGHT);
    return (text);
}

QLabel      *create_qlabel(QString text)
{
    QLabel  *label = new QLabel(text);
    label->setStyleSheet(TITLE_STYLE);
    label->setAlignment(Qt::AlignRight | Qt::AlignCenter);
    return (label);
}

Sslconf::Sslconf()
{
    QWidget *obj = qobject_cast<QWidget*>(this);
    obj->setMaximumHeight(CONT_SIZE);

    this->c = create_qtext("FR");
    this->st = create_qtext("France");
    this->l = create_qtext("Paris");
    this->o = create_qtext("Ecritel");
    this->ou = create_qtext("Service Informatique");
    this->cn = create_qtext("www.ecritel.net");
    this->mail = create_qtext("ssl.admin@ecritel.net");
    this->DNScontainer = new QTextEdit("sous-domaine.truc.com");
    this->DNScontainer->setStyleSheet(CONTENT_COLOR);
    this->DNScontainer->append("Create CSR OK");
    this->DNScontainer->append("Autosign OK");
    this->DNScontainer->append("Create CAcert OK");
    this->DNScontainer->append("Remove key passphrase OK");
    this->DNScontainer->append("Create bundles: pfx OK");
    this->DNScontainer->append("pfx xtract cert");
    this->DNScontainer->append("pfx xtract key");
    this->DNScontainer->append("CA sign");
    this->DNScontainer->append("Delete OK");

    QLabel  *c_title = create_qlabel("Country Name (2 letter code)");
    QLabel  *st_title = create_qlabel("State or Province Name (full name)");
    QLabel  *l_title = create_qlabel("Locality Name (eg, city)");
    QLabel  *o_title = create_qlabel("Organization Name (eg, company)");
    QLabel  *ou_title = create_qlabel("Organizational Unit Name (eg, section)");
    QLabel  *cn_title = create_qlabel("Common Name (e.g. server FQDN)");
    QLabel  *mail_title = create_qlabel("Email Address");
    QLabel  *DNS_title = create_qlabel("Additionnal DNS");
    DNS_title->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    DNS_title->setContentsMargins(0,5,0,0);

    QGridLayout     *cont = new QGridLayout(this);
    QGridLayout     *contDNS = new QGridLayout();
    this->container = new QGridLayout();
    cont->addLayout(this->container, 0, 0);
    cont->addLayout(contDNS, 0, 1);

    this->container->addWidget(c_title);
    this->container->addWidget(this->c, 0, 1);
    this->container->addWidget(st_title);
    this->container->addWidget(this->st);
    this->container->addWidget(l_title);
    this->container->addWidget(this->l);
    this->container->addWidget(o_title);
    this->container->addWidget(this->o);
    this->container->addWidget(ou_title);
    this->container->addWidget(this->ou);
    this->container->addWidget(cn_title);
    this->container->addWidget(this->cn);
    this->container->addWidget(mail_title);
    this->container->addWidget(this->mail);

    contDNS->addWidget(DNS_title, 0, 0);
    contDNS->addWidget(this->DNScontainer, 1, 0);
}

Sslconf::~Sslconf()
{
}

