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
		GL_ARRAY_BUFFER,				// 데이터를 쓸 버퍼의 타입
		numVerts * 3 * sizeof(float),	// 복사할 바이트 크기
		verts,							// 복사할 소스(포인터)
		GL_STATIC_DRAW					// 이 데이터를 어떻게 사용할 것인가?
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
		0,						// 속성 인덱스 (첫 번째 버텍스 속성 인덱스는 0)
		3,						// 요소의 수 (이번 예에서는 3)
		GL_FLOAT,				// 요소의 타입
		GL_FALSE,				// (정수형 타입에서만 사용된다)
		sizeof(float) * 3,		// 간격 (일반적으로 각 버텍스의 크기)
		0						// 버텍스의 시작에서 이 속성까지의 오프셋
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