#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


class Shader
{
public:
  // program ID
  unsigned int ID;

  // constructer & create program
  Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
  // 使用/激活程序
  void use();
  // uniform工具函数
  void setBool(const std::string &name, bool value) const
  {
	  glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
  }
  void setInt(const std::string &name, int value) const
  {
	  glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
  }
  void setFloat(const std::string &name, float value) const
  {
	  glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
  }
  void setVec3(const std::string &name, const glm::vec3 &value) const
  {
	  glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
  }
  void setVec3(const std::string &name, float x, float y, float z) const
  {
	  glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
  }
  void setVec4(const std::string &name, const glm::vec4 &value) const
  {
	  glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
  }
  void setVec4(const std::string &name, float x, float y, float z, float w)
  {
	  glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
  }
  void setMat4(const std::string &name, const glm::mat4 &mat) const
  {
	  glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
  }
};

#endif