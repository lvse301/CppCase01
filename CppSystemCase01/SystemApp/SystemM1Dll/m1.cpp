
#include "pch.h"
#include "m1.h"


_declspec(dllexport) int Add(int a, int b) {
	return a + b;
}

_declspec(dllexport) int Sub(int a, int b) {
	return a - b;
}


/*
创建一个文件
*/
_declspec(dllexport)  void createFile() {


    char temp;
    HANDLE h = CreateFile(L"process.txt",  // name of the file
        GENERIC_WRITE,    // open for writing
        0,                // sharing mode, none in this case
        0,                // use default security descriptor
        CREATE_ALWAYS,    // overwrite if exists
        FILE_ATTRIBUTE_NORMAL,
        0);

    if (h)
    {
        std::cout << "CreateFile() succeeded\n";
        CloseHandle(h);
    }
    else
    {
        std::cerr << "CreateFile() failed:" << GetLastError() << "\n";
    }
}
