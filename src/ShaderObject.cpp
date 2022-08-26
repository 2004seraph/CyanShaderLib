#include "ShaderObject.hpp"

#include "gl/glew.h"

#include <iostream>
#include <vector>

namespace cyan {
	namespace ShaderLib {
		std::map<std::string, ShaderSource> LoadedShaders = {};
		void LoadShaderLibrary(std::map<std::string, ShaderSource> shaderMap) {
			for (auto it = shaderMap.begin(); it != shaderMap.end(); ++it) LoadedShaders[it->first] = it->second;
		}
		void LoadShaderLibrary(std::initializer_list<std::map<std::string, ShaderSource>> shaderMap) {
			for (auto elem : shaderMap) {
				LoadShaderLibrary(elem);
			}
		};



		ShaderSource::ShaderSource() {
			source = "//no script!";
			preprocessorDirectives = "";
		}
		ShaderSource::ShaderSource(std::string src) {
			source = src;
			preprocessorDirectives = "";
		};
		ShaderSource::ShaderSource(std::list<std::string> dependenciesList, std::string src) {
			source = src;
			dependencies = dependenciesList;
		};
		ShaderSource::ShaderSource(std::string preprocessor, std::string src) {
			source = src;
			preprocessorDirectives = preprocessor;
		}
		ShaderSource::ShaderSource(std::string preprocessor, std::list<std::string> dependenciesList, std::string src) {
			source = src;
			dependencies = dependenciesList;
			preprocessorDirectives = preprocessor;
		}

		std::string ShaderSource::LinkIncludes(std::list<std::string>& notFound) {
			std::string main = preprocessorDirectives;

			for (auto it = dependencies.begin(); it != dependencies.end(); ++it) {
				std::string searchLib(it->c_str());

				if (LoadedShaders.find(searchLib) != LoadedShaders.end()) {
					main += LoadedShaders[searchLib].LinkIncludes(notFound) + "\n";
				}
				else {
					notFound.push_back("" + searchLib);//state what shader it was not found in
				}
			}

			main += source;

			return main;
		}
		std::string ShaderSource::LinkIncludes() {
			std::string main = preprocessorDirectives;

			for (auto it = dependencies.begin(); it != dependencies.end(); ++it) {
				std::string searchLib(it->c_str());

				if (LoadedShaders.find(searchLib) != LoadedShaders.end()) {
					main += LoadedShaders[searchLib].LinkIncludes() + "\n";
				}
				else {
					std::cout << "Shader library not found: " << searchLib << std::endl;
				}
			}

			main += source;

			return main;
		}

		void ShaderSource::SetSource(std::string src) {
			source = src;
		};

		void ShaderSource::SetDependencies(std::list<std::string> dependenciesList) {
			dependencies = dependenciesList;
		};

		void ShaderSource::SetPreprocessor(std::string pre) {
			preprocessorDirectives = pre;
		}


		
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

		unsigned int ShaderProgram::Build(std::list<std::string>& log) {
			ShaderID = glCreateProgram();

			GLuint vertexShader = 0;
			GLuint geometryShader = 0;
			GLuint fragmentShader = 0;

			if (VertexShaderSource != nullptr) {
				bool vertexSuccess = false;
				std::string vertexErrors;
				vertexShader = Compile(*VertexShaderSource, VERTEX, vertexSuccess, vertexErrors);
				if (!vertexSuccess) {
					log.push_back(vertexErrors);
					glDeleteProgram(ShaderID);
					ShaderID = GL_ZERO;
					return 0;
				}

				glAttachShader(ShaderID, vertexShader);
			}
			if (GeometryShaderSource != nullptr) {
				bool geometrySuccess = false;
				std::string geometryErrors;
				geometryShader = Compile(*GeometryShaderSource, GEOMETRY, geometrySuccess, geometryErrors);
				if (!geometrySuccess) {
					log.push_back(geometryErrors);
					glDeleteProgram(ShaderID);
					ShaderID = GL_ZERO;
					return 0;
				}

				glAttachShader(ShaderID, geometryShader);
			}
			if (FragmentShaderSource != nullptr) {
				bool fragmentSuccess = false;
				std::string fragmentErrors;
				fragmentShader = Compile(*FragmentShaderSource, FRAGMENT, fragmentSuccess, fragmentErrors);
				if (!fragmentSuccess) {
					log.push_back(fragmentErrors);
					glDeleteProgram(ShaderID);
					ShaderID = GL_ZERO;
					return 0;
				}

				glAttachShader(ShaderID, fragmentShader);
			}

			//link errors

			glLinkProgram(ShaderID);

			if (VertexShaderSource != nullptr) {
				glDeleteShader(vertexShader);
			}
			if (GeometryShaderSource != nullptr) {
				glDeleteShader(geometryShader);
			}
			if (FragmentShaderSource != nullptr) {
				glDeleteShader(fragmentShader);
			}

			return ShaderID;
		}

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