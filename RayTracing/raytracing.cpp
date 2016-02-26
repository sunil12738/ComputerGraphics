#include <math.h>   
#include <stdlib.h> 
#include <stdio.h>  
#include <iostream>
#include <string.h>
#include <time.h>
int objects = 32; // no of objects
// type_of_reflection;
// 1=diffuse
// 2=reflection
// 3=refractive

struct Vec {        
  double x, y, z;               
};

struct Ray { 
	Vec origin, direction; 
};

struct Sphere {
  double radius;
  Vec position;
  Vec light;
  Vec object_color;   
  int type_of_reflection;
};

Sphere * spheres = new Sphere[32];
	
Vec calculate_light(const Ray &r, int depth);

Vec operator+(const Vec &a, const Vec &b) { 
	Vec vector; 
	vector.x = a.x + b.x;
	vector.y = a.y + b.y;
	vector.z = a.z + b.z;
	return vector;
}
Vec operator-(const Vec &a, const Vec &b) {
 	Vec vector; 
	vector.x = a.x - b.x;
	vector.y = a.y - b.y;
	vector.z = a.z - b.z;
	return vector;
}
Vec operator*(const Vec &a, double b) {
	Vec vector; 
	vector.x = a.x * b;
	vector.y = a.y * b;
	vector.z = a.z * b;
	return vector;
}

Vec mult(const Vec &a, const Vec &b) { 
	Vec vector; 
	vector.x = a.x * b.x;
	vector.y = a.y * b.y;
	vector.z = a.z * b.z;
	return vector;
}
Vec cross(const Vec &a, const Vec &b){
	Vec vector; 
	vector.x = a.y*b.z-a.z*b.y;
	vector.y = b.z*b.x-a.x*b.z;
	vector.z = a.x*b.y-a.y*b.x;
	return vector;
}
Vec norm(const Vec &a){ 
	Vec vector; 
	vector.x = a.x/sqrt(a.x*a.x+a.y*a.y+a.z*a.z);
	vector.y = a.y/sqrt(a.x*a.x+a.y*a.y+a.z*a.z);
	vector.z = a.z/sqrt(a.x*a.x+a.y*a.y+a.z*a.z);
	return vector;
}

double dot(const Vec &a, const Vec &b) { 
	return a.x*b.x+a.y*b.y+a.z*b.z; 
} 
double intersection(const Sphere &s, const Ray &r) {
    Vec distance = s.position-r.origin; 
    double root0, root1;
    double B=dot(distance,r.direction), D=B*B-dot(distance,distance)+s.radius*s.radius;
    if (D<0) return 0; 
    else {
      root0=B-sqrt(D);
      root1=B+sqrt(D);
    }
    if(root0>0.01) return root0;
    else if(root1>0.01) return root1;
    else return 0;
}

bool intersect(const Ray &r, double &t, int &id){
	double d, inf=t=1e20;
  	for(int i=objects;i--;) {
		 if((d=intersection(spheres[i],r))&&d<t)
		 	{t=d;id=i;}
    }
		return true;
}
//to get random number between 0 and 1
double random_number(){
	return ((double) rand() / (RAND_MAX));
}
double normalize_0_1(double x){ 
	double temp;
	if(x<0){ temp = 0;}
	else if(x>1){
		temp = 1;
	}
	else temp = x;
	return temp*0.25; 
}

Vec diffuse_reflection(const Vec &x, int depth,const Sphere &sph,const Vec &nl,const Vec &f){
  double r1=2*M_PI*random_number(), r2=random_number(), r2s=sqrt(r2);
    Vec v11,v22;
    v11.x=0;
    v11.y=1;
    v11.z=0;
    v22.x=1;
    v22.y=0;
    v22.z=0;
    Vec w=nl, u=norm((cross((fabs(w.x)>.1?v11:v22),w)));
    Vec v=cross(w,u);
    Vec d = norm((u*cos(r1)*r2s + v*sin(r1)*r2s + w*sqrt(1-r2)));
    Ray rr1; 
    rr1.origin = x; 
    rr1.direction = d;
    return sph.light + mult(f,calculate_light(rr1,depth));  
}

