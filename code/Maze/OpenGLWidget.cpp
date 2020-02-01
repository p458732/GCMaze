#include "OpenGLWidget.h"
#include <iostream>
#include "MazeWidget.h"
#include <gl\gl.h>
#include <gl\GLU.h>
#include<qline.h>
float OpenGLWidget::degree_change(float num)
{
	return num / 180.0f * 3.14159f;
}

float degree_changee(float num)
{
	return num / 180.0f * 3.14159f;
}

OpenGLWidget::OpenGLWidget(QWidget *parent) : QGLWidget(parent)
{
	
	top_z = 1.5f;
	but_z = -1;

	QDir dir("Pic");
	if(dir.exists())
		pic_path = "Pic/";
	else
		pic_path = "../x64/Release/Pic/";
}
void OpenGLWidget::initializeGL()
{
	glClearColor(0,0,0,1);
	glEnable(GL_TEXTURE_2D);
	loadTexture2D(pic_path + "grass.png",grass_ID);
	loadTexture2D(pic_path + "sky.png",sky_ID);
}
void OpenGLWidget::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if(MazeWidget::maze!=NULL)
	{
		//View 1

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glViewport(0 , 0 , MazeWidget::w/2 , MazeWidget::h);
		glOrtho (-0.1, MazeWidget::maze->max_xp +0.1, -0.1 , MazeWidget::maze->max_yp +0.1, 0 , 10);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		Mini_Map();

		//View 2
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glViewport(MazeWidget::w/2,0, MazeWidget::w/2, MazeWidget::h);
		/*gluPerspective 定義透視
		//視野大小, nearplane, farplane, distance
		//Note: You shouldn't use this function to get view matrix, otherwise you will get 0.
		*/
		//gluPerspective(MazeWidget::maze->viewer_fov, 1 , 0.01 , 200);

		/* gluLookAt
		//原本相機位置
		//看的方向
		//哪邊是上面
		//Note: You shouldn't use this function to get view matrix, otherwise you will get 0.
		*/
		float viewerPosX = MazeWidget::maze->viewer_posn[Maze::X];
		float viewerPosY = MazeWidget::maze->viewer_posn[Maze::Y];
		float viewerPosZ = MazeWidget::maze->viewer_posn[Maze::Z];

		/*gluLookAt(viewerPosX, viewerPosZ, viewerPosY,
			viewerPosX + cos(degree_change(MazeWidget::maze->viewer_dir)), viewerPosZ, viewerPosY + sin(degree_change(MazeWidget::maze->viewer_dir)),
			0.0, -1.0, 0.0);*/
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		float tempfov = MazeWidget::maze->viewer_fov;
		MazeWidget::maze->viewer_fov = 100;
		Map_3D();
		MazeWidget::maze->viewer_fov = tempfov;
	}
}
void OpenGLWidget::resizeGL(int, int)
{
}
void OpenGLWidget::myLookAt(float eyeX, float eyeY, float eyeZ, float tarx, float tary, float tarz, float upX, float upY, float upZ)
{
	float forwardX = eyeX-tarx, forwardY = eyeY - tary, forwardZ = eyeZ - tarz;
	float forwardDistance = sqrt(pow(forwardX, 2) + pow(forwardY, 2) + pow(forwardZ, 2));
	float forwardNormX = forwardX/ forwardDistance, forwardNormY = forwardY / forwardDistance, forwardNormZ = forwardZ / forwardDistance;
	float sideX = (forwardNormY * upZ - forwardNormZ * upY);
	float sideY = (forwardNormZ * upX - forwardNormX * upZ);
	float sideZ = (forwardNormX * upY - forwardNormY * upX);
	float sideDistance = sqrt(pow(sideX, 2) + pow(sideY, 2) + pow(sideZ, 2));
	float sideNormX = sideX / sideDistance, sideNormY = sideY / sideDistance, sideNormZ = sideZ / sideDistance;
	float upNormX = -(sideNormY * forwardNormZ - sideNormZ * forwardNormY);
	float upNormY = -(sideNormZ * forwardNormX - sideNormX * forwardNormZ);
	float upNormZ = -(sideNormX * forwardNormY - sideNormY * forwardNormX);
	
	float view[4][4] = {
		sideNormX ,sideNormY,sideNormZ,-(sideNormX*eyeX+ sideNormY * eyeY+ sideNormZ* eyeZ),
		upNormX,upNormY,upNormZ,-(upNormX * eyeX + upNormY * eyeY + upNormZ * eyeZ),
		forwardNormX,forwardNormY,forwardNormZ,-(forwardNormX * eyeX + forwardNormY * eyeY + forwardNormZ * eyeZ),
		0,0,0,1
	};
	float viewT[4][4] = {
	sideNormX ,upNormX,forwardNormX,0,
	sideNormY,upNormY,forwardNormY,0,
	sideNormZ,upNormZ,forwardNormZ,0,
	-(sideNormX * eyeX + sideNormY * eyeY + sideNormZ * eyeZ),-(upNormX * eyeX + upNormY * eyeY + upNormZ * eyeZ),-(forwardNormX * eyeX + forwardNormY * eyeY + forwardNormZ * eyeZ),1
	};
	float viewTT[16] = {
	sideNormX ,upNormX,forwardNormX,0,
	sideNormY,upNormY,forwardNormY,0,
	sideNormZ,upNormZ,forwardNormZ,0,
	-(sideNormX * eyeX + sideNormY * eyeY + sideNormZ * eyeZ),-(upNormX * eyeX + upNormY * eyeY + upNormZ * eyeZ),-(forwardNormX * eyeX + forwardNormY * eyeY + forwardNormZ * eyeZ),1
	};
	//glMultMatrixf(viewTT);
	matrixmuity(view, Mat, Mat);
	
}

