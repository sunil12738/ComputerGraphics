// There are some points to note
// 	There is one centre to the solar system.
// 	There are two planets revolving around it and rotating on their axis.
// 	One planet has ring around it just like saturn has.
// 	One planet has artificial satellite rotating around it.
# include <GL/gl.h>
# include <GL/glu.h>
# include <GL/glut.h>
#include <math.h>     // Needed for sin, cos

GLfloat Tx,Ty,Tz;
GLfloat Ra,Rb,Rc;
GLfloat Sx,Sy,Sz;
GLfloat na=0.0f;
GLfloat nb=0.0f;
GLfloat nc=0.0f;
#define PI 3.14159265f


void renderScene(void){
	glClear(GL_COLOR_BUFFER_BIT);  
	glMatrixMode(GL_MODELVIEW);   
	glLoadIdentity(); 

//Sun
	//glutWireSphere(1.0f,50, 50);
	glColor3f(1.0, 0.60, 0.0); 
	glTranslatef(Tx, Ty, Tz);
	glPushMatrix();                     
		glRotatef(Rc, 0.0f, 1.0f, 0.0f);
			glutWireSphere(0.5f,30, 30);
	glPopMatrix();  



//Planet 1
	glColor3f(1.0, 1.0, 0.0); 
	//glTranslatef(Tx, Ty, Tz);
	glPushMatrix();                     
		glTranslatef(3.3f*cos(na*PI/360), 3.3f*sin(na*PI/360), 0.0f);   
		glRotatef(Ra, 1.0f, 0.0f, 0.0f);
			glutWireSphere(0.3f,30, 30);
			//glutWireTorus(0.2, 0.4, 5, 6);
	glPopMatrix();  


//Ring around planet 1
	glColor3f(1.0, 0.7, 0.1); 
	glPushMatrix();                     
		glTranslatef(3.3f*cos(na*PI/360), 3.3f*sin(na*PI/360), 0.0f);   
		glRotatef(Ra, 0.0f, 0.0f, 1.0f);
			glutWireTorus(0.005, 0.4, 5, 15);
	glPopMatrix();

//Planet 2
	glColor3f(0.2, 0.8, 0.5); 
	glTranslatef(Tx, Ty, Tz); 
	glPushMatrix();                     
		glTranslatef(4.0f*cos(nb*PI/360), 4.0f*sin(nb*PI/360), 0.0f);   
		glRotatef(Rb, 0.0f, 0.0f, 1.0f);
			//glutWireSphere(1.0f,50, 50);
			glutWireIcosahedron();
			//glutWireTorus(0.3, 0.6, 3, 4);
	glPopMatrix(); 

//Natural satellite of planet 2
	glColor3f(0.5, 0.0, 1.0); 
	glPushMatrix();                     
		glTranslatef(4.0f*cos(nb*PI/360), 4.0f*sin(nb*PI/360), 0.0f);   
		glTranslatef(2.0f*cos(nb*PI/360), -2.0f*sin(nb*PI/360), 0.0f);   
		glRotatef(Rb, 0.0f, 0.0f, 1.0f);
			glutWireTorus(0.01, 0.2, 5, 10);
	glPopMatrix();


	// glColor3f(0.0, 0.0, 1.0); 
	// glutWireTorus(0.02, 0.02, 6, 4);//For the centre of the circular path in which object is moving


	glFlush();
	glutSwapBuffers();
		na+=0.05f;
		nb+=0.08f;
}

void changeSize(int x, int y){
    if (y == 0 || x == 0) return;

    GLfloat aspect = (GLfloat)(x/y);

    glMatrixMode(GL_PROJECTION);  
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 0.0, 0.0, 0.0, -1.0, 0, 1, 0);
    gluPerspective(45.0f,aspect,0.1f, 10000.0f);
    glViewport(0,0,x,y);
    glMatrixMode(GL_MODELVIEW);
}

void animate(void){
	Ra += 0.15;
	Rb += 0.1;
	Rc += 0.05;
	renderScene();
}

int main (int argc, char **argv){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(800,800);
	glutInitWindowPosition(100,100);
	glutCreateWindow("Assignment1");
	
	glClearColor(0.0,0.0,0.0,0.0);

	Tx = 0.0; Ty = 0.0; Tz = -10.0;
	Ra = 40.0; Rb = 33.0; Rc = 23.0;

	//Assign  the function used in events
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);

	glutIdleFunc(animate);					

	//Let start glut loop
	glutMainLoop();

	return 0;
}