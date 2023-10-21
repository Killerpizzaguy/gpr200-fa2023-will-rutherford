#version 450
layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vNormal;

out vec3 Normal;
uniform mat4 _Model;
uniform mat4 _ViewProjection;

void main(){
	Normal = vNormal;
	//gl_Position = _Model * vec4(vPos,1.0);
	//gl_Position = _Model * vec4(vPos,1.0) * _ViewProjection; //closest to working

	mat4 clipPos = _ViewProjection * _Model;
	gl_Position = clipPos * vec4(vPos,1.0);
}