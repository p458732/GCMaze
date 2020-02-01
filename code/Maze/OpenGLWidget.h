#pragma once
#include <QGLWidget>
#include <QString>
#include <QDir>
#include <Cell.h>

class OpenGLWidget :public QGLWidget
{
	Q_OBJECT
public:
	explicit OpenGLWidget(QWidget *parent = 0);

	void initializeGL();
	void JudgeEdge(Cell* C, QLineF leftfrustum, QLineF rightfrustum, float& area, float& sstart, float& tstart, float& send, float& tend, int i);
	void paintGL();
	void resizeGL(int ,int );
	void myLookAt(float eyeX, float eyeY, float eyeZ, float tarx, float tary, float tarz, float upX, float upY, float upZ);
	void myPerspective(float Fov,float Aspect, float Near , float Far);
	void myViewport(float x, float y, float width, float height);
	void myMatrixdotPos(float a[][4], float(b)[4], float(&c)[4]);
	void myMatrixdotPoss(float a[][4], float(b)[4], float(&c)[4]);
	void Identify();
	void matrixmuity(float a[][4], float b[][4], float(&c)[4][4]);
	//Maze Setting
	void Mini_Map();
	void Draw_Cell(Cell* C, QLineF leftfru, QLineF rightfru);
	void Map_3D();
	void loadTexture2D(QString, GLuint &);
	float degree_change(float );

private:
	GLuint grass_ID;
	GLuint sky_ID;
	QString pic_path;
	float Mat[4][4] = {
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1 };
	float top_z;
	float but_z;
};

