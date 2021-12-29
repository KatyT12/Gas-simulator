#define OLC_PGE_APPLICATION //A macro that needs to be defined to use the PixelGameEngine class
#include <Simulation.h>

int main()
{
	Simulation simulation; //Initialize the simulation
	if (simulation.Construct(WINDOW_WIDTH, WINDOW_HEIGHT, 1, 1)) //Will return false if it fails to create an instance 
		simulation.Start(); // Starts the simulation

	return 0;
}














/*
from PyQt5 import QtGui, QtWidgets, QtCore
import pygame
import sys

class ImageWidget(QtWidgets.QWidget):
	def __init__(self,surface,parent=None):
		super(ImageWidget,self).__init__(parent)
		w=surface.get_width()
		h=surface.get_height()
		self.data=surface.get_buffer().raw
		self.image=QtGui.QImage(self.data,w,h,QtGui.QImage.Format_RGB32)

	def paintEvent(self,event):
		qp=QtGui.QPainter()
		qp.begin(self)
		qp.drawImage(0,0,self.image)
		qp.end()


class MainWindow(QtWidgets.QMainWindow):
	def __init__(self,surface,parent=None):
		super(MainWindow,self).__init__(parent)
		self.setCentralWidget(ImageWidget(surface))



pygame.init()
s=pygame.Surface((640,480))
s.fill((64,128,192,224))
pygame.draw.circle(s,(255,255,255,255),(100,100),50)

app=QtWidgets.QApplication(sys.argv)
w=MainWindow(s)
w.show()
app.exec_()
*/