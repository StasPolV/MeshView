#pragma once

#include "../renderer/Renderer.h"
#include "../scene/Camera.h"
#include <QOpenGLWidget>

#include <memory>


struct ScreenSize 
{
	float width;
	float height;
};

class OpenGLWidget : public QOpenGLWidget
{
public:
	OpenGLWidget(float screen_width, float screen_height, QWidget* parent = nullptr);

protected:
	void initializeGL() override;
	void resizeGL(int w, int h) override;
	void paintGL() override;

private:
	std::unique_ptr<Renderer> m_renderer;
	std::unique_ptr<Camera> m_camera;
	ScreenSize m_screen_size;
};
