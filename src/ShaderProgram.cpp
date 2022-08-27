#include "ShaderProgram.hpp"

#include "ShaderSource.hpp"
#include "ShaderLoader.hpp"

#include "gl/glew.h"

#include <iostream>
#include <vector>

namespace cyan {
	namespace ShaderLib {
		ShaderProgram::ShaderProgram() {}

		ShaderProgram::ShaderProgram(ShaderSource* vertexShaderSrc, ShaderSource* fragmmentShaderSrc) {
			VertexShaderSource = vertexShaderSrc;
			FragmentShaderSource = fragmmentShaderSrc;
		}

		unsigned int ShaderProgram::Compile(ShaderSource src, ShaderType type, bool& success, std::string& log) {
			std::string finalScript = src.LinkIncludes();
			//std::cout << finalScript << std::endl;
			const char* c_str = finalScript.c_str();

			unsigned int shaderPointer;

			switch (type) {
			case VERTEX:
				shaderPointer = glCreateShader(GL_VERTEX_SHADER);
				break;
			case GEOMETRY:
				shaderPointer = glCreateShader(GL_GEOMETRY_SHADER);
				break;
			case FRAGMENT:
				shaderPointer = glCreateShader(GL_FRAGMENT_SHADER);
				break;
			default:
				shaderPointer = glCreateShader(GL_FRAGMENT_SHADER);
				break;
			}

			glShaderSource(shaderPointer, 1, &c_str, NULL);
			glCompileShader(shaderPointer);

			GLint isCompiled = 0;
			glGetShaderiv(shaderPointer, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				success = false;

				GLint maxLength = 0;
				glGetShaderiv(shaderPointer, GL_INFO_LOG_LENGTH, &maxLength);

				log.resize(maxLength);
				glGetShaderInfoLog(shaderPointer, maxLength, &maxLength, &log[0]);

				glDeleteShader(shaderPointer);
			}
			else {
				success = true;
			}

			return shaderPointer;
		}

		void ShaderProgram::AddVertexShader(ShaderSource* src) {
			VertexShaderSource = src;
		}

		void ShaderProgram::AddGeometryShader(ShaderSource* src) {
			GeometryShaderSource = src;
		}

		void ShaderProgram::AddFragmentShader(ShaderSource* src) {
			FragmentShaderSource = src;
		}

		unsigned int ShaderProgram::Build(bool& success, std::vector<std::string>& log) {
			ShaderID = glCreateProgram();

			log.resize(SHADERTYPESCOUNT);

			GLuint vertexShader = 0;
			GLuint geometryShader = 0;
			GLuint fragmentShader = 0;

			if (VertexShaderSource != nullptr) {
				bool vertexSuccess = false;
				std::string vertexErrors;
				vertexShader = Compile(*VertexShaderSource, VERTEX, vertexSuccess, vertexErrors);
				if (!vertexSuccess) {
					log[VERTEX] = vertexErrors;
					glDeleteProgram(ShaderID);
					ShaderID = GL_ZERO;
					success = false;
					return GL_ZERO;
				}
				else {
					glAttachShader(ShaderID, vertexShader);
				}
			}
			if (GeometryShaderSource != nullptr) {
				bool geometrySuccess = false;
				std::string geometryErrors;
				geometryShader = Compile(*GeometryShaderSource, GEOMETRY, geometrySuccess, geometryErrors);
				if (!geometrySuccess) {
					log[GEOMETRY] = geometryErrors;
					glDeleteProgram(ShaderID);
					ShaderID = GL_ZERO;
					success = false;
					return GL_ZERO;
				}
				else {
					glAttachShader(ShaderID, geometryShader);
				}
			}
			if (FragmentShaderSource != nullptr) {
				bool fragmentSuccess = false;
				std::string fragmentErrors;
				fragmentShader = Compile(*FragmentShaderSource, FRAGMENT, fragmentSuccess, fragmentErrors);
				if (!fragmentSuccess) {
					log[FRAGMENT] = fragmentErrors;
					glDeleteProgram(ShaderID);
					ShaderID = GL_ZERO;
					success = false;
					return GL_ZERO;
				}
				else {
					glAttachShader(ShaderID, fragmentShader);
				}
			}

			//link errors

			glLinkProgram(ShaderID);
			int linkSuccess;
			std::string linkLog;
			glGetProgramiv(ShaderID, GL_LINK_STATUS, &linkSuccess);
			if (!success) {
				glGetProgramInfoLog(ShaderID, 512, NULL, &linkLog[0]);
				log[LINK] = linkLog;
				success = false;
				return 0;
			}

			if (VertexShaderSource != nullptr) {
				glDeleteShader(vertexShader);
			}
			if (GeometryShaderSource != nullptr) {
				glDeleteShader(geometryShader);
			}
			if (FragmentShaderSource != nullptr) {
				glDeleteShader(fragmentShader);
			}

			success = true;

			return ShaderID;
		}
		unsigned int ShaderProgram::Build(bool& success) {
			std::vector<std::string> log;
			return Build(success, log);
		};
		unsigned int ShaderProgram::Build() {
			std::vector<std::string> log;
			bool success;
			return Build(success, log);
		};

		unsigned int ShaderProgram::GetShaderProgram() {
			return ShaderID;
		}

		void ShaderProgram::SetActive(bool status) {
			if (status) {
				glUseProgram(ShaderID);
			}
			else {
				glUseProgram(0);
			}
		}
		void ShaderProgram::SetActive() {
			SetActive(true);
		}
	}
}
