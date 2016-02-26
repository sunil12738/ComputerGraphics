#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <stdio.h>

GLfloat Tx,Ty,Tz;
GLfloat Ra,Rb,Rc;
GLfloat Sx,Sy,Sz;
GLfloat x,y,z,angle;
#define PI 3.14159265f

struct Image {
    unsigned long sizeX;
    unsigned long sizeY;
    char *data;
};
typedef struct Image Image;

int light;;

/*  Create checkerboard texture  */
#define checkImageWidth 64
#define checkImageHeight 64
static GLubyte checkImage[checkImageHeight][checkImageWidth][4];

static GLuint texName1, texName2;

void makeCheckImage(void)
{
    int i, j, c;

    for (i = 0; i < checkImageWidth; i++) {
        for (j = 0; j < checkImageHeight; j++) {
            c = ((((i&0x8)==0)^((j&0x8)==0)))*255;
            checkImage[i][j][0] = (GLubyte) c;
            checkImage[i][j][1] = (GLubyte) c;
            checkImage[i][j][2] = (GLubyte) c;
            checkImage[i][j][3] = (GLubyte) 255;
        }
    }
}
int ImageLoad(char *filename, Image *image) {
    FILE *file;
    unsigned long size;
    unsigned long i;
    unsigned short int plane;
    unsigned short int bpp;
    char temp;
    if ((file = fopen(filename, "rb"))==NULL)
    {
        printf("File Not Found : %s\n",filename);
        return 0;
    }
    fseek(file, 18, SEEK_CUR);
    fread(&image->sizeX, 4, 1, file);
    fread(&image->sizeY, 4, 1, file);
    size = image->sizeX * image->sizeY * 3;
    fread(&plane, 2, 1, file);
    fread(&bpp, 2, 1, file);
    fseek(file, 24, SEEK_CUR);
    image->data = (char *) malloc(size);
    fread(image->data, size, 1, file);
    for (i=0;i<size;i+=3) { 
        temp = image->data[i];
        image->data[i] = image->data[i+2];
        image->data[i+2] = temp;
    }
    return 1;
}
Image * loadTexture(){
    Image *image1;
    image1 = (Image *) malloc(sizeof(Image));
    if (!ImageLoad("tiles3.bmp", image1)) {
        exit(1);
    }     
    return image1;
}


void init(void)
{
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_FLAT);

    glEnable(GL_DEPTH_TEST);

    glDepthFunc(GL_LESS);


//glGenTextures(2, texture);
    Image *image1 = loadTexture();
    if(image1 == NULL){
        printf("Image was not returned from loadTexture 1\n");
        exit(0);
    }
    makeCheckImage();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glBindTexture(GL_TEXTURE_2D, texName1);    
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); //
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); //
    glTexImage2D(GL_TEXTURE_2D, 0, 3, image1->sizeX, image1->sizeY, 0,GL_RGB, GL_UNSIGNED_BYTE, image1->data);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

    glShadeModel(GL_FLAT);
}
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
/* ---------------------------------------------------------------------- */ 

/* ---------------------------------------------------------------------- */ 

    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    
    glBindTexture(GL_TEXTURE_2D, texName1);
    glPushMatrix();
    glTranslatef(Tx, Ty, -8.0f);  
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0); glVertex3f( 4.15f, -2.55f,  8.0f);
    glTexCoord2f(1.0, 0.0); glVertex3f(-4.15f, -2.55f,  8.0f);
    glTexCoord2f(1.0, 1.0); glVertex3f(-4.15f, -2.55f, -7.0f);
    glTexCoord2f(0.0, 1.0); glVertex3f( 4.15f, -2.55f, -7.0f);
    glEnd();
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);

/* ---------------------------------------------------------------------- */ 

