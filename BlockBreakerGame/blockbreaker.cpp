#include <GL/glut.h>


float ballX = 0.0f;
float ballY = 0.0f;
float ballSpeedX = 0.005f;  
float ballSpeedY = 0.0075f; 
float ballRadius = 0.0166f; 
bool isGameOver = false;


float sliderWidth = 0.2f;
float sliderHeight = 0.03f;
float sliderX = 0.0f;
float sliderSpeed = 0.009f;



const int ROWS = 5;
const int COLUMNS = 8;
struct Brick {
    bool isBroken;
    float x;
    float y;
    float width;
    float height;
    float r, g, b;  
} bricks[ROWS][COLUMNS];

void initializeBricks() {
    float width = 0.2f;
    float height = 0.05f;
    float gap = 0.02f;
    for(int i = 0; i < ROWS; i++) {
        for(int j = 0; j < COLUMNS; j++) {
            bricks[i][j].isBroken = false;
            bricks[i][j].width = width;
            bricks[i][j].height = height;
            bricks[i][j].x = -0.8f + j * (width + gap);
            bricks[i][j].y = 0.8f - i * (height + gap);
            bricks[i][j].r = (i + j) % 2;
            bricks[i][j].g = i * 0.2f;
            bricks[i][j].b = j * 0.2f;
        }
    }
}

void drawBoundary() {
    glColor3f(1.0f, 1.0f, 1.0f);
    glRectf(-0.95f, -0.95f, 0.95f, 0.95f);
}

void drawBall() {
    glColor3f(1.0f, 0.0f, 0.0f);
    glPushMatrix();
    glTranslatef(ballX, ballY, 0.0f);
    glutSolidSphere(ballRadius, 20, 20);
    glPopMatrix();
}

void drawSlider() {
    glColor3f(0.0f, 1.0f, 0.0f);
    glRectf(sliderX - sliderWidth/2, -0.9f, sliderX + sliderWidth/2, -0.9f + sliderHeight);
}

void drawBricks() {
    for(int i = 0; i < ROWS; i++) {
        for(int j = 0; j < COLUMNS; j++) {
            if(!bricks[i][j].isBroken) {
                glColor3f(bricks[i][j].r, bricks[i][j].g, bricks[i][j].b);
                glRectf(bricks[i][j].x, bricks[i][j].y, bricks[i][j].x + bricks[i][j].width, bricks[i][j].y - bricks[i][j].height);
            }
        }
    }
}

void drawGameOver() {
    glColor3f(1.0f, 0.0f, 0.0f);
    char msg[] = "Game Over";
    glRasterPos2f(-0.15f, 0.0f);
    for (int i = 0; i < sizeof(msg) - 1; i++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, msg[i]);
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    drawBoundary();

    if(isGameOver) {
        drawGameOver();
    } else {
        drawBall();
        drawSlider();
        drawBricks();
    }

    glutSwapBuffers();
}

void handleMouse(int x, int y) {
    
    float mouseX = static_cast<float>(x) / glutGet(GLUT_WINDOW_WIDTH) * 2.0f - 1.0f;

    
    sliderX = mouseX;

    
    if (sliderX - sliderWidth / 2 < -0.95f)
        sliderX = -0.95f + sliderWidth / 2;
    if (sliderX + sliderWidth / 2 > 0.95f)
        sliderX = 0.95f - sliderWidth / 2;
}


void checkBrickCollision() {
    for(int i = 0; i < ROWS; i++) {
        for(int j = 0; j < COLUMNS; j++) {
            if(!bricks[i][j].isBroken) {
                if(ballX + ballRadius >= bricks[i][j].x && ballX - ballRadius <= bricks[i][j].x + bricks[i][j].width &&
                   ballY + ballRadius >= bricks[i][j].y - bricks[i][j].height && ballY - ballRadius <= bricks[i][j].y) {
                    bricks[i][j].isBroken = true;
                    ballSpeedY = -ballSpeedY;
                    return;
                }
            }
        }
    }
}

void update(int value) {
    if(isGameOver) return;

    ballX += ballSpeedX;
    ballY += ballSpeedY;

    

    if(sliderX - sliderWidth/2 < -0.95f) sliderX = -0.95f + sliderWidth/2;
    if(sliderX + sliderWidth/2 > 0.95f) sliderX = 0.95f - sliderWidth/2;

    if(ballX + ballRadius > 0.95f || ballX - ballRadius < -0.95f) {
        ballSpeedX = -ballSpeedX;
    }

    if(ballY + ballRadius > 0.95f) {
        ballSpeedY = -ballSpeedY;
    }

    if(ballY - ballRadius <= -0.9f + sliderHeight && ballY - ballRadius >= -0.9f &&
       ballX + ballRadius >= sliderX - sliderWidth/2 && ballX - ballRadius <= sliderX + sliderWidth/2) {
        ballSpeedY = -ballSpeedY;
        float hitPosition = ballX - sliderX;
        ballSpeedX += hitPosition * 0.05f;
    }

    checkBrickCollision();

    if(ballY - ballRadius < -0.95f) {
        isGameOver = true;
    }

    glutPostRedisplay();
    glutTimerFunc(10, update, 0);
}

int main(int argc, char** argv) {
    initializeBricks();

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Block Breaker Game");

    
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    glutDisplayFunc(display);
    glutPassiveMotionFunc(handleMouse); 
    glutTimerFunc(0, update, 0);

    glutMainLoop();
    return 0;
}
