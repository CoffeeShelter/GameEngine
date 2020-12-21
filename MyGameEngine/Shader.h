#include <GL/glew.h>
#include <string>
#include "Math.h"

class Shader
{
public:
	Shader();
	~Shader();
	// 주어진 이름으로 버텍스/프래그먼트 쉐이더 로드
	bool Load(const std::string& vertName, const std::string& fragName);
	void Unload();
	// 이 쉐이더를 활성화된 쉐이더 프로그램으로 설정
	void SetActive();

	void SetMatrixUniform(const char* name, const Matrix4& matrix);
private:
	// 지정된 쉐이더를 컴파일
	bool CompileShader(const std::string& fileName, GLenum shaderType, GLuint& outShader);
	// 쉐이더가 성공적으로 컴파일됐는지 확인
	bool IsCompiled(GLuint shader);
	// 버텍스/프래그먼트 프로그램이 연결됐는지 확인
	bool IsValidProgram();
	// 쉐이버 오브젝트 ID를 저장
	GLuint mVertexShader;
	GLuint mFragShader;
	GLuint mShaderProgram;
};