#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "top/config.h"  // BAD, but Qt generate some files

#include "canary/app_core.h"

#include <QMainWindow>
#include <QTableWidget>

namespace Ui {
class MainWindow;
}

class StartTest : public QMainWindow
{
    Q_OBJECT
    
public:
    StartTest(app_core::AppCore* const app_ptr, QWidget *parent = 0);
    ~StartTest();

private slots:
    void slotAddRecords(bool checked);

private slots:
    //int row, int column);  // будет генерится и при создании
    void slotRowIsChanged(QTableWidgetItem* item);

 public:
    void updateAction();
    
private:
    // очищает список задач
    void clearList();

    Ui::MainWindow *ui;
    QTableWidget* scoreTable_;

    app_core::AppCore* const app_ptr_;

    // FIXME: как узнать, что ячейка именно обновлена
    bool entered_;  //
    struct Point {
      Point() : c(0), r(0) { }
      int c;
      int r;
    };
};

#endif // MAINWINDOW_H
