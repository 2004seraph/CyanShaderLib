#include <string>
#include <list>

namespace cyan {
	namespace ShaderLib {
		class ShaderObject {
			std::list<std::string> includePaths;

			std::string source;

		public:
			ShaderObject() {
			}

			ShaderObject(std::string src) {
				source = src;
			}

			std::string GetSource() {
				return source;
			}

			void LinkIncludes() {

			}
		};
	}
}