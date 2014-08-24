For now all testing is done by making sure DirectX HRESULTs that are returned from various subsystem initializers indicate successful initialization
	
The HRESULTs are a 32 bit unsigned long values that describe the error if any that was received from the function.

The functions already have built in checking for wrong kinds of parameters supplied to them.

Testing is for now done completely at runtime on every run, since they do not slow down the application start practically at all.


All initializing and thus testing is done in the DirectXApp::Init function which runs all initialization functions.
	* InitDX() creates the d3d device and it's swapchain, both of them require correct parameters and initialization variables to succeed.
	* setupVisualizationData() initializes two textures that work as 2d and 3d arrays that the GPU will sample and use in the marching cubes algorithm implementation
	* createDepthStencil() initializes a depth stencil and sets it as a render target, only thing tested here is if the targeted texture is succesfully initialized
	* setupVertexAndIndexAndSOBuffer() initializes vertex and stream output buffers, only thing to test here is the successfulness of the creation of the buffers.
	* compileAndEnableShaders() compiles vertex, geometry, and pixel shaders. The function compileShaderFromFile that is used from here does some basic checking that the shader compiles succesfully, and if not, outputs some debug data. Also attempts to create a samplerstate that will be used by the gpu to sample the textures created earlier
	* setupConstantBuffer creates a buffer like we've done above and if it doesn't succeed prevents the program initialization from continuing. 
	
	If all these stages are succesfull, the program continues to the main loop at which nothing is tested anymore since everything that happens is purely visual and thus the successfullness is always subjective.
	