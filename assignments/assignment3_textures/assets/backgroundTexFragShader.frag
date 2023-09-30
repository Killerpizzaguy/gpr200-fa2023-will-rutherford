#version 450
out vec4 FragColor;
in vec2 UV;

uniform float iTime;
uniform sampler2D _BackgroundTexture;
uniform sampler2D _NoiseTexture;

void main(){
	float noise = texture(_NoiseTexture,UV + iTime * 0.3).r;
	vec2 uv = UV + noise * 0.1f;

	FragColor = texture(_BackgroundTexture,uv);
}