#include <cstdlib>
#include <vector>
#include <algorithm>

#include "engine/engine.hpp"
#include "engine/gl/shader.hpp"
#include "engine/gl/program.hpp"
#include "engine/gl/buffer.hpp"
#include "engine/gl/vertexarray.hpp"

void vertexPullingDemo() {

    struct Segment
    {
        Segment(GLintptr b, GLintptr p, GLintptr e)
          : begin{b}, ptr{p}, end{e} {}
        GLintptr begin;
        GLintptr ptr;
        GLintptr end;
    };

    std::vector<Segment>    m_freelist;
    std::vector<Segment>    m_used;

    auto align = [](GLsizei alignment, GLsizeiptr size, GLintptr& ptr, GLsizeiptr space) {
        bool success = false;
        if (size <= space) {
            GLintptr ptr2 = ((ptr + alignment - 1) / alignment) * alignment;
            GLsizeiptr diff = static_cast<GLsizeiptr>(ptr2 - ptr);
            if (diff <= space - size) {
                ptr = ptr2;
                space -= diff;
                success = true;
            }
        }
        return success;
    };

    auto alloc = [&](const GLsizeiptr size, const GLsizei alignment) {
        GLintptr offset = 0;
        std::vector<Segment>::iterator it;
        const auto it_end = m_freelist.end();
        for (it = m_freelist.begin(); it != it_end; ++it) {
            GLsizeiptr space = static_cast<GLsizeiptr>(it->end - it->begin);

            GLintptr ptr = it->begin;
            // first fit
            if (align(alignment, size, ptr, space)) {
                offset = ptr;
                Segment used(it->begin, ptr, ptr + size);

                it->begin = used.end;
                it->ptr = used.end;
                if (it->begin == it->end) {
                    m_freelist.erase(it);
                }

                auto pos = std::lower_bound(m_used.begin(), m_used.end(),
                        used, [] (const Segment& s0, const Segment& s1) -> bool
                        {
                            return s0.begin < s1.begin;
                        });
                m_used.insert(pos, std::move(used));

                break;
            }
        }
        if (it == it_end)
            abort();

        return offset;
    };

    // instance manager stuff
    gl::Buffer instanceBuffer("instance buffer");
    constexpr int MAX_NUM_INSTANCES = 12000;
    instanceBuffer.createStorage(MAX_NUM_INSTANCES, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT);
    //auto instanceDataPtr = instanceBuffer.getDataPointer(MAX_NUM_INSTANCES,
    //          GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

    // mesh manager stuff
    gl::Buffer meshDataBuffer("vertex shader storage buffer");
    meshDataBuffer.createStorage(268435456, GL_MAP_WRITE_BIT | GL_DYNAMIC_STORAGE_BIT);
    m_freelist.emplace_back(0, 0, 268435456);
    gl::Buffer meshPoolBuffer("mesh pool");
    constexpr int MAX_NUM_MESHES = 1200;
    meshPoolBuffer.createStorage(MAX_NUM_MESHES, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT);
    //auto meshPoolDataPtr = meshPoolBuffer.getDataPointer(MAX_NUM_MESHES,
    //          GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

    // initVAO
    GLsizei stride = static_cast<GLsizei>(3 * sizeof(float));
    gl::VertexArray vao("vao");
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, meshDataBuffer);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, nullptr);
    auto tmpOffset = 3 * sizeof(float);
    if (static_cast<GLsizei>(tmpOffset) != stride) {
        LOG("static_cast<GLsizei>(tmpOffset) != stride");
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshDataBuffer);
    glBindVertexArray(0);

    // add mesh
    GLenum index_type;
    GLsizeiptr per_index_size;
    per_index_size = sizeof(GLubyte);
    index_type = GL_UNSIGNED_BYTE;
    GLsizeiptr per_vertex_size = 3 * sizeof(float);

    const GLsizeiptr vertices_size = per_vertex_size * 3;
    const GLsizeiptr indices_size = per_index_size * 3;
    const GLsizeiptr size = vertices_size + indices_size;

    const auto offset = alloc(size, static_cast<GLsizei>(vertices_size));
    if (offset % vertices_size != 0) {
        LOG("offset % vertices_size != 0");
    }

    // upload to gpu
    void * ptr = glMapNamedBufferRange(meshDataBuffer,
                offset, size, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_RANGE_BIT);
    float* data = static_cast<float*>(ptr);

    *data++ = -1.f;
    *data++ = -1.f;
    *data++ = 0.f;
    *data++ = 1.f;
    *data++ = -1.f;
    *data++ = 0.f;
    *data++ = 0.f;
    *data++ = 1.f;
    *data++ = 0.f;

    GLubyte* indices = reinterpret_cast<GLubyte*>(data);
    *indices = static_cast<GLubyte>(0);
    indices += per_index_size;
    *indices = static_cast<GLubyte>(1);
    indices += per_index_size;
    *indices = static_cast<GLubyte>(2);
    indices += per_index_size;

    if (indices - static_cast<GLubyte*>(ptr) != size) {
        LOG("indices - static_cast<GLubyte*>(ptr) != size");
    }
    glUnmapNamedBuffer(meshDataBuffer);

    // Add MeshStruct on GPU
    //const GLintptr mesh_offset = m_mesh_pool.alloc();
    //const GLuint mesh_index = m_mesh_pool.offsetToIndex(mesh_offset);
    //auto* mesh_data = m_mesh_pool.offsetToPointer(mesh_offset);

    gl::Shader vert("shader/test/triangle.vert", "triangle_vert");
    gl::Shader frag("shader/test/red.frag", "triangle_frag");
    gl::Program prog("test prog");
    prog.attachShader(vert);
    prog.attachShader(frag);
    prog.use();

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);

}

