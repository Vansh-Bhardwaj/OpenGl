#include <GL/glut.h>
#include <cmath>

float squareScale = 1.0;
float squareDirection = 1.0;

float triangleAngle = 0.0;

float linePosition = -0.5;
float lineDirection = 1.0;

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw square (zoom in and out animation)
    glPushMatrix();
    glTranslatef(-0.6, 0.0, 0.0);
    glScalef(squareScale, squareScale, 1.0);
    glBegin(GL_QUADS);
    glColor3f(1.0, 0.0, 0.0);
    glVertex2f(-0.1, -0.1);
    glVertex2f(0.1, -0.1);
    glVertex2f(0.1, 0.1);
    glVertex2f(-0.1, 0.1);
    glEnd();
    glPopMatrix();

    // Draw equilateral triangle (rotation animation)
    glPushMatrix();
    glTranslatef(0.6, 0.0, 0.0);
    glRotatef(triangleAngle, 0.0, 0.0, 1.0);
    glBegin(GL_TRIANGLES);
    glColor3f(0.0, 1.0, 0.0);
    glVertex2f(-0.2, -0.1732);
    glVertex2f(0.2, -0.1732);
    glVertex2f(0.0, 0.2);
    glEnd();
    glPopMatrix();

    // Draw line (move left and right animation)
    glPushMatrix();
    glTranslatef(linePosition, -0.6, 0.0);
    glBegin(GL_LINES);
    glColor3f(0.0, 0.0, 1.0);
    glVertex2f(-0.2, 0.0);
    glVertex2f(0.2, 0.0);
    glEnd();
    glPopMatrix();

    glutSwapBuffers();
}

void timer(int extra) {
    squareScale += 0.01 * squareDirection;
    if(squareScale >= 1.5 || squareScale <= 0.5) {
        squareDirection *= -1;
    }

    triangleAngle += 2.0;
    if(triangleAngle >= 360.0) {
        triangleAngle -= 360.0;
    }

    linePosition += 0.005 * lineDirection;
    if(linePosition >= 0.5 || linePosition <= -0.5) {
        lineDirection *= -1;
    }

    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("OpenGL Animation");
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
    glutDisplayFunc(display);
    glutTimerFunc(0, timer, 0);
    glutMainLoop();

    return 0;
}
