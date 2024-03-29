#include <GL/glut.h>
#include <cmath>
#include <ctime>

float timePassed = 0.0f;
const float speedIncreaseRate = 0.00005f;  
bool gameRunning = true;
bool firstMoveMade = false;

class Square {
public:
    float x, y;
    float velocityX, velocityY;
    float size;
    float colorR, colorG, colorB;

    Square(float startX, float startY, float startSize, float r, float g, float b)
        : x(startX), y(startY), size(startSize), colorR(r), colorG(g), colorB(b),
          velocityX(0.0f), velocityY(0.0f) {}

    void setVelocity(float vx, float vy) {
        if (vx != -2.0f) velocityX = vx;
        if (vy != -2.0f) velocityY = vy;
    }

    void draw() {
        glColor3f(colorR, colorG, colorB);
        glBegin(GL_QUADS);
            glVertex2f(x - size, y - size);
            glVertex2f(x + size, y - size);
            glVertex2f(x + size, y + size);
            glVertex2f(x - size, y + size);
        glEnd();
    }

    void updatePosition() {
        if (gameRunning) {
            x += velocityX;
            y += velocityY;

            
            if (velocityX != 0.0f) {
                velocityX += copysign(speedIncreaseRate / 5, velocityX);  
            }
            if (velocityY != 0.0f) {
                velocityY += copysign(speedIncreaseRate / 5, velocityY);  
            }
        }
    }

    void checkBoundary() {
        
        if (x - size < -1.0f) x = -1.0f + size;
        if (x + size > 1.0f) x = 1.0f - size;
        if (y - size < -1.0f) y = -1.0f + size;
        if (y + size > 1.0f) y = 1.0f - size;
    }

    bool collidesWith(const Square& other) const {
        return !(x + size < other.x - other.size || x - size > other.x + other.size ||
                 y + size < other.y - other.size || y - size > other.y + other.size);
    }

    void drawGameOver() {
        glColor3f(1.0f, 1.0f, 1.0f);
        glRasterPos2f(-0.1f, 0.0f);
        const char* gameOverText = "Game Over";
        for (const char* c = gameOverText; *c != '\0'; c++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
        }
    }
};

class BouncingSquare : public Square {
public:
    BouncingSquare(float startX, float startY, float startSize, float r, float g, float b)
        : Square(startX, startY, startSize, r, g, b) {
        std::srand(std::time(0));
        float angle = (std::rand() % 360) * (3.14159f / 180.0f);
        velocityX = 0.01f * std::cos(angle);
        velocityY = 0.01f * std::sin(angle);
    }

    void checkBoundary() {
        if (x - size < -1.0f || x + size > 1.0f) {
            velocityX = -velocityX;
            addRandomAngle();
        }
        if (y - size < -1.0f || y + size > 1.0f) {
            velocityY = -velocityY;
            addRandomAngle();
        }
    }

    void addRandomAngle() {
        float angle = ((std::rand() % 20) - 10) * (3.14159f / 180.0f); // Random angle between -10 and 10 degrees
        float newVelocityX = velocityX * std::cos(angle) - velocityY * std::sin(angle);
        float newVelocityY = velocityX * std::sin(angle) + velocityY * std::cos(angle);
        
        velocityX = newVelocityX;
        velocityY = newVelocityY;
    }
};

Square mainSquare(0.0f, 0.0f, 0.1f, 1.0f, 1.0f, 1.0f);
BouncingSquare *secondSquare = NULL;

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    if (gameRunning) {
        mainSquare.draw();
        if (secondSquare) secondSquare->draw();
    } else {
        mainSquare.drawGameOver();
    }
    glutSwapBuffers();
}

void keyboard(unsigned char key, int, int) {
    if (!gameRunning) return;

    if (!firstMoveMade) {
        firstMoveMade = true;
        secondSquare = new BouncingSquare(mainSquare.x + 0.2f, mainSquare.y + 0.2f, 0.1f, 1.0f, 0.0f, 0.0f);
    }

    switch (key) {
        case 'w': case 'W': mainSquare.setVelocity(-2.0f, 0.01f); break;
        case 'a': case 'A': mainSquare.setVelocity(-0.01f, -2.0f); break;
        case 's': case 'S': mainSquare.setVelocity(-2.0f, -0.01f); break;
        case 'd': case 'D': mainSquare.setVelocity(0.01f, -2.0f); break;
    }
}

void keyboardUp(unsigned char key, int, int) {
    switch (key) {
        case 'w': case 'W': mainSquare.setVelocity(-2.0f, 0); break;
        case 'a': case 'A': mainSquare.setVelocity(0, -2.0f); break;
        case 's': case 'S': mainSquare.setVelocity(-2.0f, 0); break;
        case 'd': case 'D': mainSquare.setVelocity(0, -2.0f); break;
    }
}

void timer(int value) {
    mainSquare.updatePosition();
    mainSquare.checkBoundary();

    if (secondSquare) {
        secondSquare->updatePosition();
        secondSquare->checkBoundary();

        
        if (mainSquare.collidesWith(*secondSquare)) {
            gameRunning = false;
        }
    }

    glutPostRedisplay();
    glutTimerFunc(7, timer, 0);  
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

    glutMainLoop();
    if (secondSquare) {
        delete secondSquare;
    }
    return 0;
}
