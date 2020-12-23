#version 330

// ���� ��ȭ ��-���� ��Ŀ� ���� uniform
uniform mat4 uWorldTransform;
uniform mat4 uViewProj;

// �Ӽ� 0�� ��ġ, 1�� �ؽ�ó ��ǥ
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

// �ؽ�ó ��ǥ�� ��¿� �߰��Ѵ�
out vec2 fragTexCoord;

void main()
{
	// ��ġ�� ���� ��ǥ�� ��ȯ
	vec4 pos = vec4(inPosition, 1.0);

	// ��ġ�� ���� �������� ��ȯ�ѵ� Ŭ�� �������� ��ȯ
	gl_Position = pos * uWorldTransform * uViewProj;

	// �ؽ�ó ��ǥ�� �����׸�Ʈ ���̴��� ����
	fragTexCoord = inTexCoord;
}