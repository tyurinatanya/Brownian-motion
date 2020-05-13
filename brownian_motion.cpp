#include <iostream>
#include <GL/glut.h>
#include <vector>
#include <cstdlib>
#include <cmath>
#define M_PI 3.14159265358979323846
using namespace std;

const float DT = 0.01;
const int Size = 700;

class point2D{
public:
    double x;
    double y;
    double vx;
    double vy;
    double m;
    double R;
    point2D(double x, double y, double vx, double vy, double m)
    {
        this->x=x;
        this->y=y;
        this->vx=vx;
        this->vy=vy;
        this->m=m;

    }

    void obrabotka() //обрабатываем случаи выхода за границы экрана
    {
        if (x-R<=0)
        {
            vx*=(-1);
        }

        if (y-R<=0)
        {
            vy*=(-1);
        }

        if (x+R>=Size)
        {
            vx*=(-1);
        }
        if (y+R>=Size)
        {
            vy*=(-1);
        }

    }
};



vector<point2D> p;
void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    for (vector<point2D>::iterator i = p.begin(); i != p.end(); ++i)
    {
        if (i -> m <= 2)
        {
            glPointSize(5);
            glBegin(GL_POINTS);
            glVertex2f(i -> x, i -> y);
            glEnd();
            i->R=0;
        }
        else
        {
            glBegin(GL_POLYGON);
            for (int a = 0; a < 36; ++a)
            {
                i->R=i->m;
                float x = i -> m * cos(a * M_PI / 18);
                float y = i -> m * sin(a * M_PI / 18);
                glVertex2f(i -> x + x, i -> y + y);
                x = i -> m * cos((a + 1) * M_PI / 18);
                y = i -> m * sin((a + 1) * M_PI / 18);
                glVertex2f(i -> x + x, i -> y + y);
            }
            glEnd();
        }
    }
    glutSwapBuffers();
}

void timer(int = 0)
{
    for (vector<point2D>::iterator i = p.begin(); i != p.end(); ++i)
        for (vector<point2D>::iterator j = p.begin(); j != p.end(); ++j)
            if (i != j)
            {
                float d = sqrt((i -> x - j -> x) * (i -> x - j -> x) + (i -> y - j -> y) * (i -> y - j -> y));
                if (d < i -> m + j -> m)
                {
                    float f = 100 * (i -> m + j -> m - d);
                    i -> vx += f * (i -> x - j -> x) / d / i -> m * DT;
                    i -> vy += f * (i -> y - j -> y) / d / i -> m * DT;
                    j -> vx -= f * (i -> x - j -> x) / d / j -> m * DT;
                    j -> vy -= f * (i -> y - j -> y) / d / j -> m * DT;
                }


            }
    for (vector<point2D>::iterator i = p.begin(); i != p.end(); ++i)
    {
        i -> x += i -> vx * DT;
        i -> y += i -> vy * DT;
        i->obrabotka();
    }
    display();
    glutTimerFunc(10, timer, 0);
}

int main(int argc, char **argv)
{
    point2D p0 = { Size / 2, Size / 2, 0, 0, 40 };

    //cout<<" x: "<<p0.x<<" y: "<<p0.y<<" vx: "<<p0.vx<<" vy: "<<p0.vy<<" m: "<<p0.m<<endl;
    unsigned int N=0;
    cout<<"Vvedite kol-vo mol-l: ";
    cin>>N;
    p.push_back(p0);
    for (int i = 0; i < N; ++i)
    {
        double x=rand() % Size;
        double y=rand() % Size;

        point2D p0 (x, y, rand() % 1000 / 50.0 - 10, rand() % 1000 / 50.0 - 10, 4.0 );
        //cout<<"NUMBER: "<<i<<" x: "<<p0.x<<" y: "<<p0.y<<" vx: "<<p0.vx<<" vy: "<<p0.vy<<" m: "<<p0.m<<endl;
        p.push_back(p0);
    }
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(Size, Size);
    glutInitWindowPosition(20, 1050 - Size - 20);
    glutCreateWindow("Brownian motion");
    glClearColor(0, 0, 0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho (0, Size, Size, 0, -1, 1);
    glutDisplayFunc(display);
    timer();
    glutMainLoop();
}
