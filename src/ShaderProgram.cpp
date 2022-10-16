#include "CyanShaderLib/ShaderProgram.hpp"
#include "CyanShaderLib/ShaderSource.hpp"
#include "CyanShaderLib/ShaderLoader.hpp"

#include "gl/glew.h"

#include <iostream>
#include <vector>

#define SHADERFAIL glDeleteProgram(ShaderID);ShaderID=GL_ZERO;success=false;

namespace cyan {
	namespace ShaderLib {
		ShaderProgram::ShaderProgram() {}

		ShaderProgram::ShaderProgram(ShaderSource* vertexShaderSrc, ShaderSource* fragmmentShaderSrc) {
			VertexShaderSource = vertexShaderSrc;
			FragmentShaderSource = fragmmentShaderSrc;
		}

		unsigned int ShaderProgram::FindUniformLocation(std::string uniform) {
			std::map<std::string, unsigned int>::iterator it = uniformCache.find(uniform);

			GLuint location;

			if (it != uniformCache.end()) {
				return (it->second);
			}
			else {
				location = glGetUniformLocation(ShaderID, uniform.c_str());
				uniformCache.insert({ uniform, location });
			}

			return location;
		};

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
				std::cout << "Something has went very wrong" << std::endl;
				return 0;
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

			GLuint shaderPrograms[SHADERCOUNT];
			log.resize(FULLTYPESCOUNT);
			bool GLCompleSuccess;

			std::string errorLog;

			if (VertexShaderSource != nullptr) {
				shaderPrograms[VERTEX] = Compile(*VertexShaderSource, VERTEX, GLCompleSuccess, errorLog);
				if (!GLCompleSuccess) {
					log[VERTEX] = errorLog;
					SHADERFAIL
					return GL_ZERO;
				}
				else {
					glAttachShader(ShaderID, shaderPrograms[VERTEX]);
				}
			}
			if (GeometryShaderSource != nullptr) {
				shaderPrograms[GEOMETRY] = Compile(*GeometryShaderSource, GEOMETRY, GLCompleSuccess, errorLog);
				if (!GLCompleSuccess) {
					log[GEOMETRY] = errorLog;
					SHADERFAIL
					return GL_ZERO;
				}
				else {
					glAttachShader(ShaderID, shaderPrograms[GEOMETRY]);
				}
			}
			if (FragmentShaderSource != nullptr) {
				shaderPrograms[FRAGMENT] = Compile(*FragmentShaderSource, FRAGMENT, GLCompleSuccess, errorLog);
				if (!GLCompleSuccess) {
					log[FRAGMENT] = errorLog;
					SHADERFAIL
					return GL_ZERO;
				}
				else {
					glAttachShader(ShaderID, shaderPrograms[FRAGMENT]);
				}
			}

			GLint length;
			glGetProgramiv(ShaderID, GL_INFO_LOG_LENGTH, &length);
			int linkAndValidate;

			//link errors
			glLinkProgram(ShaderID);
			glGetProgramiv(ShaderID, GL_LINK_STATUS, &linkAndValidate);
			if (linkAndValidate != GL_TRUE) {
				glGetProgramInfoLog(ShaderID, length, NULL, &errorLog[0]);
				log[LINK] = errorLog;
				SHADERFAIL
				return GL_ZERO;
			}

			//validation
			glValidateProgram(ShaderID);
			glGetProgramiv(ShaderID, GL_VALIDATE_STATUS, &linkAndValidate);
			if (linkAndValidate != GL_TRUE) {
				glGetProgramInfoLog(ShaderID, length, &length, &errorLog[0]);
				log[VALIDATE] = errorLog;
				SHADERFAIL
				return GL_ZERO;
			}

			if (VertexShaderSource != nullptr) {
				glDeleteShader(shaderPrograms[VERTEX]);
			}
			if (GeometryShaderSource != nullptr) {
				glDeleteShader(shaderPrograms[GEOMETRY]);
			}
			if (FragmentShaderSource != nullptr) {
				glDeleteShader(shaderPrograms[FRAGMENT]);
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
