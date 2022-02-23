// Baithuchanhso1_HelloWindow.cpp : This file contains the 'main' function. Program execution begins and ends there.
/////Phần 1. Thêm các thư viện
//GLEW

#include <GL/glew.h>

//GLFW
#include <GLFW/glfw3.h>
#include <iostream>

////Phần 2. Khai báo các hằng số, biến toàn cục, định nghĩa Hàm/Phương thức

//function
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
//kích thước window
const GLuint _WIDTH = 800, _HEIGHT = 600;

/// <summary>
/// Hàm Main, mình bắt đầu ứng dụng tại đây và chạy Game Loop.
/// </summary>
/// <returns></returns>
int main()
{
	//Init GLFW (khởi tạo GLFW)
	glfwInit();
	// Cấu hình các cài đặt yêu cầu cho GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	//Tạo một đối tượng GLFWwindow 
	GLFWwindow* window = glfwCreateWindow(_WIDTH, _HEIGHT, "Hello window 64IT", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	//set key từ bàn phím
	glfwSetKeyCallback(window, key_callback);

	//Khởi tạo GLEW
	//đặt giá trị cho glewExperimental= GL_TRUE trước khi khở tạo GLEW
	glewExperimental = GL_TRUE;
	
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	//Đinh nghĩa kích thước viewport
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	//Vòng lặp trò chơi GAME LOOP
	while (!glfwWindowShouldClose(window))
	{
		//kiểm tra nếu có bất kỳ sự kiện nào được kích hoạt (key bàn phím, chuột,etc) và gọi các function
		glfwPollEvents();

		//Rendering
		//xóa color buffer
		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);






		//swap the screen buffers 
		glfwSwapBuffers(window);
	}





	// cuối cùng, giải phóng tài nguyên GLFW
	glfwTerminate();
	return 0;
}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	std::cout << key << std::endl;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

