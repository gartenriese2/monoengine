#pragma once

#include "globject.hpp"

namespace gl {

class Buffer {

	public:

		Buffer();
		Buffer(const std::string &);
		Buffer(const Buffer &) = delete;
		Buffer(Buffer &&) = default;
		Buffer & operator=(const Buffer &) = delete;
		Buffer & operator=(Buffer &&) = default;
		~Buffer() {}

		operator GLuint() const { return m_obj; }

		void bind(GLenum) const;
		void unbind(GLenum) const;

		unsigned int getSize(GLenum) const;

		void createMutableStorage(GLenum, unsigned int, GLbitfield, const void * = nullptr);
		void setData(GLenum, unsigned int, unsigned int, const void *);

		bool isMapped(GLenum) const;
		void * map(GLenum, unsigned int, unsigned int, GLbitfield);
		bool unmap(GLenum) const;

	private:

		GLObject m_obj;

};

} // namespace gl
