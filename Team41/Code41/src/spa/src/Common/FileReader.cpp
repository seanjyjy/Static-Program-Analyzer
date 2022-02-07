#include <fstream>
#include <string>
#include <cerrno>

#include "FileReader.h"

using namespace std;

// Reference from https://insanecoding.blogspot.com/2011/11/how-to-read-in-file-in-c.html
string FileReader::getFileContent(string &path) {
    std::ifstream in(path, ios::in | ios::binary);

    if (in) {
        std::string contents;
        in.seekg(0, ios::end);
        contents.resize(in.tellg());
        in.seekg(0, ios::beg);
        in.read(&contents[0], contents.size());
        in.close();
        return(contents);
    }
    throw(errno);
}
