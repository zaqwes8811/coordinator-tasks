#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <string>

#include <openssl/bn.h>
#include <openssl/sha.h>
#include <openssl/rsa.h>
#include <openssl/objects.h>

#include <QtConcurrent/QtConcurrent>
#include <QDebug>

//#include <tarlib/tarlib.h>

//#include <

static QTableWidget* report ;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    report = findChild<QTableWidget*>("tableWidget");
    if (report) {
        report->setRowCount(2);
        QTableWidgetItem* w = new QTableWidgetItem(QString("adfasdf"));
        //w->setFlags(w->flags() ^ Qt:);
        report->setItem(1, 0, w);
    }

    connect(&m_observer, SIGNAL(finished()), this, SLOT(onFinish()));

    popMenu = new QMenu(report);
    action = new QAction("Copy", this);

    // myWidget is any QWidget-derived class
    report->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(
          report, SIGNAL(customContextMenuRequested(const QPoint&)),
          this, SLOT(ShowContextMenu(const QPoint&)));

    connect(action, SIGNAL(triggered()), this, SLOT(rightClickedOperation()));
}

void extract1(std::string const &filename, std::string const &destination)
{
   // create tar file with path and read mode
  //tarlib::tarFile tarf(filename, tarlib::tarModeRead);

   // extract to folder
   //tarf.extract(destination);
    std::string s;
}

void MainWindow::rightClickedOperation()
{
  qDebug() << "act";
}

void MainWindow::ShowContextMenu(const QPoint& pos) // this is a slot
{
  popMenu->addAction(action);

  QTableWidgetItem *item;
  item = ui->tableWidget->itemAt(pos);

  if(!item)
      return;

  qDebug() << "item text" << item->text();

  popMenu->exec(QCursor::pos());

  /*// for most widgets
    QPoint globalPos = report->mapToGlobal(pos);
    // for QAbstractScrollArea and derived classes you would use:
    // QPoint globalPos = myWidget->viewport()->mapToGlobal(pos);
    QModelIndex item = report->indexAt(globalPos);
    qDebug() << item.column() << item.row();

    QTableWidgetItem* w = report->item(item.row(), item.column());

    QMenu menu(report);
    menu.addAction("Copy");

    QAction* selectedItem = menu.exec(QCursor::pos());
    if (selectedItem)
    {
        // something was chosen, do stuff
        //selectedItem->
        qDebug() << "do";
    }
    else
    {
        // nothing was chosen
        qDebug() << "do noting";
    }
    */
}

void call() {

}

// http://ynonperek.com/course/qt/threads.html
// http://doc.qt.io/qt-5/threads-reentrancy.html
// http://doc.qt.io/qt-4.8/qtconcurrentmap.html#concurrent-map-reduce
//
// 0. Make message
// 1. eventExec() - не блокируем полностью
// 2. parallel all
//
// Split work -
// http://stackoverflow.com/questions/18962056/how-to-split-a-qlist-at-position-n-to-a-new-qlist
void MainWindow::OnClick() {
    extract1("test.tar", "tmp");

    std::vector<int> in;

    QFuture<void> future = QtConcurrent::run(call);
    m_observer.setFuture(future);


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onFinish() {
  qDebug() << "Done";
}



int spc_verify(unsigned char *msg, unsigned int mlen, unsigned char *sig,
unsigned int siglen, RSA *r)
{
    unsigned char hash[20];
    BN_CTX
    *c;
    int
    ret;
    if (!(c = BN_CTX_new( ))) return 0;
    if (!SHA1(msg, mlen, hash) || !RSA_blinding_on(r, c)) {
        BN_CTX_free(c);
        return 0;
    }
    ret = RSA_verify(NID_sha1, hash, 20, sig, siglen, r);
    RSA_blinding_off(r);
    BN_CTX_free(c);
    return ret;
}
