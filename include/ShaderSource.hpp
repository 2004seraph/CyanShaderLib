#pragma once

#include <string>
#include <list>

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
	}
}
