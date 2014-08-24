The program first initializes all devices and interfaces on the CPU side for use in the rendering phase later on. All buffers etc that are needed for transferring data from the CPU to the GPU are initialized and data to show will be initialized or created or loaded or whatever
After all initialization is complete the program begins the main rendering loop in which we simply send a 3d grid of points to the gpu on every frame. What we visualize within this grid of points is entirely up to the geometry shader.
The following happens completely on the GPU:
			
		2. Divide the space into cells 
				 O(1) time and space as this is only a figurative thing, no actual dividing happens, this just determines the step sizes for loops used later
			
		3. Assign values to each of the 8 corners of a cell that determine whether a corner is inside or outside of the volume's surface at a specific point
				 O(n) time and space
		
		4. The values assigned in the previous step are used to create a single number per cell that is an index to a look-up table/array which 
		   contains all 256 different kinds of intersections of the volume's surface with the current cube's edges. 
				 O(n) time and space
				
		(If the cell is completely inside or outside of a surface, the cell is processed and completely invisible and we start processing the next cell)
				
		5. Assign points of intersection for the surface and the edges of the cell.
				 O(n) time and space
		
		6. Get vertices and the triangles they belong to from two new look-up tables using the values from previous steps combined. These can then be used to draw the
		   individual cells. Their combination is then the final visualization of the volumetric dataset.
				 O(n) time and space
				
				
				