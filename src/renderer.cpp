#include "renderer.h"
#include "io.h"
#define PI 3.141592

inline GLfloat distance(const GLfloat p0[], const GLfloat p1[])
{
	return sqrt((p0[0] - p1[0]) * (p0[0] - p1[0]) + (p0[1] - p1[1]) * (p0[1] - p1[1]) + (p0[2] - p1[2]) * (p0[2] - p1[2]));
}

vec3 arcball_vector(int x, int y, int width, int height)
{
	//マウス位置から球体上の点を計算
	vec3 point_on_ball = vec3((float)x / width * 2.0 - 1.0, (float)y / height * 2.0 - 1.0, 0.0);
	point_on_ball.y = -point_on_ball.y;
	float distance_pow2 = point_on_ball.x * point_on_ball.x + point_on_ball.y * point_on_ball.y;

	//半径1の球体を考えているので、マウス位置が1より小さければその分をz座標で調整する。
	//つまり、三平方の定理を使ってマウス位置の球体上の奥行きを計算する
	if (distance_pow2 < 1.0) point_on_ball.z = sqrt(1.0 - distance_pow2);
	//逆に、1より大きければ正規化して球体表面に投影する
	else point_on_ball = normalize(point_on_ball);

	return point_on_ball;
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

	return true;
}

