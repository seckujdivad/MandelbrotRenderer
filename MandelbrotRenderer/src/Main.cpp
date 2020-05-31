#include "Main.h"

Main::Main() : wxFrame(nullptr, wxID_ANY, "Mandelbrot Renderer", wxDefaultPosition, wxSize(800, 600))
{
	this->m_sizer = new wxGridBagSizer(0, 0);
	this->m_sizer->SetFlexibleDirection(wxBOTH);
	this->m_sizer->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

	wxGLAttributes attributes;
	attributes.PlatformDefaults().Depth(24).Stencil(8).RGBA().DoubleBuffer().EndList();
	this->m_canvas = new MandelbrotCanvas(this, wxID_ANY, attributes);
	this->m_sizer->Add(this->m_canvas, wxGBPosition(0, 0), wxGBSpan(1, 1), wxEXPAND | wxALL);

	this->m_sizer->AddGrowableCol(0);
	this->m_sizer->AddGrowableRow(0);

	this->SetSizer(this->m_sizer);
	this->Centre(wxBOTH);
	this->Layout();
}
