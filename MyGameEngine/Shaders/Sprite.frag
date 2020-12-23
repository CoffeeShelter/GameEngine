#version 330

// 버텍스 쉐이더에서 얻은 텍스처 좌표 입력값
in vec2 fragTexCoord;

// 출력 색상
out vec4 outColor;

// 텍스처 샘플링을 위해 필요
uniform sampler2D uTexture;

void main()
{
	// 텍스처로부터 색상을 샘플링
	outColor = texture(uTexture,fragTexCoord);
}