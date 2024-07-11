/*   사용 안함   */
#pragma once
#include "../Memory/Container.h"
#include "../Core/Types.h"


namespace ServerCore
{
class FileUtils
{
public:
    static Vector<BYTE> ReadFile(const WCHAR* path);
    static String Convert(std::string str);
};
}
