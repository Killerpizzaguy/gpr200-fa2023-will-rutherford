#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <ew/shader.h>
#include <ew/procGen.h>
#include <ew/transform.h>

#include <wr/camera.h>

void framebufferSizeCallback(GLFWwindow* window, int width, int height);

//Projection will account for aspect ratio!
const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

const float FOV = 60;
const float ORTHO_HEIGHT = 6;
const float NEAR_PLANE = 0.1;
const float FAR_PLANE = 100;

const bool ORTHOGRAPHIC = true;

const ew::Vec3 CAM_START_POS(0, 0, 5);
const ew::Vec3 LOOK_AT_START_POS(0, 0, 0);

const int NUM_CUBES = 4;
ew::Transform cubeTransforms[NUM_CUBES];

int main() {
	printf("Initializing...");
	if (!glfwInit()) {
		printf("GLFW failed to init!");
		return 1;
	}

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Camera", NULL, NULL);
	if (window == NULL) {
		printf("GLFW failed to create window");
		return 1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	if (!gladLoadGL(glfwGetProcAddress)) {
		printf("GLAD Failed to load GL headers");
		return 1;
	}

	//Initialize ImGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();

	//Enable back face culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	//Depth testing - required for depth sorting!
	glEnable(GL_DEPTH_TEST);

	ew::Shader shader("assets/vertexShader.vert", "assets/fragmentShader.frag");
	
	//Cube mesh
	ew::Mesh cubeMesh(ew::createCube(0.5f));

	//Cube positions
	for (size_t i = 0; i < NUM_CUBES; i++)
	{
		cubeTransforms[i].position.x = i % (NUM_CUBES / 2) - 0.5;
		cubeTransforms[i].position.y = i / (NUM_CUBES / 2) - 0.5;
	}

	wr::Camera cam(CAM_START_POS, LOOK_AT_START_POS, FOV, SCREEN_WIDTH / SCREEN_HEIGHT, NEAR_PLANE, FAR_PLANE, ORTHOGRAPHIC, ORTHO_HEIGHT);
	wr::CameraControls camController;
	float prevTime = 0;
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		//Calculate deltaTime
		float time = (float)glfwGetTime(); //Timestamp of current frame
		float deltaTime = time - prevTime;
		prevTime = time;

		wr::moveCamera(window, &cam, &camController, deltaTime);
		wr::resetCamera(window, &cam, &camController, CAM_START_POS, LOOK_AT_START_POS, 0, 0);
		glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
		//Clear both color buffer AND depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Set uniforms
		shader.use();

		//TODO: Set model matrix uniform
		for (size_t i = 0; i < NUM_CUBES; i++)
		{
			//Construct model matrix
			shader.setMat4("_Model", cubeTransforms[i].getModelMatrix());
			//View Projection Matrix
			shader.setMat4("_ViewProjection", cam.ProjectionMatrix() * cam.ViewMatrix());

			cubeMesh.draw();
		}

		//Render UI
		{
			ImGui_ImplGlfw_NewFrame();
			ImGui_ImplOpenGL3_NewFrame();
			ImGui::NewFrame();

			ImGui::Begin("Settings");
			ImGui::Text("Cubes");
			for (size_t i = 0; i < NUM_CUBES; i++)
			{
				std::string transformHeaderName = "Transform " + std::to_string(i);
				ImGui::PushID(i);
				if (ImGui::CollapsingHeader(transformHeaderName.c_str())) {
					ImGui::DragFloat3("Position", &cubeTransforms[i].position.x, 0.05f);
					ImGui::DragFloat3("Rotation", &cubeTransforms[i].rotation.x, 1.0f);
					ImGui::DragFloat3("Scale", &cubeTransforms[i].scale.x, 0.05f);
				}
				ImGui::PopID();
			}
			ImGui::Text("Camera");
			ImGui::DragFloat3("Position", &cam.position.x, 0.05f);
			ImGui::DragFloat3("Look At", &cam.target.x, 0.05f);
			ImGui::DragFloat("FOV", &cam.fov, 0.05f);
			ImGui::DragFloat("Near Plane", &cam.nearPlane, 0.05f);
			ImGui::DragFloat("Far Plane", &cam.farPlane, 0.05f);
			ImGui::DragFloat("Ortho Size", &cam.orthoSize, 0.05f);
			ImGui::Checkbox("Orthographic", &cam.orthographic);
			ImGui::End();
			
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		glfwSwapBuffers(window);
	}
	printf("Shutting down...");
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

//void moveCamera(GLFWwindow* window, wr::Camera* camera, wr::CameraControls* controls)
//{
//	if (!glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2)) {
//		//Release cursor
//		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
//		controls->firstMouse = true;
//		return;
//	}
//	//hides the cursor
//	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
//
//	//Get screen mouse position
//	double mouseX, mouseY;
//	glfwGetCursorPos(window, &mouseX, &mouseY);
//
//	if (controls->firstMouse) {
//		controls->firstMouse = false;
//		controls->prevMouseX = mouseX;
//		controls->prevMouseY = mouseY;
//	}
//
//	//Get mouse position delta for this frame
//	float mouseDeltaX = (mouseX - controls->prevMouseX) * controls->mouseSensitivity;
//	float mouseDeltaY = (mouseY - controls->prevMouseY) * controls->mouseSensitivity;
//	//Add to yaw and pitch
//	controls->yaw += mouseDeltaX;
//	controls->pitch -= mouseDeltaY;
//	//Clamp pitch between -89 and 89 degrees
//	controls->pitch = wr::clamp(controls->pitch, -89, 89);
//
//	//Remember previous mouse position
//	controls->prevMouseX = mouseX;
//	controls->prevMouseY = mouseY;
//
//	ew::Vec3 forward(ew::Radians(controls->yaw), ew::Radians(controls->pitch), 0);
//
//	camera->target = camera->position + forward;
//}
