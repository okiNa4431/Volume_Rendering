#include "renderer.h"
#include "io.h"
#define PI 3.141592

inline GLfloat distance(const GLfloat p0[], const GLfloat p1[])
{
	return sqrt((p0[0] - p1[0]) * (p0[0] - p1[0]) + (p0[1] - p1[1]) * (p0[1] - p1[1]) + (p0[2] - p1[2]) * (p0[2] - p1[2]));
}

int renderer::compile_shader(const string& shader_str, const int shader_type)
{
	int shader = glCreateShader(shader_type);
	const char* shaderSource = shader_str.c_str();
	glShaderSource(shader, 1, &shaderSource, nullptr);
	glCompileShader(shader);

	//エラー読み取り
	GLint res;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &res);
	if (res == GL_FALSE)
	{
		if (shader_type == GL_VERTEX_SHADER)
		{
			cout << "could not compile vertex shader." << endl;
		}
		else if(shader_type == GL_FRAGMENT_SHADER)
		{
			cout << "could not compile fragment shader." << endl;
		}
	}
	//エラーログ表示
	const GLsizei maxLength = 1024;
	GLsizei length;
	auto* infoLog = new GLchar[maxLength];
	glGetShaderInfoLog(shader, maxLength, &length, infoLog);
	std::cout << infoLog << std::endl;
	delete[] infoLog;
	
	return shader;
}

bool renderer::setShaderFile(string& vertex, string& fragment)
{
	vertex = read_shader("vertex.vert");
	fragment = read_shader("fragment.frag");

	return true;
}

bool renderer::setRenderer()
{
	//シェーダーファイルの読み取り
	string vertex_str = "";
	string fragment_str = "";
	if (!setShaderFile(vertex_str, fragment_str))
	{
		printf("could not set shader file\n");
		return false;
	}

	//シェーダーのコンパイル
	int vShaderId = compile_shader(vertex_str, GL_VERTEX_SHADER);
	int fShaderId = compile_shader(fragment_str, GL_FRAGMENT_SHADER);

	//シェーダープログラムにシェーダーをアタッチ、リンク
	_programId = glCreateProgram();
	glAttachShader(_programId, vShaderId);
	glAttachShader(_programId, fShaderId);
	glLinkProgram(_programId);
	glUseProgram(_programId);

	setCube();//ボリュームが収まる直方体を生成

	return true;
}

bool renderer::setCube()
{
	// 頂点データ
	float vertices[] = {
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		-0.5f, 0.5f, -0.5f,
		0.5f, 0.5f, -0.5f,
		-0.5f, -0.5f, 0.5f,
		0.5f, -0.5f, 0.5f,
		-0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f,
	};

	//インデックスデータ
	unsigned int indices[] = {
		//前
		0,1,2,
		1,3,2,
		//右
		1,5,3,
		5,7,3,
		//左
		0,2,4,
		2,6,4,
		//上
		2,3,7,
		7,6,2,
		//下
		0,4,1,
		1,4,5,
		//奥
		5,4,6,
		6,7,5,
	};

	glGenVertexArrays(1, &_VA0);
	glGenBuffers(1, &_VB0);
	glGenBuffers(1, &_IB0);

	//頂点配列の設定
	glBindVertexArray(_VA0);

	//頂点データ
	glBindBuffer(GL_ARRAY_BUFFER, _VB0);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//インデックスデータ
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _IB0);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0); // VAOのバインドを解除

	return true;
}

bool renderer::setVolume(const string& filePath)
{
	//ボリューム読み込み
	const vector<int> size = { 512, 512, 271 };
	_CT = new unsigned short[size[0] * size[1] * size[2]];
	read_volume(filePath.c_str(), _CT, size);

	//3Dテクスチャ生成
	glGenTextures(1, &_volumeId);//テクスチャオブジェクト生成
	glBindTexture(GL_TEXTURE_3D, _volumeId);//バインド
	glActiveTexture(GL_TEXTURE0);
	glTexImage3D(GL_TEXTURE_3D, 0, GL_RED, size[0], size[1], size[2], 0, GL_RED, GL_UNSIGNED_SHORT, _CT);//テクスチャ初期化、書き込み
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	//ボリューム(3Dテクスチャ)、その他パラメータ転送
	const float step = 1.0/271;
	glUniform1i(glGetUniformLocation(_programId, "volume"), 0);
	glUniform3fv(glGetUniformLocation(_programId, "camera"), 1, value_ptr(_camera));
	//glUniform3fv(glGetUniformLocation(_programId, "ray"), 1, _ray);
	glUniform1f(glGetUniformLocation(_programId, "step"), step);
		//座標変換行列
	mat4 world = mat4(1.0f);
	glUniformMatrix4fv(glGetUniformLocation(_programId, "world"), 1, GL_FALSE, value_ptr(world));
	mat4 view = lookAt(_camera, _target, _up);
	glUniformMatrix4fv(glGetUniformLocation(_programId, "view"), 1, GL_FALSE, value_ptr(view));
	mat4 proj = perspective(radians(45.0f), (float)512 / (float)512, 0.1f, 100.0f);
	glUniformMatrix4fv(glGetUniformLocation(_programId, "proj"), 1, GL_FALSE, value_ptr(proj));

	return true;
}

