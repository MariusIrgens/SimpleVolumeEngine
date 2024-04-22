## SIMPLE VOLUME ENGINE <br />
I made this Direct Volume Rendering template for anyone interested in learning direct volume rendering, 
or anyone who wants a quick setup for trying out new volume rendering ideas.

I will probably make a video walkthrough of the code at some point. Stay tuned at my Youtube channel:
https://youtube.com/@mariusirgens5555?si=ViyWO1278_H8aFdL

If you use the code for something awesome, I would love to see. Let me know!

## Step 1 - Clone this repo <br />
I recommend using Visual Studio Community, since that is what I have used.

## Step 2 - VCPKG <br />
The project uses vcpkg (https://vcpkg.io) for dependency management, so this should take care of everything. 
Please follow the instructions on the vcpkg website (https://vcpkg.io/en/getting-started.html) to set it up for your environment. 
When using visual studio, make sure to install the vcpkg integration using <br />
```
vcpkg integrate install
```
I recommend cloning vcpkg directly into your project folder. In your project folder, with git, use <br />
```
git clone https://github.com/microsoft/vcpkg.git
```

## Step 3 - Build dependencies <br />
The project uses CMake and relies on the following libraries: <br />
GLFW (https://github.com/glfw/glfw.git) for windowing and input support <br />
glm (https://github.com/g-truc/glm.git) for its math funtionality <br />
glbinding (https://github.com/cginternals/glbinding.git) for OpenGL API binding <br />
globjects (https://github.com/cginternals/globjects.git) for additional OpenGL wrapping <br />
Dear ImGui (https://github.com/ocornut/imgui.git) for GUI elements <br />
NIFTI C Libraries (https://github.com/NIFTI-Imaging/nifti_clib) for handling of volumetric data <br />

If you are using Visual Studio, you can use its integrated CMake support to build and run the project.

When instead building from the command line, run the following commands from the project root folder:

```
mkdir build
cd build
cmake ..
```

After building, the executables will be available in the ```./bin``` folder.
