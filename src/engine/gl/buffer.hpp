#pragma once

#include "globject.hpp"

namespace gl {

class Buffer {

	public:

		Buffer();
		Buffer(const Buffer &) = delete;
		Buffer(Buffer &&) = default;
		Buffer & operator=(const Buffer &) = delete;
		Buffer & operator=(Buffer &&) = default;
		~Buffer() {}

		operator GLuint() const { return m_obj; }

		void bind(GLenum) const;
		void unbind() const;

		unsigned int getSize() const;

		void createMutableStorage(unsigned int, GLbitfield, const void * = nullptr);
		void setData(unsigned int, unsigned int, const void *);

		bool isMapped() const;
		void * map(unsigned int, unsigned int, GLbitfield);
		bool unmap() const;

	private:

		GLObject m_obj;
		mutable GLenum m_target;

};

} // namespace gl