void renderer::setWorldParams(float& scrool, float deltaX, float deltaY, bool rotateF, bool translateF)
{
	//前処理
	vec3 trg2cmr = normalize(_camera - _target);

	//ワールド行列
	vec3 cameraRight = normalize(cross(_up, trg2cmr));
	vec3 cameraUp = cross(trg2cmr, cameraRight);
	mat4 world = mat4(1.0f);
	if (translateF)
	{
		_source += cameraRight * deltaX * 0.005f - cameraUp * deltaY * 0.005f;
	}
	world = translate(world, _source);
	//world = rotate(world, radians(0.0f), vec3(0.0f, 0.0f, 1.0f));
	glUniformMatrix4fv(glGetUniformLocation(_programId, "world"), 1, GL_FALSE, value_ptr(world));

	//ビュー行列
		//拡大縮小
	_close += scrool * 0.1f;
	if (_close <= 0.2f) _close = 0.2f;
	_camera = _target + trg2cmr * _close;
		//回転
	if (rotateF)
	{
		vec4 homogeneousCamera(_camera, 1.0f);
		vec4 homogeneousTarget(_target, 1.0f);
		float rotateAngleUnitX = 2.0f * PI / 512.0f;
		float rotateAngleUnitY = PI / 512.0f;
		float xAngle = -deltaX * rotateAngleUnitX;
		float yAngle = -deltaY * rotateAngleUnitY;

		float cosAngle = dot(trg2cmr, _up);
		float yAngleSgn = 0.0f;
		if (yAngle > 0.0f) yAngleSgn = 1.0f;
		else if (yAngle < 0.0f) yAngleSgn = -1.0f;
		if (cosAngle * yAngleSgn> 0.99f) yAngleSgn = 0.0f;

		mat4 rotateX = rotate(mat4(1.0f), xAngle, cameraUp);
		homogeneousCamera = (rotateX * (homogeneousCamera - homogeneousTarget)) + homogeneousTarget;
		mat4 rotateY = rotate(mat4(1.0f), yAngle, cameraRight);
		_camera = (rotateY * (homogeneousCamera - homogeneousTarget)) + homogeneousTarget;
	}

	mat4 view = lookAt(_camera, _target, _up);
	glUniformMatrix4fv(glGetUniformLocation(_programId, "view"), 1, GL_FALSE, value_ptr(view));

	//プロジェクション行列
	mat4 proj = perspective(radians(45.0f), (float)512 / (float)512, 0.1f, 100.0f);
	glUniformMatrix4fv(glGetUniformLocation(_programId, "proj"), 1, GL_FALSE, value_ptr(proj));

	scrool = 0.0;
}

void renderer::draw()
{
	glBindVertexArray(_VA0);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

void renderer::terminate()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_3D, _volumeId);//バインド
	glTexImage3D(GL_TEXTURE_3D, 0, GL_RED, 512, 512, 271, 0, GL_R, GL_UNSIGNED_SHORT, _CT);//テクスチャ初期化、書き込み
	glBindTexture(GL_TEXTURE_3D, 0);
	glUniform1i(glGetUniformLocation(_programId, "volume"), 0);

	glDeleteVertexArrays(1, &_VA0);
	glDeleteBuffers(1, &_VB0);
	glDeleteProgram(_programId);
}

renderer::renderer()
{
	_camera = vec3(0.0f, 0.0f, 5.0f);
	_source = vec3(0.0f, 0.0f, 0.0f);
	_target = vec3(0.0f, 0.0f, 0.0f);
	_up = vec3(0.0f, 1.0f, 0.0f);
	_close = distance(_camera, _source);
}
renderer::~renderer()
{

}