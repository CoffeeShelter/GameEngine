// GLSL 3.3 ��û
#version 330

// ��� ���ؽ� �Ӽ��� ���⿡
// ���� ���ؽ� �Ӽ��� ��ġ �ϳ�����
in vec3 inPosition;

void main()
{
	// inPosition�� gl_Position�� ���� ����
	gl_Position = vec4(inPosition,1.0);
}