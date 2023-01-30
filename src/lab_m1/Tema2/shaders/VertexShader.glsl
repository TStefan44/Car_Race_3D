#version 330

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;
layout(location = 3) in vec3 v_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

uniform vec3 object_color;
uniform vec3 pos_car;
uniform float scaleFactor;

// Output
out vec3 frag_color;

void main()
{
    // Send output to fragment shader
    frag_color = object_color;

    // Compute gl_Position
    vec4 world_position =  Model * vec4(v_position, 1.0);
    float pos_y = world_position.y - pow(length(pos_car - world_position.xyz), 2) * scaleFactor;
    world_position.y = pos_y;
    gl_Position = Projection * View * world_position;

}
