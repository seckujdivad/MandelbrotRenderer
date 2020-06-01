#version 430 core
layout (location = 0) in vec2 inPos;

out highp vec2 vertPosition;

uniform highp dvec2 cam_position;
uniform highp double cam_zoom;
uniform vec2 window_aspect;

void main()
{
	highp dvec2 pos = inPos;
	//vertPosition = vec2((pos - cam_position).xy / cam_zoom) * window_aspect;
	vertPosition = inPos;
	gl_Position = vec4(inPos, 0.0f, 1.0f);
}