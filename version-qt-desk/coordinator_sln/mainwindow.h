#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class StartTest : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit StartTest(QWidget *parent = 0);
    ~StartTest();

private slots:
    void slotButtonClicked(bool checked) {

    }
    
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
