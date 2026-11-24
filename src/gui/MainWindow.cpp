#include "MainWindow.h"

#include <QSize>
#include <QGuiApplication>
#include <QScreen>

MainWindow::MainWindow(QObject* parent) 
{
	setObjectName("MainWindow");
	setWindowTitle("MeshView");

	QScreen* screen = QGuiApplication::primaryScreen();
	QSize screen_size = screen->size();
	resize(screen_size);

	m_openGl = new OpenGLWidget(screen_size.width(), screen_size.height(), this);

	setCentralWidget(m_openGl);
}
