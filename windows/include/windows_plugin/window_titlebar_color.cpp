#include <map>
#include <memory>
#include <sstream> 
#include <fstream>
#include <windows.h>

using namespace std;

namespace 
{ 
    void setTitleBarColor(int color, HWND window)
    {
        // window = ::GetAncestor(registrar1->GetView()->GetNativeWindow(), GA_ROOT);
        HMODULE dwmapi_module = LoadLibraryA("dwmapi.dll"); 

        using DwmSetWindowAttribute = int __stdcall(HWND hwnd, int attr, int* attrValue, int attrSize);
        auto func_DwmSetWindowAttribute =
            reinterpret_cast<DwmSetWindowAttribute*>(
                GetProcAddress(dwmapi_module, "DwmSetWindowAttribute")); 

        if (func_DwmSetWindowAttribute != nullptr)
        {  
            func_DwmSetWindowAttribute(window, (int)34, &color, sizeof(color)); 
            func_DwmSetWindowAttribute(window, (int)35, &color, sizeof(color)); 
            FreeLibrary(dwmapi_module);
        } 
    }
}


