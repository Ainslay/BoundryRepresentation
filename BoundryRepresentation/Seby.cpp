#include <GL/glut.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>

using namespace std;

// wpó³rzêdne po³o¿enia obserwatora
GLdouble eyex = 0;
GLdouble eyey = 0;
GLdouble eyez = 3;

// wspó³rzêdne punktu w którego kierunku jest zwrócony obserwator,
GLdouble centerx = 0;
GLdouble centery = 0;
GLdouble centerz = 0;

class Edge {
public:
    GLfloat xpos, ypos, zpos;
};

class Figure {
public:
    int edgeCount = 0;
    Edge* edges = nullptr;
    Figure() = delete;
    /*{
        edges = new Edge[0];
    }*/
    Figure(string file) {
        ifstream fin(file);
        if (fin.good()) 
        {
            fin >> edgeCount;
            edges = new Edge[2*edgeCount];
            for (int i = 0; i < 2*edgeCount; i++) {
                fin >> edges[i].xpos >> edges[i].ypos >> edges[i].zpos;
            }
        }
        else {
            cerr << "Blad otwarcia pliku" << endl;
        }
    }

    void draw(float r, float g, float b) 
    {
        glBegin(GL_LINES);
        glColor3f(r, g, b);
        for (int i = 0; i < 2*edgeCount; i++) {
            glVertex3f(edges[i].xpos, edges[i].ypos, edges[i].zpos);
        }
        glEnd();
    }

    ~Figure() 
    {
        delete[] edges;
    }
};
Figure shape("edges.txt");
// funkcja generuj¹ca scenê 3D
void Display()
{
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // ustawienie obserwatora
    gluLookAt(eyex, eyey, eyez, centerx, centery, centerz, 0, 1, 0);

    shape.draw(0, 0, 0);
    
    
    glFlush();
    glutSwapBuffers();
}

// zmiana wielkoœci okna

void Reshape(int width, int height)
{
    // obszar renderingu - ca³e okno
    glViewport(0, 0, width, height);

    // wybór macierzy rzutowania
    glMatrixMode(GL_PROJECTION);

    // macierz rzutowania = macierz jednostkowa
    glLoadIdentity();

        // wysokoœæ okna wiêksza od wysokoœci okna
    if (width < height && width > 0)
        glFrustum(-5.0, 5.0, -5.0 * height / width, 5.0 * height / width, 1.0, 5.0);
        else
    if (width >= height && height > 0)
        glFrustum(-5.0 * width / height, 5.0 * width / height, -5.0, 5.0, 1.0, 5.0);


    // generowanie sceny 3D
    Display();
}

// obs³uga klawiatury

void Keyboard(unsigned char key, int x, int y)
{
    // klawisz +
    if (key == '+')
        eyez -= 0.1;
    else

        // klawisz -
        if (key == '-')
            eyez += 0.1;

    // odrysowanie okna
    Reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
}

// obs³uga klawiszy funkcyjnych i klawiszy kursora

void SpecialKeys(int key, int x, int y)
{
    switch (key)
    {
        // kursor w lewo
    case GLUT_KEY_LEFT:
        eyex += 0.1;
        break;

        // kursor w górê
    case GLUT_KEY_UP:
        eyey -= 0.1;
        break;

        // kursor w prawo
    case GLUT_KEY_RIGHT:
        eyex -= 0.1;
        break;

        // kursor w dó³
    case GLUT_KEY_DOWN:
        eyey += 0.1;
        break;
    }

    // odrysowanie okna
    Reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
}


int main(int argc, char* argv[])
{
    // inicjalizacja biblioteki GLUT
    glutInit(&argc, argv);

    // inicjalizacja bufora ramki
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

    // rozmiary g³ównego okna programu
    glutInitWindowSize(800, 800);

    glutCreateWindow("Modelowanie");


    // do³¹czenie funkcji generuj¹cej scenê 3D
    glutDisplayFunc(Display);

    // do³¹czenie funkcji wywo³ywanej przy zmianie rozmiaru okna
    glutReshapeFunc(Reshape);

    // do³¹czenie funkcji obs³ugi klawiatury
    glutKeyboardFunc(Keyboard);

    // do³¹czenie funkcji obs³ugi klawiszy funkcyjnych i klawiszy kursora
    glutSpecialFunc(SpecialKeys);

    
    // wprowadzenie programu do obs³ugi pêtli komunikatów
    glutMainLoop();
    return 0;
}