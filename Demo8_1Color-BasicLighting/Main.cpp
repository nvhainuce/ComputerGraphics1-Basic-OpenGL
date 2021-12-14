//khai báo và định thư viện stb_image
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
#include "Camera.h"
// Include GLFW
#include <GLFW/glfw3.h>

//include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


GLFWwindow* window;
//function
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

//window
const GLuint SCR_WIDTH = 800;
const GLuint SCR_HEIGHT = 600;
//camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

////timing
float deltaTime = 0.0f; //thời gian giữa khung hình hiện tại và khung hình sau
float lastframe = 0.0f;  //thời gian khung hình cuối

//lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
int main(void)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Tạo 1 cửa sổ GLFWwindow 
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Bai thuc hanh so 8 - Color", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);

	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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
	Shader ourShader("vShader_b8.vertex", "fShader_b8.frag");
	Shader LightCubeShader("light_cube.vertex", "light_cube.frag");

	//bước 1: khai báo vertex input (vertex data)
	GLfloat vertices[] = {
		// vị trí - position         
		 -0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,

		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,

		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,

		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,

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
	
	//Bước 2: Khởi tạo VBO, VAO
//VAO của cube
	//b2.1 VAO_cube
	GLuint VAO_cube;
	glGenVertexArrays(1, &VAO_cube);
	//bind VAO
	glBindVertexArray(VAO_cube);
	//b2.2 VBO
	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // liên kết (bind) VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//position attribute 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0); //unbind VBO , cho phép gọi hàm glVertexAttribPointer trong VBO
//unbind VAO_cube
	glBindVertexArray(0);

////VAO Light Cube (nguồn sáng)
	//b2.1 VAO_lightCube
	GLuint VAO_lightCube;
	glGenVertexArrays(1, &VAO_lightCube);
	//bind VAO
	glBindVertexArray(VAO_lightCube);
	//b2.2 VBO
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // liên kết (bind) VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//position attribute 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0); //unbind VBO , cho phép gọi hàm glVertexAttribPointer trong VBO
//unbind VAO_cube
	glBindVertexArray(0);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//Game Loop
	while (!glfwWindowShouldClose(window))
	{
		//thời gian mỗi khung hình (per-frame time)
		float currentFrame = glfwGetTime();       //               =0.01        -0                                    =0.011        0.01 
		deltaTime = currentFrame - lastframe;    //deltaTime0 = currentFrame0 - lastframe0;=0.01    //deltaTime1 = currentFrame1 - lastframe1;=0.001
		lastframe = currentFrame;                //lastframe0 = currentFrame0;=0.01                  //lastframe1 = currentFrame1;=0.001

		//input bàn phím 
		processInput(window);
		// check sự kiện  (ấn nút bàn phím, chuột,...)
		glfwPollEvents();
		//Render
		//xóa color buffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f); //set màu background
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//active shader 
		ourShader.use();
		glm::vec3 objectColor = glm::vec3(1.0f, 0.5f, 0.31f);
		GLuint UniformLocation_objectColor = glGetUniformLocation(ourShader.IDProgram, "objectColor");
		glUniform3fv(UniformLocation_objectColor, 1, glm::value_ptr(objectColor));

		glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
		GLuint UniformLocation_lightColor = glGetUniformLocation(ourShader.IDProgram, "lightColor");
		glUniform3fv(UniformLocation_objectColor, 1, glm::value_ptr(lightColor));

		///xác định ma trận biến đổi (các bạn muốn biến đổi đối tượng như nào )
		glm::mat4 model = glm::mat4(1.0f); // chắc chắn rằng đã tạo ma trận đơn vị

		//camera/view transformation
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		//lấy vị trị của uniform
		GLuint UniformLocation_model = glGetUniformLocation(ourShader.IDProgram, "model");
		GLuint UniformLocation_view = glGetUniformLocation(ourShader.IDProgram, "view");
		GLuint UniformLocation_projection = glGetUniformLocation(ourShader.IDProgram, "projection");
		//gán giá trị của 3 uniform đến shader

		glUniformMatrix4fv(UniformLocation_view, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(UniformLocation_projection, 1, GL_FALSE, glm::value_ptr(projection));
		//Buoc 3 Vẽ hình
		glBindVertexArray(VAO_cube);
		int lengA = sizeof(List_CubePosition) / sizeof(List_CubePosition[0]);
		for (int i = 0; i < lengA; i++)
		{
			model = glm::translate(view, List_CubePosition[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			glUniformMatrix4fv(UniformLocation_model, 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		


		////Light Cube////// (nguồn sáng)
		//active
		LightCubeShader.use();

		 UniformLocation_model = glGetUniformLocation(LightCubeShader.IDProgram, "model");
		 UniformLocation_view = glGetUniformLocation(LightCubeShader.IDProgram, "view");
		 UniformLocation_projection = glGetUniformLocation(LightCubeShader.IDProgram, "projection");
		 UniformLocation_lightColor = glGetUniformLocation(LightCubeShader.IDProgram, "lightColor");
		glUniformMatrix4fv(UniformLocation_view, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(UniformLocation_projection, 1, GL_FALSE, glm::value_ptr(projection));
		  //model
		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f));
		glUniformMatrix4fv(UniformLocation_model, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(UniformLocation_objectColor, 1, glm::value_ptr(lightColor));
			//vẽ nguồn sáng
		glBindVertexArray(VAO_lightCube);
		glDrawArrays(GL_TRIANGLES, 0, 36);


		///swap
		glfwSwapBuffers(window);
	}
	glDeleteVertexArrays(1, &VAO_cube);
	glDeleteVertexArrays(1, &VAO_lightCube);
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
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}