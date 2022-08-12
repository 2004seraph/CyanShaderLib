#include <string>
#include <map>

#include "ShaderObject.hpp"

namespace cyan {
	namespace ShaderLib {
		class ShaderTree {
		public:
			static class ShaderDirectory {
				std::map<std::string, ShaderDirectory> childrenDirectories;
				std::map<std::string, ShaderObject> childrenFiles;

			public:
				//nothing
				ShaderDirectory() {
					childrenDirectories = std::map<std::string, ShaderDirectory>();
					childrenFiles = std::map<std::string, ShaderObject>();
				}

				//dirs and files
				ShaderDirectory(std::map<std::string, ShaderDirectory> dirs, std::map<std::string, ShaderObject> files) {
					childrenDirectories = dirs;
					childrenFiles = files;
				}

				//no files, just dirs
				ShaderDirectory(std::map<std::string, ShaderDirectory> dirs) {
					childrenDirectories = dirs;

					childrenFiles = std::map<std::string, ShaderObject>();
				}

				//no dirs, just files
				ShaderDirectory(std::map<std::string, ShaderObject> files) {
					childrenDirectories = std::map<std::string, ShaderDirectory>();

					childrenFiles = files;
				}
			};
		};
	}
}