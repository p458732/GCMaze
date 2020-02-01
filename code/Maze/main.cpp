#include "MazeWidget.h"
#include "windows.h"
#pragma comment(lib,"winmm.lib")
#include <QtWidgets/QApplication>
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MazeWidget w;
	PlaySound(TEXT(".\\music.wav"), NULL, SND_FILENAME | SND_ASYNC);
	w.show();
	return a.exec();
}
