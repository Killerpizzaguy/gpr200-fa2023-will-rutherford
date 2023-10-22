
#pragma once
#include "../ew/ewMath/mat4.h"
#include "../ew/ewMath/vec3.h"
#include "../ew/ewMath/ewMath.h"
#include "wrMath.h"
#include <math.h>
#include <GLFW/glfw3.h>

namespace wr {
	struct Camera {
		Camera();
		Camera(ew::Vec3 position, ew::Vec3 target, float fov, float aspectRatio, float nearPlane, float farPlane, bool orthographic, float orthoSize);
		ew::Vec3 position; //Camera body position
		ew::Vec3 target; //Position to look at
		float fov; //Vertical field of view in degrees
		float aspectRatio; //Screen width / Screen height
		float nearPlane; //Near plane distance (+Z)
		float farPlane; //Far plane distance (+Z)
		bool orthographic; //Perspective or orthographic?
		float orthoSize; //Height of orthographic frustum
		ew::Mat4 ViewMatrix(); //World->View
		ew::Mat4 ProjectionMatrix(); //View->Clip
	};

	struct CameraControls {
		double prevMouseX, prevMouseY; //Mouse position from previous frame
		float yaw = 0, pitch = 0; //Degrees
		float mouseSensitivity = 0.1f; //How fast to turn with mouse
		bool firstMouse = true; //Flag to store initial mouse position
		float moveSpeed = 5.0f; //How fast to move with arrow keys (M/S)
	};

	void moveCamera(GLFWwindow* window, Camera* camera, CameraControls* controls, float deltaTime);

	void resetCamera(GLFWwindow* window, Camera* cam, CameraControls* controls, ew::Vec3 pos, ew::Vec3 target, float pitch, float yaw);
}
