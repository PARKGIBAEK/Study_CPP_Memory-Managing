#pragma once
#include <string>

namespace ServerCore
{


class Path
{
public:
    
    static std::string GetExecutablePath();
    static std::string GetRelativeFilePath(const std::string& _filePath);
};
}