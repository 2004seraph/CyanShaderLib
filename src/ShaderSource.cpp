#include "CyanShaderLib/ShaderSource.hpp"
#include "CyanShaderLib/ShaderLoader.hpp"

#include <iostream>

namespace cyan {
	namespace ShaderLib {
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
	}
}
