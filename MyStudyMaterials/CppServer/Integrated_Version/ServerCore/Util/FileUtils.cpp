#define _CRT_SECURE_NO_WARNINGS
#include "FileUtils.h"
#include <filesystem>
#include <fstream>
#include <cstdlib>


namespace ServerCore
{
namespace fs = std::filesystem;

Vector<BYTE> FileUtils::ReadFile(const WCHAR* path)
{
    Vector<BYTE> ret;

    fs::path filePath{path};

    const uint32 fileSize = static_cast<uint32>(fs::file_size(filePath));
    ret.resize(fileSize);

    std::basic_ifstream<BYTE> inputStream{filePath};
    inputStream.read(&ret[0], fileSize);

    return ret;
}

String FileUtils::Convert(std::string str)
{
    String ret;
    if (str.empty())
        return ret;

    // 문자열의 길이를 구합니다.
    size_t srcLen = str.size();
    size_t retLen = std::mbstowcs(nullptr, str.c_str(), 0);

    if (retLen == static_cast<size_t>(-1))
    {
        // 변환에 실패한 경우 처리 (예: 잘못된 UTF-8 입력)
        // 필요한 경우 적절한 예외 처리를 추가합니다.
        throw std::runtime_error("Invalid UTF-8 sequence");
    }
    // 변환된 문자열의 크기를 조정합니다.
    ret.resize(retLen);

    // 멀티바이트 문자열을 와이드 문자열로 변환합니다.
    std::mbstowcs(&ret[0], str.c_str(), retLen);

    return ret;
}

}
