Problem: 
	The internet has fairly few open source examples of the marching cubes algorithm
	being used in real-time rendering of volumetric datasets and/or noise. Especially 
	there are no fully open source real-time implementations using DirectX instead of
	OpenGL
 
 
Goal: 
	To implement the marching cubes algorithm on the GPU along with it's surrounding
	C++/DirectX program.
 
 
Program inputs: 
	3D volumetric dataset of some sort, describing density in every arbitrary point within it.
	Or coherent 3D noise, f.ex simplex or perlin noise which can also be used to describe densities
	
Performance goal:
	Real-time rendered (>30fps) perlin noise. Actual time and working space requirements described in 
	big O notation for every algorithm and data structure used unknown for the time being.
	

The implementation will probably work using these six steps as a guideline:
	
		1. Define/create/load the volumetric data and define it's boundaries.
				What kind of data depends on how much time I have left after I've implemented the rendering algorithm.
				time and space requirements unknown until data type is decided on
			
		2. Divide the space into cells 
				Probably O(1) time and space as this is only a figurative thing, no actual dividing happens, this just determines the step sizes for loops used later
			
		3. Assign values to each of the 8 corners of a cell that determine whether a corner is inside or outside of the volume's surface at a specific point
				Probably O(n) time and space
		
		4. The values assigned in the previous step are used to create a single number per cell that is an index to a look-up table/array which 
		   contains all 256 different kinds of intersections of the volume's surface with the current cube's edges. 
				Probably O(n) time and space
				
		(If the cell is completely inside or outside of a surface, the cell is processed and completely invisible and we start processing the next cell)
				
		5. Assign points of intersection for the surface and the edges of the cell.
				Probably O(n) time and space
		
		6. Get vertices and the triangles they belong to from two new look-up tables using the values from previous steps combined. These can then be used to draw the
		   individual cells. Their combination is then the final visualization of the volumetric dataset.
				Probably O(n) time and space
	
Sources:
http://www.icare3d.org/codes-and-projects/codes/opengl_geometry_shader_marching_cubes.html
https://github.com/smistad/GPU-Marching-Cubes
https://github.com/chriskiefer/MarchingCubesGPU
http://http.developer.nvidia.com/GPUGems3/gpugems3_ch01.html
http://paulbourke.net/geometry/polygonise/