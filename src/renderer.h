#pragma once
#include <string>
#include <include/GL/glew.h>

using namespace std;

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
	GLfloat _camera[3];//カメラ位置
	GLfloat _ray[3];//レイの方向
public:
	bool setRenderer();
	bool setVolume(const string& filePath);
	void setWorldParams(float& scrool);
	void draw();
	void terminate();
	renderer();
	~renderer();
};