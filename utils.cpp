#include "common.h"

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		printf("Expect file path as parm!\n");
		exit(1);
	}
	auto pDataVec = getCompressedDisptachData(string(argv[1]));
	if (nullptr == pDataVec)
	{
		printf("Failed to read compressed data file: %s.\n", getCompressedDataFilePath().c_str());
		return 1;
	}

	int index = 0;
	for (auto it = pDataVec->begin(); it != pDataVec->end(); it++)
	{
		printf("%02X", *it);
		if (3 == index % 4)
			printf(" ");
		if (15 == index % 16)
			printf("\n");
		index++;
	}
	return 0;
}
