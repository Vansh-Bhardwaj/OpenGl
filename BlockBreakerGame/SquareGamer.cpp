#include <GL/glut.h>

// Initial square position
float x = 0.0f;
float y = 0.0f;

// Velocity of the square
float velocityX = 0.0f;
float velocityY = 0.0f;

// Size of the square
float squareSize = 0.1f;

// Color of the square
float colorR = 1.0f;
float colorG = 1.0f;
float colorB = 1.0f;

// Step size for smoother movement
float step = 0.01f;

void setColor(float r, float g, float b) {
    colorR = r;
    colorG = g;
    colorB = b;
}

void setVelocity(float vx, float vy) {
    velocityX = vx;
    velocityY = vy;
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Set the color
    glColor3f(colorR, colorG, colorB);

    // Draw the square
    glBegin(GL_QUADS);
        glVertex2f(x - squareSize, y - squareSize);
        glVertex2f(x + squareSize, y - squareSize);
        glVertex2f(x + squareSize, y + squareSize);
        glVertex2f(x - squareSize, y + squareSize);
    glEnd();

    glutSwapBuffers();
}

void keyboard(unsigned char key, int, int) {
    switch (key) {
        case 'w': case 'W': setVelocity(0, step); break;  // Move up
        case 'a': case 'A': setVelocity(-step, 0); break; // Move left
        case 's': case 'S': setVelocity(0, -step); break; // Move down
        case 'd': case 'D': setVelocity(step, 0); break;  // Move right
    }
}

void keyboardUp(unsigned char key, int, int) {
    switch (key) {
        case 'w': case 'W':
        case 'a': case 'A':
        case 's': case 'S':
        case 'd': case 'D': setVelocity(0, 0); break;  // Stop movement when key is released
    }
}

void timer(int value) {
    // Update the position using the velocity
    x += velocityX;
    y += velocityY;

    // Check boundaries and adjust position if necessary
    if (x - squareSize < -1.0f) x = -1.0f + squareSize;
    if (x + squareSize > 1.0f) x = 1.0f - squareSize;
    if (y - squareSize < -1.0f) y = -1.0f + squareSize;
    if (y + squareSize > 1.0f) y = 1.0f - squareSize;

    // Request redisplay
    glutPostRedisplay();

    // Re-register the timer callback to keep the animation running
    glutTimerFunc(16, timer, 0);  // Approximately 60 frames per second
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Move Square with WASD");

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardUp);
    glutTimerFunc(0, timer, 0);

    // Set an initial color (white)
    setColor(1.0f, 1.0f, 1.0f);

    glutMainLoop();
    return 0;
}
