#version 400
in vec3 position;
in vec2 textureCoord;
uniform mat4 mvp;

out vec2 p_texture;
void main(void) {
	gl_Position = mvp * vec4(position, 1.0);
	p_texture = textureCoord;
}
