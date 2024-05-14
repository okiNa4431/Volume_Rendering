#pragma once
#include <iostream>
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

void read_volume(const char filename[], unsigned short*** &volume, const std::vector<int>& voxelSize)
{
	ifstream ifs(filename, ios::in | ios::binary);
	
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