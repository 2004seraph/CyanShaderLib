#include <iostream>

#include "ShaderTree.hpp"

namespace cs = cyan::ShaderLib;
namespace cst = cyan::ShaderLib;
	
int main(int argc, char* argv[]) {
	std::cout << "hello world" << std::endl;

	cs::ShaderTree::ShaderDirectory root(std::map<std::string, 
		cs::ShaderTree::ShaderDirectory>{
			//folders
			{"AWE", 
				cs::ShaderTree::ShaderDirectory(std::map<std::string, cs::ShaderTree::ShaderDirectory>{
					//folders
					{"Types", cs::ShaderTree::ShaderDirectory(std::map<std::string, cs::ShaderObject>{
							//files
							{"Light", cs::ShaderObject("")}
					})}
					//files
				}
			)},
			{"Lythe", cs::ShaderTree::ShaderDirectory()}
		}
	);

	return EXIT_SUCCESS;
}