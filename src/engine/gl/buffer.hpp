#pragma once

#include "globject.hpp"

namespace gl {

class Buffer {

	public:

		Buffer();
		Buffer(const std::string & name);
		Buffer(const Buffer &) = delete;
		Buffer(Buffer &&) = default;
		Buffer & operator=(const Buffer &) = delete;
		Buffer & operator=(Buffer &&) & = default;
		~Buffer() {}

		operator GLuint() const { return m_obj; }

		GLint getSize() const;

		void createImmutableStorage(unsigned int size, GLbitfield flags, const void * data = nullptr);
		void createMutableStorage(unsigned int size, GLbitfield usage, const void * data = nullptr);
		void setData(unsigned int offset, unsigned int size, const void * data);

		bool isMapped() const;
		void * map(unsigned int offset, unsigned int size, GLbitfield access);
		bool unmap() const;

	private:

		GLObject m_obj;

};

static_assert(std::is_nothrow_move_constructible<Buffer>(), "Should be noexcept MoveConstructible");
static_assert(!std::is_copy_constructible<Buffer>(), "Should not be CopyConstructible");
static_assert(std::is_nothrow_move_assignable<Buffer>(), "Should be noexcept MoveAssignable");
static_assert(!std::is_copy_assignable<Buffer>(), "Should not be CopyAssignable");

} // namespace gl
