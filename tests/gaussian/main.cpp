#include <iostream>
#include <sstream>
#include <iomanip>
#include <cmath>

#include "CyanShaderLib/ShaderProgram.hpp"
#include "CyanShaderLib/ShaderLoader.hpp"

#include "gl/glew.h"
#include "GLFW/glfw3.h"
#include "glm/matrix.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


namespace csl = cyan::ShaderLib;

#pragma region GLFW_CALLBACKS

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

#pragma endregion

int window_width = 1000;
int window_height = 600;

struct vector3 {
	float x, y, z;
};

template<typename V>
void SetUniformV3(cyan::ShaderLib::ShaderProgram prog, std::string name, V v3) {
	prog.SetActive();
	GLfloat col[] = { v3.x, v3.y, v3.z };
	glUniform3fv(prog.FindUniformLocation(name), 1, &col[0]);
}

//from: https://stackoverflow.com/a/10848293
template <typename T>
T Gauss(const T x, const T m, const T s)
{
	static const T inv_sqrt_2pi = 0.3989422804014327;
	T a = (x - m) / s;

	return inv_sqrt_2pi / s * std::exp(-T(0.5) * a * a);
}

template<unsigned int SIZE, typename T>
void GaussMatrix(T* buffer, T range, T mu, T standard_deviation) {
	T step = (range * 2) / (T)(SIZE);

	for (int y = 0; y < SIZE; y++) {
		T y_value = Gauss<T>(std::lerp(-range, range, ((double)y) / ((double)SIZE)), mu, standard_deviation);
		for (int x = 0; x < SIZE; x++) {
			T val = Gauss<T>(std::lerp(-range, range, ((double)x) / ((double)SIZE)), mu, standard_deviation) * y_value;
			buffer[y * SIZE + x] = val;

			//std::stringstream stream;
			//stream << std::fixed << std::setprecision(4) << val;
			//std::cout << stream.str() << '	';
		}
		//std::cout << std::endl << std::endl;
	}
}

const unsigned int GUASS_MATRIX_SIZE = 4;
const double GUASS_MATRIX_RANGE = 2;
const double GUASS_MU = 0;
const double GAUSS_STANDARD_DEVIATION = 0.8;

