#include "includes/header.h"
#include "includes/canvas.h"
#include <iostream>
using namespace std;

void redraw(void);
bool local_rotate = false;
bool global_rotate = false;

Canvas cvs(600,600,"Rotations!"); //global canvas object

class Rectangle2 {
	private:
		float center;
		RealRect r;
	public:
		Rectangle2() {
			cvs.moveTo(0,0);
			center = 0.0;
			r = RealRect();
			reposition();
		}

		void reposition() {
			r.set(center - 10.0, center + 10.0, center - 20.0, center + 20.0);
		}

		void setCenter(float x, float y) {
		}

		void draw() {
			r.draw();
		}
};

int tran_x, tran_y;
float rot_amt = 0;
float glob_rot_amt = 0;
void rotate();

Rectangle2 temp = Rectangle2();

void display(void)
{	

	// Draw axis
	cvs.clearScreen();//clear screen
	cvs.setColor(1.0,0.0,0.0);
	cvs.moveTo(-100,0);
	cvs.lineTo(100,0);
	cvs.moveTo(0,-100);
	cvs.lineTo(0,100);
	
	//glMatrixMode(GL_MODELVIEW); // Must be done before drawing

	//glLoadIdentity(); // I
	//glMultMatrixf(N); // 
	//glMultMatrixf(M); // 
	//glMultMatrixf(L); // NMLI
	glPushMatrix();

	glRotatef(glob_rot_amt,0.0,0.0,-1.0);
	glTranslated(tran_x,tran_y,0);
	glRotatef(rot_amt,0.0,0.0,-1.0);

	cvs.setColor(0.0,0.0,1.0);
	temp.draw();
	glPopMatrix();

	glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) {

	switch(key) {
			case 'G':
				global_rotate = true;
				glutIdleFunc(rotate); // This starts the callback
				break;
			case 'g':
				global_rotate = false;
				if(!local_rotate)
					glutIdleFunc(NULL);
				break;
			case 'L':
				local_rotate = true;
				glutIdleFunc(rotate); // This starts the callback
				return;
			case 'l':
				local_rotate = false;
				if(!global_rotate)
					glutIdleFunc(NULL);
				break;
			case 'X':
				tran_x += 1;
				break;
			case 'x':
				tran_x -= 1;
				break;
			case 'Y':
				tran_y += 1;
				break;
			case 'y':
				tran_y -= 1;
				break;
			case 'R':
			case 'r':
				break;
			default:
				return;
	}
	glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		// We need to stop the rotating, otherwise it will mess up
		glutIdleFunc(NULL);
		glob_rot_amt = 0;

		x -= 300;
		y -= 300;
		y *= -1;
		float asp_ratio = 100.00 / 300.00;
		// This may look weird, but I need to do it this way
		// in order for it to work right
		float xx = (float)x * asp_ratio;
		float yy = (float)y * asp_ratio;
		global_rotate = false;
		local_rotate = false;
		tran_x = (int)xx;
		tran_y = (int)yy;
		glutPostRedisplay();
	}
	else if(button != GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		tran_x = 0;
		tran_y = 0;
		rot_amt = 0;
		glob_rot_amt = 0;
		global_rotate = false;
		local_rotate = false;
		glutIdleFunc(NULL);
		glutPostRedisplay();
	}
}

void rotate() {
	if(local_rotate)
		rot_amt += 1;
	if(global_rotate)
		glob_rot_amt += 1;
	glutPostRedisplay();
}

int main(int argc, char* argv[])
{
	//the window is opened in the Canvas constructor 
	cvs.setBackgroundColor(1.0,1.0,1.0); //background is white
	cvs.setColor(0.0,0.0,0.0); //set drawing color

	cvs.setWindow(-100.0,100.0,-100.0,100.0);
	cvs.setViewport(0,600,0,600);

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse); 
	glutMainLoop();

	return 0;
}
