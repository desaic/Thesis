#ifndef CONFIGFILE_HPP
#define CONFIGFILE_HPP
#include <map>
#include <string>
class ConfigFile
{
public:
  void load(const char * filename);
  float getFloat(const std::string & key)const;
  int getInt(const std::string & key)const;
  bool getBool(const std::string & key)const;
  std::string getString(const std::string & key)const;

  bool hasOpt(const std::string & key)const;

private:
  std::map<std::string, std::string> m;
};
#endif