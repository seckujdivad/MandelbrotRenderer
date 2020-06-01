#version 430 core
#define MAX_ITERATIONS 1000

//#define NORMALISE_ITERATIONS

layout (location = 0) out vec4 frag_out;

in highp vec2 vertPosition;

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
	highp double x = 0;
	highp double y = 0;
	int i = 0;
	highp double x_temp;
	while (((x * x) + (y * y) < 4) && (i < MAX_ITERATIONS))
	{
		x_temp = (x * x) - (y * y) + x0;
		y = (2 * x * y) + y0;
		x = x_temp;
		i++;
	}

#ifdef NORMALISE_ITERATIONS
	return i + 1 - log(log2(length(vec2(x, y)))); //normalise
#else
	return i;
#endif
}

void main()
{
	float iterations = get_mandelbrot_iterations(vertPosition.x, vertPosition.y);
	frag_out = vec4(vec3(iterations / float(MAX_ITERATIONS)), 1.0f);
}