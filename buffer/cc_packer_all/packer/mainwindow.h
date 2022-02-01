#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtConcurrent>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void OnClick();

    void onFinish();

    void ShowContextMenu(const QPoint& pos);

    void rightClickedOperation();

private:
    Ui::MainWindow *ui;

    QFutureWatcher<void> m_observer;

    QMenu *popMenu;
    QAction *action;


};

#endif // MAINWINDOW_H
