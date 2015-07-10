#pragma once

#include <iostream>
#include <string>
#include <glm/glm.hpp>

#define DEB core::Log::log("DEBUG: " + std::string(__FILE__) + " " + __FUNCTION__ + " " + std::to_string(__LINE__));
#define LOG(...) core::Log::log(__VA_ARGS__)
#define LOG_WARNING(...) core::Log::logWarning(__VA_ARGS__)
#define LOG_ERROR(...) core::Log::logError(__VA_ARGS__)
#define LOG_ASSERT(assertion, output) core::Log::logAssert(assertion, output)

namespace core {

class Log {

	public:

		template<typename T>
		static void logError(T && output, const bool exiting) {
			std::cerr << "ERROR: " << std::forward<T>(output) << std::endl;
			if (exiting) {
				std::cerr << "Exiting ..." << std::endl;
				exit(EXIT_FAILURE);
			}
		}

		template<typename T>
		static void logError(T && output) {
			logError(std::forward<T>(output), true);
		}

		template <typename T, typename ... Args>
		static void logError(T && output, Args && ... args) {
			logError(std::forward<T>(output), false);
			logError(std::forward<Args>(args) ...);
		}

		template<typename T>
		static void logWarning(T && output) {
			std::cout << "WARNING: ";
			log(std::forward<T>(output));
		}

		template <typename T, typename ... Args>
		static void logWarning(T && output, Args && ... args) {
			logWarning(std::forward<T>(output));
			logWarning(std::forward<Args>(args) ...);
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
		static void logAssert(bool assertion, const std::string & output);

	protected:

		static Log * s_instance;

		friend class Cleanup;
		class Cleanup {
			public:
				~Cleanup();
		};

	private:

		Log() noexcept {}
		Log(const Log &) = delete;
		Log(Log &&) = delete;
		Log & operator=(const Log &) = delete;
		Log & operator=(Log &&) = delete;
		virtual ~Log() {}

		auto & instance() noexcept;

};

static_assert(!std::is_move_constructible<Log>(), "Should not be MoveConstructible");
static_assert(!std::is_copy_constructible<Log>(), "Should not be CopyConstructible");
static_assert(!std::is_move_assignable<Log>(), "Should not be MoveAssignable");
static_assert(!std::is_copy_assignable<Log>(), "Should not be CopyAssignable");

} // namespace core
