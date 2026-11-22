#pragma once

#include <QOpenGLWidget>

class OpenGLWidget : public QOpenGLWidget
{
protected:
	void initializeGL() override;
	void resizeGL(int w, int h) override;
	void paintGL() override;
};
