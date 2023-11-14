/************************************************
   CTIS164 - Lab06 Part1
   Line Equations
   Ver01: Coordinate system on static dimensions
************************************************/
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 500
#define TIMER_PERIOD 1000 // Period for the timer
#define TIMER_ON     0    // 0:Disable timer, 1:Enable timer
#define PI 3.1415

// Global Variables for Template File
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; //current Window width and height

// Structure to store point properties
typedef struct {
   float x, y;
} point_t;

// Mouse position
point_t mouse;

void print(int x, int y, const char *string, void *font) {
   int len, i;

   glRasterPos2f(x, y);
   len = (int)strlen(string);
   for (i = 0; i<len; i++)
      glutBitmapCharacter(font, string[i]);
}

// To display text with variables
// vprint(-winWidth / 2 + 10, winHeight / 2 - 20, GLUT_BITMAP_8_BY_13, "ERROR: %d", numClicks);
void vprint(int x, int y, void *font, const char *string, ...) {
   va_list ap;
   va_start(ap, string);
   char str[1024];
   vsprintf_s(str, string, ap);
   va_end(ap);

   int len, i;
   glRasterPos2f(x, y);
   len = (int)strlen(str);
   for (i = 0; i<len; i++)
      glutBitmapCharacter(font, str[i]);
}

void drawLine(float x1, float y1, float x2, float y2) {
   glBegin(GL_LINES);
	   glVertex2f(x1, y1);
	   glVertex2f(x2, y2);
   glEnd();
}

void drawCoordinateSystem() {
   glColor3f(1, 1, 0); // Yellow
   drawLine(0, -250, 0, 250); // Y-axis
   drawLine(-400, 0, 400, 0); // X-axis

   glColor3f(1, 1, 1);
   // White dashes on the x-axis
   for (int i = -350; i <= 350; i += 50)
      drawLine(i, 3, i, -3);
   // White dashes on the y-axis
   for (int i = -200; i <= 200; i += 50)
      drawLine(3, i, -3, i);
}

void showCursorPosition() {
   // Show mouse coordinate at top right region
   glColor3f(1, 1, 1);
   vprint(300, 220, GLUT_BITMAP_8_BY_13, "(%.0f %.0f)", mouse.x, mouse.y);
}

// To display onto window using OpenGL commands
void display() {
   // Clear window to black
   glClearColor(0, 0, 0, 0);
   glClear(GL_COLOR_BUFFER_BIT);

   // Draw x and y axis with marks
   drawCoordinateSystem();

   // Show mouse coordinate at top right region
   showCursorPosition();

   glutSwapBuffers();
}

// Key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
void onKeyDown(unsigned char key, int x, int y) {
   // Exit when ESC is pressed.
   if (key == 27)
      exit(0);

   // To refresh the window it calls display() function
   glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y) {
   // Exit when ESC is pressed.
   if (key == 27)
      exit(0);

   // To refresh the window it calls display() function
   glutPostRedisplay();
}

// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
void onSpecialKeyDown(int key, int x, int y) {
   switch (key) {
	   case GLUT_KEY_UP:
		  up = true;
		  break;
	   case GLUT_KEY_DOWN:
		  down = true;
		  break;
	   case GLUT_KEY_LEFT:
		  left = true;
		  break;
	   case GLUT_KEY_RIGHT:
		  right = true;
		  break;
   }
   // To refresh the window it calls display() function
   glutPostRedisplay();
}

// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
void onSpecialKeyUp(int key, int x, int y) {
   switch (key) {
	   case GLUT_KEY_UP:
		  up = false;
		  break;
	   case GLUT_KEY_DOWN:
		  down = false;
		  break;
	   case GLUT_KEY_LEFT:
		  left = false;
		  break;
	   case GLUT_KEY_RIGHT:
		  right = false;
		  break;
   }
   // To refresh the window it calls display() function
   glutPostRedisplay();
}

// When a click occurs in the window, It provides which button
// Buttons : GLUT_LEFT_BUTTON , GLUT_RIGHT_BUTTON
// States  : GLUT_UP , GLUT_DOWN
// x, y is the coordinate of the point that mouse clicked
void onClick(int button, int stat, int x, int y) {
   // Write your codes here.

   // To refresh the window it calls display() function
   glutPostRedisplay();
}

// This function is called when the window size changes.
// w : is the new width of the window in pixels
// h : is the new height of the window in pixels
void onResize(int w, int h) {
   winWidth = w;
   winHeight = h;
   glViewport(0, 0, w, h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1, 1);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   display(); // Refresh window
}

void onMoveDown(int x, int y) {
	// Write your codes here

	// To refresh the window it calls display() function
	glutPostRedisplay();
}

// GLUT to OpenGL coordinate conversion
// x2 = x1 - winWidth / 2
// y2 = winHeight / 2 - y1
void onMove(int x, int y) {
   mouse.x = x - winWidth / 2;
   mouse.y = winHeight / 2 - y;

   // To refresh the window it calls display() function
   glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v) {
   glutTimerFunc(TIMER_PERIOD, onTimer, 0);
   // Write your codes here

   // To refresh the window it calls display() function
   glutPostRedisplay();
}
#endif

void init() {
   // Smoothing shapes
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

int main(int argc, char *argv[]) {
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
   glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
   glutCreateWindow("Lab06-Part1: Line Equations");

   // Window Events
   glutDisplayFunc(display);
   glutReshapeFunc(onResize);

   // Keyboard Events
   glutKeyboardFunc(onKeyDown);
   glutSpecialFunc(onSpecialKeyDown);
   glutKeyboardUpFunc(onKeyUp);
   glutSpecialUpFunc(onSpecialKeyUp);

   // Mouse Events
   glutMouseFunc(onClick);
   glutMotionFunc(onMoveDown);
   glutPassiveMotionFunc(onMove);

#if TIMER_ON == 1
   // Timer event
   glutTimerFunc(TIMER_PERIOD, onTimer, 0);
#endif

   init();

   glutMainLoop();
   return 0;
}