bool renderer::setCube()
{
	// 頂点データ
		//ボクセルサイズに合わせて直方体の辺の長さ変更
	const float y = 0.5f * (float)_size[1] / (float)_size[0];
	const float z = 0.5f * (float)_size[2] / (float)_size[0];
	float vertices[] = {
		-0.5f, -y, -z,
		 0.5f, -y, -z,
		-0.5f,  y, -z,
		 0.5f,  y, -z,
		-0.5f, -y,  z,
	 	 0.5f, -y,  z,
		-0.5f,  y,  z,
		 0.5f,  y,  z,
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

bool renderer::setVolume(const string& filePath, int size[3])
{
	//ボリューム読み込み
	_size = {size[0], size[1], size[2]};
	_CT = new unsigned short[_size[0] * _size[1] * _size[2]];
	read_volume(filePath.c_str(), _CT, _size);

	setCube();//ボリュームが収まる直方体を生成

	//3Dテクスチャ生成
	glGenTextures(1, &_volumeId);//テクスチャオブジェクト生成
	glBindTexture(GL_TEXTURE_3D, _volumeId);//バインド
	glActiveTexture(GL_TEXTURE0);
	glTexImage3D(GL_TEXTURE_3D, 0, GL_RED, _size[0], _size[1], _size[2], 0, GL_RED, GL_UNSIGNED_SHORT, _CT);//テクスチャ初期化、書き込み
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	//定数のデータを転送
	const float step = 0.01f;
	vec3 boxSize = vec3(0.5f, 0.5f * (float)_size[1] / (float)_size[0], 0.5f * (float)_size[2] / (float)_size[0]);
	glUniform1f(glGetUniformLocation(_programId, "step"), step);
	glUniform2fv(glGetUniformLocation(_programId, "window"), 1, value_ptr(vec2(windowSize.first, windowSize.second)));
	glUniform3fv(glGetUniformLocation(_programId, "box"), 1, value_ptr(boxSize));

	return true;
}

void renderer::setWorldParams(float& scrool, int lastX, int nowX, int lastY, int nowY, bool rotateF, bool translateF)
{
	//前処理
	vec3 trg2cmr = normalize(_camera - _target);
	int manhattan_mouse = abs(lastX - nowX) + abs(lastY - nowY);

	//ワールド行列
	vec3 cameraRight = normalize(cross(_up, trg2cmr));
	vec3 cameraUp = cross(trg2cmr, cameraRight);
	mat4 world = mat4(1.0f);
		//平行移動
	if (translateF && manhattan_mouse > 1)_source += cameraRight * (float)(nowX - lastX) * 0.005f - cameraUp * (float)(nowY - lastY) * 0.005f;
	world = translate(world, _source);

	//ビュー行列
	mat4 view = lookAt(_camera, _target, _up);
		//拡大縮小
	_close += scrool * 0.1f;
	if (_close <= 0.2f) _close = 0.2f;
	_camera = _target + trg2cmr * _close;
		//回転
	if (rotateF && manhattan_mouse > 1)
	{
		//arc ball上の点を求める
		vec3 lastPoint = arcball_vector(lastX, lastY, windowSize.first, windowSize.second);
		vec3 nowPoint = arcball_vector(nowX, nowY, windowSize.first, windowSize.second);
		//二つの点を原点からのベクトルとみて、ベクトルの成す角度を求める
		//ここで、0.99fでminをとっているのは1.0fを数値誤差の関係で超えたときにangleがnanになってクラッシュするのを防ぐため
		float angle = acos(glm::min(0.99f, dot(lastPoint, nowPoint)));
		//二つのベクトルの外積から回転軸を求める。この回転軸はカメラ座標のものなので、オブジェクト座標に変換する。
		vec3 axis_cameraCoord = cross(lastPoint, nowPoint);
		mat3 cmr2obj = inverse(view * world);
		vec3 axis_objectCoord = cmr2obj * axis_cameraCoord;
		//変換後の軸で回転行列を計算し、カメラを回転させる。視線ベクトルが上ベクトルと同じもしくは反対の方向を向いていたら挙動が不安定になるので制限をかける
		//angleを3で割っているのは感度が高くなっているから。本当はそのままでいいはずなので、やや無理やりな解決をしている。
		vec3 newCamera = rotate(mat4(1.0f), -angle/3.0f, axis_objectCoord) * vec4(_camera, 1.0f);
		if (abs(dot(_up, normalize(_target-newCamera))) < 0.99f) _camera = newCamera;
	}
	view = lookAt(_camera, _target, _up);

	//プロジェクション行列
	mat4 proj = perspective(radians(45.0f), (float)windowSize.first / (float)windowSize.second, 0.1f, 100.0f);

	//転送
	glUniformMatrix4fv(glGetUniformLocation(_programId, "MVP"), 1, GL_FALSE, value_ptr(proj*view*world));

	//フラグメントシェーダーで使う用の変数を送る
	vec4 camera_proj = proj * view * world * vec4(_camera, 1.0f);
	vec4 ray_proj = proj * view * world * vec4(-trg2cmr, 1);
	glUniform3fv(glGetUniformLocation(_programId, "camera"), 1, value_ptr(_camera));
	glUniform3fv(glGetUniformLocation(_programId, "ray"), 1, value_ptr(-trg2cmr));
	glUniform3fv(glGetUniformLocation(_programId, "up"), 1, value_ptr(cameraUp));
	glUniform3fv(glGetUniformLocation(_programId, "right"), 1, value_ptr(cameraRight));
	glUniform3fv(glGetUniformLocation(_programId, "source"), 1, value_ptr(_source));

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
	glTexImage3D(GL_TEXTURE_3D, 0, GL_RED, _size[0], _size[1], _size[2], 0, GL_R, GL_UNSIGNED_SHORT, _CT);//テクスチャ初期化、書き込み
	glBindTexture(GL_TEXTURE_3D, 0);
	glUniform1i(glGetUniformLocation(_programId, "volume"), 0);

	glDeleteVertexArrays(1, &_VA0);
	glDeleteBuffers(1, &_VB0);
	glDeleteProgram(_programId);
}

renderer::renderer(pair<int, int> window)
{
	_camera = vec3(0.0f, 0.0f, 2.0f);
	_source = vec3(0.0f, 0.0f, 0.0f);
	_target = vec3(0.0f, 0.0f, 0.0f);
	_up = vec3(0.0f, 1.0f, 0.0f);
	_close = distance(_camera, _source);
	windowSize = window;
}
renderer::renderer()
{
}
renderer::~renderer()
{

}