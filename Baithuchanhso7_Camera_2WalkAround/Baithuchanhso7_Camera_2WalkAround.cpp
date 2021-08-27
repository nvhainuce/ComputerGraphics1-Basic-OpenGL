﻿//khai báo và định thư viện stb_image
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
// Include GLEW
#include <GL/glew.h>

//
#include "Shader.h"

// Include GLFW
#include <GLFW/glfw3.h>

//include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


GLFWwindow* window;
//function
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void key_callback_Input_dichchuyenCamera_BanPhim(GLFWwindow* window);

//window
const GLuint WIDTH = 800;
const GLuint HEIGHT = 600;
//camera
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 5.0f);        //vi tri camera
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);     //vector hướng của camera
glm::vec3 cameraUp= glm::vec3(0.0f, 1.0f, 0.0f);

////timing
float deltaTime = 0.0f; //thời gian giữa khung hình hiện tại và khung hình sau
float lastframe = 0.0f;  //thời gian khung hình cuối

int main(void)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Tạo 1 cửa sổ GLFWwindow 
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Bai thuc hanh so 7 - camera", nullptr, nullptr);
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
	//cấu hình trạng thái opengl
	glEnable(GL_DEPTH_TEST);

	// Định nghĩa kích thước viewport
	 // Define the viewport dimensions
	int widthW, heightW;
	glfwGetFramebufferSize(window, &widthW, &heightW);    // lấy kích thước framebuffer   (chú ý)
	glViewport(0, 0, widthW, heightW);

	//khởi tạo shader
	Shader ourShader("vShader_b7.vertex", "fShader_b7.frag");


	//bước 1: khai báo vertex input (vertex data)
	GLfloat vertices[] = {
		// vị trí - position         //texture coordinate (s,t)(u,v)
		   //x       y     z       u     v
		   //Behind-Back(Z-)(XY)  (left) (down)
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f

	};
	/// khai báo vị trí của các cube trong tọa độ thế giới
	glm::vec3 List_CubePosition[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};
	//GLuint indices[] = {
	//	0,1,2,  //tam giac 1
	//	3,2,0    //tam giac 2
	//};
	//Bước 2: Khởi tạo VBO, VAO
		//b2.1 VAO
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	//bind VAO
	glBindVertexArray(VAO);
	//b2.2 VBO
	GLuint VBO;


	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // liên kết (bind) VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	////b2.3 EBO
	//GLuint EBO;
	//glGenBuffers(1, &EBO);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // liên kết (bind) VBO
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//position attribute 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	//texture coordinate attribute 
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (GLvoid*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0); //unbind VBO , cho phép gọi hàm glVertexAttribPointer trong VBO
//unbind VAO
	glBindVertexArray(0);

	///Load và tạo 1 texture
	//khởi tạo texture
	GLuint texture_wood;
	glGenTextures(1, &texture_wood);
	glBindTexture(GL_TEXTURE_2D, texture_wood);
	//cài đặt tham số texture wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//cài đặt tham số texture filtering 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//tải image, tạo texture + mipmaps
	int width, height, nrChannels;
	unsigned char* image = stbi_load("image_cat_y.jpg", &width, &height, &nrChannels, 0);
	if (image)
	{
		///tạo mipmaps
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		//đưa ra thông báo lỗi load image
		std::cout << "Failed to Load texture" << std::endl;
	}
	//delete image 
	stbi_image_free(image);

	//unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);  //unbind texture




	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//Game Loop
	while (!glfwWindowShouldClose(window))
	{
		//thời gian mỗi khung hình (per-frame time)
		float currentFrame = glfwGetTime();       //               =0.01        -0                                    =0.011        0.01 
		deltaTime = currentFrame - lastframe;    //deltaTime0 = currentFrame0 - lastframe0;=0.01    //deltaTime1 = currentFrame1 - lastframe1;=0.001
		lastframe = currentFrame;                //lastframe0 = currentFrame0;=0.01                  //lastframe1 = currentFrame1;=0.001
		           
		//input bàn phím 
		key_callback_Input_dichchuyenCamera_BanPhim(window);
		// check sự kiện  (ấn nút bàn phím, chuột,...)
		glfwPollEvents();
		//Render
		//xóa color buffer
		glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//bind texture
		glBindTexture(GL_TEXTURE_2D, texture_wood);


		///xác định ma trận biến đổi (các bạn muốn biến đổi đối tượng như nào )
		glm::mat4 model = glm::mat4(1.0f); // chắc chắn rằng đã tạo ma trận đơn vị
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);

		//model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(1.0f, 1.0f, 0.0f));//muốn xoay mô hình đối tượng 1 góc 30 9theo trục x
		//camera/view transformation
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		projection = glm::perspective(45.0f, (float)WIDTH / (float)HEIGHT, 0.01f, 100.0f);
		//projection = glm::ortho(0.0f, (float)WIDTH ,0.0f, (float)HEIGHT, 0.1f, 100.0f);
		//lấy vị trị của uniform
		GLuint UniformLocation_model = glGetUniformLocation(ourShader.IDProgram, "model");
		GLuint UniformLocation_view = glGetUniformLocation(ourShader.IDProgram, "view");
		GLuint UniformLocation_projection = glGetUniformLocation(ourShader.IDProgram, "projection");
		//gán giá trị của 3 uniform đến shader

		glUniformMatrix4fv(UniformLocation_view, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(UniformLocation_projection, 1, GL_FALSE, glm::value_ptr(projection));
		//Buoc 3 Vẽ hình
		ourShader.use();

		glBindVertexArray(VAO);
		int lengA = sizeof(List_CubePosition) / sizeof(List_CubePosition[0]);
		for (int i = 0; i < lengA; i++)
		{
			model = glm::translate(view, List_CubePosition[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			glUniformMatrix4fv(UniformLocation_model, 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glBindVertexArray(0);

		///swap
		glfwSwapBuffers(window);
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	//	glDeleteBuffers(1, &EBO);

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

void key_callback_Input_dichchuyenCamera_BanPhim(GLFWwindow* window)
{
	float cameraSpeed = 2.0f*deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos +=   cameraSpeed * cameraFront; //cameraPos = cameraPos + cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize( glm::cross(cameraFront,cameraUp) )*cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}