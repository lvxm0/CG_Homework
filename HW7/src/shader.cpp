#include<glad\glad.h>
#include"shader.h"
#include<string>
#include<iostream>
#include<sstream>
#include<fstream>

Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath) {
  // ------------- read shader code from file -----------
  // string & file
  std::string vertexSrc;
  std::string fragmentSrc;
  std::ifstream vShaderFile;
  std::ifstream fShaderFile;
  // exception
  vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  try {
    // open file
    vShaderFile.open(vertexPath);
    fShaderFile.open(fragmentPath);
    std::stringstream vShaderStream, fShaderStream;
    // read file into bufer
    vShaderStream << vShaderFile.rdbuf();
    fShaderStream << fShaderFile.rdbuf();
    // close file
    vShaderFile.close();
    fShaderFile.close();
    vertexSrc = vShaderStream.str();
    fragmentSrc = fShaderStream.str();
  }
  catch (const std::exception&) {
    std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
  }
  const char* vShaderCode = vertexSrc.c_str();
  const char* fShaderCode = fragmentSrc.c_str();

  // --------------- compile shader ------------
  unsigned int vertex, fragment;
  int success;
  char compileLog[512];
  // compile vertex shader

  vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex, 1, &vShaderCode, NULL);
  glCompileShader(vertex);

  // compile fragment shader
  fragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment, 1, &fShaderCode, NULL);
  glCompileShader(fragment);
 
  // shader program
  ID = glCreateProgram();
  glAttachShader(ID, vertex);
  glAttachShader(ID, fragment);
  glLinkProgram(ID);

 
  glDeleteShader(vertex);
  glDeleteShader(fragment);
}
void Shader::use() {
  glUseProgram(ID);
}