void OpenGLWidget::myPerspective(float Fov, float Aspect, float Near, float Far)
{
	float projection[4][4] =
	{
		(1 / tan(Fov / 2)) / Aspect,0,0,0,
		0,(1 / tan(Fov / 2)),0,0,
		0,0,-(Far + Near) / (Far - Near),(-2 * Far * Near) / (Far - Near),
		0,0,-1,0
	};
	float projectionT[4][4] =
	{
		(1 / tan(Fov / 2)) / Aspect,0,0,0,
		0,(1 / tan(Fov / 2)),0,0,
		0,0,-(Far + Near) / (Far - Near),-1,
		0,0,(-2 * Far * Near) / (Far - Near) ,0
	};
	float projectionTT[16]=
	{
		(1 / tan(Fov / 2)) / Aspect,0,0,0,
		0,(1 / tan(Fov / 2)),0,0,
		0,0,-(Far + Near) / (Far - Near),-1,
		0,0,(-2 * Far * Near) / (Far - Near) ,0
	};
	//glMultMatrixf(projectionTT);
	matrixmuity(projection, Mat, Mat);
}

void OpenGLWidget::myViewport(float x , float y, float width , float height)
{
	
	
}

void OpenGLWidget::myMatrixdotPos(float a[][4], float(b)[4], float(&c)[4])
{
	float temp[4] = { 0 };

	for (int j = 0; j < 4; j++)
	{
		temp[j] = b[j];
	}
	for (int j = 0; j < 4; j++)
	{
		c[j] = 0;
	}
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			c[i] += a[i][j] * temp[j];
		}	
	}


	for (int l = 0; l < 2; l++)
	{
		c[l] /= c[3] ;
	}
	//MazeWidget::w / 2, 0, MazeWidget::w / 2, MazeWidget::h
}

void OpenGLWidget::myMatrixdotPoss(float a[][4], float(b)[4], float(&c)[4])
{
	float temp[4] = { 0 };
	

	for (int j = 0; j < 4; j++)
	{
		temp[j] = b[j];
	}
	for (int j = 0; j < 4; j++)
	{
		c[j] = 0;
	}
	for (int j = 0; j < 4; j++)
	{
		temp[j] = c[j];
	}
	for (int j = 0; j < 4; j++)
	{
		c[j] = 0;
	}
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			c[i] += a[i][j] * temp[j];
		}
	}
}

void OpenGLWidget::Identify()
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (i == j)
				Mat[i][j] = 1;
			else
				Mat[i][j] = 0;
		}
	}
}

