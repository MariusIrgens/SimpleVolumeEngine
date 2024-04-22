cmake_minimum_required (VERSION 3.10)

# Add source to this project's executable.
add_executable (simplevolumeengine "main.cpp" "SystemManager.h" "SystemManager.cpp" "Scene.h" "Scene.cpp" "Volume.h" "Volume.cpp" "RendererVolume.h" "RendererVolume.cpp" "Camera.h" "Camera.cpp" "InteractorKeyboard.h" "InteractorKeyboard.cpp" "InteractorMouse.h" "InteractorMouse.cpp" "InteractorGUI.h" "InteractorGUI.cpp")

# Packages:

find_package(glm CONFIG REQUIRED)
target_link_libraries(simplevolumeengine PRIVATE glm::glm)

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(simplevolumeengine PRIVATE glfw)

find_package(glbinding CONFIG REQUIRED)
target_link_libraries(simplevolumeengine PRIVATE glbinding::glbinding glbinding::glbinding-aux)

find_package(globjects CONFIG REQUIRED)
target_link_libraries(simplevolumeengine PRIVATE globjects::globjects)

find_package(imgui CONFIG REQUIRED)
target_link_libraries(simplevolumeengine PRIVATE imgui::imgui)

# Use tinyfiledialogs to immplement loading and saving
find_package(tinyfiledialogs CONFIG REQUIRED)
target_link_libraries(simplevolumeengine PRIVATE tinyfiledialogs::tinyfiledialogs)

find_package(NIFTI CONFIG REQUIRED)
target_link_libraries(simplevolumeengine PRIVATE NIFTI::niftiio)

set(STB_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/vcpkg/packages/stb_x64-windows/include")
find_path(STB_INCLUDE_DIRS NAMES "stb.h")
target_include_directories(simplevolumeengine PRIVATE ${STB_INCLUDE_DIRS})