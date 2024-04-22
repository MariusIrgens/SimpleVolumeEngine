Step 1 - VCPKG (clone locally):
The project uses vcpkg (https://vcpkg.io) for dependency management, so this should take care of everything. 
Please follow the instructions on the vcpkg website (https://vcpkg.io/en/getting-started.html) to set it up for your environment. 
When using visual studio, make sure to install the vcpkg integration using 
```
vcpkg integrate install
```
I recommend cloning vcpkg directly into your project folder. In your project folder, with git, use 
```
git clone https://github.com/microsoft/vcpkg.git
```

Step 2 - Build all dependencies

Notes:
On my system, I need to use x64.