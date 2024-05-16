#pragma once
#include <iostream>
#include <vector>
#include <iostream>
#include <fstream>
#include <filesystem>

using namespace std;

void read_volume(const char filename[], unsigned short*** &volume, const std::vector<int>& voxelSize)
{
	ifstream ifs(filename, ios::in | ios::binary);
	volume = new unsigned short** [voxelSize[2]];
	for (int i = 0; i < voxelSize[2]; i++)
	{
		volume[i] = new unsigned short*[voxelSize[1]];
		for (int j = 0; j < voxelSize[1]; j++)
		{
			volume[i][j] = new unsigned short[voxelSize[0]];
			ifs.read(reinterpret_cast<char*>(volume[i][j]), sizeof(unsigned short) * voxelSize[0]);
		}
	}

	ifs.close();
}

string read_shader(const char filename[])
{
	const string currentPath_str = filesystem::current_path().string();
	const filesystem::path trueCurrentPath = currentPath_str.substr(0, currentPath_str.find("Volume_Rendering")+17);
	const filesystem::path absolutePath = trueCurrentPath / "src" / "shader" / filename;
	const string path_str = absolutePath.string();
	cout << path_str << endl;

	ifstream ifs(absolutePath, ios::in);
	istreambuf_iterator<char> shaderBegin(ifs);
	istreambuf_iterator<char> shaderEnd;
	return { shaderBegin, shaderEnd };
}