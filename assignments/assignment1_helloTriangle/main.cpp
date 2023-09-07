#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;



unsigned int MakeVAO(float* vertexData, int vertexNum);
unsigned int MakeShader(GLenum shaderType, const char* shaderSource);
unsigned int createShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource);


int main() {
	//0,0 is the center of the window
	//1,1 is the top right
	//-1,-1 is the bottom left
	float vertices[9] = {
		//x   //y  //z
		-0.5, -0.5, 0.0, //Left
		 0.5, -0.5, 0.0, //Right
		 0.0,  0.5, 0.0  //Top 
	};

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

	if (!gladLoadGL(glfwGetProcAddress)) {	//Loads the GL thing, put all GL code under it
		printf("GLAD Failed to load GL headers");
		return 1;
	}

	unsigned int vao = MakeVAO(vertices, sizeof(vertices));

	const char* vertexShaderSource = R"(
		#version 450
		layout(location = 0) in vec3 vPos;
		void main(){
		gl_Position = vec4(vPos,1.0);
		}
	)";

	const char* fragmentShaderSource = R"(
		#version 450
		out vec4 FragColor;
		void main(){
		FragColor = vec4(1.0);
		}
	)";

	unsigned int shader = createShaderProgram(vertexShaderSource, fragmentShaderSource);

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shader);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glfwSwapBuffers(window);
	}
	printf("Shutting down...");
}

unsigned int MakeVAO(float* vertexData, int vertexNum)
{
	//Makes new buffer ID
	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//Allocates space and sends data to GPU
	glBufferData(GL_ARRAY_BUFFER, vertexNum, vertexData, GL_STATIC_DRAW);

	//Makes a VAO that sends inputs to the vertex shader
	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	//Tell vao to pull vertex data from vbo
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	//sets position atributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (const void*)0);
	glEnableVertexAttribArray(0);

	return vao;
}

unsigned int MakeShader(GLenum shaderType, const char* shaderSource)
{
	//Make shader object
	unsigned int shader = glCreateShader(shaderType);
	//Give it the source code
	glShaderSource(shader, 1, &shaderSource, NULL);
	//Compile
	glCompileShader(shader);

	int success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		//512 is arbitrary
		char infoLog[512];
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		printf("Failed to compile shader: %s", infoLog);
		return 0;
	}
	else
		return shader;
}

unsigned int createShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource)
{
	unsigned int vertexShader = MakeShader(GL_VERTEX_SHADER, vertexShaderSource);
	unsigned int fragmentShader = MakeShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

	unsigned int shaderProgram = glCreateProgram();
	//Attach stages
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	//Link stages
	glLinkProgram(shaderProgram);

	//Check for errors
	int success;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		printf("Failed to link shader program: %s", infoLog);
		return 0;
	}
	//delete junk
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	return shaderProgram;
}