void instancingDemo() {

    // shader
    gl::Shader vert("shader/test/instancedraw.vert", "instance_vert");
    gl::Shader frag("shader/test/red.frag", "red_frag");
    gl::Program prog("basic prog");
    prog.attachShader(vert);
    prog.attachShader(frag);

}

void multidrawDemo() {

    // shader
    gl::Shader vert("shader/test/multidraw.vert", "multi_vert");
    gl::Shader frag("shader/test/red.frag", "red_frag");
    gl::Program prog("basic prog");
    prog.attachShader(vert);
    prog.attachShader(frag);

}

void basicDrawDemo(engine::Engine & e) {

    // shader
    gl::Shader vert("shader/test/basicdraw.vert", "basic_vert");
    gl::Shader frag("shader/test/red.frag", "red_frag");
    gl::Program prog("basic prog");
    prog.attachShader(vert);
    prog.attachShader(frag);

    // vbo
    std::vector<GLfloat> tri {
        -1.f, -1.f, 0.f,
        1.f, -1.f, 0.f,
        0.f, 1.f, 0.f
    };
    gl::Buffer vbo("Basic Draw VBO");
    vbo.createStorage(9 * sizeof(float), 0, tri.data());

    // vao
    gl::VertexArray vao("Basic Draw VAO");
    glBindVertexArray(vao);
    vao.bindVertexBuffer(vbo, 0, 0, 3 * sizeof(float), 0);
    vao.bindVertexFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
    prog.use();
    while (e.render()) {
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }
}

int main() {

	const std::string title("monoEngine");
	const auto width = 1280u;
	const auto height = 720u;
	engine::Engine e(width, height, title, true);

	// DEMO
	// auto time = glfwGetTime();

    // vertexPullingDemo();
    // instancingDemo();
    // multidrawDemo();
    basicDrawDemo(e);

	// LOG("time: " + std::to_string(glfwGetTime() - time) + "s");

	//while (e.render());

	return EXIT_SUCCESS;

}
