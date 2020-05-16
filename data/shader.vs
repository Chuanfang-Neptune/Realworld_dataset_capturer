#version 330

layout (location = 0) in vec3 Position; //corespond to glEnableVertexAttribArray(0);

uniform mat4 gWorld;

void main()
{
    gl_Position = gWorld * vec4(Position, 1.0);
    gl_Position = vec4(-gl_Position.x,gl_Position.y,gl_Position.z,gl_Position.w);
}
