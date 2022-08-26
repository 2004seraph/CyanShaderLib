#include <iostream>

#include "ShaderObject.hpp"

int main() {
	/*
	std::map<std::string, cyan::ShaderLib::ShaderSource> myShaddyLib = {
		{"net.cyanseraph.glsl.random", cyan::ShaderLib::ShaderSource(cyan::ShaderLib::VERTEX,
R"(
The_magic
)")},
		{"net.cyanseraph.glsl.constants", cyan::ShaderLib::ShaderSource(cyan::ShaderLib::VERTEX, std::list<std::string>{"net.cyanseraph.glsl.awe.core"},
R"(
#define PI 3.14158
)")},
		{"net.cyanseraph.glsl.awe.core", cyan::ShaderLib::ShaderSource(cyan::ShaderLib::VERTEX,
R"(
awe-stuff
)")},
	};

	std::map<std::string, cyan::ShaderLib::ShaderSource> myShaddyLibExt = {
	{"net.cyanseraph.glsl.ext", cyan::ShaderLib::ShaderSource(cyan::ShaderLib::VERTEX, std::list<std::string>{"net.cyanseraph.glsl.constants"},
R"(
external mister library
)")}
	};

	cyan::ShaderLib::LoadShaderLibrary({ myShaddyLib, myShaddyLibExt });

	cyan::ShaderLib::ShaderSource cubeShader(cyan::ShaderLib::VERTEX, std::list<std::string>{"net.cyanseraph.glsl.random", "net.cyanseraph.glsl.ext"}, R"(Apples are cool)");

	std::cout << cubeShader.LinkIncludes() << std::endl;
	*/
	return 0;
}