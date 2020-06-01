#pragma once

#include <GL/glew.h>
#include <GL/wglew.h>

#include <wx/wx.h>
#include <wx/glcanvas.h>
#include <wx/dcclient.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>
#include <cmath>
#include <ctime>

#include "Resource.h"

class MandelbrotCanvas : public wxGLCanvas
{
private:
	wxGLContext* m_glcontext;
	
	void Paint(wxPaintEvent& evt);

	GLuint m_vao;
	GLuint m_vbo;

	GLuint m_shader_program;

	void MouseMoveEvent(wxMouseEvent& evt);
	void MouseScrollEvent(wxMouseEvent& evt);
	void MouseDownEvent(wxMouseEvent& evt);
	void MouseUpEvent(wxMouseEvent& evt);

	int m_last_mouse_position[2] = { 0, 0 };
	bool m_mouse_down;

	glm::dvec2 m_cam_positon;
	double m_cam_zoom;
	
	GLuint m_uniform_cam_position;
	GLuint m_uniform_cam_zoom;
	GLuint m_uniform_window_aspect;
	GLuint m_uniform_max_iterations;

	glm::dvec2 ScreenPosToNDCPos(int x, int y);

	double m_last_fps = 0;
	int m_last_max_iterations = 0;

public:
	MandelbrotCanvas(wxWindow* parent, wxWindowID id, wxGLAttributes& args);
	~MandelbrotCanvas();

	void Render(bool adjust_iterations = true, bool swap_buffers = true);
};

void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);