#define _USE_MATH_DEFINES
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include "../Header Files/Shader.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const float steps = 10;
const float angle = M_PI * 2.f / steps;
int main()
{
	glfwInit();
	//openGL version 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glEnable(GL_DEPTH_TEST);

	Shader ourShader("shaders/triangle.vert", "shaders/trianglef.frag");

	// generate sphere verticies
	std::vector<float> allCircleVertices1;

	float xPos = 0; float yPos = 0; float radius = 1.0f;
	float prevX = xPos;
	float prevY = yPos - radius;

	for (size_t i = 0; i <= steps; i++)
	{
		float newX = radius * sin(angle * i);
		float newY = -radius * cos(angle * i);

		allCircleVertices1.push_back(0.0f);
		allCircleVertices1.push_back(0.0f);
		allCircleVertices1.push_back(0.0f);

		allCircleVertices1.push_back(prevX);
		allCircleVertices1.push_back(prevY);
		allCircleVertices1.push_back(0.0f);
		
		allCircleVertices1.push_back(newX);
		allCircleVertices1.push_back(newY);
		allCircleVertices1.push_back(0.0f);

		prevX = newX;
		prevY = newY;
	}
	for (float x : allCircleVertices1) {
		std::cout << x << "\n";
	}

	//float vertices[] = {
	//	// positions          
	//	-0.5f, -0.5f, -0.5f,
	//	 0.5f, -0.5f, -0.5f,
	//	 0.5f,  0.5f, -0.5f,
	//	 0.5f,  0.5f, -0.5f,
	//	-0.5f,  0.5f, -0.5f,
	//	-0.5f, -0.5f, -0.5f,

	//	-0.5f, -0.5f,  0.5f,
	//	 0.5f, -0.5f,  0.5f,
	//	 0.5f,  0.5f,  0.5f,
	//	 0.5f,  0.5f,  0.5f,
	//	-0.5f,  0.5f,  0.5f,
	//	-0.5f, -0.5f,  0.5f,

	//	-0.5f,  0.5f,  0.5f,
	//	-0.5f,  0.5f, -0.5f,
	//	-0.5f, -0.5f, -0.5f,
	//	-0.5f, -0.5f, -0.5f,
	//	-0.5f, -0.5f,  0.5f,
	//	-0.5f,  0.5f,  0.5f,

	//	 0.5f,  0.5f,  0.5f,
	//	 0.5f,  0.5f, -0.5f,
	//	 0.5f, -0.5f, -0.5f,
	//	 0.5f, -0.5f, -0.5f,
	//	 0.5f, -0.5f,  0.5f,
	//	 0.5f,  0.5f,  0.5f,

	//	-0.5f, -0.5f, -0.5f,
	//	 0.5f, -0.5f, -0.5f,
	//	 0.5f, -0.5f,  0.5f,
	//	 0.5f, -0.5f,  0.5f,
	//	-0.5f, -0.5f,  0.5f,
	//	-0.5f, -0.5f, -0.5f,

	//	-0.5f,  0.5f, -0.5f,
	//	 0.5f,  0.5f, -0.5f,
	//	 0.5f,  0.5f,  0.5f,
	//	 0.5f,  0.5f,  0.5f,
	//	-0.5f,  0.5f,  0.5f,
	//	-0.5f,  0.5f, -0.5f
	//};

	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, allCircleVertices1.size() * sizeof(GLfloat), allCircleVertices1.data() , GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
	
	

	while (!glfwWindowShouldClose(window))
	{
		
		processInput(window);
		glClearColor(0.2, 0.3f, 0.3f, 0.3f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// draw our first triangle
		ourShader.use();

		//create transformations
		glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);
		model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
		projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

		// retrieve the matrix uniform locations
		unsigned int modelLoc = glGetUniformLocation(ourShader.ID, "model");
		unsigned int viewLoc = glGetUniformLocation(ourShader.ID, "view");
		// pass them to the shaders (3 different ways)
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);

		// note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
		ourShader.setMat4("projection", projection);
		
		// Render
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 90);
	

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;

}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}
