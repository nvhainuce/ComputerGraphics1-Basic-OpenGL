// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;


//function
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

const GLuint WIDTH = 800, HEIGHT = 600;
//bước 0.0: chuẩn bị shader
const GLchar* vertexShaderSource = "#version 330 core\n"
"layout (location=0) in vec3 position;\n"
"void main()\n"
"{\n"
"gl_Position= vec4(position.x,position.y,position.z,1);\n"
"}\0";
const GLchar* fragmentShaderYELLOWSource = "#version 330 core\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"color= vec4(1.0f,1.0f,0.0f,1.0f);\n"
"}\0";
const GLchar* fragmentShaderREDSource = "#version 330 core\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"color= vec4(1.0f,0.0f,0.0f,1.0f);\n"
"}\0";
int main(void)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Tạo 1 cửa sổ GLFWwindow 
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Bai thuc hanh so 2 - Hello Triangle", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);

	//Đặt biến glewExperimental về true  (bắt buộc)
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	// Định nghĩa kích thước viewport
	 // Define the viewport dimensions
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);    // lấy kích thước framebuffer   (chú ý)
	glViewport(0, 0, width, height);;

	////// bước 0.1 Build và biên dịch shader program
	// Vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	// Check for compile time errors
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// Fragment shader 
	GLuint fragmentShaderRED = glCreateShader(GL_FRAGMENT_SHADER);
	GLuint fragmentShaderYELLOW = glCreateShader(GL_FRAGMENT_SHADER);
	    //biên dịch fragment shader red///////////////////////////
	glShaderSource(fragmentShaderRED, 1, &fragmentShaderREDSource, NULL);
	glCompileShader(fragmentShaderRED);
	// Check for compile time errors
	glGetShaderiv(fragmentShaderRED, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShaderRED, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
			//biên dịch fragment shader Yellow///////////////////////////
	glShaderSource(fragmentShaderYELLOW, 1, &fragmentShaderYELLOWSource, NULL);
	glCompileShader(fragmentShaderYELLOW);
	// Check for compile time errors
	glGetShaderiv(fragmentShaderYELLOW, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShaderYELLOW, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Link shaders
	/////////////////////////////////shaderProgramRED////////////////////////
	GLuint shaderProgramRED = glCreateProgram();
	glAttachShader(shaderProgramRED, vertexShader);
	glAttachShader(shaderProgramRED, fragmentShaderRED);
	glLinkProgram(shaderProgramRED);
	// Check for linking errors
	glGetProgramiv(shaderProgramRED, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgramRED, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShaderRED);
	/////////////////////////////////shaderProgramYELLOW////////////////////////
	GLuint shaderProgramYELLOW = glCreateProgram();
	glAttachShader(shaderProgramYELLOW, vertexShader);
	glAttachShader(shaderProgramYELLOW, fragmentShaderYELLOW);
	glLinkProgram(shaderProgramYELLOW);
	// Check for linking errors
	glGetProgramiv(shaderProgramYELLOW, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgramYELLOW, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShaderYELLOW);
	//bước 1: khai báo vertex input (vertex data)
	GLfloat vertices_Tamgiac1[] = {
		// tam giac 1
		-0.5f,-0.5f,0.0f,  //bottom-left
		0.5f,-0.5f,0.0f,   //bottom right
		-0.5f,0.5f,0.0f   //Top Left 
	};
	GLfloat vertices_Tamgiac2[] = {
		//tam giac 2
		-0.5f,0.5f,0.0f,  //Top-left
		0.5f,0.5f,0.0f,   //Top right
		0.5f,-0.5f,0.0f    //Bot Right 
	};
	//Bước 2: Khởi tạo VBO, VAO
	//b2.1 VAO
	GLuint VAOs[2],VBOs[2];
	glGenVertexArrays(2, VAOs);
	glGenBuffers(2, VBOs);

	///////////////////////////////VAO - TAM GIAC 1//////////////////////////////////////
	//bind VAOs[0] 
	glBindVertexArray(VAOs[0]);
		//b2.2 VBO	
		glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]); // liên kết (bind) VBO
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_Tamgiac1), vertices_Tamgiac1, GL_STATIC_DRAW);

		//set attribute point
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0); //unbind VBO , cho phép gọi hàm glVertexAttribPointer trong VBO
	//unbind VAO
	glBindVertexArray(0);

	///////////////////////////////VAO - TAM GIAC 2//////////////////////////////////////
    //bind VAOs[1] 
	glBindVertexArray(VAOs[1]);
	//b2.2 VBO	
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]); // liên kết (bind) VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_Tamgiac2), vertices_Tamgiac2, GL_STATIC_DRAW);

	//set attribute point
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0); //unbind VBO , cho phép gọi hàm glVertexAttribPointer trong VBO
//unbind VAO
	glBindVertexArray(0);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//Game Loop
	while (!glfwWindowShouldClose(window))
	{
		// check sự kiện  (ấn nút bàn phím, chuột,...)
		glfwPollEvents();
		//Render
		//xóa color buffer
		glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//Buoc 3 Vẽ hình 1 hình tam giác

		glUseProgram(shaderProgramRED);
		glBindVertexArray(VAOs[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		glUseProgram(shaderProgramYELLOW);
		glBindVertexArray(VAOs[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		///swap
		glfwSwapBuffers(window);
	}
	glDeleteVertexArrays(2, VAOs);
	glDeleteBuffers(2, VBOs);
	glDeleteProgram(shaderProgramRED);
	glDeleteProgram(shaderProgramYELLOW);
	//Terminate GLFW, xóa và dọn dẹp tài nguyên sau khi thoát
	glfwTerminate();
	return 0;

}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	std::cout << key << std::endl;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}
