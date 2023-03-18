#pragma once
#include<iostream>
#include<unordered_map>
#include<list>
#include<vector>
#include <map>
#include "HRTree.h"
#include "Timer.h"

using namespace std;

class HRTreeIVSPFor {
public:
    Timer timer;
    double compressionTime = 0;
    double conversionTime = 0;
    vector<double> times;
};