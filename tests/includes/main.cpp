#include <iostream>

#include "ShaderSource.hpp"
#include "ShaderLoader.hpp"

int main() {
	
	std::map<std::string, cyan::ShaderLib::ShaderSource> myShaddyLib = {
		{"net.cyanseraph.glsl.random", cyan::ShaderLib::ShaderSource(
R"(
The_magic
)")},
		{"net.cyanseraph.glsl.constants", cyan::ShaderLib::ShaderSource(std::list<std::string>{"net.cyanseraph.glsl.awe.core"},
R"(
#define PI 3.14158
)")},
		{"net.cyanseraph.glsl.awe.core", cyan::ShaderLib::ShaderSource(
R"(
awe-stuff
)")},
	};

	std::map<std::string, cyan::ShaderLib::ShaderSource> myShaddyLibExt = {
	{"net.cyanseraph.glsl.ext", cyan::ShaderLib::ShaderSource(std::list<std::string>{"net.cyanseraph.glsl.constants"},
R"(
external mister library
)")}
	};

	cyan::ShaderLib::LoadShaderLibrary({ myShaddyLib, myShaddyLibExt });

	cyan::ShaderLib::ShaderSource cubeShader(std::list<std::string>{"net.cyanseraph.glsl.random", "net.cyanseraph.glsl.ext"}, R"(Apples are cool)");

	std::cout << cubeShader.LinkIncludes() << std::endl;
	
	return 0;
}