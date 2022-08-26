#include <iostream>

#include "ShaderObject.hpp"

#include "gl/glew.h"
#include "GLFW/glfw3.h"

//namespace cs = cyan::ShaderLib;
//namespace cst = cyan::ShaderLib;

void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

namespace csl = cyan::ShaderLib;

int width = 1000;
int height = 600;
	
int main(int argc, char* argv[]) {
	std::cout << "hello world" << std::endl;

	if (!glfwInit())
	{
		std::cout << "glfw init failed" << std::endl;
		glfwTerminate();
	}

	glfwSetErrorCallback(error_callback);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(width, height, "test", NULL, NULL);
	if (!window)
	{
		std::cout << "window init failed" << std::endl;
	}
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		std::cout << "glew init failed: " << glewGetErrorString(err) << std::endl;
	}

	std::cout << "Set up success" << std::endl;

	glViewport(0, 0, width, height);


	std::map<std::string, cyan::ShaderLib::ShaderSource> myShaderLib = {
{"net.cyanseraph.glsl.vertex.interface", cyan::ShaderLib::ShaderSource(
R"(
#ifndef net_cyanseraph_glsl_vertex_interface
#define net_cyanseraph_glsl_vertex_interface

layout (location = 0) in vec3 aPos;
out vec3 vPos;

#endif
)")}
	};
	cyan::ShaderLib::LoadShaderLibrary({ myShaderLib });


	csl::ShaderProgram myCubeShader(new csl::ShaderSource("#version 330 core", std::list<std::string>{"net.cyanseraph.glsl.vertex.interface"},
//VERTEX
R"(
//layout (location = 0) in vec3 aPos;
//out vec3 vPos;

void main() {
	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
	vPos = aPos;
};

)"), new csl::ShaderSource("#version 330 core",
//FRAGMENT
R"(

out vec4 FragColor;
in vec3 vPos;

void main()
{
	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f) + vec4(vPos, 1.0f);
};
)"));

	std::list<std::string> log;
	GLuint shaderProgram = myCubeShader.Build(log);

	for (auto elem : log) {
		std::cout << elem << std::endl;
	}
	//std::cout << log.size() << std::endl;
	
	float vertices[] = {
		 0.5f,  0.5f, 0.0f,  // top right
		 0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  // bottom left
		-0.5f,  0.5f, 0.0f   // top left 
	};
	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,  // first Triangle
		1, 2, 3   // second Triangle
	};
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	myCubeShader.SetActive();

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	return EXIT_SUCCESS;
}