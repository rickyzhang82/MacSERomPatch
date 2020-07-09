#ifndef COMMON_H
#define COMMON_H

#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>


using namespace std;

string getHomeDir();

string getCompressedDataFilePath();

shared_ptr<vector<uint8_t>> getCompressedDisptachData(string path);
#endif // COMMON_H
