/*****************************************************
   CTIS164 - Lab06 Part1
   Line Equations
   Ver06: Drawing x-intercept and y-intercept points
*****************************************************/
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 800
#define TIMER_PERIOD   20 // Period for the timer
#define TIMER_ON        1 // 0:Disable timer, 1:Enable timer
#define PI 3.1415

// Global Variables for Template File
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; //current Window width and height

// Structure to store point properties
typedef struct {
	float x, y;
} point_t;

// Structure to store line properties
typedef struct {
	point_t start, end;
	float m, b;    // Slope-Intercept form
	float A, B, C; // General form
	float dx, dy;  // Parametric form
} line_t;

// Mouse position
point_t mouse;

// Line properties are stored in this variable
line_t  line;

int numPoint = 0;

// Parametric form parameter for moving circular object.
float t = 0;

// To draw a filled circle, centered at (x,y) with radius r
void circle(int x, int y, int r) {
	float angle;
	glBegin(GL_POLYGON);
	for (int i = 0; i < 100; i++) {
		angle = 2 * PI*i / 100;
		glVertex2f(x + r * cos(angle), y + r * sin(angle));
	}
	glEnd();
}

void print(int x, int y, const char *string, void *font) {
	int len, i;

	glRasterPos2f(x, y);
	len = (int)strlen(string);
	for (i = 0; i < len; i++)
		glutBitmapCharacter(font, string[i]);
}

// To display text with variables
void vprint(int x, int y, void *font, const char *string, ...) {
	va_list ap;
	va_start(ap, string);
	char str[1024];
	vsprintf_s(str, string, ap);
	va_end(ap);

	int len, i;
	glRasterPos2f(x, y);
	len = (int)strlen(str);
	for (i = 0; i < len; i++)
		glutBitmapCharacter(font, str[i]);
}

void drawLine(float x1, float y1, float x2, float y2) {
	glBegin(GL_LINES);
		glVertex2f(x1, y1);
		glVertex2f(x2, y2);
	glEnd();
}

void drawCoordinateSystem() {
	glColor3f(1, 1, 0.5); // Yellow
	drawLine(0, -winHeight / 2, 0, winHeight / 2); // Y-axis
	drawLine(-winWidth / 2, 0, winWidth, 0); // X-axis

	glColor3f(1, 1, 1);
	// White dashes on the x-axis
	for (int i = -winWidth / 2 / 50 * 50; i <= winWidth / 2; i += 50)
		drawLine(i, 3, i, -3);
	// White dashes on the y-axis
	for (int i = -winHeight / 2 / 50 * 50; i <= winHeight / 2; i += 50)
		drawLine(3, i, -3, i);
}

void showCursorPosition() {
	// Show mouse coordinate at top right region
	glColor3f(1, 1, 1);
	vprint(winWidth / 2 - 100, winHeight / 2 - 30, GLUT_BITMAP_8_BY_13, "(%.0f %.0f)", mouse.x, mouse.y);
}

// Display line properties
void showLineEquations() {
	//Slope-Intercept Form
	glColor3f(1, 1, 1);
	vprint(-winWidth / 2 + 10, -winHeight / 2 + 80, GLUT_BITMAP_8_BY_13, "Slope-Intercept Form:");

	glColor3f(1, 1, 0);
	if (line.dx != 0)
		vprint(-winWidth / 2 + 190, -winHeight / 2 + 80, GLUT_BITMAP_8_BY_13, "y = (%.2f)x + %.2f ", line.m, line.b);
	else
		vprint(-winWidth / 2 + 190, -winHeight / 2 + 80, GLUT_BITMAP_8_BY_13, "Undefined");

	// General Form
	glColor3f(1, 1, 1);
	vprint(-winWidth / 2 + 10, -winHeight / 2 + 60, GLUT_BITMAP_8_BY_13, "General Line Equation:");

	glColor3f(1, 1, 0);
	vprint(-winWidth / 2 + 190, -winHeight / 2 + 60, GLUT_BITMAP_8_BY_13, "(%.2f)x + y = %.2f", line.A, line.C);

	// Parameteric Form
	glColor3f(1, 1, 1);
	vprint(-winWidth / 2 + 10, -winHeight / 2 + 40, GLUT_BITMAP_8_BY_13, "Parametric Form:");

	glColor3f(1, 1, 0);
	vprint(-winWidth / 2 + 190, -winHeight / 2 + 40, GLUT_BITMAP_8_BY_13, "x(t) = %.0f + (%.0f)t", line.start.x, line.dx);
	vprint(-winWidth / 2 + 190, -winHeight / 2 + 20, GLUT_BITMAP_8_BY_13, "y(t) = %.0f + (%.0f)t", line.start.y, line.dy);
}

