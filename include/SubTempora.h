#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include "BuildManager.h"
#include <bitset>
#include "ewah/ewah.h"
#include "../include/Timer.h"
#include "../include/VF2GraphReaders.h"
#include "../include/MatchManager.h"
#include <string>

using namespace ewah;

typedef EWAHBoolArray<uint32_t> bitmap;
map<int, vector<bitset<5>>> convertGraphInfoBM(map<int, vector<int>> gI);
int main();
