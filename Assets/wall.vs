#version 400
in vec3 position;
in vec2 textureCoord;
in vec3 normal;
uniform mat4 mvp;
uniform mat4 m;
uniform vec3 scale;

out vec2 p_texture;
void main(void) {
	gl_Position = mvp * vec4(position, 1.0);
	
	vec4 worldSpace = m * vec4(position, 1.0);
	vec2 up = worldSpace.xz;
	vec2 left = worldSpace.xy;
	p_texture = mix(up, left, normal.y);
}