int main(int argc, char* argv[]) {
	std::cout << "hello world" << std::endl;

	float gauss_matrix[GUASS_MATRIX_SIZE * GUASS_MATRIX_SIZE];
	GaussMatrix<GUASS_MATRIX_SIZE, float>(gauss_matrix, GUASS_MATRIX_RANGE, GUASS_MU, GAUSS_STANDARD_DEVIATION);
	std::cout << std::endl << std::endl;
	for (int i = 0; i < GUASS_MATRIX_SIZE * GUASS_MATRIX_SIZE; i++) {
		std::stringstream stream;
		stream << std::fixed << std::setprecision(4) << gauss_matrix[i];
		std::cout << stream.str() << '	';
		if ((i + 1) % (GUASS_MATRIX_SIZE) == 0) {
			std::cout << std::endl << std::endl;
		}
	}

	std::cout << std::endl;

#pragma region WINDOW_SETUP

	if (!glfwInit())
	{
		std::cout << "glfw init failed" << std::endl;
		glfwTerminate();
	}

	glfwSetErrorCallback(error_callback);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(window_width, window_height, "Gaussian", NULL, NULL);
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

#pragma endregion

	glViewport(0, 0, window_width, window_height);

#pragma region CSL_LIBRARY
//init library of interfaces
	std::map<std::string, cyan::ShaderLib::ShaderSource> myShaderLib = {
//VERTEX
	{"net.cyanseraph.glsl.vertex.pos_interface", cyan::ShaderLib::ShaderSource(
	R"(
	#ifndef net_cyanseraph_glsl_vertex_pos_interface
	#define net_cyanseraph_glsl_vertex_pos_interface

	layout (location = 0) in vec3 aPos;
	out vec3 vPos;

	#endif
	)")
	},
	{"net.cyanseraph.glsl.vertex.tex_interface", cyan::ShaderLib::ShaderSource(
	R"(
	#ifndef net_cyanseraph_glsl_vertex_tex_interface
	#define net_cyanseraph_glsl_vertex_tex_interface

	layout (location = 1) in vec2 aTex;
	out vec2 vTex;

	#endif
	)")

//FRAGMENT
	},{"net.cyanseraph.glsl.fragment.pos_interface", cyan::ShaderLib::ShaderSource(
	R"(
	#ifndef net_cyanseraph_glsl_fragment_pos_interface
	#define net_cyanseraph_glsl_fragment_pos_interface

	out vec4 FragColor;
	in vec3 vPos;

	#endif
	)")
	},
	{"net.cyanseraph.glsl.fragment.tex_interface", cyan::ShaderLib::ShaderSource(
	R"(
	#ifndef net_cyanseraph_glsl_fragment_tex_interface
	#define net_cyanseraph_glsl_fragment_tex_interface

	in vec2 vTex;

	#endif
	)")
	}
	};
	cyan::ShaderLib::LoadShaderLibrary({ myShaderLib });

#pragma endregion

#pragma region TEXTURE

	unsigned int texture;

	{
		int image_width, image_height, nrChannels;
		unsigned char* data = stbi_load("C:\\Users\\User\\source\\repos\\testGTK\\CyanShaderLib\\tests\\gaussian\\jet.jpg", &image_width, &image_height, &nrChannels, 0);//, C:\\Users\\Sam Taseff\\Documents\\cpp\\CyanShaderLib\\tests\\gaussian
		if (data)
		{
			//opengl
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image_width, image_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			stbi_image_free(data);
		}
		else {
			std::cout << "Failed to load texture" << '\n';
			return -1;
		}
	}

#pragma endregion

	csl::ShaderProgram myShader(new csl::ShaderSource("#version 330 core", std::list<std::string>{"net.cyanseraph.glsl.vertex.pos_interface", "net.cyanseraph.glsl.vertex.tex_interface"},
	//VERTEX
	R"(
	void main() {
		gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
		vPos = aPos;
		vTex = aTex;
	};

	)"), new csl::ShaderSource("#version 330 core", std::list<std::string>{"net.cyanseraph.glsl.fragment.pos_interface", "net.cyanseraph.glsl.fragment.tex_interface"},
	//FRAGMENT
	R"(
	#extension GL_EXT_gpu_shader4 : enable

	uniform vec3 tint;

	uniform sampler2D vTexture;
	uniform int gaussMatrixSize;
	uniform float gaussMatrix[)" + std::to_string(GUASS_MATRIX_SIZE * GUASS_MATRIX_SIZE) + R"(];

	void main()
	{
		vec3 finalColor = vec3(0.0, 0.0, 0.0);
		for(int i = 0; i < gaussMatrixSize * gaussMatrixSize; ++i)
		{
			vec2 point = vec2( mod(i, float(gaussMatrixSize)) - 1, floor(float(i) / float(gaussMatrixSize)) ) / textureSize(vTexture, 0);

			vec3 int_c = texture(
				vTexture, 
				vTex + (point - vec2(gaussMatrixSize / 2f, gaussMatrixSize / 2f)) 
			).xyz;

			finalColor = finalColor + int_c * gaussMatrix[i];


		}
		FragColor = vec4(finalColor, 1.0);	//vec4(tint, 1.0);
	};
	)"));

#pragma region BUILD_SHADER

	std::vector<std::string> log;
	bool success;
	myShader.Build(success, log);
	if (!success) {
		std::cout << "VERTEX ERRORS" << std::endl << log[cyan::ShaderLib::VERTEX] << std::endl;
		std::cout << "FRAGMENT ERRORS" << std::endl << log[cyan::ShaderLib::FRAGMENT] << std::endl;
		std::cout << "LINKING ERRORS" << std::endl << log[cyan::ShaderLib::LINK] << std::endl;
		std::cout << "VALIDATION ERRORS" << std::endl << log[cyan::ShaderLib::VALIDATE] << std::endl;
	}

#pragma endregion
	
	float vertices[] = {
		/* positions		    tex coords */
		 0.5f,  0.5f, 0.0f,		1.0f, 1.0f, /* top right	 */
		 0.5f, -0.5f, 0.0f,		1.0f, 0.0f, /* bottom right  */
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f, /* bottom left	 */
		-0.5f,  0.5f, 0.0f,		0.0f, 1.0f, /* top left		 */
	};
	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,				// first Triangle
		1, 2, 3					// second Triangle
	};

#pragma region GL_BUFFER_SETUP

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

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

#pragma endregion

	myShader.SetActive();
	//vector3 color2 = vector3{ 0.0f, 1.0f, 0.8f };
	//SetUniformV3(myShader, "tint", color2);
	glUniform1fv(myShader.FindUniformLocation("gaussMatrix"), GUASS_MATRIX_SIZE * GUASS_MATRIX_SIZE, gauss_matrix);
	glUniform1i(myShader.FindUniformLocation("gaussMatrixSize"), GUASS_MATRIX_SIZE);

	float frameCount = 0;

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 0.6f);
		glClear(GL_COLOR_BUFFER_BIT);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();

		//vector3 color = vector3{ 0.0f, (float)pow(sin(frameCount / 4), 2), 0.8f};
		//SetUniformV3(myShader, "tint", color);

		++frameCount;
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	return EXIT_SUCCESS;
}