#include "VertexArray.h"
#include <GL/glew.h>

VertexArray::VertexArray(const float* verts, unsigned int numVerts,
	const unsigned int* indices, unsigned int numIndices)
	:mNumVerts(numVerts)
	,mNumIndices(numIndices)
{
	// Create vertex array
	glGenVertexArrays(1, &mVertexArray);
	glBindVertexArray(mVertexArray);

	// Create vertex buffer
	glGenBuffers(1, &mVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
	glBufferData(
		GL_ARRAY_BUFFER,				// �����͸� �� ������ Ÿ��
		numVerts * 8 * sizeof(float),	// ������ ����Ʈ ũ��
		verts,							// ������ �ҽ�(������)
		GL_STATIC_DRAW					// �� �����͸� ��� ����� ���ΰ�?
	);

	// Create index buffer
	glGenBuffers(1, &mIndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		numIndices * sizeof(unsigned int),
		indices,
		GL_STATIC_DRAW
	);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,						// �Ӽ� �ε��� (ù ��° ���ؽ� �Ӽ� �ε����� 0)
		3,						// ����� �� (�̹� �������� 3)
		GL_FLOAT,				// ����� Ÿ��
		GL_FALSE,				// (������ Ÿ�Կ����� ���ȴ�)
		sizeof(float) * 8,		// ���� (�Ϲ������� �� ���ؽ��� ũ��)
		0						// ���ؽ��� ���ۿ��� �� �Ӽ������� ������
	);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		1,	// ���ؽ� �Ӽ� �ε���
		3,	// ������Ʈ �� (UV,2)
		GL_FLOAT,	// �� ������Ʈ�� Ÿ��
		GL_FALSE,	// GL_FLOAT ������ ������ ����
		sizeof(float) * 8,	// ���� ( ������ �׻� ���ؽ��� ũ��� )
		reinterpret_cast<void*>(sizeof(float) * 3)	// ������ ������
	);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(
		2,						// �Ӽ� �ε��� (ù ��° ���ؽ� �Ӽ� �ε����� 0)
		2,						// ����� �� (�̹� �������� 3)
		GL_FLOAT,				// ����� Ÿ��
		GL_FALSE,				// (������ Ÿ�Կ����� ���ȴ�)
		sizeof(float) * 8,		// ���� (�Ϲ������� �� ���ؽ��� ũ��)
		reinterpret_cast<void*>(sizeof(float) * 6)	// ������ ������
	);
}

VertexArray::~VertexArray()
{
	glDeleteBuffers(1, &mVertexBuffer);
	glDeleteBuffers(1, &mIndexBuffer);
	glDeleteVertexArrays(1, &mVertexArray);
}

void VertexArray::SetActive()
{
	glBindVertexArray(mVertexArray);
}