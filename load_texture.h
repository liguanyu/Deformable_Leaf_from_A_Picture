#pragma once
#ifndef LOAD_TEXTURE_H
#define LOAD_TEXTURE_H

#include <gl/glew.h>
#include <windows.h>
#include <gl/glut.h>
#include <iostream>
#include <vector>
#include <cmath>

#define BMP_Header_Length 54  

GLuint load_texture(const char* file_name); //¥”"texture.bmp"∂¡»°Œ∆¿Ì


#endif // !LOAD_TEXTURE_H

