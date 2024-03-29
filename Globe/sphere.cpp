#include <GL/glut.h>
#include <cmath>
#include <string.h> 
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

float angle = 0.5f;
GLuint texture;

void loadTexture(const char* filename) {
    int width, height, nrChannels;
    unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);
    if (!data) {
        fprintf(stderr, "Failed to load texture: %s\n", filename);
        exit(EXIT_FAILURE);
    }
    
    if(texture) glDeleteTextures(1, &texture); 
    
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (nrChannels == 3)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    else if (nrChannels == 4)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    
    stbi_image_free(data);
}

void renderBitmapString(float x, float y, void *font, const char *string) {
    const char *c;
    glRasterPos2f(x, y);
    for (c = string; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    
    glColor3f(1.0f, 1.0f, 1.0f); 

    
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(-2.5, 2.5, -2.5, 2.5);

   
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    renderBitmapString(-0.8f, 2.3f, GLUT_BITMAP_HELVETICA_18, "1 - Night");
    renderBitmapString(-0.8f, 2.1f, GLUT_BITMAP_HELVETICA_18, "2 - Day");
    glPopMatrix(); 

    
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0f, 0.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
    glRotatef(angle, 0.0f, 1.0f, 0.0f);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);

glBegin(GL_QUADS);
for(int i = 0; i < 360; i+=10) {
    for(int j = -90; j < 90; j+=10) {
        float theta1 = i * (M_PI / 180), phi1 = j * (M_PI / 180);
        float theta2 = (i + 10) * (M_PI / 180), phi2 = (j + 10) * (M_PI / 180);
        
        float rotationAdjustment = 0.0f; 
        
        glTexCoord2f(fmod((i / 360.0f) + rotationAdjustment, 1.0f), (j + 90) / 180.0f);
        glVertex3f(cos(theta1) * cos(phi1), sin(phi1), sin(theta1) * cos(phi1));
        
        glTexCoord2f(fmod(((i + 10) / 360.0f) + rotationAdjustment, 1.0f), (j + 90) / 180.0f);
        glVertex3f(cos(theta2) * cos(phi1), sin(phi1), sin(theta2) * cos(phi1));
        
        glTexCoord2f(fmod(((i + 10) / 360.0f) + rotationAdjustment, 1.0f), (j + 100) / 180.0f);
        glVertex3f(cos(theta2) * cos(phi2), sin(phi2), sin(theta2) * cos(phi2));
        
        glTexCoord2f(fmod((i / 360.0f) + rotationAdjustment, 1.0f), (j + 100) / 180.0f);
        glVertex3f(cos(theta1) * cos(phi2), sin(phi2), sin(theta1) * cos(phi2));
    }
}
glEnd();


    glDisable(GL_TEXTURE_2D);
    glutSwapBuffers();
}

void idle() {
    angle += 0.02f;
    if (angle > 360) angle -= 360;
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
    switch(key) {
        case '1':
            loadTexture("earthtex.jpg");
            break;
        case '2':
            loadTexture("earthtex2.jpg");
            break;
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Textured Sphere");
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    gluPerspective(45.0f, 1.0f, 0.1f, 100.0f);
    loadTexture("earthtex.jpg");
    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}
