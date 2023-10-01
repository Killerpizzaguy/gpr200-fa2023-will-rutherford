#version 450
out vec4 FragColor;
in vec2 UV;

uniform float iTime;
uniform float _X;
uniform float _Y;
uniform sampler2D _Texture;

void main(){
	vec2 p = UV;

	p = UV + vec2(_X, _Y+sin(iTime)*0.1);

	FragColor = texture(_Texture,p);
}