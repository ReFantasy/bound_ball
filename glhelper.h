#ifndef __GLHELPER_H__
#define __GLHELPER_H__
#include <string>


std::string ReadShaderFile(const char* file_path);
unsigned int CreateShaderProgram(const std::string& vertexShader, const std::string& fragmentShader);
int CompileShader(unsigned int type, const std::string& source);
void glfw_error_callback(int error, const char* description);

#endif//__GLHELPER_H__