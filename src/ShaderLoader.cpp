#include "ShaderLoader.hpp"

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
	}
}