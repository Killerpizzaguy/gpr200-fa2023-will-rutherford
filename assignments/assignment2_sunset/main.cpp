#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "wr/shader.h"

//Assignment 2

struct Vertex {
	float x, y, z;
	float u, v;
};

unsigned int createShader(GLenum shaderType, const char* sourceCode);
unsigned int createShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource);
unsigned int createVAO(Vertex* vertexData, int numVertices, unsigned int* indicesData, int numIndices);
void framebufferSizeCallback(GLFWwindow* window, int width, int height);

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;



//float vertices[12] = {
//	//x   //y  //z   
//	-1.0, -1.0, 0.0,
//	 1.0, -1.0, 0.0,
//	 1.0,  1.0, 0.0,
//	-1.0,  1.0, 0.0,
//};

Vertex vertices[4] = {
	//x    y    z    u    v
	{-1.0, -1.0, 0.0, 0.0, 0.0},
	{1.0, -1.0, 0.0, 1.0, 0.0},
	{1.0,  1.0, 0.0, 1.0, 1.0},
	{-1.0,  1.0, 0.0, 0.0, 1.0}
};

unsigned int indices[6] = {
	0, 1, 2,
	2, 3, 0
};

bool showImGUIDemoWindow = false;
float skyTopColor[3] = { 0.851, 0.447, 0.541 };
float skyBottomColor[3] = { 0.482, 0.42, 0.969 };
float topSunColor[3] = { 1, 1, 0.9 };
float bottomSunColor[3] = { 1.0, 0.3, 0.2 };
float foregroundColor[3] = { 0.1, 0.1, 0.1 };
float sunSpeed = 1.0f;
float sunRadius = 0.2f;

int main() {
	printf("Initializing...");
	if (!glfwInit()) {
		printf("GLFW failed to init!");
		return 1;
	}

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello Triangle", NULL, NULL);
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

	wr::Shader shader("assets/vertexShader.vert", "assets/tatooineSunsetShader.frag");
	//wr::Shader shader("assets/vertexShader.vert", "assets/fragmentShader.frag");
	unsigned int vao = createVAO(vertices, 4, indices, 6);

	shader.use();
	glBindVertexArray(vao);
	shader.setVec3("iResolution", SCREEN_WIDTH, SCREEN_HEIGHT, 1);

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//Set uniforms
		shader.setFloat("iTime", (float)glfwGetTime());
		shader.setVec3("_SkyTop", skyTopColor[0], skyTopColor[1], skyTopColor[2]);
		shader.setVec3("_SkyBottom", skyBottomColor[0], skyBottomColor[1], skyBottomColor[2]);
		shader.setVec3("_TopSunColor", topSunColor[0], topSunColor[1], topSunColor[2]);
		shader.setVec3("_BottomSunColor", bottomSunColor[0], bottomSunColor[1], bottomSunColor[2]);
		shader.setVec3("_ForegroundColor", foregroundColor[0], foregroundColor[1], foregroundColor[2]);
		shader.setFloat("_SunRadius", sunRadius);
		shader.setFloat("_SunSpeed", sunSpeed);
		
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);


		//Render UI
		{
			ImGui_ImplGlfw_NewFrame();
			ImGui_ImplOpenGL3_NewFrame();
			ImGui::NewFrame();

			ImGui::Begin("Settings");
			ImGui::Checkbox("Show Demo Window", &showImGUIDemoWindow);
			ImGui::ColorEdit3("Sky Top Color", skyTopColor);
			ImGui::ColorEdit3("Sky Bottom Color", skyBottomColor);
			ImGui::ColorEdit3("Top Sun Color", topSunColor);
			ImGui::ColorEdit3("Bottom Sun Color", bottomSunColor);
			ImGui::ColorEdit3("Foreground Color", foregroundColor);
			ImGui::SliderFloat("Sun Radius", &sunRadius, 0.0f, 1.0f);
			ImGui::SliderFloat("Sun Speed", &sunSpeed, 0.0f, 2.0f);
			ImGui::End();
			if (showImGUIDemoWindow) {
				ImGui::ShowDemoWindow(&showImGUIDemoWindow);
			}

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		glfwSwapBuffers(window);
	}
	printf("Shutting down...");
}




unsigned int createVAO(Vertex* vertexData, int numVertices, unsigned int* indicesData, int numIndices) {
	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	unsigned int ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * numIndices, indicesData, GL_STATIC_DRAW);

	//Define a new buffer id
	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//Allocate space for + send vertex data to GPU.
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * numVertices, vertexData, GL_STATIC_DRAW);

	//Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, x));
	glEnableVertexAttribArray(0);

	//UV
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(offsetof(Vertex, u)));
	glEnableVertexAttribArray(1);

	return vao;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

