#include <QApplication>
#include <QSurfaceFormat>

int main(int argc, char* argv[])
{
	QSurfaceFormat format;
	format.setVersion(4, 5);
	format.setProfile(QSurfaceFormat::CoreProfile);
	format.setDepthBufferSize(24);
	QSurfaceFormat::setDefaultFormat(format);
	

	QApplication app(argc, argv);
	return app.exec();
}