// Drawing the Line passing through Start and End points
void showLine() {
	point_t start;
	start.x = line.start.x + line.dx * -200;
	start.y = line.start.y + line.dy * -200;

	point_t end;
	end.x = line.start.x + line.dx * 200;
	end.y = line.start.y + line.dy * 200;

	glColor3f(0.5, 0.5, 0.5);
	glBegin(GL_LINES);
		glVertex2f(start.x, start.y);
		glVertex2f(end.x, end.y);
	glEnd();
}

// Draw the Line Segment with a red color from Start to End
void showLineSegment() {
	glColor3f(1, 0.3, 0.3);
	glLineWidth(3);
	glBegin(GL_LINES);
		glVertex2f(line.start.x, line.start.y);
		glVertex2f(line.end.x, line.end.y);
	glEnd();
	glLineWidth(1);
	// Holes in the start and end points
	glColor3f(0, 0, 0);
	circle(line.start.x, line.start.y, 6);
	circle(line.end.x, line.end.y, 6);
}

// Moving circle from Start to End point
void showMovingCircle() {
	point_t movingPoint;
	movingPoint.x = line.start.x + line.dx * t;
	movingPoint.y = line.start.y + line.dy * t;
	glColor3f(0.4, 0.4, 1.0);
	circle(movingPoint.x, movingPoint.y, 6);
	glColor3f(1.0, 1.0, 1.0);
	vprint(movingPoint.x + 10, movingPoint.y + 10, GLUT_BITMAP_HELVETICA_12, "t = %.2f", t);
}

// Draw green circle at x-intersection (not horizontal line) 
void showXIntercept() {
	if (line.dy != 0) { // Not a horizontal line
		point_t xInter = { 0, 0 };  // X-intersection point (y=0)
		if (line.dx != 0) // Not a vertical line
			xInter.x = -line.b / line.m; // Find x where y = 0 (b=-m.x+y)
		else
			xInter.x = line.start.x; // xInter.x = line.end.x

		glColor3f(0.2, 1.0, 0.2);
		circle(xInter.x, 0, 4);
	}
}

// Draw green circle at y-intersection (not vertical line)
void showYIntercept() {
	if (line.dx != 0) {
		glColor3f(0.2, 1.0, 0.2);
		circle(0, line.b, 4);
	}
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

	// Draw the first point if it is available
	if (numPoint > 0)
		circle(line.start.x, line.start.y, 8);

	// Draw the second point and a line if there is more than one point
	if (numPoint > 1) {
		circle(line.end.x, line.end.y, 8);
		// Show three forms of line equations
		showLineEquations();
		// Show line passing through given two points
		showLine();
		// Show line segment between start and end points
		showLineSegment();
		// Moving object (circle) along the line segment
		showMovingCircle();
		// Show x-intercept point if it exists
		showXIntercept();
		// Show y-intercept point if it exists
		showYIntercept();
	}
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

line_t calculateLine(line_t ln) {
	// Delta(diff) calculations
	ln.dx = ln.end.x - ln.start.x;
	ln.dy = ln.end.y - ln.start.y;
	// General Form
	ln.A = -ln.dy;
	ln.B = ln.dx;
	ln.C = ln.A * ln.start.x + ln.B * ln.start.y;
	// Simplify: coefficient of y (B) will be 1
	if (ln.B != 0.0) {
		ln.A = ln.A / ln.B;
		ln.C = ln.C / ln.B;
		ln.B = 1;
	}
	// Slope Intercept Form
	if (ln.dx != 0) { // If not a vertical line
		ln.m = ln.dy / ln.dx;
		ln.b = -ln.m * ln.start.x + ln.start.y;
	}
	return ln;
}

// When a click occurs in the window, It provides which button
// Buttons : GLUT_LEFT_BUTTON , GLUT_RIGHT_BUTTON
// States  : GLUT_UP , GLUT_DOWN
// x, y is the coordinate of the point that mouse clicked
void onClick(int button, int stat, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && stat == GLUT_DOWN) {
		// First click for the first point
		if (numPoint == 0)
			line.start = mouse;
		else {
			line.end = mouse;
			// Line properties will be calculated here
			line = calculateLine(line);
			t = 0.0f;
		}
		numPoint = (numPoint + 1) % 3; // Modulus operation for the next line
	}
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

	t += 0.01f;
	if (t > 1.00f)
		t = 0.0f;  // Go to starting point.

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