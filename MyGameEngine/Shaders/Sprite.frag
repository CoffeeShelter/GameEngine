#version 330

// ���ؽ� ���̴����� ���� �ؽ�ó ��ǥ �Է°�
in vec2 fragTexCoord;

// ��� ����
out vec4 outColor;

// �ؽ�ó ���ø��� ���� �ʿ�
uniform sampler2D uTexture;

void main()
{
	// �ؽ�ó�κ��� ������ ���ø�
	outColor = texture(uTexture,fragTexCoord);
}