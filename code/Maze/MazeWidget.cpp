#include "MazeWidget.h"
Maze *MazeWidget::maze;					//Static 一定有一個實體，不然會出現link error
MazeWidget::MazeWidget(QWidget *parent): QMainWindow(parent)
{
	ui.setupUi(this);
	ui.centralWidget->setFixedSize(w,h);
	timer = new QTimer(this);
	//ui.centralWidget->set
	connect(ui.actionRead_Maze,		SIGNAL(triggered()),	this,	SLOT(Read_Maze()));
	connect(ui.actionExit,			SIGNAL(triggered()),	this,	SLOT(Exit()));
	connect(timer,					SIGNAL(timeout()),		this,	SLOT(Refrush_Widget()));

	move_speed = 0.1f;
	move_FB = move_LR = move_Dir = 0;
}
//讀檔案進來
void MazeWidget::Read_Maze()
{
	QDir dir("Maze Map");
	QString temp;
	if(dir.exists())
		temp = "Maze Map/";
	else
		temp = "../x64/Release/Maze Map/";
	filename = QFileDialog::getOpenFileName(this,tr("Read Maze"),temp,tr("Any File (*.*)"));
	maze = new Maze(filename.toStdString().c_str());
	ui.widget->updateGL();
	timer->start(20);
	//maze->ReadFile_byName(filename);
}
void MazeWidget::Refrush_Widget()
{
	if(maze != NULL)
	{
		float angle_0 = maze->viewer_dir / 180 * 3.14;
		float angle_90 = (maze->viewer_dir - 90) / 180 * 3.14;
		float inX = move_FB * cos(angle_0) + move_LR * cos(angle_90);
		float inY = move_FB * sin(angle_0) + move_LR * sin(angle_90);
		if(inX != 0 || inY != 0)
			CollisionDetection(inX,inY);
		maze->viewer_dir += 30 * move_Dir;
		ui.widget->updateGL();
	}
	else
		timer->stop();
}
void MazeWidget::CollisionDetection(float inx,float iny)
{
	int big_X = maze->edges[maze->num_edges - 1]->endpoints[Edge::END]->posn[Vertex::X];
	int big_Y = maze->edges[maze->num_edges - 1]->endpoints[Edge::END]->posn[Vertex::Y];
	float const_dis = 0.01f;									//撞牆前面的誤差值
	bool collision_bool_X = false;
	bool collision_bool_Y = false;
	for (int i = 0 ; i < (int)maze->num_edges && (!collision_bool_X || !collision_bool_Y); i++)
		if (maze->edges[i]->opaque) {

			float edgeStartX = maze->edges[i]->endpoints[Edge::START]->posn[Vertex::X];
			float edgeStartY = maze->edges[i]->endpoints[Edge::START]->posn[Vertex::Y];
			float edgeEndX = maze->edges[i]->endpoints[Edge::END]->posn[Vertex::X];
			float edgeEndY = maze->edges[i]->endpoints[Edge::END]->posn[Vertex::Y];
			float viewerPosX = maze->viewer_posn[Maze::X];
			float viewerPosY = maze->viewer_posn[Maze::Y];
			float viewerPosZ = maze->viewer_posn[Maze::Z];

			switch (Check_Same_X_or_Y(edgeStartX, edgeStartY, edgeEndX, edgeEndY, big_X, big_Y))
			{
			case 1:
				if (((viewerPosX < edgeStartX + const_dis && edgeStartX - const_dis < viewerPosX + inx) || (viewerPosX + inx < edgeStartX + const_dis && edgeStartX - const_dis < viewerPosX))
					&& edgeStartY <= viewerPosY && edgeEndY >= viewerPosY)
					collision_bool_X = true;
				break;
			case 2:
				if (((viewerPosY < edgeStartY + const_dis && edgeStartY - const_dis < viewerPosY + iny) || (viewerPosY + iny  < edgeStartY + const_dis && edgeStartY - const_dis < viewerPosY))
					&& edgeStartX <= viewerPosX && edgeEndX >= viewerPosX)
					collision_bool_Y = true;
				break;
			}
		}
			
	if (!collision_bool_X)
	{
		//maze->viewer_posn[Maze::X] += inx;
		maze->Move_View_Posn(inx, 0, 0);
	}
	if (!collision_bool_Y)
	{
		//maze->viewer_posn[Maze::Y] += iny;
		maze->Move_View_Posn(0, iny, 0);
	}
		
}
int MazeWidget::Check_Same_X_or_Y(int maze_X1,int maze_Y1,int maze_X2,int maze_Y2,int big_X,int big_Y)
{
	//不考慮斜的邊
	if (maze_X1 == maze_X2)
		return 1;
		/*
		if(maze_X1 == 0 || maze_X1 == big_X)
			return 0;
		else
			return 1;*/
	else if(maze_Y1 == maze_Y2)
		return 2;
		/*
		if(maze_Y1 == 0 || maze_Y1 == big_Y)
			return 0;
		else
			return 2;*/
	else 
		return 3;
}
void MazeWidget::Exit()
{
	QApplication::exit();
}

//Keyboard Down
void MazeWidget::keyPressEvent(QKeyEvent *event)
{
	if(maze != NULL)
		switch (event->key())
		{
			//Forward Backward
			case (Qt::Key_W):
				move_FB = move_speed;
				break;
			case (Qt::Key_S):
				move_FB = -move_speed;
				break;

			//Lift Right
			case (Qt::Key_A):
				move_LR = -move_speed;
				break;
			case (Qt::Key_D):
				move_LR = move_speed;
				break;

			case (Qt::Key_Left):
				move_Dir = move_speed;
				break;
			case (Qt::Key_Right):
				move_Dir = -move_speed;
				break;
		}
}
void MazeWidget::keyReleaseEvent(QKeyEvent *event)
{
	if(maze != NULL)
		switch (event->key())
		{
			case (Qt::Key_W):
			case (Qt::Key_S):
				move_FB = 0;
				break;

			case (Qt::Key_A):
			case (Qt::Key_D):
				move_LR = 0;
				break;

			case (Qt::Key_Left):
			case (Qt::Key_Right):
				move_Dir = 0;
				break;
		}
}