/* ---------------------------------------------------------------------- */ 

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glShadeModel(GL_SMOOTH);
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);//to enable lightening
    GLfloat ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);//ambient light model
    glShadeModel(GL_SMOOTH);// to select the shading model GL_FLAT or GL_SMOOTHs
    GLfloat specular[] = {1.0f, 1.0f, 1.0f , 1.0f};
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
    GLfloat diffuse[]={1.0,0.8,0.8,1.0};
    glLightfv(GL_LIGHT0,GL_DIFFUSE,diffuse);
    GLfloat position0[] = { 3.8f, 2.0f, -7.5f, 1.0f }; //position of light source 1
    glLightfv(GL_LIGHT0, GL_POSITION, position0);
    glEnable(GL_LIGHT0);//enable light source 1


    glEnable(GL_COLOR_MATERIAL); //to enable the color tracking. enalbe these 2 lines and then use glcolor
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    float mat_specular[4]={0.0,0.3f,0.8f,1.0f};
    float mat_diffuse[4]={0.0,1.0f,0.0f,1.0f};
    float mat_ambient[4]={0.1,0.1f,0.1f,1.0f};
    float mat_shininess=50.0;

    glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,mat_shininess);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,mat_specular);
    glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,mat_diffuse);
    glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,mat_ambient);

    GLfloat emission[] = { .5,0.0,0.0,1.0};
    glMaterialfv(GL_BACK,GL_EMISSION,emission);


    //glColor3f(1.0, 0.60, 0.0);

    GLfloat position1[] = { 1.7f,-0.1f,-8.0f, 1.0f }; //position of light source 2
    GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 }; //other properties of light source 2
    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    glLightfv(GL_LIGHT1, GL_POSITION, position1);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
    glEnable(GL_LIGHT1);//enable light source 1

    //glDisable(GL_LIGHT0);//disable light source 1
    //glDisable(GL_LIGHT1);//disable light source 1

    //light source 1 i.e. bulb
    glPushMatrix();
    glColor3f(1.0f, 1.0f, 1.0f); 
    glTranslatef(4.1f, 2.0f, -8.0f);
    glutSolidSphere(0.15f,30, 30);
    glPopMatrix();
    

    // //test
    // glPushMatrix();
    // glColor3f(1.0f, 1.0f, 1.0f); 
    // glTranslatef(1.7f,-0.2f,-8.0f);
    // glutSolidSphere(0.15f,30, 30);
    // glPopMatrix();

    //sphere
    glTranslatef(Tx, Ty, Tz);
    glPushMatrix();
    glColor3f(1.0, 0.72, 0.55); 
    glTranslatef(0.0f, -0.1f, 2.0f);
    glutSolidSphere(0.5f,30, 30);
    glPopMatrix();



    //Walls
    glPushMatrix();
    glBegin(GL_QUADS);
        //right
    glColor3f(0.67, 1.0, 0.18); 
    glVertex3f(4.15f,  2.55f, -8.0f);
    glVertex3f(4.15f,  2.55f,  8.0f);
    glVertex3f(4.15f, -2.55f,  8.0f);
    glVertex3f(4.15f, -2.55f, -8.0f);
        //left
    glColor3f(0.67, 1.0, 0.18); 
    glVertex3f(-4.15f,  2.55f,  8.0f);
    glVertex3f(-4.15f,  2.55f, -8.0f);
    glVertex3f(-4.15f, -2.55f, -8.0f);
    glVertex3f(-4.15f, -2.55f,  8.0f);
        //back
    glColor3f(0.67, 1.0, 0.18); 
    glVertex3f(4.15f, -2.55f, -8.0f);
    glVertex3f(4.15f,  2.55f, -8.0f);
    glVertex3f(-4.15f,  2.55f, -8.0f);
    glVertex3f(-4.15f, -2.5f,  -8.0f);
        //top
    glColor3f(1.0, 0.85, 0.7); 
    glVertex3f( 4.15f, 2.55f, -8.0f);
    glVertex3f(-4.15f, 2.55f, -8.0f);
    glVertex3f(-4.15f, 2.55f,  8.0f);
    glVertex3f( 4.15f, 2.55f,  8.0f);
    glEnd();
    glPopMatrix();


    //table
    glBegin(GL_QUADS);
        //top
    glColor3f(0.0, 0.5, 0.14); 
    glVertex3f(1.0f,  -0.6f, 4.0f);
    glVertex3f(-2.5f, -0.6f, 4.0f);
    glVertex3f(-2.5f,  -0.6f,  -0.0f);
    glVertex3f(1.0f, -0.6f, -0.0f);
        //bottom
    glVertex3f( 1.0f, -0.8f,  4.0f);
    glVertex3f(-2.5f, -0.8f, 4.0f);
    glVertex3f(-2.5f, -0.8f,  -0.0f);
    glVertex3f( 1.0f, -0.8f, -0.0f);
        //right
    glVertex3f(1.0f,  -0.6f,  4.0f);
    glVertex3f(1.0f,  -0.8f, 4.0f);
    glVertex3f(1.0f, -0.8f, -0.0f);
    glVertex3f(1.0f, -0.6f,  -0.0f);
        //front
    glColor3f(0.0, 0.0, 0.0); 
    glVertex3f( 1.0f, -0.6f, -0.0f);
    glVertex3f(-2.5f, -0.6f, -0.0f);
    glVertex3f(-2.5f, -0.8f,  -0.0f);
    glVertex3f( 1.0f, -0.8f,  -0.0f);
        //left
    glColor3f(1.0, 0.5, 0.14); 
    glVertex3f(-2.5f,  -0.6f,  -0.0f);
    glVertex3f(-2.5f,  -0.8f, -0.0f);
    glVertex3f(-2.5f, -0.8f, 4.0f);
    glVertex3f(-2.5f, -0.6f,  4.0f);
        //back
        //left
    glVertex3f(-2.5f,  -0.8f,  4.0f);
    glVertex3f(1.0f,  -0.8f, 4.0f);
    glVertex3f(1.0f, -0.6f, 4.0f);
    glVertex3f(-2.5f, -0.6f,  4.0f);
        //leg
    glEnd();

    //teapot
    glPushMatrix();
    glColor3f(1.0,0.99553,0.977678); 
    glTranslatef(-1.5f,-0.1f,3.5f);
    glutSolidTeapot(0.5);
    glPopMatrix();


    //torus
    glPushMatrix();
    glColor3f(0.0, 1.0, 1.0); 
    glTranslatef(2.5f,-1.5f,5.5f);
    glRotatef(90, 0.5f, 0.5f, 0.0f);
    glutSolidTorus(0.2,0.4,50,80);
    glPopMatrix();

    //cone chair 1
    glPushMatrix();
    glColor3f(0.62,0.47,0.93);
    glTranslatef(-0.5f,-1.5f,5.0f);
    glRotatef(90, 1.0f, 0.0f, 0.0f);
    glutSolidCone(0.8,2.0,50,80);
    glPopMatrix();

    //cone chair 2
    glPushMatrix();
    glTranslatef(1.5f,-1.6f,3.5f);
    glRotatef(90, 1.0f, 0.0f, 0.0f);
    glutSolidCone(0.8,2.0,50,80);
    glPopMatrix();

    //cylinder below table
    glPushMatrix();
    glColor3f(0.0, 0.5, 0.14); 
    glTranslatef(-0.75f,-0.8f,2.0f);
    glRotatef(90, 1.0f, 0.0f, 0.0f);
    GLUquadric *quad = gluNewQuadric();           
    gluCylinder(quad,0.15,0.15,2.0,10,10);
    glPopMatrix();

    //cylinder lamp
    glPushMatrix();
    glColor3f(1.0, 1.0, 0.87); 
    glTranslatef(3.2f,-0.3f,-5.0f);
    glRotatef(90, 1.0f, 0.0f, 0.0f);
    GLUquadric *quad1 = gluNewQuadric();           
    gluCylinder(quad1,0.250,0.50,0.6,50,50);
    glPopMatrix();

    //cylinder below lamp
    glPushMatrix();
    glColor3f(0.4, 0.74, 0.4); 
    glTranslatef(3.2f,-0.3f,-5.0f);
    glRotatef(90, 1.0f, 0.0f, 0.0f);
    GLUquadric *quad2 = gluNewQuadric();           
    gluCylinder(quad2,0.1,0.1,2.0,10,10);
    glPopMatrix();

    glFlush();

    glutSwapBuffers();
}
void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (GLfloat)w/(GLfloat)h, 1.0, 30.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -3.6);
}
void keyboard (unsigned char key, int x, int y)
{
    switch (key) {
        case 27:
        exit(0);
        break;
        default: 
        break;
    }
}
int main(int argc, char** argv)
{
    /* initialize GLUT, using any commandline parameters passed to the program */
    glutInit(&argc, argv);

    /* setup the size, position, and display mode for new windows */
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1200, 800);
    glutInitWindowPosition(100, 100);
    
    /* create and set up a window */
    glutCreateWindow("Assignment 2 : Lightening, Shading, Illumination and Shading");
    Tx = 0.0; Ty = 0.0; Tz = -10.0;

    init();
    glutReshapeFunc (reshape);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}