#-*- coding:utf-8 -*-
#import pychecker.checker
import sys
from PySide.QtCore import *
from PySide.QtGui import *
from PySide.QtWebKit import *
class RoundedRectWidget(QWidget):
	''' Чтобы реально работало нужно очень много добавить кажется 
		метод переопределно, но что он еще должен делать? События может какие
			тоже нужно переопределить?
	'''
	def paintEvent(self, event):
		roundness = 12
		opacity = 0.675
		widget_rect = self.rect()
		painter = QPainter(self)
		painter.save();
		painter.setRenderHint(QPainter.Antialiasing)
		painter.setPen(Qt.red)
		
		# clip
		rounded_rect = QPainterPath()
		rounded_rect.addRoundRect(1, 1, widget_rect.width() - 2, widget_rect.height() - 2, roundness, roundness);
		painter.setClipPath(rounded_rect)

		# get clipping region
		maskregion = painter.clipRegion()
		
		# mask the widget
		self.setMask(maskregion)
		painter.setOpacity(opacity)
 
		# fill path with color
		painter.fillPath(rounded_rect,QBrush(Qt.black))
 
		# restore painter
		painter.restore()
		
app = QApplication(sys.argv)

''' создаем виждеты'''
# Добавляем WebKit
web = QWebView()
web.load(QUrl("Planthi.html"))

wrapper = RoundedRectWidget()
# нужно бы добавить виджет для move!


''' создаем окно приложения '''
# Попытка создать красивок окно и 
mw = QMainWindow()
'''
roundness = 12
opacity = 0.675
widget_rect = mw.rect()
painter = QPainter()
painter.save()
painter.setRenderHint(QPainter.Antialiasing)
painter.setPen(Qt.red)

# clip
rounded_rect = QPainterPath()
rounded_rect.addRoundRect(1, 1, widget_rect.width() - 2, widget_rect.height() - 2, roundness, roundness);
painter.setClipPath(rounded_rect)
# get clipping region
maskregion = painter.clipRegion()

# mask the widget
mw.setMask(maskregion)'''

# Удаляем настоящее окно
mw.setAttribute(Qt.WA_TranslucentBackground);
mw.setWindowFlags(Qt.FramelessWindowHint)

# Масштабируем и добавляем виджеты
mw.resize(400, 400)
mw.setCentralWidget(web)	# QWebKit

''' RunApp() '''
mw.show()

sys.exit(app.exec_())

''' 
#include <QtGui>
 class  myMainWindow:public QMainWindow
 {
 public:
     myMainWindow():QMainWindow()
     {
	QRegion* region = new QRegion(*(new QRect(x()+5,y()+5,370,370)),QRegion::Ellipse);
	setMask(*region);
 	
 	
 	QPalette* palette = new QPalette();
 	palette->setBrush(QPalette::Background,*(new QBrush(*(new QPixmap("b.jpg")))));
 	setPalette(*palette); 	
 	
 	setWindowFlags(Qt::FramelessWindowHint); 	 
 	QWidget *centralWidget = new QWidget(this);
 	QGridLayout *layout = new QGridLayout();
 	
 	centralWidget->setLayout(layout);
 

 	layout->addWidget(new QPushButton("Button  1"),0,0);
 	layout->addWidget(new QPushButton("Button  2"),0,1);		
 	layout->addWidget(new QPushButton("Button  3"),0,2);		

 	
 	setCentralWidget(centralWidget);
 
     };
     ~myMainWindow(){};

 };

int main(int argc, char **argv)
{
	QApplication app(argc, argv);
	myMainWindow *window = new myMainWindow();    
    
        window->resize(376, 301);	       
	window->show();
	return app.exec();
}
'''