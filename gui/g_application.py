from PyQt5.QtWidgets import *
from PyQt5.QtGui import QFont

def main():
  app = QApplication([])
  window = QWidget()
  window.setGeometry(100, 100, 200, 300)
  window.setWindowTitle("My Application")

  layout = QVBoxLayout()
  label = QLabel("Pressed the button below: ")
  button = QPushButton("Press Me")
  button.clicked.connect(OnClicked)
  layout.addWidget(label)
  layout.addWidget(button)
  window.setLayout(layout)

  window.show()
  app.exec_()

def OnClicked():
  message = QMessageBox()
  message.setText("Hello world")
  message.exec_()

if __name__ == "__main__":
  main()
