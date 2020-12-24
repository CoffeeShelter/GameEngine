#include <GL/glew.h>
#include <string>
#include "Math.h"

class Shader
{
public:
	Shader();
	~Shader();
	// �־��� �̸����� ���ؽ�/�����׸�Ʈ ���̴� �ε�
	bool Load(const std::string& vertName, const std::string& fragName);
	void Unload();
	// �� ���̴��� Ȱ��ȭ�� ���̴� ���α׷����� ����
	void SetActive();

	// Sets a Matrix uniform
	void SetMatrixUniform(const char* name, const Matrix4& matrix);
	// Sets a Vector3 uniform
	void SetVectorUniform(const char* name, const Vector3& vector);
	// Sets a float uniform
	void SetFloatUniform(const char* name, float value);
private:
	// ������ ���̴��� ������
	bool CompileShader(const std::string& fileName, GLenum shaderType, GLuint& outShader);
	// ���̴��� ���������� �����ϵƴ��� Ȯ��
	bool IsCompiled(GLuint shader);
	// ���ؽ�/�����׸�Ʈ ���α׷��� ����ƴ��� Ȯ��
	bool IsValidProgram();
	// ���̹� ������Ʈ ID�� ����
	GLuint mVertexShader;
	GLuint mFragShader;
	GLuint mShaderProgram;
};