#include "renderer.h"
#include "io.h"

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

	setCube();//�{�����[�������܂钼���̂𐶐�

	return true;
}

bool renderer::setCube()
{
	// ���_�f�[�^
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

	//�C���f�b�N�X�f�[�^
	unsigned int indices[] = {
		//�O
		0,1,2,
		1,3,2,
		//�E
		1,5,3,
		5,7,3,
		//��
		0,2,4,
		2,6,4,
		//��
		2,3,7,
		7,6,2,
		//��
		0,4,1,
		1,4,5,
		//��
		5,4,6,
		6,7,5,
	};

	glGenVertexArrays(1, &_VA0);
	glGenBuffers(1, &_VB0);
	glGenBuffers(1, &_IB0);

	//���_�z��̐ݒ�
	glBindVertexArray(_VA0);

	//���_�f�[�^
	glBindBuffer(GL_ARRAY_BUFFER, _VB0);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//�C���f�b�N�X�f�[�^
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _IB0);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

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
	fill(_camera, _camera + 3, 0.0); _camera[2] = -1.0;
	fill(_ray, _ray + 3, 0.0); _ray[2] = 1.0;
	const float step = 1.0/271;
	glUniform1i(glGetUniformLocation(_programId, "volume"), 0);
	glUniform3fv(glGetUniformLocation(_programId, "camera"), 1, _camera);
	glUniform3fv(glGetUniformLocation(_programId, "ray"), 1, _ray);
	glUniform1f(glGetUniformLocation(_programId, "step"), step);

	return true;
}

void renderer::setWorldParams(float& scrool)
{
	const GLfloat center[3] = { 0.0, 0.0, 0.0 };
	GLfloat distance_camera_center = distance(_camera, center);
	if (distance_camera_center > 0.1)
	{
		const GLfloat ray2center[3] = { center[0] - _camera[0], center[1] - _camera[1], center[2] - _camera[2] };
		for (int i = 0; i < 3;i++) _camera[i] += scrool * 0.1 * ray2center[i];
	}
	glUniform3fv(glGetUniformLocation(_programId, "camera"), 1, _camera);

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