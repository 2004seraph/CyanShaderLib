#pragma once

#include <string>
#include <list>
#include <map>
#include <initializer_list>

namespace cyan {
	namespace ShaderLib {
		class ShaderSource {
		public:
			ShaderSource();
			/// <summary>
			/// For library scripts where the version must be left up to the user
			/// </summary>
			/// <param name="src"></param>
			ShaderSource(std::string src);
			ShaderSource(std::list<std::string> dependenciesList, std::string src);
			/// <summary>
			/// For simple GLSL programs
			/// </summary>
			/// <param name="preprocessor"></param>
			/// <param name="src"></param>
			ShaderSource(std::string preprocessor, std::string src);
			/// <summary>
			/// For GLSL programs requiring libaries
			/// </summary>
			/// <param name="preprocessor"></param>
			/// <param name="dependenciesList"></param>
			/// <param name="src"></param>
			ShaderSource(std::string preprocessor, std::list<std::string> dependenciesList, std::string src);

			std::string LinkIncludes(std::list<std::string>& notFound);
			std::string LinkIncludes();

			void SetSource(std::string src);

			void SetDependencies(std::list<std::string> dependenciesList);

			void SetPreprocessor(std::string pre);

			//std::string GetSource();

		protected:
			std::list<std::string> dependencies = {};

			std::string source;

			std::string preprocessorDirectives;
		};

		class ShaderProgram {
			ShaderSource* VertexShaderSource = nullptr;
			//tessallation control+eval shader
			ShaderSource* GeometryShaderSource = nullptr;
			ShaderSource* FragmentShaderSource = nullptr;

			unsigned int ShaderID = 0;

			enum ShaderType {
				VERTEX = 0,
				GEOMETRY,
				FRAGMENT
			};

			unsigned int Compile(ShaderSource src, ShaderType type, bool& success, std::string& log);

		public:
			ShaderProgram();
			ShaderProgram(ShaderSource* vertexShaderSrc, ShaderSource* fragmmentShaderSrc);

			void AddVertexShader(ShaderSource* src);

			void AddGeometryShader(ShaderSource* src);

			void AddFragmentShader(ShaderSource* src);

			unsigned int Build(std::list<std::string>& log);

			unsigned int GetShaderProgram();

			void SetActive();
			void SetActive(bool status);
		};

		void LoadShaderLibrary(std::map<std::string, ShaderSource> shaderMap);
		void LoadShaderLibrary(std::initializer_list<std::map<std::string, ShaderSource>> shaderMap);
	}
}