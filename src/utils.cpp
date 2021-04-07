#pragma once

#include <string>
#include <vector>

using namespace std;

/*
 * Code extracted from
 * https://stackoverflow.com/questions/5888022/split-string-by-single-spaces
 */
size_t split(const string &txt, vector<string> &strs, char ch)
{
  size_t pos = txt.find(ch);
  size_t initialPos = 0;
  strs.clear();
  while (pos != string::npos)
  {
    strs.push_back(txt.substr(initialPos, pos - initialPos));
    initialPos = pos + 1;

    pos = txt.find(ch, initialPos);
  }
  strs.push_back(txt.substr(initialPos, min(pos, txt.size()) - initialPos + 1));
  return strs.size();
}

template <typename T>
vector<T> vec_stoi(const vector<string> vec)
{
  vector<T> output;
  for (auto it = vec.begin(); it != vec.end(); ++it)
    output.push_back((T)stoi(*it));
  return output;
}