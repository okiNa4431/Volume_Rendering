#include "renderer.h"
#include "io.h"

int renderer::compile_shader(const string& shader_str, const int shader_type)
{
	int shader = glCreateShader(shader_type);
	const char* shaderSource = shader_str.c_str();
	glShaderSource(shader, 1, &shaderSource, nullptr);
	glCompileShader(shader);

	//�G���[�ǂݎ��
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
	//�G���[���O�\��
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
	//�V�F�[�_�[�t�@�C���̓ǂݎ��
	string vertex_str = "";
	string fragment_str = "";
	if (!setShaderFile(vertex_str, fragment_str))
	{
		printf("could not set shader file\n");
		return false;
	}

	//�V�F�[�_�[�̃R���p�C��
	int vShaderId = compile_shader(vertex_str, GL_VERTEX_SHADER);
	int fShaderId = compile_shader(fragment_str, GL_FRAGMENT_SHADER);

	//�V�F�[�_�[�v���O�����ɃV�F�[�_�[���A�^�b�`�A�����N
	_programId = glCreateProgram();
	glAttachShader(_programId, vShaderId);
	glAttachShader(_programId, fShaderId);
	glLinkProgram(_programId);
	glUseProgram(_programId);

	return true;
}

bool renderer::setScreen()
{
	// ���_�f�[�^
	float vertices[] = {
		-1.0f, -1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		 1.0f,  1.0f, 0.0f,
	};

	glGenVertexArrays(1, &_VA0);
	glGenBuffers(1, &_VB0);

	// VAO��VBO�̐ݒ�
	glBindVertexArray(_VA0);

	glBindBuffer(GL_ARRAY_BUFFER, _VB0);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0); // VBO�̃o�C���h������
	glBindVertexArray(0); // VAO�̃o�C���h������

	return true;
}

bool renderer::setVolume(const string& filePath)
{
	//�{�����[���ǂݍ���
	const vector<int> size = { 512, 512, 271 };
	_CT = new unsigned short[size[0] * size[1] * size[2]];
	read_volume(filePath.c_str(), _CT, size);

	//3D�e�N�X�`������
	glGenTextures(1, &_volumeId);//�e�N�X�`���I�u�W�F�N�g����
	glBindTexture(GL_TEXTURE_3D, _volumeId);//�o�C���h
	glActiveTexture(GL_TEXTURE0);
	glTexImage3D(GL_TEXTURE_3D, 0, GL_RED, size[0], size[1], size[2], 0, GL_RED, GL_UNSIGNED_SHORT, _CT);//�e�N�X�`���������A��������
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	//�{�����[��(3D�e�N�X�`��)�A���̑��p�����[�^�]��
	const GLfloat camera[] = {0,0,-1};
	const GLfloat ray[] = {0,0,1};
	const float step = 1.0/271;
	glUniform1i(glGetUniformLocation(_programId, "volume"), 0);
	glUniform3fv(glGetUniformLocation(_programId, "camera"), 1, camera);
	glUniform3fv(glGetUniformLocation(_programId, "ray"), 1, ray);
	glUniform1f(glGetUniformLocation(_programId, "step"), step);

	setScreen();//��ʒ��x�̎l�p�`��`��

	return true;
}

void renderer::draw()
{
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_3D, _volumeId);//�o�C���h
	//glUniform1i(glGetUniformLocation(_programId, "volume"), 0);

	glBindVertexArray(_VA0);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void renderer::terminate()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_3D, _volumeId);//�o�C���h
	glTexImage3D(GL_TEXTURE_3D, 0, GL_RED, 512, 512, 271, 0, GL_R, GL_UNSIGNED_SHORT, _CT);//�e�N�X�`���������A��������
	glBindTexture(GL_TEXTURE_3D, 0);
	glUniform1i(glGetUniformLocation(_programId, "volume"), 0);

	glDeleteVertexArrays(1, &_VA0);
	glDeleteBuffers(1, &_VB0);
	glDeleteProgram(_programId);
}

renderer::renderer()
{

}
renderer::~renderer()
{

}