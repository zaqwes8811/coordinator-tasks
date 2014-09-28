
/*
window->setWindowTitle("Enter Your Age");

QPushButton* button = new QPushButton("Quit");
QObject::connect(button, SIGNAL(clicked()), &app, SLOT(quit()));

QTableWidget* tableWidget = new QTableWidget(window);
tableWidget->setRowCount(10);
tableWidget->setColumnCount(5);

QHBoxLayout *layout = new QHBoxLayout;
layout->addWidget(button);
layout->addWidget(tableWidget);
*/

/*

int main(int argc, char** argv)
{
 QApplication app(argc, argv);
 QWidget wid;
 QPushButton *button = new QPushButton(&wid);
 button->setMinimumWidth(68);
 button->setMinimumHeight(60);
 QVBoxLayout *layout = new QVBoxLayout(&wid);
 layout->addWidget(button);
 wid.show();
 return app.exec();
}
*/

// How add widget
// http://stackoverflow.com/questions/17989231/how-to-add-buttons-to-a-main-window-in-qt
//
// Signals and slots:
// http://qt-project.org/wiki/Qt_for_beginners_Signals_and_slots_2

/*
//http://developer.nokia.com/community/wiki/How_to_use_QTableWidget_in_Qt#Header_File
#include <QtGui/QWidget>
//#include "ui_tableWidget.h"
#include<QStringList>
#include<QTableWidget>
#include<QHBoxLayout>
#include<QPushButton>
#include<QLabel>

class tableWidget : public QWidget
{
    Q_OBJECT

public:
  tableWidget(QWidget *parent = 0);
  ~tableWidget();

private:
   QTableWidget *widget;
   QHBoxLayout *layout;
   QStringList list;
   QPushButton *but1;
   QLabel *lbl;
};

tableWidget::tableWidget(QWidget *parent)
    : QWidget(parent)
{
  layout = new QHBoxLayout(this);
  list<<"No."<<"Name"<<"Adder";
  widget = new QTableWidget(this);
  but1 = new QPushButton("Press",this);
  widget->setRowCount(3);
  widget->setColumnCount(15);
  widget->setHorizontalHeaderLabels(list);
  widget->setCellWidget(0,0,but1);
  widget->setCellWidget(0,1,lbl);
  layout->addWidget(widget);
  setLayout(layout);
}

tableWidget::~tableWidget()
{
    // No need to delete any object it has got a parent which is properly deleted.

}
*/
