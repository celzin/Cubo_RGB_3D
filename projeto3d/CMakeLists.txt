cmake_minimum_required(VERSION 3.12)

project(Projeto3D)

add_executable(Projeto3D "main.cpp"   )

target_include_directories(Projeto3D PRIVATE dependencias/glm
											dependencias/stb
											dependencias/glfw/include
											dependencias/glew/include
											dependencias/glu/include)

target_link_directories(Projeto3D PRIVATE dependencias/glfw/lib-vc2022
										 dependencias/glew/lib/Release/x64)

target_link_libraries(Projeto3D PRIVATE glfw3.lib
									    glew32.lib
										opengl32.lib
										glu32.lib)