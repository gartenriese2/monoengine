#ifndef _DEBUG_
#define _DEBUG_

#include <iostream>
#include <glm/glm.hpp>

#define DEB engine::Debug::log("DEBUG: " + std::string(__FILE__) + " " + __FUNCTION__ + " " + std::to_string(__LINE__));
#define LOG(...) engine::Debug::log(__VA_ARGS__)
#define LOG_WARNING(...) engine::Debug::logWarning(__VA_ARGS__)
#define LOG_ERROR(...) engine::Debug::logError(__VA_ARGS__)

namespace engine {

class Debug {

	public:

		template<typename T>
		static void logError(const T & output, const bool exiting) {
			std::cerr << "ERROR: " << output << std::endl;
			if (exiting) {
				std::cerr << "Exiting ..." << std::endl;
				exit(EXIT_FAILURE);
			}
		}

		template<typename T>
		static void logError(const T & output) {
			logError(output, true);
		}

		template <typename T, typename ... Args>
		static void logError(const T & output, Args ... args) {
			logError(output, false);
			logError(args ...);
		}

		template<typename T>
		static void logWarning(const T & output) {
			std::cout << "WARNING: ";
			log(output);
		}

		template <typename T, typename ... Args>
		static void logWarning(const T & output, Args ... args) {
			logWarning(output);
			logWarning(args ...);
		}

		template<typename T>
		static void log(const T & output) {
			std::cout << output << std::endl;
		}

		template <typename T, typename ... Args>
		static void log(const T & output, Args ... args) {
			log(output);
			log(args ...);
		}

		static void log(const glm::vec3 & vec) {
			log("vec3: " + std::to_string(vec.x) + "|" + std::to_string(vec.y) + "|" + std::to_string(vec.z));
		}

		static void log(const glm::vec4 & vec) {
			log("vec3: " + std::to_string(vec.x) + "|" + std::to_string(vec.y) + "|" + std::to_string(vec.z) + "|" + std::to_string(vec.w));
		}

		static void log(const glm::mat4 & mat) {
			log("mat4:\n"
				+ std::to_string(mat[0][0]) + "\t|" + std::to_string(mat[0][1]) + "\t|" + std::to_string(mat[0][2]) + "\t|"
				+ std::to_string(mat[0][3]) + "\n" + std::to_string(mat[1][0]) + "\t|" + std::to_string(mat[1][1]) + "\t|"
				+ std::to_string(mat[1][2]) + "\t|" + std::to_string(mat[1][3]) + "\n" + std::to_string(mat[2][0]) + "\t|"
				+ std::to_string(mat[2][1]) + "\t|" + std::to_string(mat[2][2]) + "\t|" + std::to_string(mat[2][3]) + "\n"
				+ std::to_string(mat[3][0]) + "\t|" + std::to_string(mat[3][1]) + "\t|" + std::to_string(mat[3][2]) + "\t|"
				+ std::to_string(mat[3][3]));
		}

		static void logGL();

	protected:

		static Debug * s_instance;

		friend class Cleanup;
		class Cleanup {
			public:
				~Cleanup();
		};

	private:

		Debug() {}
		Debug(const Debug &) = delete;
		Debug(Debug &&) = delete;
		Debug & operator=(const Debug &) = delete;
		Debug & operator=(Debug &&) = delete;
		virtual ~Debug() {}

		Debug & instance();

};

} // namespace engine

#endif // _DEBUG_
