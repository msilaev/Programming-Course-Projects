/* Yatzy game for several players written using GUI in Qt.
 *
 * Desc:
 *   This program implements a Yatzy game for several players.
 *   There are buttons allowing to set number of players, roll,
 *   transit and determining the winner "Quit".
 *
 *
 * Program author
 * Name: Mikhail Silaev
 * Student number: 151861415
 * UserID: xdmisi ( Necessary due to gitlab folder naming. )
 * E-Mail: mikhail.silaev@tuni.fi
 */

#include "mainwindow.hh"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
