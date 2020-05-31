#pragma once

#include <wx/wx.h>
#include <wx/gbsizer.h>

#include "MandelbrotCanvas.h"

class Main : public wxFrame
{
private:
	wxGridBagSizer* m_sizer;
	MandelbrotCanvas* m_canvas;

public:
	Main();
};