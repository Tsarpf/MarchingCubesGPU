Problem: 
	The internet has fairly few open source examples of the marching cubes algorithm
 being used in real-time rendering of volumetric datasets and/or noise. Especially on newer
 versions of the DirectX API.
 
 
Goal: 
	To implement the marching cubes algorithm on the GPU along with it's surrounding
 C++/DirectX program.
 
 
Program inputs: 
	3D volumetric dataset of some sort, describing density in every arbitrary point within it.
	Or coherent 3D noise, f.ex simplex or perlin noise which can also be used to describe densities
	
Performance goal:
	Real-time rendered (>30fps) perlin noise. Actual time and working space requirements described in 
	big O notation for every algorithm and data structure used unknown for the time being.

	
Sources:
http://www.icare3d.org/codes-and-projects/codes/opengl_geometry_shader_marching_cubes.html
https://github.com/smistad/GPU-Marching-Cubes
https://github.com/chriskiefer/MarchingCubesGPU
http://http.developer.nvidia.com/GPUGems3/gpugems3_ch01.html
http://paulbourke.net/geometry/polygonise/