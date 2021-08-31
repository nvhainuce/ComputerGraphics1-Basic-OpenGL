#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <GL/glew.h>
// Xác định một số tùy chọn khả thi cho chuyển động của máy ảnh. Được sử dụng dưới dạng trừutượng(abstract) để tránh các phương thức nhập cụ thể windows ứng dụng
//Trong lập trình C/C++, Enum hay Enumeration là kiểu dữ liệu cố định, chỉ cho phép biến nhận sốsố giá trị nhất định nào đó.Các giá trị enum có thể coi là một hằng số.Việc sử dụng enum giúpđảm bảo giá trị các biến chỉ nhận các giá trị mong đợi.
enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};
// Các giá trị mặc định của camera
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;
// (An abstract camera class) một class camera trừu tượng xử lý ử lý đầu vào và tính toán cácGóc, Vectơ và Ma trận Euler tương ứng để sử dụng trong OpenGL.
class Camera
{
public:
	// camera Attributes
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;
	// euler Angles
	float Yaw;
	float Pitch;
	// camera options
	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;
	//Phương thức
	// constructor với các vector
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f,
		0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)),
		MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		Position = position;
		WorldUp = up;
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}
	// constructor với scalar values
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) :
		Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY),
		Zoom(ZOOM)
	{
		Position = glm::vec3(posX, posY, posZ);
		WorldUp = glm::vec3(upX, upY, upZ);
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}
	// trả về ma trận chế độ xem (view matrix) được tính bằng Euler Angles và Ma trận LookAt
	glm::mat4 GetViewMatrix()
	{
		return glm::lookAt(Position, Position + Front, Up);
	}
	// xử lý đầu vào nhận được từ hệ thống keyboard- input. Chấp nhận tham số đầu vào ở dạng ENUM do camera defined ENUM(to abstract it from windowing systems)
		void ProcessKeyboard(Camera_Movement direction, float deltaTime)
	{
		float velocity = MovementSpeed * deltaTime;
		if (direction == FORWARD)
			Position += Front * velocity;
		if (direction == BACKWARD)
			Position -= Front * velocity;
		if (direction == LEFT)
			Position -= Right * velocity;
		if (direction == RIGHT)
			Position += Right * velocity;
	}// xử lý đầu vào nhận được từ mouse input system. Giá trị offset theo cả phương x và y direction.
		void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
	{
		xoffset *= MouseSensitivity;
		yoffset *= MouseSensitivity;
		Yaw += xoffset;
		Pitch += yoffset;
		// đảm bảo rằng khi pitch nằm ngoài giới hạn, màn hình không bị lật
		if (constrainPitch)
		{
			if (Pitch > 89.0f)
				Pitch = 89.0f;
			if (Pitch < -89.0f)
				Pitch = -89.0f;
		}
		// update Front, Right and Up Vectors using the updated Euler angles
		updateCameraVectors();
	}
	// xử lý đầu vào nhận được từ sự kiện con lăn chuột (a mouse scroll-wheel event). Chỉ yêu cầu đầu vào trên trục dọc con lăn(vertical wheel - axis)
		void ProcessMouseScroll(float yoffset)
	{
		Zoom -= (float)yoffset;
		if (Zoom < 1.0f)
			Zoom = 1.0f;
		if (Zoom > 45.0f)
			Zoom = 45.0f;
	}
private:
	// tính toán vector front từ Góc Euler của Camera (updated)
	void updateCameraVectors()
	{
		// tính vector Front mới
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Front = glm::normalize(front);
		// cũng tính toán lại vectơ Right và Up
		Right = glm::normalize(glm::cross(Front, WorldUp));
		// chuẩn hóa các vectơ, bởi vì độ dài của chúng càng gần 0 thì bạn càng nhìn lên hoặc xuống, dẫn	đến chuyển động chậm hơn..
			Up = glm::normalize(glm::cross(Right, Front));
	}
};
#endif