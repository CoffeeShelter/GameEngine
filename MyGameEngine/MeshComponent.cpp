#include "MeshComponent.h"
#include "Actor.h"
#include "Shader.h"
#include "Actor.h"
#include "Mesh.h"
#include "Renderer.h"
#include "Texture.h"
#include "VertexArray.h"
#include "Game.h"

MeshComponent::MeshComponent(Actor* owner)
	:Component(owner)
	,mMesh(nullptr)
	,mTextureIndex(0)
{
	mOwner->GetGame()->GetRenderer()->AddMeshComp(this);
}

MeshComponent::~MeshComponent()
{
	mOwner->GetGame()->GetRenderer()->RemoveMeshComp(this);
}

void MeshComponent::Draw(Shader* shader)
{
	if (mMesh)
	{
		// ���� ��ȯ ����
		shader->SetMatrixUniform("uWorldTransform", mOwner->GetWorldTransform());
		// Ȱ��ȭ�� �ؽ�ó ����
		Texture* t = mMesh->GetTexture(mTextureIndex);
		if (t) { t->SetActive(); }
		// Ȱ��ȭ�� �޽��� ���ؽ� �迭 ����
		VertexArray* va = mMesh->GetVertexArray();
		va->SetActive();
		// �޽ø� �׸���
		glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
	}
}