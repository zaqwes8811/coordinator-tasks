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

 public:
    void updateAction();
    
private:
    Ui::MainWindow *ui;
    QTableWidget* scoreTable_;

    app_core::AppCore* const app_ptr_;
};

#endif // MAINWINDOW_H
