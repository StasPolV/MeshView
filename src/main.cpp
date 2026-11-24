#include <QApplication>
#include <QSurfaceFormat>

#include "gui/MainWindow.h"

int main(int argc, char* argv[])
{
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
