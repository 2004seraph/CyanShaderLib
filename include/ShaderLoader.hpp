#pragma once

#include <map>
#include <initializer_list>

#include "ShaderSource.hpp"

namespace cyan {
	namespace ShaderLib {
		extern std::map<std::string, ShaderSource> LoadedShaders;

		void LoadShaderLibrary(std::map<std::string, ShaderSource> shaderMap);
		void LoadShaderLibrary(std::initializer_list<std::map<std::string, ShaderSource>> shaderMap);
	}
}