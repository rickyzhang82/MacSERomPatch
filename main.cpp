#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>


using namespace std;

const string REL_FILE_PATH = "/repo/github/qt-MacSERomPatch/MacSERomPatch/MacSE-Dispatch-Table.compressed";

string getHomeDir()
{
	struct passwd *pw = getpwuid(getuid());
	return string(pw->pw_dir);
}

string getCompressedDataFilePath()
{
	string filePath(getHomeDir());
	filePath +=REL_FILE_PATH;
	return filePath;
}

shared_ptr<vector<uint8_t>> getCompressedDisptachData()
{
	auto filePath = getCompressedDataFilePath();
	fstream is(filePath.c_str(), ios::in | ios::binary);
	if (!is)
	{
		return nullptr;
	}
	// get length of file:
	is.seekg (0, is.end);
	int length = is.tellg();
	is.seekg (0, is.beg);

	char * buffer = new char [length];

	cout << "Reading " << length << " bytes... " << endl;
	// read data as a block:
	is.read (buffer,length);

	if (is)
		std::cout << "all characters read successfully." << endl;
	else
		std::cout << "error: only " << is.gcount() << " could be read";

	auto pVec = make_shared<vector<uint8_t>>();
	for (int i = 0; i < length; i++)
	{
		pVec->push_back(static_cast<uint8_t>(buffer[i]));
	}
	delete[] buffer;
	is.close();
	return pVec;

}

uint32_t readOneByte(uint32_t d1, shared_ptr<vector<uint8_t>> pVec, int& index)
{
	d1 = d1 & 0xFFFFFF00; // clear lower one byte
	d1 = d1 + pVec->at(index++);
	return d1;
}

void printTraps(vector<uint32_t>& traps, const char* msg)
{
	cout << msg << endl;
	int index = 0;
	for (auto it = traps.begin(); it != traps.end(); it++)
	{
		printf("%08X ", *it);
		if (3 == index % 4)
			printf("\n");
		++index;
	}
	cout << "END!" << endl << endl;
}

void initDispatchTable(shared_ptr<vector<uint8_t>> pVec)
{
	const uint32_t ROM_BASE = 0x400000;
	uint32_t d0 = 0; // counter
	uint32_t d1 = 0; // dispatch address
	uint32_t d2 = 0; // bitmask
	uint32_t a0 = 0; // compressed table address (source)
	uint32_t a1 = 0; // dispatch table address (target)
	uint32_t a2 = ROM_BASE; // ROM base address

	const uint32_t UNIMPLEMNTED_TRAP_ADDR = 0x400768;
	vector<uint32_t> osTraps;
	vector<uint32_t> toolboxTraps;
	const int numOSTraps = 0x100;
	const int numToolBoxTraps = 0x200;
	bool isToolBoxTraps = true;

	// initialize trap with unimplemented trap
	for (int i = 0; i < numOSTraps; i++)
		osTraps.push_back(UNIMPLEMNTED_TRAP_ADDR);

	for (int i = 0; i < numToolBoxTraps; i++)
		toolboxTraps.push_back(UNIMPLEMNTED_TRAP_ADDR);

	d0 = 0x200;
	d2 = 0x7F;

	int index = 0;
	while(1)
	{
		//loc 710
		d1 = readOneByte(d1, pVec, index);
		if (d1 >= 0x80)
		{
			d1 = d1 & d2;
			if (0 == d1)
			{
				// skip
				goto Next_Address;
			} else
			{
				// d1 != 0x80
				if (d1 == 0x7F)
				{
					for (int i = 0; i < 4; i++)
					{
						d1 = d1 << 8;
						d1 = readOneByte(d1, pVec, index);
					}
					a2 = ROM_BASE;
					goto Update_Address;

				} else
				{
					//d1 != 0x7F
					d1 = d1 + d1;
					goto Update_Address;
				}
			}
		} else
		{
			// d1 < 0x80
			d1 = d1 << 8;
			d1 = readOneByte(d1, pVec, index);
			d1 = d1 + d1;

			// 15th bit is 0
			if ((d1 & 0x8000) == 0)
			{
				d1 = d1 & 0x0000FFFF;
			} else
			{
				d1 = d1 | 0xFFFF0000;
			}

			if (0 == d1)
			{
				//exit the loop
				break;
			} else
			{
				goto Update_Address;
			}
		}
//loc_71E
Update_Address:
		a2 = a2 + d1;
		if (isToolBoxTraps)
		{
			if (a1/4 >= toolboxTraps.size())
				break;
			toolboxTraps.at(a1/4)=a2;
		} else
		{
			if (a1/4 >= osTraps.size())
				break;
			osTraps.at(a1/4) = a2;
		}
		//loc_722
Next_Address:
		d0 = d0 - 1;
		a1 = a1 + 4;
		if (d0 == 0)
		{
			a1 = 0;
			d0 = 0xFFFFFFFF;
			isToolBoxTraps = false;
		}
	}// end while

	//print traps
	printTraps(toolboxTraps, "ToolBox Traps");
	printTraps(osTraps, "OS Traps");
}

int main()
{
	auto pDataVec = getCompressedDisptachData();
	if (nullptr == pDataVec)
	{
		printf("Failed to read compressed data file: %s.\n", getCompressedDataFilePath().c_str());
		return 1;
	}
	initDispatchTable(pDataVec);
	return 0;
}

