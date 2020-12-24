// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

// Request GLSL 3.3
#version 330

// Inputs from vertex shader
// Tex coord
in vec2 fragTexCoord;
// Normal (in world space)
in vec3 fragNormal;
// Position (in world space)
in vec3 fragWorldPos;

// This corresponds to the output color to the color buffer
out vec4 outColor;

// This is used for the texture sampling
uniform sampler2D uTexture;

// Create a struct for directional light
struct DirectionalLight
{
	// Direction of light
	vec3 mDirection;
	// Diffuse color
	vec3 mDiffuseColor;
	// Specular color
	vec3 mSpecColor;
};

// Uniforms for lighting
// Camera position (in world space)
uniform vec3 uCameraPos;
// Specular power for this surface
uniform float uSpecPower;
// Ambient light level
uniform vec3 uAmbientLight;

// Directional Light
uniform DirectionalLight uDirLight;

void main()
{
	// ǥ�� ����
	vec3 N = normalize(fragNormal);
	// ǥ�鿡�� ���������� ����
	vec3 L = normalize(-uDirLight.mDirection);
	// ǥ�鿡�� ī�޶�� ���ϴ� ����
	vec3 V = normalize(uCameraPos - fragWorldPos);
	// N�� ���� -L�� �ݻ�
	vec3 R = normalize(reflect(-L, N));

	// �� �ݻ� ���
	vec3 Phong = uAmbientLight;
	float NdotL = dot(N, L);
	if (NdotL > 0)
	{
		vec3 Diffuse = uDirLight.mDiffuseColor * NdotL;
		vec3 Specular = uDirLight.mSpecColor * pow(max(0.0, dot(R, V)), uSpecPower);
		Phong += Diffuse + Specular;
	}

	// �������� �ؽ�ó ���� ���ϱ� �� ���� (alpha = 1)
    outColor = texture(uTexture, fragTexCoord) * vec4(Phong, 1.0f);
}
