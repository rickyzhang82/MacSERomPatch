#include "common.h"

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

shared_ptr<vector<uint8_t>> getCompressedDisptachData(string filePath)
{
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
#ifdef DEBUG
			cout << "Reading " << length << " bytes... " << endl;
#endif
			// read data as a block:
			is.read (buffer,length);
#ifdef DEBUG
			if (is)
						std::cout << "all characters read successfully." << endl;
			else
						std::cout << "error: only " << is.gcount() << " could be read";
#endif
			auto pVec = make_shared<vector<uint8_t>>();
			for (int i = 0; i < length; i++)
			{
						pVec->push_back(static_cast<uint8_t>(buffer[i]));
			}
			delete[] buffer;
			is.close();
			return pVec;

}
