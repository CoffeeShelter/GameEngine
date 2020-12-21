#version 330

// 세계 변환 및 뷰-투영 행렬을 위한 uniform
uniform mat4 uWorldTransform;
uniform mat4 uViewProj;

// 버텍스 속성
in vec3 inPosition;

void main()
{
	vec4 pos = vec4(inPosition,1.0);
	gl_Position = pos * uWorldTransform * uViewProj;
}