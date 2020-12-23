#version 330

// 세계 변화 뷰-투영 행렬에 대한 uniform
uniform mat4 uWorldTransform;
uniform mat4 uViewProj;

// 속성 0은 위치, 1은 텍스처 좌표
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

// 텍스처 좌표를 출력에 추가한다
out vec2 fragTexCoord;

void main()
{
	// 위치를 동차 좌표로 변환
	vec4 pos = vec4(inPosition, 1.0);

	// 위치를 세계 공간으로 변환한뒤 클립 공간으로 변환
	gl_Position = pos * uWorldTransform * uViewProj;

	// 텍스처 좌표를 프래그먼트 쉐이더에 전달
	fragTexCoord = inTexCoord;
}