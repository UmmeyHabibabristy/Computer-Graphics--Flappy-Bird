#include<windows.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>
#include <unistd.h>
#include <algorithm>
#include <list>
#include <fstream>
#include <iostream>
#include<mmsystem.h>

int score =0;

using namespace std;

int windowX = 1200, windowY = 600 ;
GLfloat positionX = 0.0f;
GLfloat positionY = 0.0f;
GLfloat speed = .1f;
GLfloat speedEnvironment = .05f;

list <double> rX;
list <double> rY;
list <int> life;

GLfloat birdX = -5.00f;
GLfloat birdY = 0.50f;
bool checkOnlyOneTime = true;


char text[] = "Your score is: ";

void Sprint( float x, float y, char *st)
{
    int l,i;
    l=strlen( st ); // see how many characters are in text string.
    glColor3f(0.0,0.0,0.0);
    glRasterPos3f( x, y,2); // location to start printing text
    for( i=0; i < l; i++) // loop until i is greater then l
    {
       glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[i]);

    }
}
void Sprint1( float x, float y, char *st)
{
    int l,i;
    l=strlen( st ); // see how many characters are in text string.
    glColor3f(0.0,0.0,0.0);
    glRasterPos3f( x, y,2.5); // location to start printing text
    for( i=0; i < l; i++) // loop until i is greater then l
    {
       glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[i]);

    }
}


void initRendering()
{
    gluOrtho2D(0.0, 1200.0, 0, 600.0);
    glEnable(GL_DEPTH_TEST);
}

//Called when the window is resized
void handleResize(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)w / (double)h, 1.0, 200.0);
}

void generateRandomNumber(){
    double ryArr[9] = {-1, -.5, 0, .5, 1, 1.5, 2, 2.5, 3};
    /// For bar/cube
     for(int i=0; i<30; i++){
        int rx= rand() % 55 + 0;
        rX.push_back(rx);
        /// 0 to 55..
        int ryi= rand() % 9 + 0;
        rY.push_back(ryArr[ryi]);
        /// -1 to 3
    }

}


bool collision()
{
    /// Check collision of obstacles/bar/cube
    rX.sort();
    list<double>::iterator ity = rY.begin();
    for(list<double>::iterator it = rX.begin(); it != rX.end(); ++it){
        ++ity;
        if((positionX+(*it)-.55)<=birdX){
            if(positionY+(*ity)==birdY){
                return true;
            }
            else{
                rX.erase(it);
                rY.erase(ity);
                return false;
            }
        }
        else{
            return false;
        }
    }
}

int checkLife = 0;

float colorSky1 = .2, colorSky2 = 0.765, colorSky3= 1.0;

static bool  startingPage = true;
static bool endingPage = false;

void drawScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity(); //Reset the drawing perspective
    glTranslatef(0.0, 0.0, -7.0);
    /// Starting Page
    if(startingPage){
            glPushMatrix();
            glColor3f(0.0f, 0.5f, 1.0f);
            glBegin(GL_QUADS);
            glVertex3f(-30, -3.0, 1.0);
            glVertex3f(30, -3.0, 1.0);
            glVertex3f(30, 5, 1.0);
            glVertex3f(-30,  5, 1.0);
            glEnd();
            glPopMatrix();

            char startWriting[100] = "Press X to start...";
            Sprint(-1,0,startWriting);

            char startWriting1[100] = "***Settings***";
            Sprint(-.5,-.5,startWriting1);

            char startWriting2[100] = "Movement : up,down,Right,Left key";
            Sprint(-1.5,-.65,startWriting2);

            char startWriting3[100] = "# Speed up: S";
            Sprint(-1.5,-.8,startWriting3);
    }
    /// LIFE
   glColor3f(1.0f, 0.0f, 1.0f);
    for(list<int>::iterator it1 = life.begin(); it1 != life.end(); ++it1){
    glPushMatrix();
        glBegin(GL_QUADS);
        glVertex3f((*it1),2.5,0);
        glVertex3f((*it1)+1,2.5,0);
        glVertex3f((*it1)+1,2.7,0);
        glVertex3f((*it1),2.7,0);
        glEnd();
    glPopMatrix();
    }

    /// SKY
    glColor3f(colorSky1, colorSky2, colorSky3);
    glBegin(GL_QUADS);
    glVertex3f(-30, -3.0, -1.0);
    glVertex3f(30, -3.0, -1.0);
    glVertex3f(30, 5, -1.0);
    glVertex3f(-30,  5, -1.0);
    glEnd();

   /// GROUND
    glColor3f(0.0, 1.0, 0.0);
    glBegin(GL_QUADS);
    glVertex3f(-30, -1.0, 0.0);
    glVertex3f(30, -1.0, 0.0);
    glVertex3f(30, -3, 0.0);
    glVertex3f(-30,  -3, 0.0);
    glEnd();