void OpenGLWidget::Draw_Cell(Cell* C, QLineF leftFruLine, QLineF rightFruLine) {
	C->visited = 1;
	//reset the length of frustum
	leftFruLine.setLength(leftFruLine.length() * 10000);
	rightFruLine.setLength(rightFruLine.length() * 10000);
	//for each edge
	for (int i = 0; i < 4; i++) {
		//switchcase : to decided the situation of the edge
		//0 represent no intersection for right and left frustum lines
		//1 represent there are two intersections  for right and left frustum lines
		//2 represent there is one intersection for right frustum line
		//3 represent there is one intersection for left frustum line
		int switchcase = 0;
		int canOutput = 0;
		QLineF currentEdge = QLineF(C->edges[i]->endpoints[Edge::START]->posn[Vertex::X], C->edges[i]->endpoints[Edge::START]->posn[Vertex::Y],
			C->edges[i]->endpoints[Edge::END]->posn[Vertex::X], C->edges[i]->endpoints[Edge::END]->posn[Vertex::Y]);
		//the intersection point 
		QPointF rightPoint, leftPoint;
		//a minor Change to move the point of intetsection 
		//In order to Clip!
		float minorChange = 0.00001;
		// the output point of screen
		float screenspace[4][4];
		//now edge's start X point
		float CedgeStartX = C->edges[i]->endpoints[Edge::START]->posn[Vertex::X];
		//now edge's start Y point
		float CedgeStartY = C->edges[i]->endpoints[Edge::START]->posn[Vertex::Y];
		//now edge's end X point
		float CedgeEndX = C->edges[i]->endpoints[Edge::END]->posn[Vertex::X];
		//now edge's end Y point
		float CedgeEndY = C->edges[i]->endpoints[Edge::END]->posn[Vertex::Y];
		//now viewer X
		float X = MazeWidget::maze->viewer_posn[Maze::X];
		//now viewer Y
		float Y = MazeWidget::maze->viewer_posn[Maze::Y];

		//to decide can output?
		float canLookArea = 0.5 * (-leftFruLine.y2() * rightFruLine.x2() + Y * (-leftFruLine.x2() + rightFruLine.x2()) + X * (leftFruLine.y2() - rightFruLine.y2()) + leftFruLine.x2() * rightFruLine.y2());
		float sStart = 1 / (2 * canLookArea) * (Y * rightFruLine.x2() - X * rightFruLine.y2() + (rightFruLine.y2() - Y) * CedgeStartX + (X - rightFruLine.x2()) * CedgeStartY);
		float tStart = 1 / (2 * canLookArea) * (X * leftFruLine.y2() - Y * leftFruLine.x2() + (Y - leftFruLine.y2()) * CedgeStartX + (leftFruLine.x2() - X) * CedgeStartY);
		float sEnd = 1 / (2 * canLookArea) * (Y * rightFruLine.x2() - X * rightFruLine.y2() + (rightFruLine.y2() - Y) * CedgeEndX + (X - rightFruLine.x2()) * CedgeEndY);
		float tEnd = 1 / (2 * canLookArea) * (X * leftFruLine.y2() - Y * leftFruLine.x2() + (Y - leftFruLine.y2()) * CedgeEndX + (leftFruLine.x2() - X) * CedgeEndY);

		//decides situation
		if (rightFruLine.intersect(currentEdge, &rightPoint) == 1 && leftFruLine.intersect(currentEdge, &leftPoint) == 1)switchcase = 1;
		else if (rightFruLine.intersect(currentEdge, &rightPoint) == 1)switchcase = 2;
		else if (leftFruLine.intersect(currentEdge, &leftPoint) == 1)switchcase = 3;

		//avoid to hit the wall
		if (MazeWidget::maze->viewer_posn[Maze::X]<CedgeStartX + 0.001 && MazeWidget::maze->viewer_posn[Maze::X] >CedgeStartX - 0.001)MazeWidget::maze->viewer_posn[Maze::X] += 0.1;

		//according to "switchcase" to calculate the leftpoint and rightpoint 
		switch (switchcase)
		{
		case 0:
			if ((0 <= sStart && sStart <= 1.0f) && (0 <= tStart && tStart <= 1.0f) && (sStart + tStart <= 1.0f))
			{
				leftPoint = QPointF(CedgeStartX, CedgeStartY);
				rightPoint = QPointF(CedgeEndX, CedgeEndY);
				canOutput = 1;
			}
			break;
		case 1:
			canOutput = 1;
			break;
		case 2:
			if ((0 <= sStart && sStart <= 1.0f) && (0 <= tStart && tStart <= 1.0f) && (sStart + tStart <= 1.0f))
			{
				leftPoint = QPointF(CedgeStartX, CedgeStartY);
				canOutput = 1;
			}
			if ((0 <= sEnd && sEnd <= 1.0f) && (0 <= tEnd && tEnd <= 1.0f) && (sEnd + tEnd <= 1.0f))
			{
				leftPoint = QPointF(CedgeEndX, CedgeEndY);
				canOutput = 1;
			}

			break;

		case 3:
			if ((0 <= sStart && sStart <= 1.0f) && (0 <= tStart && tStart <= 1.0f) && (sStart + tStart <= 1.0f))
			{
				rightPoint = QPointF(CedgeStartX, CedgeStartY);
				canOutput = 1;
			}
			if ((0 <= sEnd && sEnd <= 1.0f) && (0 <= tEnd && tEnd <= 1.0f) && (sEnd + tEnd <= 1.0f))
			{
				rightPoint = QPointF(CedgeEndX, CedgeEndY);
				canOutput = 1;
			}
			break;
		}



		float viewSpace[4][4] = { leftPoint.x(),leftPoint.x(),rightPoint.x(),rightPoint.x(),1,-1,-1,1,leftPoint.y(),leftPoint.y(),rightPoint.y(),rightPoint.y(),1,1,1,1 };
		if (canOutput == 0)
		{
			//can't output
		}
		else
		{
			//if wall can see then output
			if (C->edges[i]->opaque)
			{
				matrixmuity(Mat, viewSpace, screenspace);
				glColor3f(C->edges[i]->color[0], C->edges[i]->color[1], C->edges[i]->color[2]);
				glBegin(GL_QUADS);
				glVertex2f(screenspace[0][0] / screenspace[2][0], screenspace[1][0] / screenspace[2][0]);
				glVertex2f(screenspace[0][1] / screenspace[2][1], screenspace[1][1] / screenspace[2][1]);
				glVertex2f(screenspace[0][2] / screenspace[2][2], screenspace[1][2] / screenspace[2][2]);
				glVertex2f(screenspace[0][3] / screenspace[2][3], screenspace[1][3] / screenspace[2][3]);
				glEnd();
			}
			//else then check this cell's neighbors
			else
			{
				//check left
				if (C->edges[i]->neighbors[Edge::LEFT] != NULL)
				{
					//isvisited?
					if (C->edges[i]->neighbors[Edge::LEFT]->visited == 0)
					{
						//according to "switchcase" to clip the frustum
						switch (switchcase)
						{
						case 0:
						{
							QLineF newrightFruLine = QLineF(MazeWidget::maze->viewer_posn[Maze::X], MazeWidget::maze->viewer_posn[Maze::Y], rightPoint.x() + minorChange, rightPoint.y() + minorChange);
							QLineF newleftFruLine = QLineF(MazeWidget::maze->viewer_posn[Maze::X], MazeWidget::maze->viewer_posn[Maze::Y], leftPoint.x() + minorChange, leftPoint.y() + minorChange);
							Draw_Cell(C->edges[i]->neighbors[Edge::LEFT], newleftFruLine, newrightFruLine);
						}
						break;
						case 1:
						{
							QLineF newrightFruLine = QLineF(MazeWidget::maze->viewer_posn[Maze::X], MazeWidget::maze->viewer_posn[Maze::Y], rightPoint.x() + minorChange, rightPoint.y() + minorChange);
							QLineF newleftFruLine = QLineF(MazeWidget::maze->viewer_posn[Maze::X], MazeWidget::maze->viewer_posn[Maze::Y], leftPoint.x() + minorChange, leftPoint.y() + minorChange);
							Draw_Cell(C->edges[i]->neighbors[Edge::LEFT], newleftFruLine, newrightFruLine);
						}
						break;

						case 2:
						{
							QLineF newleftFruLine = QLineF(MazeWidget::maze->viewer_posn[Maze::X], MazeWidget::maze->viewer_posn[Maze::Y], leftPoint.x() + minorChange, leftPoint.y() + minorChange);
							Draw_Cell(C->edges[i]->neighbors[Edge::LEFT], newleftFruLine, rightFruLine);
						}
						break;

						case 3:
						{
							QLineF newrightFruLine = QLineF(MazeWidget::maze->viewer_posn[Maze::X], MazeWidget::maze->viewer_posn[Maze::Y], rightPoint.x() + minorChange, rightPoint.y() + minorChange);
							Draw_Cell(C->edges[i]->neighbors[Edge::LEFT], leftFruLine, newrightFruLine);
						}
						break;
						}

					}
				}
				//check right
				if (C->edges[i]->neighbors[Edge::RIGHT] != NULL) {
					//isvisited?
					if (C->edges[i]->neighbors[Edge::RIGHT]->visited == 0)
					{
						//according to "switchcase" to clip the frustum
						switch (switchcase)
						{
						case 0:
						{
							QLineF newleftFruLine = QLineF(MazeWidget::maze->viewer_posn[Maze::X], MazeWidget::maze->viewer_posn[Maze::Y], leftPoint.x() + minorChange, leftPoint.y() + minorChange);
							QLineF newrightFruLine = QLineF(MazeWidget::maze->viewer_posn[Maze::X], MazeWidget::maze->viewer_posn[Maze::Y], rightPoint.x() + minorChange, rightPoint.y() + minorChange);
							Draw_Cell(C->edges[i]->neighbors[Edge::RIGHT], newleftFruLine, newrightFruLine);
						}
						break;
						case 1:
						{
							QLineF newleftFruLine = QLineF(MazeWidget::maze->viewer_posn[Maze::X], MazeWidget::maze->viewer_posn[Maze::Y], leftPoint.x() + minorChange, leftPoint.y() + minorChange);
							QLineF newrightFruLine = QLineF(MazeWidget::maze->viewer_posn[Maze::X], MazeWidget::maze->viewer_posn[Maze::Y], rightPoint.x() + minorChange, rightPoint.y() + minorChange);
							Draw_Cell(C->edges[i]->neighbors[Edge::RIGHT], newleftFruLine, newrightFruLine);
						}
						break;

						case 2:
						{
							QLineF newleftFruLine = QLineF(MazeWidget::maze->viewer_posn[Maze::X], MazeWidget::maze->viewer_posn[Maze::Y], leftPoint.x() + minorChange, leftPoint.y() + minorChange);
							Draw_Cell(C->edges[i]->neighbors[Edge::RIGHT], newleftFruLine, rightFruLine);
						}
						break;
						case 3:
						{
							QLineF newrightFruLine = QLineF(MazeWidget::maze->viewer_posn[Maze::X], MazeWidget::maze->viewer_posn[Maze::Y], rightPoint.x() + minorChange, rightPoint.y() + minorChange);
							Draw_Cell(C->edges[i]->neighbors[Edge::RIGHT], leftFruLine, newrightFruLine);
						}
						break;
						}
					}
				}
			}

		}
	}
}


