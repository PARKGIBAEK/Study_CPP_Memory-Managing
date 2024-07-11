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

    // ���ڿ��� ���̸� ���մϴ�.
    size_t srcLen = str.size();
    size_t retLen = std::mbstowcs(nullptr, str.c_str(), 0);

    if (retLen == static_cast<size_t>(-1))
    {
        // ��ȯ�� ������ ��� ó�� (��: �߸��� UTF-8 �Է�)
        // �ʿ��� ��� ������ ���� ó���� �߰��մϴ�.
        throw std::runtime_error("Invalid UTF-8 sequence");
    }
    // ��ȯ�� ���ڿ��� ũ�⸦ �����մϴ�.
    ret.resize(retLen);

    // ��Ƽ����Ʈ ���ڿ��� ���̵� ���ڿ��� ��ȯ�մϴ�.
    std::mbstowcs(&ret[0], str.c_str(), retLen);

    return ret;
}

}
