#pragma once

#include <string>
#include <list>
#include <vector>
#include <map>

#include "gl/glew.h"

#include "ShaderSource.hpp"

namespace cyan {
	namespace ShaderLib {
		enum ShaderType {
			VERTEX = 0,
			GEOMETRY,
			FRAGMENT,
			SHADERCOUNT,
			LINK,
			VALIDATE,
			FULLTYPESCOUNT
		};

		class ShaderProgram {
			ShaderSource* VertexShaderSource = nullptr;
			//tessallation control+eval shader
			ShaderSource* GeometryShaderSource = nullptr;
			ShaderSource* FragmentShaderSource = nullptr;

			std::map<std::string, GLuint> uniformCache;

			unsigned int Compile(ShaderSource src, ShaderType type, bool& success, std::string& log);

		protected:
			unsigned int ShaderID = 0;

		public:
			ShaderProgram();
			ShaderProgram(ShaderSource* vertexShaderSrc, ShaderSource* fragmmentShaderSrc);

			void AddVertexShader(ShaderSource* src);

			void AddGeometryShader(ShaderSource* src);

			void AddFragmentShader(ShaderSource* src);

			/// <summary>
			/// Builds the shader program, allows logging.
			/// </summary>
			/// <param name="log"></param>
			/// <returns>a handle to the underlying openGL shader program</returns>
			unsigned int Build(bool& success, std::vector<std::string>& log);
			unsigned int Build(bool& success);
			unsigned int Build();

			/// <summary>
			/// Returns a handle to the underlying openGL shader program
			/// </summary>
			/// <returns></returns>
			unsigned int GetShaderProgram();

			void SetActive();
			void SetActive(bool status);

			GLuint FindUniformLocation(std::string uniform);

			//void SetBool(const std::string& name, bool value) const;
			//void SetInt(const std::string& name, int value) const;
		};
		
	}
}