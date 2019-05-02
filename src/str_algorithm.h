#pragma once

#ifndef STR_ALOGIRTHM_H
#define STR_ALOGIRTHM_H

#include <algorithm>
#include <string>
#include <cmath>

namespace stralgo {

void EraseSpace(std::string &str);

void CompressSpace(const std::string &str1, std::string &str2);

void ReplaceMark(const std::string &src, std::string &dst);

bool str2int(const std::string &str, int &val);

int str2int(const std::string &str);

bool str2double(const std::string &str, double &val);

double str2double(const std::string &str);

}

#endif