///head
      glPushMatrix();
    glTranslatef(birdX+.4, birdY+.07, 0.0);
    glutSolidSphere(.15,50,50);
    glPopMatrix();

    ///bird Body
    glColor3f(1, 1, 0);
    glPushMatrix();
    glTranslatef(birdX, birdY, 0.0);
    glutSolidSphere(.3,50,50);
    glPopMatrix();
    glColor3f(1, .5, 0);
 /// Lips

    glPushMatrix();
    glBegin(GL_TRIANGLES);
    glVertex3f(birdX+.55,birdY+.1,0.0);
    glVertex3f(birdX+.7,birdY+.04,0.0);
    glVertex3f(birdX+.55,birdY+.01,0.0);
    glEnd();
    glPopMatrix();

    /// tail
    glColor3f(0, .5, 0);
    glPushMatrix();
    glBegin(GL_TRIANGLES);
    glVertex3f(birdX-.1,birdY+.03,0.0);
    glVertex3f(birdX-.7,birdY+.4,0.0);
    glVertex3f(birdX-.3,birdY+.00,0.0);
    glEnd();
    glPopMatrix();


    /// bars

     list<double>::iterator ity = rY.begin();
    for(list<double>::iterator it = rX.begin(); it != rX.end(); ++it){
        ++ity;
        glPushMatrix();
        glColor3f(1.0f, 0.5f, 0.0f);
        glTranslatef(positionX+(*it), positionY+(*ity), 0.0);
        glutSolidCube(.6);

        glPopMatrix();
    }
    /// Check Collision/ end screen
    if(collision()){
        life.pop_back();
        checkLife++;
        if(life.empty()){
                endingPage= true;
            glColor3f(0.0f, 0.5f, 1.0f);
            glBegin(GL_QUADS);
            glVertex3f(-6, -3.0, 2.0);
            glVertex3f(6, -3.0, 2.0);
            glVertex3f(6, 3, 2.0);
            glVertex3f(-6,  3, 2.0);
            glEnd();
            Sprint1(-.5,0,"[[[[GAME OVER]]]]");

            char Score_string[100];
            sprintf(Score_string,"%d",score);
            char print_string[100] = "Your score: ";
            strcat(print_string,Score_string);
            Sprint1(-0.5,-.5,print_string);
            speed=0.0;
            speedEnvironment = 0.0;

            int getLastHighScore;
        ifstream infile;
        infile.open("Score.txt");

        infile>>getLastHighScore;

        if(score>getLastHighScore){
            ofstream scoreFile;
            scoreFile.open("Score.txt");
            scoreFile << score << endl;
            checkOnlyOneTime= false;
        }
        }

    }
    else{
        if(startingPage==false && endingPage==false){
            score++;
        }
    }
///Score
    int getLastHighScore;

    ifstream infile;
    infile.open("Score.txt");

    infile>>getLastHighScore;

    if(score>getLastHighScore){
            char highScoreDone[100] = "[[[[Winner]]]]";
            Sprint(-1.3,-1.6,highScoreDone);
        }
    char highScore_string[100];
    sprintf(highScore_string,"%d",getLastHighScore);

    char print_high_string[100] = "High score: ";
    strcat(print_high_string,highScore_string);
    Sprint(-0.5,1.95,print_high_string);

    char score_string[100];
    sprintf(score_string,"%d",score);

    char print_string[100] = "Your score: ";
    strcat(print_string,score_string);

    Sprint(-4,1.95,print_string);
    glutSwapBuffers();
}

///Key press
int checkS=0;
int checkJump=0;
int birdYa=0;
void handleKeypress(int key, int x, int y)
{
    switch (key)
    {
        /// UP
    case GLUT_KEY_UP:
        if(birdY<2.5){
            birdY+=0.5;
        }
        break;
        /// DOWN
    case GLUT_KEY_DOWN:
        if(birdY>-1){
             birdY-=0.5;
        }
        glutPostRedisplay();

        break;
        /// LEFT
    case GLUT_KEY_LEFT:
        if(birdX>-5.5){
            birdX-=0.5;
        }
        glutPostRedisplay();
        break;
        ///right
    case GLUT_KEY_RIGHT:
        if(birdX<5.5){
            birdX+=0.5;
        }
        glutPostRedisplay();

        break;
    }
}

void handleKeypress1(unsigned char key, int x, int y)
{
    switch (key)
    {
    case ' ':
        if(checkS==0){
            speed = 0.0f;
            checkS=1;
        }
        else{
            speed = 0.05f;
            checkS=0;
        }
        break;
    case 's':
        speed += 0.02f;
        break;
    case 'x':
        startingPage=false;
        positionX=0;
        glutPostRedisplay();
        break;
    }
}

void update(int value)
{
        if(positionX < -60){
            positionX =5.0f;
            speed += 0.05;
            generateRandomNumber();
        }
        positionX -= speed;
        glutPostRedisplay();
        glutTimerFunc(50, update, 0);
}

int main(int argc, char** argv)
{
   generateRandomNumber();
   for(int i= -6; i<=6; i++){
        life.push_back(i);
   }
    ///Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1200, 600);
    glutInitWindowPosition(200,100);

    ///Create the window
    glutCreateWindow("Flappy Bird");
    initRendering();

    ///Set handler functions
    glutDisplayFunc(drawScene);
    glutReshapeFunc(handleResize);
    glutSpecialFunc(handleKeypress);
    glutKeyboardFunc(handleKeypress1);

 ///Add a timer
    glutTimerFunc(200, update, 0);

    glutMainLoop();

    return 0;
}
