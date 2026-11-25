#include <QApplication>
#include <QSurfaceFormat>

#include "gui/MainWindow.h"

#include <stb_image.h>

int main(int argc, char* argv[])
{
	stbi_set_flip_vertically_on_load(true);

	QSurfaceFormat format;
	format.setVersion(4, 5);
	format.setProfile(QSurfaceFormat::CoreProfile);
	format.setDepthBufferSize(24);
	format.setSwapInterval(1);
	QSurfaceFormat::setDefaultFormat(format);
	
	QApplication app(argc, argv);

	MainWindow window;
	window.show();

	return app.exec();
}
