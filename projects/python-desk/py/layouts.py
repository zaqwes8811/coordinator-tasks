# http://stackoverflow.com/questions/23407727/is-it-possible-to-horizontally-add-a-qtablewidget-to-a-layout

import sys, os, random
# from PyQt4 import QtCore, QtGui
from PySide import QtCore, QtGui

StudentNames = []
StudentScores = []


class StartTest(QtGui.QMainWindow):
    def __init__(self, questions, parent=None):
        global StudentNames
        global StudentScores
        super(StartTest, self).__init__(parent)

        self.stack = QtGui.QStackedWidget(self)
        StudentNames.append('Hamzah Akhtar')
        StudentNames.append('Bob Marley')
        StudentNames.append('John Meadows')
        StudentScores.append(int(0))
        StudentScores.append(int(0))
        StudentScores.append(int(0))
        for index, question in enumerate(questions):
            page = Question(question, self)
            page.submit.clicked[()].connect(
                lambda index=index: self.handleSubmit(index))
            self.stack.addWidget(page)
        self.answers = []

        centralWidget = QtGui.QWidget(self)
        self.setCentralWidget(centralWidget)
        mainLayout = QtGui.QHBoxLayout(centralWidget)

        self.scoreTable = QtGui.QTableWidget(self)
        self.scoreTable.setRowCount(len(StudentNames))
        self.scoreTable.setColumnCount(1)
        self.scoreTable.setVerticalHeaderLabels(StudentNames)
        self.scoreTable.setHorizontalHeaderLabels(['Scores'])
        for i, score in enumerate(StudentScores):
            item = QtGui.QTableWidgetItem(str(score))
            self.scoreTable.setItem(i, 0, item)

        mainLayout.addWidget(self.stack)
        #mainLayout.addWidget(self.scoreTable)

        self.i = 0

    def handleSubmit(self, index):
        page = self.stack.widget(index)

        answer = page.answer.text()
        # validate submitted answer...
        self.answers.append(answer)
        if index < self.stack.count() - 1:
            self.stack.setCurrentIndex(index + 1)


class Question(QtGui.QWidget):
    def __init__(self, question, parent=None):
        super(Question, self).__init__(parent)
        self.question = QtGui.QLabel(question, self)
        self.answer = QtGui.QLineEdit(self)
        self.submit = QtGui.QPushButton('Submit', self)
        form = QtGui.QFormLayout(self)
        form.addRow(self.question, self.answer)
        form.addRow(self.submit)


if __name__ == '__main__':
    User = ''
    app = QtGui.QApplication([])
    questions = [
        'What is 5+5?',
        'What is 45+10?',
        'What is 28+47?',
        'What is 22+13?',
    ]
    window = StartTest(questions)
    window.show()
    # window.showFullScreen()
    app.exec_()