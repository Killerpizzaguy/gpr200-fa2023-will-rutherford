#include "camera.h"
#include "transform.h"

namespace wr {

	Camera::Camera()
	{
		this->position = ew::Vec3(0,0,5);
		this->target = ew::Vec3(0,0,0);
		this->fov = 60;
		this->aspectRatio = 1080/720;
		this->nearPlane = 0.1;
		this->farPlane = 100;
		this->orthographic = true;
		this->orthoSize = 6;
	}

	Camera::Camera(ew::Vec3 position, ew::Vec3 target, float fov, float aspectRatio, float nearPlane, float farPlane, bool orthographic, float orthoSize)
	{
		this->position = position;
		this->target = target;
		this->fov = fov;
		this->aspectRatio = aspectRatio;
		this->nearPlane = nearPlane;
		this->farPlane = farPlane;
		this->orthographic = orthographic;
		this->orthoSize = orthoSize;
	}

	ew::Mat4 Camera::ViewMatrix() //World->View
	{
		return LookAt(position, target, ew::Vec3(0, 1, 0));
	}
	ew::Mat4 Camera::ProjectionMatrix() //View->Clip
	{
		if (orthographic)
		{
			return Orthographic(orthoSize, aspectRatio, nearPlane, farPlane);
		}
		else 
		{
			return Perspective(ew::Radians(fov), aspectRatio, nearPlane, farPlane);
		}
	}

	void moveCamera(GLFWwindow* window, Camera* camera, CameraControls* controls, float deltaTime)
	{
		//Change cam direction
		if (!glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2)) {
			//Release cursor
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			controls->firstMouse = true;
			return;
		}
		//hides the cursor
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		//Get screen mouse position
		double mouseX, mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);

		if (controls->firstMouse) {
			controls->firstMouse = false;
			controls->prevMouseX = mouseX;
			controls->prevMouseY = mouseY;
		}

		//Get mouse position delta for this frame
		float mouseDeltaX = (mouseX - controls->prevMouseX) * controls->mouseSensitivity;
		float mouseDeltaY = (mouseY - controls->prevMouseY) * controls->mouseSensitivity;
		//Add to yaw and pitch
		controls->yaw += mouseDeltaX;
		controls->pitch -= mouseDeltaY;
		//Clamp pitch between -89 and 89 degrees
		controls->pitch = wr::clamp(controls->pitch, -89, 89);

		//Remember previous mouse position
		controls->prevMouseX = mouseX;
		controls->prevMouseY = mouseY;

		//ew::Vec3 forward(ew::Radians(controls->yaw), ew::Radians(controls->pitch), -1);
		float yawRadians = ew::Radians(controls->yaw);
		float pitchRadians = ew::Radians(controls->pitch);

		ew::Vec3 forward(
			sin(yawRadians) * cos(pitchRadians),
			sin(pitchRadians),
			-cos(yawRadians) * cos(pitchRadians)
		);


		//change cam position

		ew::Vec3 right = ew::Cross(ew::Vec3(0,1,0), forward) / ew::Magnitude(ew::Cross(ew::Vec3(0, 1, 0), forward));
		ew::Vec3 up = ew::Cross(right, forward) / ew::Magnitude(ew::Cross(right, forward));

		//TODO: Keyboard controls for moving along forward, back, right, left, up, and down. See Requirements for key mappings.
		//EXAMPLE: Moving along forward axis if W is held.
		//Note that this is framerate dependent, and will be very fast until you scale by deltaTime. See the next section.

		if (glfwGetKey(window, GLFW_KEY_W)) {
			camera->position += forward * controls->moveSpeed * deltaTime;
		}
		if (glfwGetKey(window, GLFW_KEY_S)) {
			camera->position -= forward * controls->moveSpeed * deltaTime;
		}
		if (glfwGetKey(window, GLFW_KEY_D)) {
			camera->position -= right * controls->moveSpeed * deltaTime;
		}
		if (glfwGetKey(window, GLFW_KEY_A)) {
			camera->position += right * controls->moveSpeed * deltaTime;
		}
		if (glfwGetKey(window, GLFW_KEY_E)) {
			camera->position -= up * controls->moveSpeed * deltaTime;
		}
		if (glfwGetKey(window, GLFW_KEY_Q)) {
			camera->position += up * controls->moveSpeed * deltaTime;
		}
		


		//Setting camera.target should be done after changing position. Otherwise, it will use camera.position from the previous frame and lag behind
		camera->target = camera->position + forward;

	}

	void resetCamera(GLFWwindow* window, Camera* cam, CameraControls* controls, ew::Vec3 pos, ew::Vec3 target, float pitch, float yaw)
	{
		if (glfwGetKey(window, GLFW_KEY_R)) {
			cam->position = pos;
			cam->target = target;
			controls->pitch = pitch;
			controls->yaw = yaw;
		}
	}
}