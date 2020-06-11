#version 430 core

//#define NORMALISE_ITERATIONS
#define SMOOTH_SHADING
#define GREYSCALE

layout (location = 0) out vec4 frag_out;

in highp vec2 vertPosition;

uniform highp dvec2 cam_position;
uniform highp double cam_zoom;
uniform vec2 window_aspect;
uniform int max_iterations;

vec3 hsv2rgb(vec3 c) //source: https://stackoverflow.com/questions/15095909/from-rgb-to-hsv-in-opengl-glsl
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

vec3 hsv2rgb(float h, float s, float v)
{
	return hsv2rgb(vec3(h, s, v));
}

float get_mandelbrot_iterations(highp double x0, highp double y0) //source: https://en.wikipedia.org/wiki/Mandelbrot_set#Computer_drawings
{
	highp dvec2 p = dvec2(0.0f);
	int i = 0;
	highp double x_temp;
	while (((p.x * p.x) + (p.y * p.y) < 4) && (i < max_iterations))
	{
		x_temp = (p.x * p.x) - (p.y * p.y) + x0;
		p.y = (2 * p.x * p.y) + y0;
		p.x = x_temp;
		i++;
	}

#ifdef NORMALISE_ITERATIONS
	return i + 1 - log(log2(float((p.x * p.x) + (p.y * p.y)))); //normalise
#else
	return i;
#endif
}

float set_output_range(float iterations)
{
#ifdef SMOOTH_SHADING
	float value = iterations / float(max_iterations);
#else
	float value = iterations >= max_iterations * 0.5 ? 1.0f : 0.0f;
#endif

	//value = iterations >= max_iterations * 0.95 ? 1.0f : value;

	return value;
}

vec3 sample_colour(float value)
{
#ifdef GREYSCALE
	return vec3(value);
#else
	return hsv2rgb(value, 1.0f, 1.0f);
#endif
}

void main()
{
	highp dvec2 pos = dvec2((vertPosition - cam_position).xy / cam_zoom) * dvec2(window_aspect);
	float iterations = get_mandelbrot_iterations(pos.x, pos.y);

	frag_out = vec4(sample_colour(set_output_range(iterations)), 1.0f);
}