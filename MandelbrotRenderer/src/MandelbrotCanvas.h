#pragma once

#include <GL/glew.h>
#include <GL/wglew.h>

#include <wx/wx.h>
#include <wx/glcanvas.h>
#include <wx/dcclient.h>

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>

#include "Resource.h"

class MandelbrotCanvas : public wxGLCanvas
{
private:
	wxGLContext* m_glcontext;
	
	void Paint(wxPaintEvent& evt);

	GLuint m_vao;
	GLuint m_vbo;

	GLuint m_shader_program;

public:
	MandelbrotCanvas(wxWindow* parent, wxWindowID id, wxGLAttributes& args);
	~MandelbrotCanvas();

	void Render();
};

void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);