#ifndef FILEUTIL_HPP
#define FILEUTIL_HPP
#include <vector>
#include <iostream>
#include <fstream>
#include "Eigen/Dense"
class FileUtilIn
{
public:
  FileUtilIn();
  FileUtilIn(const char * filename);
  void open(const char * filename, std::ios_base::openmode mode= std::ios_base::in);
  void close();
  void readFArr(std::vector<float> & arr);
  void readFArr(std::vector<float> & arr, int size);
  void readArr(std::vector<Eigen::Vector3f> & arr);
  bool good(){ return in.good(); }
  template <typename T>
  friend std::istream & operator>>(std::istream & ii, std::vector<T> & arr);
  std::ifstream in;
};

class FileUtilOut
{
public:

  FileUtilOut();
  FileUtilOut(const char * filename);
  void open(const char * filename,std::ios_base::openmode mode = std::ios_base::out);
  void close();

  template <typename T>
  friend std::ostream & operator<<(std::ostream & o, const std::vector<T> & arr);

  void writeArr(const std::vector<Eigen::Vector3f> & arr);
  std::ofstream out;
};

#endif // FILEUTIL_HPP
