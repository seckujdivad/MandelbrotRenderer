#version 430 core
#define MAX_ITERATIONS 1000

layout (location = 0) out vec4 frag_out;

in highp vec2 vertPosition;

int get_mandelbrot_iterations(highp double x0, highp double y0)
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
	return i;
}

void main()
{
	int iterations = get_mandelbrot_iterations(vertPosition.x, vertPosition.y);
	frag_out = vec4(vec3(float(iterations) / float(1000)), 1.0f);
}