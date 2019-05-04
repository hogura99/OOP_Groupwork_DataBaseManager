#pragma once

#ifndef STR_ALOGIRTHM_H
#define STR_ALOGIRTHM_H

#include <algorithm>
#include <string>
#include <cmath>

namespace stralgo {

// Erase the space (and newlines) of a string.
// @param str: the chosen string.
void EraseSpace(std::string &str);

// Compress the space of a string.
// @param src: the source string.
// @param dst: the string after compression.
void CompressSpace(const std::string &src, std::string &dst);

// Replace '(', ')', ',' , ';' to space of a string.
// @param src: the source string.
// @param dst: the string after transform.
void ReplaceMark(const std::string &src, std::string &dst);

// Transform a string to an int.
bool str2int(const std::string &str, int &val);

// Transform a string to an int.
int str2int(const std::string &str);

// Transform a string to a double.
bool str2double(const std::string &str, double &val);

// Transform a string to a double.
double str2double(const std::string &str);

}

#endif