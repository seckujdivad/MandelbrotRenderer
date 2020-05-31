#include "MandelbrotCanvas.h"

void MandelbrotCanvas::Paint(wxPaintEvent& evt)
{
	this->Render();
	evt.Skip();
}

MandelbrotCanvas::MandelbrotCanvas(wxWindow* parent, wxWindowID id, wxGLAttributes& args) : wxGLCanvas(parent, args, id)
{
	wxGLContextAttrs ctx_attrs;
	ctx_attrs.PlatformDefaults().CoreProfile().MajorVersion(4).MinorVersion(3).EndList();
	this->m_glcontext = new wxGLContext(this, NULL, &ctx_attrs);
	this->SetCurrent(*this->m_glcontext);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		throw std::runtime_error("Couldn't initialise glew");
	}

	glLoadIdentity();

	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);

	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	std::remove("gl_log.txt");
	glDebugMessageCallback(MessageCallback, 0);

	//send vertices to GPU
	float vertices[] = {
		1.0f, 1.0f,
		1.0f, -1.0f,
		-1.0f, -1.0f,
		-1.0f, -1.0f,
		-1.0f, 1.0f,
		1.0f, 1.0f
	};

	glGenBuffers(1, &this->m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, this->m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//compile shaders
	GLint shader_compile_successful;

	// compile vertex shader
	std::string vertex_shader = GetEmbeddedTextfile(RCID_TF_SHADER_VERT);
	const char* vertex_shader_chars = vertex_shader.c_str();
	GLuint shader_vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(shader_vertex, 1, &vertex_shader_chars, NULL);
	glCompileShader(shader_vertex);

	glGetShaderiv(shader_vertex, GL_COMPILE_STATUS, &shader_compile_successful);
	if (!shader_compile_successful)
	{
		GLchar shader_log[1024];
		GLsizei shader_log_size;
		glGetShaderInfoLog(shader_vertex, 1024, &shader_log_size, shader_log);
		std::string shader_log_string = std::string(shader_log, shader_log_size);
		throw std::runtime_error("Error compiling vertex shader: " + shader_log_string);
	}

	// compile fragment shader
	std::string fragment_shader = GetEmbeddedTextfile(RCID_TF_SHADER_FRAG);
	const char* fragment_shader_chars = fragment_shader.c_str();
	GLuint shader_fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(shader_fragment, 1, &fragment_shader_chars, NULL);
	glCompileShader(shader_fragment);
	
	glGetShaderiv(shader_fragment, GL_COMPILE_STATUS, &shader_compile_successful);
	if (!shader_compile_successful)
	{
		GLchar shader_log[1024];
		GLsizei shader_log_size;
		glGetShaderInfoLog(shader_fragment, 1024, &shader_log_size, shader_log);
		std::string shader_log_string = std::string(shader_log, shader_log_size);
		throw std::runtime_error("Error compiling fragment shader: " + shader_log_string);
	}

	this->m_shader_program = glCreateProgram();
	glAttachShader(this->m_shader_program, shader_vertex);
	glAttachShader(this->m_shader_program, shader_fragment);
	glLinkProgram(this->m_shader_program);

	glGetProgramiv(this->m_shader_program, GL_LINK_STATUS, &shader_compile_successful);
	if (!shader_compile_successful)
	{
		GLchar shader_log[1024];
		GLsizei shader_log_size;
		glGetProgramInfoLog(this->m_shader_program, 1024, &shader_log_size, shader_log);
		std::string shader_log_string = std::string(shader_log, shader_log_size);
		throw std::runtime_error("Error linking shaders together into a shader program: " + shader_log_string);
	}

	glDeleteShader(shader_vertex);
	glDeleteShader(shader_fragment);

	// configure shader program inputs
	glUseProgram(this->m_shader_program);

	//create vertex array object
	glGenVertexArrays(1, &this->m_vao);
	glBindVertexArray(this->m_vao);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//set up paint event
	this->Bind(wxEVT_PAINT, &MandelbrotCanvas::Paint, this);

	this->Render();
}

MandelbrotCanvas::~MandelbrotCanvas()
{
	glDeleteProgram(this->m_shader_program);
	glDeleteVertexArrays(1, &this->m_vao);
	glDeleteBuffers(1, &this->m_vbo);

	delete this->m_glcontext;
}

void MandelbrotCanvas::Render()
{
	glViewport(0, 0, this->GetSize().x, this->GetSize().y);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindVertexArray(this->m_vao);
	glDrawArrays(GL_TRIANGLES, 0, 12);

	glFlush();
	this->SwapBuffers();
}

void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	std::ofstream output_file;
	output_file.open("gl_log.txt");
	output_file << "(type: " << type << ", severity: ";

	if (severity == GL_DEBUG_SEVERITY_HIGH)
	{
		output_file << "high";
	}
	else if (severity == GL_DEBUG_SEVERITY_MEDIUM)
	{
		output_file << "medium";
	}
	else if (severity == GL_DEBUG_SEVERITY_LOW)
	{
		output_file << "low";
	}
	else if (severity == GL_DEBUG_SEVERITY_NOTIFICATION)
	{
		output_file << "notification";
	}
	else
	{
		output_file << severity;
	}
	output_file << "): " << message << std::endl;

	output_file.close();

	if ((type == GL_INVALID_ENUM
		|| type == GL_INVALID_VALUE
		|| type == GL_INVALID_OPERATION
		|| type == GL_STACK_OVERFLOW
		|| type == GL_STACK_UNDERFLOW
		|| type == GL_OUT_OF_MEMORY
		|| type == GL_INVALID_FRAMEBUFFER_OPERATION
		|| type == GL_TABLE_TOO_LARGE)
		|| (severity == GL_DEBUG_SEVERITY_HIGH))
	{
		throw std::runtime_error(message);
	}
}