Vec reflection_reflection(const Vec &x , int depth, const Sphere &sph, const Ray &r, const Vec &n, const Vec &f){
  Ray rr2; 
    rr2.origin = x; 
    rr2.direction = r.direction-n*2*dot(n,r.direction);
    return sph.light + mult(f,calculate_light(rr2,depth));
}

Vec refraction_reflection(const Vec &x , int depth, const Sphere &sph, const Ray &r, const Vec &n, const Vec &f, const Vec &nl){
	Ray ray_refraction; 
  ray_refraction.origin= x; 
  ray_refraction.direction= r.direction-n*2*dot(n,r.direction);    


  double r_index_air=1, r_index_glass=1.5;
  double r_index, ddn=dot(r.direction,nl), cos2t=0;
  double temp111, temp112;
  if(dot(n,nl)>0){
  	r_index = r_index_air/r_index_glass;
  	temp111 = 1;
  }
  else {
  	r_index = r_index_glass/r_index_air;
  	temp111 = -1;
  }

  Vec tdir = norm((r.direction*r_index - n*((temp111)*(ddn*r_index+sqrt(cos2t)))));

  if(dot(n,nl)>0){
  	temp112 = -ddn;
  }
  else {
  	temp112 = dot(tdir,n);
  }
  
  double a=r_index_glass-r_index_air;
  double b=r_index_glass+r_index_air;
  double c = 1-(temp112);
  double R0=a*a/(b*b), Re=R0+(1-R0)*c*c*c*c*c,Tr=1-Re,P=.25+.5*Re,RP=Re/P,TP=Tr/(1-P);
  Ray rr3; 
  rr3.origin = x; 
  rr3.direction =tdir;

  Vec temp_vector;
  if(depth>2){
    if(random_number()<P){
      temp_vector = calculate_light(ray_refraction,depth)*RP;
    }
    else{
      temp_vector = calculate_light(rr3,depth)*TP;
    }
  }
  else{
    temp_vector = calculate_light(ray_refraction,depth)*Re+calculate_light(rr3,depth)*Tr;
  }
  return sph.light + mult(f,temp_vector);

}


// void print_image(int w, int h, const Vec &c[w*h]){
// 	printf( "P3\n%d %d\n%d\n", w, h, 255);
//   for (int i=0; i<w*h; i++)
//     	printf("%0.0f %0.0f %0.0f \n", (c[i].x)*1000, (c[i].y)*1000, (c[i].z)*1000);
// }

Vec calculate_light(const Ray &r, int depth){
  double t;                               
  int id=0;                               
  //srand(time(NULL));

  //printf("h%d\n",intersect(r, t, id));
  if (!intersect(r, t, id)) {
    Vec dark; 
    dark.x=1;
    dark.y=1;
    dark.z=1; 
    return dark;
  }

  const Sphere &sph = spheres[id];       
  
  Vec x=r.origin+r.direction*t, n=norm((x-sph.position)), f=sph.object_color;
  
  Vec nl;
  if(dot(n,r.direction)<0){nl = n;}
  else{nl = n*-1;}
  
  double p;                                                  
  if( f.x>f.z && f.x>f.y) p = f.x;
  else if(f.y>f.z) p = f.y;
  else p = f.z;
  

  if (++depth>5) return sph.light;
//diffuse
  if (sph.type_of_reflection == 1){                  
    Vec temporary_diff = diffuse_reflection(x, depth, sph, nl, f);
    return temporary_diff;
  } 
//reflection
  else if (sph.type_of_reflection == 2){
  	Vec temporary_reflec = reflection_reflection(x, depth, sph, r, n, f);
    return temporary_reflec;
  	}
//refraction
  else if(sph.type_of_reflection == 3){
  	Vec temporary_refrac = refraction_reflection(x, depth, sph, r, n, f, nl);
    return temporary_refrac;
  }
}

