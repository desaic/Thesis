#include "ConfigFile.hpp"
#include "FileUtil.hpp"
#include <sstream>

void ConfigFile::load(const char * filename)
{
  FileUtilIn in(filename);
  if (!in.good()){
    return;
  }
  std::string line;
  while (std::getline(in.in, line)){
    std::istringstream ss(line);
    if (line.size() < 3){
      continue;
    }
    std::string token;
    ss >> token;
    if (token[0] == '#'){
      continue;
    }
    std::string val;
    ss >> val;
    m[token] = val;
    std::cout << token << ": " << m[token] << "\n";
  }
  in.close();
}

float ConfigFile::getFloat(const std::string & key)const
{
  if (!hasOpt(key)){
    return 0.0f;
  }
  return std::stof(m.find(key)->second);
}

int ConfigFile::getInt(const std::string & key)const
{
  if (!hasOpt(key)){
    return 0;
  }
  return std::stoi(m.find(key)->second);
}

std::string ConfigFile::getString(const std::string & key)const
{
  if (!hasOpt(key)){
    return std::string();
  }
  return m.find(key)->second;
}

bool ConfigFile::getBool(const std::string & key)const
{
  if (!hasOpt(key)){
    return false;
  }
  return m.find(key)->second == "true";
}

bool ConfigFile::hasOpt(const std::string & key)const
{
  return m.find(key) != m.end();
}