void OpenGLWidget::matrixmuity(float a[][4], float b[][4], float(&c)[4][4])
{
	float temp[4][4] = { 0 };
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			temp[i][j] = b[i][j];
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			c[i][j] = 0;
		}
	}


	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			for (int k = 0; k < 4; k++)
				c[i][j] += a[i][k] * temp[k][j];
}

//Draw Left Part
void OpenGLWidget::Mini_Map()	
{					
	glBegin(GL_LINES);

		float viewerPosX = MazeWidget::maze->viewer_posn[Maze::X];
		float viewerPosY = MazeWidget::maze->viewer_posn[Maze::Y];
		float viewerPosZ = MazeWidget::maze->viewer_posn[Maze::Z];

		for(int i = 0 ; i < (int)MazeWidget::maze->num_edges; i++)
		{
			float edgeStartX = MazeWidget::maze->edges[i]->endpoints[Edge::START]->posn[Vertex::X];
			float edgeStartY = MazeWidget::maze->edges[i]->endpoints[Edge::START]->posn[Vertex::Y];
			float edgeEndX = MazeWidget::maze->edges[i]->endpoints[Edge::END]->posn[Vertex::X];
			float edgeEndY = MazeWidget::maze->edges[i]->endpoints[Edge::END]->posn[Vertex::Y];
			glColor3f(MazeWidget::maze->edges[i]->color[0] , MazeWidget::maze->edges[i]->color[1], MazeWidget::maze->edges[i]->color[2]);
			if(MazeWidget::maze->edges[i]->opaque)
			{
				glVertex2f(edgeStartX, edgeStartY);
				glVertex2f(edgeEndX, edgeEndY);
			}
		}

		//draw frustum
		float len = 0.1;
		glColor3f(1, 1, 1);
		glVertex2f(viewerPosX, viewerPosY);
		glVertex2f(viewerPosX + (MazeWidget::maze->max_xp) * len * cos(degree_change(MazeWidget::maze->viewer_dir - MazeWidget::maze->viewer_fov/2)) ,
			viewerPosY + (MazeWidget::maze->max_yp) * len * sin(degree_change(MazeWidget::maze->viewer_dir - MazeWidget::maze->viewer_fov/2)));
		glVertex2f(viewerPosX, viewerPosY);
		glVertex2f(viewerPosX + (MazeWidget::maze->max_xp) * len* cos(degree_change(MazeWidget::maze->viewer_dir + MazeWidget::maze->viewer_fov/2)) ,
			viewerPosY + (MazeWidget::maze->max_yp) * len*  sin(degree_change(MazeWidget::maze->viewer_dir + MazeWidget::maze->viewer_fov/2)));
	glEnd();
}

