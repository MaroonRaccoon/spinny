attribute vec4 position;
varying vec3 color;

uniform mat4 transform;

void main()
{
    gl_Position = transform * vec4(position.xyz, 1.0);
    color = gl_Position.xyz + vec3(0.5);  
}
