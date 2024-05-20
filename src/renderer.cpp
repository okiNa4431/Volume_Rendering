#include "renderer.h"
#include "io.h"

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
	{ //エラーログ表示
		const GLsizei maxLength = 1024;
		GLsizei length;
		auto* infoLog = new GLchar[maxLength];
		glGetShaderInfoLog(shader, maxLength, &length, infoLog);
		if(infoLog) std::cout << "log: " << infoLog << std::endl;
		delete[] infoLog;
	}
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
	int programId = glCreateProgram();
	glAttachShader(programId, vShaderId);
	glAttachShader(programId, fShaderId);
	glLinkProgram(programId);
	glUseProgram(programId);

	return true;
}

bool renderer::setObject()
{
	// 頂点データ
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};

	glGenVertexArrays(1, &_VA0);
	glGenBuffers(1, &_VB0);

	// VAOとVBOの設定
	glBindVertexArray(_VA0);

	glBindBuffer(GL_ARRAY_BUFFER, _VB0);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0); // VBOのバインドを解除
	glBindVertexArray(0); // VAOのバインドを解除

	return true;
}

bool renderer::setVolume(const string& filePath)
{
	//ボリューム読み込み
	unsigned short*** CT;
	const vector<int> size = { 512, 512, 271 };
	read_volume(filePath.c_str(), CT, size);

	//3Dテクスチャ生成
	GLuint volumeId;
	glGenTextures(1, &volumeId);//テクスチャオブジェクト生成
	glBindTexture(GL_TEXTURE_3D, volumeId);//バインド
	glTexImage3D(GL_TEXTURE_3D, 0, GL_R, size[0], size[1], size[2], 0, GL_R, GL_UNSIGNED_SHORT, CT);//テクスチャ初期化、書き込み

	//ボリューム(3Dテクスチャ)転送
	

	return true;
}

void renderer::draw()
{
	glBindVertexArray(_VA0);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0); // VAOのバインドを解除
}

renderer::renderer()
{

}
renderer::~renderer()
{

}