/*
void OpenGLWidget::Draw_Cell(Cell* C, QLineF leftfru, QLineF rightfru)
{

	C->visited = 1;
	for (int i = 0; i < 4; i++)
	{
		glColor3f(C->edges[i]->color[0], C->edges[i]->color[1], C->edges[i]->color[2]);
		float edgeStartX = C->edges[i]->endpoints[Edge::START]->posn[Vertex::X];
		float edgeStartY = C->edges[i]->endpoints[Edge::START]->posn[Vertex::Y];
		float edgeEndX = C->edges[i]->endpoints[Edge::END]->posn[Vertex::X];
		float edgeEndY = C->edges[i]->endpoints[Edge::END]->posn[Vertex::Y];
		float screenStartLow[4] = { 0.0, 0.0, 0.0, 0.0 };
		float screenStartHigh[4] = { 0.0, 0.0, 0.0, 0.0 };
		float  screenEndLow[4] = { 0.0, 0.0, 0.0, 0.0 };
		float screenEndHigh[4] = { 0.0, 0.0, 0.0, 0.0 };
		float viewStartLow[4] = { edgeStartX,-0.5,edgeStartY,1.0 };
		float viewStartHigh[4] = { edgeStartX,0.5,edgeStartY,1.0 };
		float viewEndLow[4] = { edgeEndX,-0.5,edgeEndY,1.0 };
		float viewEndHigh[4] = { edgeEndX,0.5,edgeEndY,1.0 };
		QLineF currentEdge = QLineF(C->edges[i]->endpoints[Edge::START]->posn[Vertex::X], C->edges[i]->endpoints[Edge::START]->posn[Vertex::Y], C->edges[i]->endpoints[Edge::END]->posn[Vertex::X], C->edges[i]->endpoints[Edge::END]->posn[Vertex::Y]);
		QPointF rightPoint, rightPointT;
		QPointF leftPoint, leftPointT;
		int choose = 0;
		if (MazeWidget::maze->viewer_posn[Maze::X]<C->edges[i]->endpoints[Edge::START]->posn[Vertex::X] + 0.001 && MazeWidget::maze->viewer_posn[Maze::X] > C->edges[i]->endpoints[Edge::START]->posn[Vertex::X] - 0.001)
			MazeWidget::maze->viewer_posn[Maze::X] += 0.01;
		glBegin(GL_QUADS);
		if (leftfru.intersect(currentEdge, &leftPoint)==1&& rightfru.intersect(currentEdge, &rightPoint) == 1)  //兩條線交於同1塊牆壁
		{
			choose = 6;
		}
		else if (leftfru.intersect(currentEdge, &leftPoint) == 1)
		{
			float area = 0.5 * (-leftfru.y2() * rightfru.x2() + MazeWidget::maze->viewer_posn[Maze::Y] * (-leftfru.x2() + rightfru.x2()) + MazeWidget::maze->viewer_posn[Maze::X] * (leftfru.y2() - rightfru.y2()) + leftfru.x2() * rightfru.y2());
			float sStart = 1 / (2 * area)*(MazeWidget::maze->viewer_posn[Maze::Y]*rightfru.x2() - MazeWidget::maze->viewer_posn[Maze::X]*rightfru.y2() + (rightfru.y2() - MazeWidget::maze->viewer_posn[Maze::Y])*C->edges[i]->endpoints[Edge::START]->posn[Vertex::X] + (MazeWidget::maze->viewer_posn[Maze::X] - rightfru.x2())*C->edges[i]->endpoints[Edge::START]->posn[Vertex::Y]);
			float tStart = 1 / (2 * area)*(MazeWidget::maze->viewer_posn[Maze::X]*leftfru.y2() - MazeWidget::maze->viewer_posn[Maze::Y]*leftfru.x2() + (MazeWidget::maze->viewer_posn[Maze::Y] - leftfru.y2())*C->edges[i]->endpoints[Edge::START]->posn[Vertex::X] + (leftfru.x2() - MazeWidget::maze->viewer_posn[Maze::X])*C->edges[i]->endpoints[Edge::START]->posn[Vertex::Y]);
			float sEnd = 1 / (2 * area)*(MazeWidget::maze->viewer_posn[Maze::Y]*rightfru.x2() - MazeWidget::maze->viewer_posn[Maze::X]*rightfru.y2() + (rightfru.y2() - MazeWidget::maze->viewer_posn[Maze::Y])*C->edges[i]->endpoints[Edge::END]->posn[Vertex::X] + (MazeWidget::maze->viewer_posn[Maze::X] - rightfru.x2())*C->edges[i]->endpoints[Edge::END]->posn[Vertex::Y]);
			float tEnd = 1 / (2 * area)*(MazeWidget::maze->viewer_posn[Maze::X]*leftfru.y2() - MazeWidget::maze->viewer_posn[Maze::Y]*leftfru.x2() + (MazeWidget::maze->viewer_posn[Maze::Y] - leftfru.y2())*C->edges[i]->endpoints[Edge::END]->posn[Vertex::X] + (leftfru.x2() - MazeWidget::maze->viewer_posn[Maze::X])*C->edges[i]->endpoints[Edge::END]->posn[Vertex::Y]);
				if ( (0 <= sStart && sStart <= 1.0f) && (0 <= tStart && tStart <= 1.0f) && (sStart + tStart <= 1.0f))
				{
						choose = 1;
						rightPoint = QPointF(edgeStartX, edgeStartY);							
				}
				else if ( (0 <= sEnd && sEnd <= 1.0f) && (0 <= tEnd && tEnd <= 1.0f) && (sEnd + tEnd <= 1.0f))
				{
						choose = 2;
						rightPoint = QPointF(edgeEndX, edgeEndY);					
				}
				else
				{
					//在線上
				}

		
		}
		//右邊的線有焦點
		else if (rightfru.intersect(currentEdge, &rightPoint) == 1)
		{
			float area = 0.5 * (-leftfru.y2() * rightfru.x2() + MazeWidget::maze->viewer_posn[Maze::Y] * (-leftfru.x2() + rightfru.x2()) + MazeWidget::maze->viewer_posn[Maze::X] * (leftfru.y2() - rightfru.y2()) + leftfru.x2() * rightfru.y2());
			float sStart = 1 / (2 * area)*(MazeWidget::maze->viewer_posn[Maze::Y]*rightfru.x2() - MazeWidget::maze->viewer_posn[Maze::X]*rightfru.y2() + (rightfru.y2() - MazeWidget::maze->viewer_posn[Maze::Y])*C->edges[i]->endpoints[Edge::START]->posn[Vertex::X] + (MazeWidget::maze->viewer_posn[Maze::X] - rightfru.x2())*C->edges[i]->endpoints[Edge::START]->posn[Vertex::Y]);
			float tStart = 1 / (2 * area)*(MazeWidget::maze->viewer_posn[Maze::X]*leftfru.y2() - MazeWidget::maze->viewer_posn[Maze::Y]*leftfru.x2() + (MazeWidget::maze->viewer_posn[Maze::Y] - leftfru.y2())*C->edges[i]->endpoints[Edge::START]->posn[Vertex::X] + (leftfru.x2() - MazeWidget::maze->viewer_posn[Maze::X])*C->edges[i]->endpoints[Edge::START]->posn[Vertex::Y]);
			float sEnd = 1 / (2 * area)*(MazeWidget::maze->viewer_posn[Maze::Y]*rightfru.x2() - MazeWidget::maze->viewer_posn[Maze::X]*rightfru.y2() + (rightfru.y2() - MazeWidget::maze->viewer_posn[Maze::Y])*C->edges[i]->endpoints[Edge::END]->posn[Vertex::X] + (MazeWidget::maze->viewer_posn[Maze::X] - rightfru.x2())*C->edges[i]->endpoints[Edge::END]->posn[Vertex::Y]);
			float tEnd = 1 / (2 * area)*(MazeWidget::maze->viewer_posn[Maze::X]*leftfru.y2() - MazeWidget::maze->viewer_posn[Maze::Y]*leftfru.x2() + (MazeWidget::maze->viewer_posn[Maze::Y] - leftfru.y2())*C->edges[i]->endpoints[Edge::END]->posn[Vertex::X] + (leftfru.x2() - MazeWidget::maze->viewer_posn[Maze::X])*C->edges[i]->endpoints[Edge::END]->posn[Vertex::Y]);
			
			if (C->edges[i]->Point_Side(leftfru.x1(), leftfru.y1()) == Edge::LEFT&& (0 <= sStart && sStart <= 1.0f) && (0 <= tStart && tStart <= 1.0f) && (sStart + tStart <= 1.0f))//點在左邊或上面
			{
				choose = 3;
				leftPoint = QPointF(edgeStartX, edgeStartY);
				
			}
			 if (C->edges[i]->Point_Side(leftfru.x1(), leftfru.y1()) == Edge::RIGHT&& (0 <= sEnd && sEnd <= 1.0f) && (0 <= tEnd && tEnd <= 1.0f) && (sEnd + tEnd <= 1.0f))
			{
				choose = 4;
				leftPoint = QPointF(edgeEndX, edgeEndY);
			}
			else
			{
				//在線上
			}
		}
		else
		{
			//兩條線都沒焦點
			float area = 0.5 * (-leftfru.y2() * rightfru.x2() + MazeWidget::maze->viewer_posn[Maze::Y] * (-leftfru.x2() + rightfru.x2()) + MazeWidget::maze->viewer_posn[Maze::X] * (leftfru.y2() - rightfru.y2()) + leftfru.x2() * rightfru.y2());
			float s = 1 / (2 * area)*(MazeWidget::maze->viewer_posn[Maze::Y] * rightfru.x2() - MazeWidget::maze->viewer_posn[Maze::X] * rightfru.y2() + (rightfru.y2() - MazeWidget::maze->viewer_posn[Maze::Y])*C->edges[i]->endpoints[Edge::START]->posn[Vertex::X] + (MazeWidget::maze->viewer_posn[Maze::X] - rightfru.x2())*C->edges[i]->endpoints[Edge::START]->posn[Vertex::Y]);
			float t = 1 / (2 * area)*(MazeWidget::maze->viewer_posn[Maze::X] * leftfru.y2() - MazeWidget::maze->viewer_posn[Maze::Y] * leftfru.x2() + (MazeWidget::maze->viewer_posn[Maze::Y] - leftfru.y2())*C->edges[i]->endpoints[Edge::START]->posn[Vertex::X] + (leftfru.x2() - MazeWidget::maze->viewer_posn[Maze::X])*C->edges[i]->endpoints[Edge::START]->posn[Vertex::Y]);
			if ((0 <= s && s <= 1.0f) && (0 <= t && t <= 1.0f) && (s + t <= 1.0f)) {
				choose = 5;
				leftPoint = QPointF(edgeStartX, edgeStartY);
				rightPoint = QPointF(edgeEndX, edgeEndY);
			}

		}

		if (choose!=0 && C->edges[i]->opaque) {

			viewStartLow[0] = leftPoint.rx();
			viewStartHigh[0] = leftPoint.rx();
			viewEndLow[0] = rightPoint.rx();
			viewEndHigh[0] = rightPoint.rx();
			viewStartLow[2] = leftPoint.ry();
			viewStartHigh[2] = leftPoint.ry();
			viewEndLow[2] = rightPoint.ry();
			viewEndHigh[2] = rightPoint.ry();
			myMatrixdotPos(Mat, viewStartLow, screenStartLow);
			myMatrixdotPos(Mat, viewStartHigh, screenStartHigh);
			myMatrixdotPos(Mat, viewEndLow, screenEndLow);
			myMatrixdotPos(Mat, viewEndHigh, screenEndHigh);
			glVertex2f(screenStartLow[0], screenStartLow[1]);
			glVertex2f(screenEndLow[0], screenEndLow[1]);
			glVertex2f(screenEndHigh[0], screenEndHigh[1]);
			glVertex2f(screenStartHigh[0], screenStartHigh[1]);
			glEnd();
		}
		else if (choose==0) {

		}
		else if (!C->edges[i]->opaque) {
			if (C->edges[i]->neighbors[Edge::LEFT] != NULL) {
				if (C->edges[i]->neighbors[Edge::LEFT]->visited == 0) {
					if ((rightfru.intersect(currentEdge, &rightPointT) != 1 && leftfru.intersect(currentEdge, &leftPointT) != 1) || (rightfru.intersect(currentEdge, &rightPointT) == 1 && leftfru.intersect(currentEdge, &leftPointT) == 1)) {

						QLineF newrightFruLine = QLineF(MazeWidget::maze->viewer_posn[Maze::X], MazeWidget::maze->viewer_posn[Maze::Y], rightPoint.x() + 0.00001, rightPoint.y() + 0.00001);
						newrightFruLine.setLength(newrightFruLine.length() * 10000);
						QLineF newleftFruLine = QLineF(MazeWidget::maze->viewer_posn[Maze::X], MazeWidget::maze->viewer_posn[Maze::Y], leftPoint.x() + 0.00001, leftPoint.y() + 0.00001);
						newleftFruLine.setLength(newleftFruLine.length() * 10000);
						Draw_Cell(C->edges[i]->neighbors[Edge::LEFT], newleftFruLine, newrightFruLine);
					}
					else if ((rightfru.intersect(currentEdge, &rightPointT) == 1 || leftfru.intersect(currentEdge, &leftPointT) == 1)) {
						if (rightfru.intersect(currentEdge, &rightPointT) == 1) {
							QLineF newleftFruLine = QLineF(MazeWidget::maze->viewer_posn[Maze::X], MazeWidget::maze->viewer_posn[Maze::Y], leftPoint.x() +minorChange, leftPoint.y() +minorChange);
							newleftFruLine.setLength(newleftFruLine.length() * 10000);
							Draw_Cell(C->edges[i]->neighbors[Edge::LEFT], newleftFruLine, rightfru);
						}
						else if (leftfru.intersect(currentEdge, &leftPointT) == 1) {
							QLineF newrightFruLine = QLineF(MazeWidget::maze->viewer_posn[Maze::X], MazeWidget::maze->viewer_posn[Maze::Y], rightPoint.x() +minorChange, rightPoint.y() +minorChange);
							newrightFruLine.setLength(newrightFruLine.length() * 10000);
							Draw_Cell(C->edges[i]->neighbors[Edge::LEFT], leftfru, newrightFruLine);
						}
					}

				}
			}
			if (C->edges[i]->neighbors[Edge::RIGHT] != NULL) {
				if (C->edges[i]->neighbors[Edge::RIGHT]->visited == 0) {
					if ((rightfru.intersect(currentEdge, &rightPointT) != 1 && leftfru.intersect(currentEdge, &leftPointT) != 1) || (rightfru.intersect(currentEdge, &rightPointT) == 1 && leftfru.intersect(currentEdge, &leftPointT) == 1)) {
						QLineF newleftFruLine = QLineF(MazeWidget::maze->viewer_posn[Maze::X], MazeWidget::maze->viewer_posn[Maze::Y], leftPoint.x() +minorChange, leftPoint.y() +minorChange);
						newleftFruLine.setLength(newleftFruLine.length() * 10000);
						QLineF newrightFruLine = QLineF(MazeWidget::maze->viewer_posn[Maze::X], MazeWidget::maze->viewer_posn[Maze::Y], rightPoint.x() +minorChange, rightPoint.y() +minorChange);
						newrightFruLine.setLength(newrightFruLine.length() * 10000);
						Draw_Cell(C->edges[i]->neighbors[Edge::RIGHT], newleftFruLine, newrightFruLine);
					}
					else if ((rightfru.intersect(currentEdge, &rightPointT) == 1 || leftfru.intersect(currentEdge, &leftPointT) == 1)) {
						if (rightfru.intersect(currentEdge, &rightPointT) == 1) {
							QLineF newleftFruLine = QLineF(MazeWidget::maze->viewer_posn[Maze::X], MazeWidget::maze->viewer_posn[Maze::Y], leftPoint.x() +minorChange, leftPoint.y() +minorChange);
							newleftFruLine.setLength(newleftFruLine.length() * 10000);
							Draw_Cell(C->edges[i]->neighbors[Edge::RIGHT], newleftFruLine, rightfru);
						}
						else if (leftfru.intersect(currentEdge, &leftPointT) == 1) {
							QLineF newrightFruLine = QLineF(MazeWidget::maze->viewer_posn[Maze::X], MazeWidget::maze->viewer_posn[Maze::Y], rightPoint.x() +minorChange, rightPoint.y() +minorChange);
							newrightFruLine.setLength(newrightFruLine.length() * 10000);
							Draw_Cell(C->edges[i]->neighbors[Edge::RIGHT], leftfru, newrightFruLine);
						}

					}


				}
			}
		}

		glEnd();
	}

}

*/
//**********************************************************************
//
// * Draws the first-person view of the maze.
//   THIS IS THE FUINCTION YOU SHOULD MODIFY.
//
//Note: You must not use any openGL build-in function to set model matrix, view matrix and projection matrix.
//		ex: gluPerspective, gluLookAt, glTraslatef, glRotatef... etc.
//		Otherwise, You will get 0 !
//======================================================================
void OpenGLWidget::Map_3D()
{
	glLoadIdentity();
	// 畫右邊區塊的所有東西


	float viewerPosX = MazeWidget::maze->viewer_posn[Maze::X];
	float viewerPosY = MazeWidget::maze->viewer_posn[Maze::Y];
	float viewerPosZ = MazeWidget::maze->viewer_posn[Maze::Z];

	/*gluPerspective(MazeWidget::maze->viewer_fov, 1, 0.01, 200);
	gluLookAt(viewerPosX, viewerPosZ, viewerPosY,
		viewerPosX + cos(degree_change(MazeWidget::maze->viewer_dir)), viewerPosZ, viewerPosY + sin(degree_change(MazeWidget::maze->viewer_dir)),
		0.0, -1.0, 0.0);*/
	float maxWH = MazeWidget::maze->max_xp;
	if (MazeWidget::maze->max_xp < MazeWidget::maze->max_yp)maxWH = MazeWidget::maze->max_yp;
	Identify();
	myLookAt(viewerPosX, viewerPosZ, viewerPosY,
		viewerPosX + cos(degree_change(MazeWidget::maze->viewer_dir)), viewerPosZ, viewerPosY + sin(degree_change(MazeWidget::maze->viewer_dir)),
		0.0, -1.0, 0.0);
	myPerspective(MazeWidget::maze->viewer_fov, 1, 0.0001, 200);
	QLineF rightfru = QLineF(MazeWidget::maze->viewer_posn[Maze::X], MazeWidget::maze->viewer_posn[Maze::Y],
		MazeWidget::maze->viewer_posn[Maze::X] + 2 * maxWH*   cos(degree_change(MazeWidget::maze->viewer_dir - MazeWidget::maze->viewer_fov / 2)),
		MazeWidget::maze->viewer_posn[Maze::Y] + 2 * maxWH*   sin(degree_change(MazeWidget::maze->viewer_dir - MazeWidget::maze->viewer_fov / 2)));

	QLineF leftfru = QLineF(MazeWidget::maze->viewer_posn[Maze::X], MazeWidget::maze->viewer_posn[Maze::Y],
		MazeWidget::maze->viewer_posn[Maze::X] + 2 * maxWH * cos(degree_change(MazeWidget::maze->viewer_dir + MazeWidget::maze->viewer_fov / 2)),
		MazeWidget::maze->viewer_posn[Maze::Y] + 2 * maxWH*  sin(degree_change(MazeWidget::maze->viewer_dir + MazeWidget::maze->viewer_fov / 2)));
	glBindTexture(GL_TEXTURE_2D, sky_ID);
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);

	glTexCoord2f(0, 0); glVertex2f(-1, 0);
	glTexCoord2f(0, 1); glVertex2f(1, 0);
	glTexCoord2f(1, 1); glVertex2f(1, 1);
	glTexCoord2f(1, 0); glVertex2f(-1, 1);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, grass_ID);
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);

	glTexCoord2f(0, 0); glVertex2f(-1, 0);
	glTexCoord2f(0, 1); glVertex2f(1, 0);
	glTexCoord2f(1, 1); glVertex2f(1, -1);
	glTexCoord2f(1, 0); glVertex2f(-1, -1);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	      // Cell* currentcell = MazeWidget::maze->view_cell;
			//Draw_Cell(MazeWidget::maze->view_cell, leftfru, rightfru);
			Draw_Cell(MazeWidget::maze->view_cell, leftfru, rightfru);
			Identify();
			myLookAt(viewerPosX, viewerPosZ, viewerPosY,
				viewerPosX + cos(degree_change(MazeWidget::maze->viewer_dir)), viewerPosZ, viewerPosY + sin(degree_change(MazeWidget::maze->viewer_dir)),
				0.0, -1.0, 0.0);
			myPerspective(MazeWidget::maze->viewer_fov, 1, 0.0001, 200);
			for (int i = 0; i < (int)MazeWidget::maze->num_cells; i++)MazeWidget::maze->cells[i]->visited = 0;
	
	/*若有興趣的話, 可以為地板或迷宮上貼圖, 此項目不影響評分*/
	glBindTexture(GL_TEXTURE_2D, sky_ID);
	// 畫貼圖 & 算 UV

	glDisable(GL_TEXTURE_2D);
}
void OpenGLWidget::loadTexture2D(QString str,GLuint &textureID)
{
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	
	QImage img(str);
	QImage opengl_grass = QGLWidget::convertToGLFormat(img);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, opengl_grass.width(), opengl_grass.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, opengl_grass.bits());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glDisable(GL_TEXTURE_2D);
}
		
