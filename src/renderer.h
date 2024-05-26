#pragma once
#include <string>
#include <include/GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;
using namespace glm;

class renderer
{
private:
	bool setShaderFile(string& vertex, string& fragment);
	int compile_shader(const string& shader_str, const int shader_type);
	bool setCube();
	GLuint _VA0, _VB0,_IB0;//頂点データ、頂点バッファ、インデックスバッファ
	int _programId;//シェーダーオブジェクト
	GLuint _volumeId;//ボリュームのID
	unsigned short* _CT;//ボリュームデータ
	vec3 _camera;//カメラ位置
	vec3 _source;//オブジェクトの位置
	vec3 _target;//注視点
	vec3 _up;//上ベクトル
	GLfloat _close;//注視点とカメラの距離

public:
	bool setRenderer();
	bool setVolume(const string& filePath);
	void setWorldParams(float& scrool, float deltaX, float deltaY, bool rotateF, bool translateF);
	void draw();
	void terminate();
	renderer();
	~renderer();
};