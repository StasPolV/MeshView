#pragma once

#include "OpenGLWidget.h"
#include <QMainWindow>
#include <QObject>

class MainWindow : public QMainWindow 
{
public:
	MainWindow(QObject* parent = nullptr);

private:
	OpenGLWidget* m_openGl;
};
