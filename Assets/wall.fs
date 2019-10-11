#version 400
uniform sampler2D defuse;
in vec2 p_texture;

out vec4 FragColor;
void main(void) {
	FragColor = texture(defuse, p_texture);
}
