#version 330

// Input
in vec3 frag_color;

// Output
out vec4 out_color;


void main()
{
    // Write pixel out color
    out_color = vec4(frag_color, 1);

}
