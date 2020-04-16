#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <Windows.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

GLdouble eyeX = 0;
GLdouble eyeY = 0;
GLdouble eyeZ = 4;

GLdouble centerX = 0;
GLdouble centerY = 0;
GLdouble centerZ = 0;

struct Vertex
{
	GLdouble X, Y, Z;
};

struct Edge
{
	Vertex A, B;
};

class Block
{
public:
	Edge* edges = nullptr;
	int edgeCount = 0;

	Block() = delete;

	Block(fstream& inputData)
	{
		LoadData(inputData);
	}

	~Block()
	{
		delete[] edges;
	}

	void Draw()
	{
		glBegin(GL_LINES);

		for (int i = 0; i < edgeCount; i++)
		{
			glVertex3d(edges[i].A.X, edges[i].A.Y, edges[i].A.Z);
			glVertex3d(edges[i].B.X, edges[i].B.Y, edges[i].B.Z);
		}
		glEnd();
	}

private:
	void LoadData(fstream& inputData)
	{
		inputData >> edgeCount;
		edges = new Edge[edgeCount];

		for (int i = 0; i < edgeCount; i++)
		{
			inputData >> edges[i].A.X >> edges[i].A.Y >> edges[i].A.Z;
			inputData >> edges[i].B.X >> edges[i].B.Y >> edges[i].B.Z;
		}
	}
};

void ChangeViewPort(int width, int height)
{
	const float aspectRatio = (float)width / (float)height;

	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if (width < height && width > 0)
	{
		glFrustum(-5.0, 5.0, -5.0 * height / width, 5.0 * height / width, 1.0, 20.0);
	}
	else if (width >= height && height > 0)
	{
		glFrustum(-5.0 * width / height, 5.0 * width / height, -5.0, 5.0, 1.0, 20.0);
	}
}

Block* tetrahedron;
Block* cube;

void Render()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glLineWidth(2.5);
	glColor3d(1.0, 0.2, 0.0);
	
	gluLookAt(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, 0.0, 1.0, 0.0);

	tetrahedron->Draw();
	cube->Draw();
	
	glFlush();
	glutSwapBuffers();
}

void Key(unsigned char key, int x, int y)
{
	switch (key)
	{
		case '+': eyeZ -= 0.1; break;
		case '-': eyeZ += 0.1; break;
		default: break;
	}
	ChangeViewPort(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
}

void SpecialKey(int key, int x, int y)
{
	switch (key)
	{
		case GLUT_KEY_LEFT:  eyeX += 0.1; break;
		case GLUT_KEY_RIGHT: eyeX -= 0.1; break;
		case GLUT_KEY_UP:    eyeY -= 0.1; break;
		case GLUT_KEY_DOWN:  eyeY += 0.1; break;
		default: break;
	}
	ChangeViewPort(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
}

void Idle()
{
	glutPostRedisplay();
}

int main(int argc, char* argv[]) 
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(1280, 720);
	glutInitWindowPosition(300, 150);

	glutCreateWindow("Boundry Representation - Jakub Spalek Lab1 Gr1 INF II");

	glutReshapeFunc(ChangeViewPort);
	glutDisplayFunc(Render);
	glutIdleFunc(Idle);
	glutKeyboardFunc(Key);
	glutSpecialFunc(SpecialKey);

	glClearColor(0, 0, 0, 0);

	cout << "Poruszanie kamera: klawisze strzalek" << endl;
	cout << "Zoom: [-]/[+]\n" << endl;

	fstream inputData("edges.txt");

	if (!inputData.good())
	{
		cerr << "Could not load data. Path is incorrect or file does not exist." << endl;
		exit(-1);
	}
	tetrahedron = new Block(inputData);
	cube = new Block(inputData);
	
	inputData.close();

	glutMainLoop();
	
	delete tetrahedron, cube;
	
	return 0;
}

// Zawartoœæ pliku edges.txt
//8
//0 8 0 - 2 2 2
//0 8 0 - 2 2 - 2
//0 8 0     2 2 - 2
//0 8 0     2 2 2
//- 2 2 2 - 2 2 - 2
//- 2 2 - 2   2 2 - 2
//2 2 - 2    2 2 2
//2 2 2 - 2 2 2
//12
//- 2 2 2 - 2 2 - 2
//- 2 2 - 2   2 2 - 2
//2 2 - 2    2 2 2
//2 2 2 - 2 2 2
//- 2 2 2 - 2 - 2 2
//2 2 2     2 - 2 2
//2 2 - 2    2 - 2 - 2
//- 2 2 - 2 - 2 - 2 - 2
//- 2 - 2 2 - 2 - 2 - 2
//- 2 - 2 - 2  2 - 2 - 2
//2 - 2 - 2   2 - 2 2
//2 - 2 2 - 2 - 2 2