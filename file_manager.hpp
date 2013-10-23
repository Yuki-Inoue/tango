#ifndef YUKI_INOUE_FILE_MANAGER_HPP
#define YUKI_INOUE_FILE_MANAGER_HPP

#include <string>
#include <fstream>


class RawOutputArchive {
  std::ostream *os_;
public:
  RawOutputArchive(std::ostream &os) : os_(&os) {}
  template <class T>
  RawOutputArchive &operator<<(const T &arg) {
    *os_ << arg; return *this;
  }
};

class RawInputArchive {
  std::istream *is_;
public:
  RawInputArchive(std::istream &is) : is_(&is) {}
  template <class T>
  RawInputArchive &operator>>(T &arg){
    *is_ >> arg; return *this;
  }
};


// class for managing file
// given content, wraps it, and provides additional write and read and changeFilename function.
template <class Content, class InputArchive = RawInputArchive, class OutputArchive = RawOutputArchive>
class FileManager : public Content {
  std::string filename_;
  //constructor
public:
  explicit FileManager(const std::string &str) : filename_(str) {}
private:
  FileManager();
  // operations
public:
  // read/write returns boolean of success or not in read/write-ing
  bool write();
  bool read();
  void changeFilename(const std::string &str){ filename_ = str; }
};

template <class Content, class I, class OutputArchive>
bool FileManager<Content,I,OutputArchive>::write() {
  std::ofstream ofs(filename_.c_str());
  if(ofs.fail())
    return false;

  OutputArchive ar(ofs);
  ar << *static_cast<Content *>(this);
  ofs.close();
  return true;
}

template <class Content, class InputArchive, class O>
bool FileManager<Content,InputArchive,O>::read() {
  std::ifstream ifs(filename_.c_str());
  if(ifs.fail())
    return false;

  InputArchive ar(ifs);
  ar >> *static_cast<Content *>(this);
  ifs.close();
  return true;
}

#endif
