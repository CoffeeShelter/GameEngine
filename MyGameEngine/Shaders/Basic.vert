// GLSL 3.3 요청
#version 330

// 모든 버텍스 속성은 여기에
// 현재 버텍스 속성은 위치 하나뿐임
in vec3 inPosition;

void main()
{
	// inPosition을 gl_Position로 직접 전달
	gl_Position = vec4(inPosition,1.0);
}