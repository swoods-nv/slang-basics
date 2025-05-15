uniform mat4 modelView;

in vec3 position;

void main() 
{
    gl_Position = modelView * vec4(position, 1.0);
}