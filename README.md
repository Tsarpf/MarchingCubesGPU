DirectX Marching Cubes
=========

Marching Cubes algorithm implementation with DirectX / C++. Heavy lifting done on the geometry shader

##How to use
For now the only way to use the program is to download the project, fix DirectX SDK paths if needed, run the project and observe the results.
- To change the precision of the resulting geometry, change the data size (height, depth, width) variables in the beginning of DirectXApp::setupVisualizationData().
- To change how the geometry is coloured, play with the variables that are named something related to colors in the geometry shader.
- To visualize different kinds of density data, select (comment/uncomment) one of the functions in the beginning of VolumetricData::CreateTestData(). The descriptions of the images below have the name of the function that was used to create the densities for that image in the parentheses.

##Example outputs

A sphere which we've distorted with coherent 3d noise (used VolumetricData::createBumpySphere())
![A sphere which we've distorted with coherent 3d noise](http://i.imgur.com/dJnqqzc.jpg)

Only coherent 3d noise (used VolumetricData::createRandomNoise())
![Only coherent 3d noise](http://i.imgur.com/CkYlzzK.jpg)

No noise, just a sphere (used VolumetricData::createSphere())
![Sphere](http://i.imgur.com/zw58gVh.jpg)
