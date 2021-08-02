#ifndef SHADER1_H
#define SHADER1_H
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
	GLuint IDProgram;

	//khởi tạo constructor 
	Shader(const char* vertexPath, const char* fragmentPath)
	{
		//1. Lấy đường dẫn dẫn vertex/fragment source code từ file  *.vs *.frag
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		// đảm bảo ifstream có thể thông qua các ngoại lệ
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
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

		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();

		//2. Biên dịch (Compile) Shader 
		GLuint vertexShader, fragmentShader;
		//Vertex Shader
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vShaderCode, NULL);
		glCompileShader(vertexShader);
		CheckCompileError(vertexShader, "VERTEX");
		//fragment Shader
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
		glCompileShader(fragmentShader);
		CheckCompileError(fragmentShader, "FRAGMENT");
		//shader program
		IDProgram = glCreateProgram();
		glAttachShader(IDProgram, vertexShader);
		glAttachShader(IDProgram, fragmentShader);
		glLinkProgram(IDProgram);
		CheckCompileError(IDProgram, "PROGRAM");
		// delete shader
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}
	//active shader
	void use()
	{
		glUseProgram(IDProgram);
	}
	//uniform
	//...
private:
	
	//khởi tạo 1 hàm cho việc check quá trình compie,linking shader, 
	void CheckCompileError(GLuint shader, std::string type)   //type: VERTEX,FRAGMENT,PROGRAM
	{
		int success;
		char infolog[1024];
		if (type != "PROGRAM")
		{
			//check compile shader
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shader, 1024, NULL, infolog);
				std::cout << "ERROR: COMPILE SHADER :" << "\n" << infolog << std::endl;
			}
		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(shader, 1024, NULL, infolog);
				std::cout << "ERROR: LINKING PROGRAM :" << "\n" << infolog << std::endl;
			}
		}
	}

};
#endif