int main(int argc, char *argv[]){

	int w,h,samples;
	double radius;
	double xposition, yposition, zposition;
	double xcolor, ycolor, zcolor;
	double xemission,yemission,zemission;
	int material;
	//height and width of frame
	scanf("%d %d",&w,&h);
	//number of samples
	scanf("%d",&samples);
	for(int i=0;i<objects;++i){
		scanf("%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %d",&radius,&xposition,&yposition,&zposition,&xcolor,&ycolor,&zcolor,&xemission,&yemission,&zemission,&material);
  		spheres[i].radius=radius;
  		spheres[i].position.x=xposition;
		  spheres[i].position.y=yposition;
		  spheres[i].position.z=zposition;

  		spheres[i].light.x=xemission;
  		spheres[i].light.y=yemission;
  		spheres[i].light.z=zemission;

  		spheres[i].object_color.x=xcolor;
  		spheres[i].object_color.y=ycolor;
  		spheres[i].object_color.z=zcolor;

  		spheres[i].type_of_reflection=material;
		//printf("hi%lf\t %lf\t %lf\t %lf\t %lf\t %lf\t %lf\t %lf\t %lf\t %lf\t %d\n",radius,xposition,yposition,zposition,xcolor,ycolor,zcolor,xemission,yemission,zemission,material);
  	}


  	Vec camera_origin;
  	camera_origin.x=50;
  	camera_origin.y=52;
  	camera_origin.z=295.6;
  	Vec camera_direction;
  	camera_direction.x=0;
  	camera_direction.y=-0.042612;
  	camera_direction.z=-1;
  	Ray cam; 
  	cam.origin =camera_origin;
  	cam.direction =norm(camera_direction);

  	Vec cx;
  	cx.x = w*0.5/h;
  	cx.y = 0;
  	cx.z = 0;
  	Vec cy=norm(cross(cx,cam.direction))*0.5;
    Vec r;
    Vec *image=new Vec[w*h];
  
	for (int y=0; y<h; y++){                      
	    for (int x=0; x<w; x++)  { 
	    	
        //if(x%10==0)
	    	//fprintf(stderr,".");
    		int sy=0, i=(h-y-1)*w+x;
    		int sx=1;r=Vec();
    		for (int s=0; s<samples; s++){
    			Vec d = cx*( (0.75 + x)/w - .5) + cy*( ( 0.25 + y)/h - .5) + cam.direction;
    			Ray rr0; 
    			rr0.origin = cam.origin+d*140; 
    			rr0.direction = norm(d);
	    		r = r + calculate_light(rr0,0)*(1./samples);
	        } 
          	image[i].x = image[i].x + (normalize_0_1(r.x));
          	image[i].y = image[i].y + (normalize_0_1(r.y));
          	image[i].z = image[i].z + (normalize_0_1(r.z));
            //not working properly so made the function normalize
            double temp1;
            if(r.x<0) temp1=0;
            else if(r.x>1) temp1=1;
            else temp1=r.x;
           //  image[i].x = image[i].x + temp1;

            double temp2;
            if(r.y<0) temp2=0;
            else if(r.y>1) temp2=1;
            else temp2=r.y;
            // image[i].y = image[i].y + temp2;

            double temp3;
            if(r.z<0) temp3=0;
            else if(r.z>1) temp3=1;
            else temp3=r.z;
            // image[i].z = image[i].z + temp3;
      	}
        if(y%10==0)fprintf(stderr,".");
  	}
  
//print_image(c,w,h);
 //estimate of maximum value of colour for normalization
  double max = -999999999;
  for(int i=0;i<w*h;++i){
   if(image[i].x>max) max = image[i].x;
   if(image[i].y>max) max = image[i].y;
   if(image[i].z>max) max = image[i].z;
  }
//find the normalization constant
  double n_c = 255/max;
//  printf("%lf\n",n_c );

 printf( "P3\n%d %d\n%d\n", w, h, 255);
   for (int i=0; i<w*h; i++){
     	printf("%0.0f %0.0f %0.0f \n", (image[i].x)*n_c, (image[i].y)*n_c, (image[i].z)*n_c);
   }

}