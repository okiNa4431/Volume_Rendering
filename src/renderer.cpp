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
	{ //�G���[���O�\��
		const GLsizei maxLength = 1024;
		GLsizei length;
		auto* infoLog = new GLchar[maxLength];
		glGetShaderInfoLog(shader, maxLength, &length, infoLog);
		if (sizeof(infoLog) > 8) std::cout << "log: " << infoLog << std::endl;
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
	unsigned short*** CT;
	const vector<int> size = { 512, 512, 271 };
	read_volume(filePath.c_str(), CT, size);

	//3D�e�N�X�`������
	GLuint volumeId;
	glGenTextures(1, &volumeId);//�e�N�X�`���I�u�W�F�N�g����
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_3D, volumeId);//�o�C���h
	glTexImage3D(GL_TEXTURE_3D, 0, GL_R, size[0], size[1], size[2], 0, GL_R, GL_UNSIGNED_SHORT, CT);//�e�N�X�`���������A��������

	//�{�����[��(3D�e�N�X�`��)�]��
	glUniform1i(glGetUniformLocation(_programId, "volume"), 0);

	return true;
}

void renderer::draw()
{
	setScreen();
	glBindVertexArray(_VA0);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0); // VAO�̃o�C���h������
}

void renderer::terminate()
{
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