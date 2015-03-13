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
	GLuint prog;
} GuiData;

void renderDrawLists(ImDrawList ** const cmd_lists, const int cmd_lists_count) {

	if (cmd_lists_count == 0) {
		return;
	}

	glUseProgram(GuiData.prog);

	// Setup render state: alpha-blending enabled, no face culling, no depth testing, scissor enabled
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_SCISSOR_TEST);

	// Setup texture
	GuiData.fontTex->bindToTextureUnit(k_fontTexUnit);

	// Setup orthogonal projection matrix
	const auto width = ImGui::GetIO().DisplaySize.x;
	const auto height = ImGui::GetIO().DisplaySize.y;
	const auto ortho_projection = glm::mat4{
		{ 2.0f/width, 0.0f, 0.0f, 0.0f },
		{ 0.0f, 2.0f/-height, 0.0f, 0.0f },
		{ 0.0f, 0.0f, -1.0f, 0.0f },
		{ -1.0f, 1.0f, 0.0f, 1.0f },
	};
	const auto loc = glGetUniformLocation(GuiData.prog, "ProjMtx");
	glProgramUniformMatrix4fv(GuiData.prog, loc, 1, GL_FALSE, glm::value_ptr(ortho_projection));

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

	GuiData.vao->bind();
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
	GuiData.vao->unbind();
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
	m_leftMouseButtonDown{false},
	m_leftMouseButtonRelease{false},
	m_controlPressed{false},
	m_shiftPressed{false}
{

	auto & io = ImGui::GetIO();
	const auto size = m_window->getFrameBufferSize();
	io.DisplaySize.x = static_cast<float>(size.x);
	io.DisplaySize.y = static_cast<float>(size.y);
	io.RenderDrawListsFn = renderDrawLists;
	io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
	io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
	io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
	io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
	io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
	io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
	io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
	io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
	io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
	io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
	io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
	io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
	io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
	io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
	io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
	io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
	io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;

	m_input->addMouseCursorFunc([&](const double xpos, const double ypos){
		m_mousePos = {xpos, ypos};
	});
	m_input->addMouseButtonFunc([&](const int button, const int action, const int){
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
			m_leftMouseButtonDown = true;
		}
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
			m_leftMouseButtonRelease = true;
		}
	});
	m_input->addMouseScrollFunc([&](const double, const double yoffset){
		m_scrollOffset += yoffset;
	});
	m_input->addKeyFunc([&](const int key, const int, const int action, const int mods){
		if (action == GLFW_PRESS) {
			m_keysPressed[key] = true;
			if ((mods & GLFW_MOD_CONTROL) != 0) {
				m_controlPressed = true;
			}
			if ((mods & GLFW_MOD_SHIFT) != 0) {
				m_shiftPressed = true;
			}
		}
		if (action == GLFW_RELEASE) {
			m_keysReleased[key] = true;
		}
	});
	m_input->addCharFunc([](const unsigned int c){
		if (c > 0 && c < 0x10000) {
        	ImGui::GetIO().AddInputCharacter(static_cast<unsigned short>(c));
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
	GuiData.prog = glCreateProgram();
	{
		const std::string str = " \
		#version 450 core\n \
		layout(location = 0) in vec2 Position;\n \
		layout(location = 1) in vec2 UV;\n \
		layout(location = 2) in vec4 Color;\n \
		out vec2 Frag_UV;\n \
		out vec4 Frag_Color;\n \
		uniform mat4 ProjMtx;\n \
		void main() {\n \
		   Frag_UV = UV;\n \
		   Frag_Color = Color;\n \
		   gl_Position = ProjMtx * vec4(Position.xy, 0.0, 1.0);\n \
		}";
		const auto charcode = str.c_str();
		const auto len = static_cast<GLint>(str.size());
		auto handle = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(handle, 1, &charcode, &len);
		glCompileShader(handle);
		auto success = GL_FALSE;
		glGetShaderiv(handle, GL_COMPILE_STATUS, &success);
		if (!success) {
			LOG_ERROR("Could not compile Gui vertex shader");
		}
		glAttachShader(GuiData.prog, handle);
	}
	{
		const std::string str = " \
		#version 450 core\n \
		layout(location = 0) out vec4 outColor;\n \
		in vec2 Frag_UV;\n \
		in vec4 Frag_Color;\n \
		uniform sampler2D Texture;\n \
		void main() {\n \
		   outColor = Frag_Color * texture(Texture, Frag_UV.st);\n \
		}";
		const auto charcode = str.c_str();
		const auto len = static_cast<GLint>(str.size());
		auto handle = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(handle, 1, &charcode, &len);
		glCompileShader(handle);
		auto success = GL_FALSE;
		glGetShaderiv(handle, GL_COMPILE_STATUS, &success);
		if (!success) {
			LOG_ERROR("Could not compile Gui fragment shader");
		}
		glAttachShader(GuiData.prog, handle);
	}
	glLinkProgram(GuiData.prog);
	GLint success = GL_FALSE;
	glGetProgramiv(GuiData.prog, GL_LINK_STATUS, &success);
	if (!success) {
		LOG_ERROR("Could not link Gui program");
	}
	const auto loc = glGetUniformLocation(GuiData.prog, "Texture");
	glProgramUniform1i(GuiData.prog, loc, k_fontTexUnit);
}

void Gui::update() {

	auto & io = ImGui::GetIO();

	const auto size = m_window->getFrameBufferSize();
	io.DisplaySize.x = static_cast<float>(size.x);
	io.DisplaySize.y = static_cast<float>(size.y);

	io.MousePos = ImVec2(static_cast<float>(m_mousePos.x), static_cast<float>(m_mousePos.y));

	io.MouseDown[0] = m_leftMouseButtonDown;
	if (m_leftMouseButtonRelease) {
		m_leftMouseButtonDown = false;
		m_leftMouseButtonRelease = false;
	}

	io.MouseWheel += static_cast<float>(m_scrollOffset);
	m_scrollOffset = 0.0;

	for (const auto & key : m_keysPressed) {
		io.KeysDown[key.first] = key.second;
	}
	io.KeyCtrl = m_controlPressed;
	io.KeyShift = m_shiftPressed;
	for (auto & key : m_keysReleased) {
		if (key.second) {
			m_keysPressed[key.first] = false;
			key.second = false;
			m_controlPressed = false;
			m_shiftPressed = false;
		}
	}

}

void Gui::render() {
	ImGui::Render();
}

} // namespace engine
