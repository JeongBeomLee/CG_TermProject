#pragma once
#include "pch.h"

enum camera_movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.5f;
const float ZOOM = 45.0f;


class CAMERA
{
public:
	glm::vec3 Position = glm::vec3(250., 1.f, -250.0);
	float first_person_view_x=-290.0;
	float first_person_view_y=0.5;
	float first_person_view_z=10.0;
	glm::vec3 Front = glm::vec3(0.f, 0.f, 0.f);
	glm::vec3 Up = glm::vec3(0.0, 1.0, 0.0);
	glm::vec3 Right;
	glm::vec3 WorldUp;
	// euler Angles
	float Yaw;
	float Pitch;
	// camera options
	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;

	CAMERA(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		Position = position;
		WorldUp = up;
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}

public:
	int camera_change=1;
public:
	glm::mat4 get_view()
	{
		return glm::lookAt(Position, Front, Up);
	}
	glm::mat4 get_view_frist()
	{
		updateCameraVectors();
		return glm::lookAt(Position, Position+Front, Up);
	}
	void set_frist_person()
	{
		Position = glm::vec3(first_person_view_x, first_person_view_y, first_person_view_z);
		unsigned int viewLocation = glGetUniformLocation(shaderID, "viewTransform");
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(get_view_frist()));
	}
	void set_third_person()
	{
		Position = glm::vec3(0.0, 400.f, 600.0);
		Front = glm::vec3(0.f, 0.f, 0.f);
		Up = glm::vec3(0.0, 1.0, 0.0);
		unsigned int viewLocation = glGetUniformLocation(shaderID, "viewTransform");
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(get_view()));
	}
	void set_top()
	{
		Position = glm::vec3(0.0, 900.f, 0.1);
		Front = glm::vec3(0.f, 0.f, 0.f);
		Up = glm::vec3(0.0, 1.0, 0.0);
		unsigned int viewLocation = glGetUniformLocation(shaderID, "viewTransform");
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(get_view()));
	}
	void process_mouse_movement(float xoffset, float yoffset,bool constrainPitch = TRUE)
	{
		
		xoffset *= MouseSensitivity;
		yoffset *= MouseSensitivity;

		Yaw += xoffset;
		Pitch += yoffset;

		 //make sure that when pitch is out of bounds, screen doesn't get flipped
			 if (constrainPitch)
			 {
				 if (Pitch > 0.0)
					 Pitch = 0.0;
				 if (Pitch < -0.0)
					 Pitch = -0.0;

			 }
		// update Front, Right and Up Vectors using the updated Euler angles
		updateCameraVectors();
	}
private:
	void updateCameraVectors()
	{
		// calculate the new Front vector
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Front = glm::normalize(front);
		// also re-calculate the Right and Up vector
		Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		Up = glm::normalize(glm::cross(Right, Front));
	}
};

