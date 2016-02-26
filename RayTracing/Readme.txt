------------
Description:
------------
The given assignment on ray tracing explores the various aspects of it like reflection, refraction etc. 
The various objects in the output are:
1. Two light sources
2. Three walls(back wall, top wall, bottom wall)
3. Spherical figures
	(i) "IITK" written in the bottom left
	(ii) Combination of 3 spheres
	(iii) One individual sphere

The various properties of the objects such as light colour, position of various objects, material properties, number of samples per pixel etc can also be changed.

Some output images have been attached which shows some variations.
image 1: 1000X600, depth 5, samples 1000
image 2: 500X300, depth 5, samples 1000
image 3: 500X300, depth 3, samples 500, properties of the spheres have been reversed in this case-refractive to reflective and vice versa

----------------
To run the code:
----------------
run the bash script "compile.sh" on the terminal by the command "bash compile.sh". This compiles the c++ file, takes in the input and displays the final image.

The code takes a lot of time to run.
1. For 1000X600 with depth 5, samples 1000 = 30 minutes

---------------------
Description of input:
---------------------
The objects are given by a separate input file in which following things are given in the order
1st line: width of image	height of image
2nd line: Number of samples per pixel
from 3rd line onwards, for each object:
Diameter of sphere x_position y_position z_position r_colour_component g_colour_component b_colour_component r_light_component g_light_component b_light_component property(diffuse/reflective/refractive)
A sample input file has been given

----------------------
Description of Output:
----------------------
The output is a .ppm image file 