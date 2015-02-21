#include "gui.hpp"

#include "gl/buffer.hpp"
#include "gl/vertexarray.hpp"
#include "gl/program.hpp"
#include "gl/texture.hpp"
#include "gl/shader.hpp"

#include "extern/imgui.h"

#include "core/log.hpp"

#include <memory>

namespace {

constexpr auto k_fontTexUnit = 0;
static auto s_maxVBOSize = 20000u;

struct {
	std::unique_ptr<gl::Texture> fontTex;
	std::unique_ptr<gl::Buffer> vbo;
	std::unique_ptr<gl::VertexArray> vao;
	std::unique_ptr<gl::Program> prog;
} GuiData;

void renderDrawLists(ImDrawList ** const cmd_lists, const int cmd_lists_count) {

	if (cmd_lists_count == 0) {
		return;
	}

	GuiData.prog->use();

	// Setup render state: alpha-blending enabled, no face culling, no depth testing, scissor enabled
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_SCISSOR_TEST);

	// Setup texture
	GuiData.fontTex->bindToTextureUnit(k_fontTexUnit);
	(*GuiData.prog)["Texture"] = k_fontTexUnit;

	// Setup orthogonal projection matrix
	const auto width = ImGui::GetIO().DisplaySize.x;
	const auto height = ImGui::GetIO().DisplaySize.y;
	const auto ortho_projection = glm::mat4{
		{ 2.0f/width, 0.0f, 0.0f, 0.0f },
		{ 0.0f, 2.0f/-height, 0.0f, 0.0f },
		{ 0.0f, 0.0f, -1.0f, 0.0f },
		{ -1.0f, 1.0f, 0.0f, 1.0f },
	};
	(*GuiData.prog)["ProjMtx"] = ortho_projection;

	// Grow our buffer according to what we need
	size_t total_vtx_count = 0;
	for (auto n = 0; n < cmd_lists_count; ++n) {
		total_vtx_count += cmd_lists[n]->vtx_buffer.size();
	}
	auto neededBufferSize = total_vtx_count * sizeof(ImDrawVert);
	if (neededBufferSize > s_maxVBOSize) {
		s_maxVBOSize = static_cast<unsigned int>(neededBufferSize) + 5000; // Grow buffer
		GuiData.vbo->createMutableStorage(s_maxVBOSize, GL_STREAM_DRAW);
	}

	// Copy and convert all vertices into a single contiguous buffer
	unsigned char * buffer_data = static_cast<unsigned char *>(GuiData.vbo->map(0, GuiData.vbo->getSize(), GL_MAP_WRITE_BIT));
	if (!buffer_data) {
		return;
	}
	for (int n = 0; n < cmd_lists_count; n++) {
		const ImDrawList* cmd_list = cmd_lists[n];
		std::memcpy(buffer_data, &cmd_list->vtx_buffer[0], cmd_list->vtx_buffer.size() * sizeof(ImDrawVert));
		buffer_data += cmd_list->vtx_buffer.size() * sizeof(ImDrawVert);
	}
	GuiData.vbo->unmap();

	glBindVertexArray(*GuiData.vao);
	int cmd_offset = 0;
	for (int n = 0; n < cmd_lists_count; n++) {
		const ImDrawList* cmd_list = cmd_lists[n];
		int vtx_offset = cmd_offset;
		for (const auto& pcmd : cmd_list->commands) {
			glScissor((int)pcmd.clip_rect.x,
					  (int)(height - pcmd.clip_rect.w),
					  (int)(pcmd.clip_rect.z - pcmd.clip_rect.x),
					  (int)(pcmd.clip_rect.w - pcmd.clip_rect.y));
			glDrawArrays(GL_TRIANGLES, vtx_offset, static_cast<GLsizei>(pcmd.vtx_count));
			vtx_offset += pcmd.vtx_count;
		}
		cmd_offset = vtx_offset;
	}

	// Restore modified state
	glBindVertexArray(0);
	glUseProgram(0);
	glDisable(GL_SCISSOR_TEST);
	glDisable(GL_BLEND);

}

} // anonymous namespace

namespace engine {

Gui::Gui(std::unique_ptr<Window> & window, std::unique_ptr<core::Input> & input)
  : m_window{window},
	m_input{input},
	m_mousePos{-1., -1.},
	m_leftMouseButtonDown{false}
{

	auto & io = ImGui::GetIO();
	const auto size = m_window->getFrameBufferSize();
	io.DisplaySize.x = size.x;
	io.DisplaySize.y = size.y;
	io.RenderDrawListsFn = renderDrawLists;

	m_input->addMouseCursorFunc([&](const double xpos, const double ypos){
		m_mousePos = {xpos, ypos};
	});
	m_input->addMouseButtonFunc([&](const int button, const int action, const int){
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
			m_leftMouseButtonDown = true;
		} else {
			m_leftMouseButtonDown = false;
		}
	});

	initFontTexture();
	initVBO();
	initVAO();
	initProgram();

}

Gui::~Gui() {
	ImGui::Shutdown();
}

void Gui::initFontTexture() {

	ImGuiIO& io = ImGui::GetIO();

	unsigned char* pixels;
	int width, height;
	io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

	GuiData.fontTex = std::make_unique<gl::Texture>("Gui Font Texture");
	GuiData.fontTex->setParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	GuiData.fontTex->setParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	GuiData.fontTex->createImmutableStorage(static_cast<unsigned int>(width),
			static_cast<unsigned int>(height), GL_RGBA32F);
	GuiData.fontTex->fillSubImage(0, 0, 0, static_cast<unsigned int>(width),
			static_cast<unsigned int>(height), GL_RGBA, GL_UNSIGNED_BYTE, pixels);

	io.Fonts->TexID = static_cast<void *>(GuiData.fontTex->getHandlePtr());

}

void Gui::initVBO() {
	GuiData.vbo = std::make_unique<gl::Buffer>("Gui VBO");
	GuiData.vbo->createMutableStorage(s_maxVBOSize, GL_DYNAMIC_DRAW);
}

void Gui::initVAO() {
	GuiData.vao = std::make_unique<gl::VertexArray>("Gui VAO");
	GuiData.vao->bindVertexBuffer(0, *GuiData.vbo, 0, sizeof(ImDrawVert));
	GuiData.vao->enableAttribBinding(0);
	GuiData.vao->bindVertexFormat(0, 0, 2, GL_FLOAT, GL_FALSE, offsetof(ImDrawVert, pos));
	GuiData.vao->enableAttribBinding(1);
	GuiData.vao->bindVertexFormat(0, 1, 2, GL_FLOAT, GL_FALSE, offsetof(ImDrawVert, uv));
	GuiData.vao->enableAttribBinding(2);
	GuiData.vao->bindVertexFormat(0, 2, 4, GL_UNSIGNED_BYTE, GL_TRUE, offsetof(ImDrawVert, col));
}

void Gui::initProgram() {
	GuiData.prog = std::make_unique<gl::Program>("Gui program");
	gl::Shader vert("shader/gui/gui.vert");
	gl::Shader frag("shader/gui/gui.frag");
	GuiData.prog->attachShader(vert);
	GuiData.prog->attachShader(frag);
}

void Gui::update() {

	auto & io = ImGui::GetIO();
	io.MousePos = ImVec2(static_cast<float>(m_mousePos.x), static_cast<float>(m_mousePos.y));
	io.MouseDown[0] = m_leftMouseButtonDown;
	// io.KeysDown[i] = ...

	ImGui::NewFrame();

}

void Gui::render() {
	ImGui::Render();
}

} // namespace engine
