#include "FileUtil.hpp"

std::istream & operator>> (std::istream  & in, Eigen::Vector3f & v)
{
  for(int ii= 0; ii<3; ii++){
    in>>v[ii];
  }
  return in;
}

void
FileUtilIn::readArr(std::vector<Eigen::Vector3f> & arr)
{
  for(unsigned int ii = 0;ii<arr.size();ii++){
    in >> arr[ii];
  }
}

void
FileUtilIn::open(const char * filename,std::ios_base::openmode mode )
{
  in.open(filename,mode);
  if(!in.good()){
    std::cout<<"cannot open "<<filename<<"\n";
  }
}

void FileUtilIn::close()
{
  in.close();
}

void
FileUtilIn::readFArr(std::vector<float> & arr)
{
  int size;
  in>>size;
  arr.resize(size);
  for(int ii = 0;ii<size;ii++){
    in>>arr[ii];
  }
}

void
FileUtilIn::readFArr(std::vector<float> & arr, int size)
{
  arr.resize(size);
  for(int ii = 0;ii<size;ii++){
    in>>arr[ii];
  }
}

template <typename T>
std::istream &
operator>>(std::istream & i, std::vector<T> & arr)
{
  int size = 0;
  i>>size;
  arr.resize(size);
  for(int ii = 0;ii<size;ii++){
    i>>arr[ii];
  }
}

FileUtilIn::FileUtilIn(const char * filename):in(filename)
{
  if(!in.good()){
    std::cout<<"cannot open "<<filename<<"\n";
  }
}

FileUtilIn::FileUtilIn()
{}

void
FileUtilOut::writeArr(const std::vector<Eigen::Vector3f> & arr)
{
  for(unsigned int ii = 0;ii<arr.size();ii++){
    out<<arr[ii]<<"\n";
  }
}

void
FileUtilOut::open(const char * filename, std::ios_base::openmode mode)
{
  out.open(filename,mode);
  if(!out.good()){
    std::cout<<"cannot open output"<<filename<<"\n";
  }
}

void
FileUtilOut::close()
{
  out.close();
}

FileUtilOut::FileUtilOut(const char * filename):out(filename)
{
  if(!out.good()){
    std::cout<<"cannot open output"<<filename<<"\n";
  }
}

FileUtilOut::FileUtilOut()
{}

template <typename T>
std::ostream &
operator<<(std::ostream & o, const std::vector<T> & arr)
{
  o<<arr.size()<<"\n";
  for(unsigned int ii = 0 ;ii<arr.size();ii++){
    o<<arr[ii]<<"\n";
  }
}
