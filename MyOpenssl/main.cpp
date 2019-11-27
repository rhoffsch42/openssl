#include "mainwindow.h"
#include <QApplication>
#include "myopenssl.h"

int main(int argc, char *argv[])
{
    cout << "cwd:\n" << qPrintable(QDir::currentPath()) << endl;
    QApplication    a(argc, argv);
    MainWindow      w;
    w.show();

    return a.exec();
}


/*  -pas possible de differencier un dossier normal de rangement, d'un dossier de certificat
 *      creer un dossier propre a notre application avec une extension perso .ssl pour que QTreeFile ou autre QView ne le prenne pas pour un dossier.
 *  -fichier de conf: utilise actuellement une varaiable d'environnement system
 *      passer avec flag -config path_of_conf_file
 *      editer/generer ce file selon les infos fournies
 *  -bundles
 *      verifier que la key et le pem existent et match bien
 *      faire choisir un CA, puis type de certificat (Alpha, Domain, Organization, Extended) pour prendre les intermediaire et root correct
 *  -key passphrase: le faire automatiquement dans Create CSR
 *  -bug model2: pareil que model1: si index = x et qu'on charge un autre model avec moins d'index => plantage
 *  -extract pfx: message de confirmation si ***.extract.key/pem existe deja
 */
