/*   사용 안함   */
#pragma once
#include "Container.h"
#include "Types.h"

/*-----------------
	FileUtils
------------------*/

class FileUtils
{
public:
	static Vector<BYTE>		ReadFile(const WCHAR* path);
	static String			Convert(std::string str);
};