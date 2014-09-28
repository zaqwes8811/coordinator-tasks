#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>

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
    void update() {

    }
    
private:
    Ui::MainWindow *ui;
    QTableWidget* scoreTable_;
};

#endif // MAINWINDOW_H
