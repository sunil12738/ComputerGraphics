#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <bits/stdc++.h>
using namespace std;

int mousecoordinate_x[2000];
int mousecoordinate_y[2000];
int i=0;
int arr_size=1000000;
double store_x[1000000],store_y[1000000];
int store_color[1000000];
int total_counter=0;

//to store the points
int draw_x[2000],draw_y[2000];
int ttl_points=0;
//to store the group of points
int group[1000];
int temp_group=0;
int color_pen=0;//0=black, 1=red, 2=green, 3=blue
int n_curve=0;//for new curve 0=old 1=new
GLint x = 0;
GLint y = 0;
GLint Win_w = 800;
GLint Win_h = 600;
 
void init()
{
    glClearColor(1, 1, 1, 0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, Win_w, 0, Win_h);
}
void draw()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glPointSize(5);
    glFlush();
}
double N(double x, int n, int i, int l, double *u){
	double temp1,temp2;
	if((u[i+n]-u[i])==0) {
		temp1 = 1;
	}
	else temp1=(x-u[i])/(u[i+n]-u[i]);
	if((u[i+n+1]-u[i+1])==0){
		temp2=0;
	}
	else temp2=(u[i+n+1]-x)/(u[i+n+1]-u[i+1]);
	//printf("hi %d \t %d %lf %lf\n",i,n,temp1,temp2 );
	if(n==0){
		if(x<u[i+1] && x>=u[i]) return 1;
		else return 0;
	}
	else{
		double t1 = N(x,n-1,i,l,u);
		double t2 = N(x,n-1,i+1,l,u);
		double result = temp1*t1 + temp2*t2;
		//printf("j %lf %lf %lf x=%lf n=%d i=%d\n",result,t1,t2,x,n,i );
		return result;
	}
}
void mouse(int button, int state, int mouse_x, int mouse_y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        x = mouse_x;
        mousecoordinate_x[i]=x;
        draw_x[ttl_points]=x;

        y = Win_h - mouse_y;
        mousecoordinate_y[i]=y;
        draw_y[ttl_points]=y;
        ++ttl_points;
        
        for(int j = 0;j<2 && i>1;++j){
            glColor3f(0.0, 0.0, 0.0);
            glBegin(GL_LINES);
            glVertex2i(mousecoordinate_x[j], mousecoordinate_y[j]);
            glVertex2i(mousecoordinate_x[j+1], mousecoordinate_y[j+1]);
            glEnd();
        }
        
   if (i>=3){
     	glClearColor(1, 1, 1, 0);
     	glClear(GL_COLOR_BUFFER_BIT);

      //some initialization
	int n=3;//degree
	int l;//counter in which x belongs
	double x;
	//int i;//temp intermediate
	int control_p=i+1;// no of control points
	int no_knot_vector=control_p + 4;// no of knot vectors
	double u[no_knot_vector];//knot vector
	double p_x[control_p];//x values
	double p_y[control_p];//y values
	double s_x, s_y;
	for(int j = 0;j<control_p-1;++j){
   		glColor3f(0.0, 0.0, 0.0);
        glBegin(GL_LINES);
        glVertex2i(mousecoordinate_x[j], mousecoordinate_y[j]);
        glVertex2i(mousecoordinate_x[j+1], mousecoordinate_y[j+1]);
        glEnd();
    }
	//user input knot vector
	// for(int j=0;j<no_knot_vector;++j){
	// 	scanf("%lf",&u[j]);
	// }
	//program defined knot vector
	for(int j = 0;j<no_knot_vector;++j){
		if(j<4){
			u[j]=0;
		}
		else if(j<control_p && j>=4){
			u[j]=(j-4+1);
		}
		else if(j>=control_p){
			u[j]=(control_p-4+1);
		}
	}
	// for(int j=0;j<no_knot_vector;++j){
	// 	printf("knot %lf\n",u[j]);
	// }
	//u={0,0,0,0,0.25,0.50,0.75,1.0,1.0,1.0,1.0};


	for(int j=0;j<control_p;++j){
		p_x[j]=mousecoordinate_x[j];
		//scanf("%lf",&p_x[j]);
	}
	for(int j=0;j<control_p;++j){
		p_y[j]=mousecoordinate_y[j];
		//scanf("%lf",&p_y[j]);
	}
	
	for(x=0;x<control_p-4+1;x+=0.001,++total_counter){
		//find the interval in which x belongs
		for(int j=0;j<no_knot_vector;++j){
			if(x<u[j+1] && x>=u[j]){
				l=j;
				//printf("%d %lf\n",l,x );
			}
			//printf("j=%d\n",j );
		}
		//x = (l-3) + (l-2) + (l-1) + (l)
		double t1, t2, t3, t4;
		t1=N(x,n,l-3,l,u);
		t2=N(x,n,l-2,l,u);
		t3=N(x,n,l-1,l,u);
		t4=N(x,n,l,l,u);
		//printf("%lf %lf %lf %lf\n",t1,t2,t3,t4 );
		s_x = p_x[l-3] * t1 + p_x[l-2] * t2 + p_x[l-1] * t3 + p_x[l] * t4;
		s_y = p_y[l-3] * t1 + p_y[l-2] * t2 + p_y[l-1] * t3 + p_y[l] * t4;
		//printf("%lf,%lf \n",s_x,s_y);
    	// else glColor3f(0.0f, 0.0f, 0.0f);  
        if(color_pen==1){store_color[total_counter]=1;}
        else if(color_pen==2){store_color[total_counter]=2;}
        else if(color_pen==3){store_color[total_counter]=3;}
        else {store_color[total_counter]=1;}                  
        store_x[total_counter]=s_x;
        store_y[total_counter]=s_y;
        }
    }
    	if(n_curve==1)
        store_x[total_counter]=99999;
    	else
        store_x[total_counter]=-99999;
    	++total_counter;
    	
        for(int j=total_counter;j>=0;--j){
        	if(store_x[j]==99999.00000 && store_x[j-1]==-99999){
        		for(j=j-2;j>=0;--j){
        			if(store_x[j]==-99999.000000){break;}
        			else{
                        if(store_color[j]==1) {glColor3f(1.0f, 0.0f, 0.0f);}
                        else if(store_color[j]==2) {glColor3f(0.0f, 1.0f, 0.0f);}
                        else if(store_color[j]==3) {glColor3f(0.0f, 0.0f, 1.0f);}
                        else glColor3f(0.0f, 0.0f, 0.0f);
                        glPointSize(2);    
                        glBegin(GL_POINTS);  
            			glVertex2i(store_x[j], store_y[j]);
        				glEnd();
        			}
        		}
        	}
        }
        for(int j=total_counter-3;j>=0;j--){
        	if(store_x[j]==-99999.000000){break;}
        	else{
                if(store_color[j]==1) {glColor3f(1.0f, 0.0f, 0.0f);}
                else if(store_color[j]==2) {glColor3f(0.0f, 1.0f, 0.0f);}
                else if(store_color[j]==3) {glColor3f(0.0f, 0.0f, 1.0f);}
                else glColor3f(0.0f, 0.0f, 0.0f);        
                glPointSize(2);
                glBegin(GL_POINTS);  
          		glVertex2i(store_x[j], store_y[j]);
        		glEnd();
        	}
        }
        for(int j=0;j<ttl_points-1;++j){
            glPointSize(5);
            glColor3f(0.0, 0.0, 0.0);
            glBegin(GL_POINTS);
            glVertex2i(draw_x[j], draw_y[j]);
            glEnd();

            
        }
        
        i++;
        n_curve=0;
        glColor3f(0.5, 0.5, 0.9);
        glBegin(GL_POINTS);
        glVertex2i(x, y);
        glEnd();
        glFlush();
    }
}
 
void keyboard (unsigned char key, int x, int y)
{
    switch (key) {
        case 'd': color_pen=0; break;
        case 'r': color_pen=1; break;
        case 'g': color_pen=2; break;
        case 'b': color_pen=3; break;
        case 'n': n_curve=1; i=0; break;
        //case 's':for(int j=0;j<10;++j) printf("%d\n",group[j] ); 
        //break;
        default: 
        	break;
    }
}
 
int main(int agrc, char ** argv)
{
    for(int j=0;j<arr_size;++j){
    	store_x[j]=0.0;
    	store_y[j]=0.0;
    }
    for(int j=0;j<1000;++j){
        group[j]=-1;
    }
    glutInit(&agrc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(Win_w, Win_h);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Assignment 4: B-Spline Curve");
    init();
    glutDisplayFunc(draw);
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);

    glutMainLoop();
}
