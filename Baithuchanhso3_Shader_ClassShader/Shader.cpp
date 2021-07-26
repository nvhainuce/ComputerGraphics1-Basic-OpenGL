#ifndef SHADER_H
#define SHADER_H
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;
#include <GL/glew.h>;

class Shader
{
public:
	//ID Program
	GLuint ID;

	//khởi tạo constructor 
	Shader(const char*vertexPath, const char* fragmentPath)
	{
		//1. Lấy đường dẫn dẫn vertex/fragment source code từ file  *.vs *.frag
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		// đảm bảo ifstream có thể thông qua các ngoại lệ
		vShaderFile.exceptions(std::ifstream::failbit| std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			//openfile
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			std::stringstream vShaderStream, fShaderStream;
			//read file và lưu vào bộ nhớ
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			//close file
			vShaderFile.close();
			fShaderFile.close();
			// conver stream to string
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure& e)
		{
			std::cout << "ERROR::SHADER::FILE NOT SUCCESFULLY READ" << std::endl;
		}
	}
};