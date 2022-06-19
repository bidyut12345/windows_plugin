#include <map>
#include <memory>
#include <sstream> 
#include <fstream>
#include <windows.h>

using namespace std;
namespace 
{ 
      struct ACCENTPOLICY
    {
        int nAccentState;
        int nFlags;
        int nColor;
        int nAnimationId;
    };
    struct WINCOMPATTRDATA
    {
        int nAttribute;
        PVOID pData;
        ULONG ulDataSize;
    };

    void setBlur(HWND window)
    { 
        //  = ::GetAncestor(registrar1->GetView()->GetNativeWindow(), GA_ROOT);
        HMODULE user32_module = LoadLibraryA("User32.dll");

        using SetWindowCompositionAttribute = int __stdcall(HWND hwnd, WINCOMPATTRDATA* pCompData);
        auto func_SetWindowCompositionAttribute =
            reinterpret_cast<SetWindowCompositionAttribute*>(
                GetProcAddress(user32_module, "SetWindowCompositionAttribute"));
        if (func_SetWindowCompositionAttribute != nullptr)
        { 
            // Acrylic  blur windows data
            ACCENTPOLICY policy = { 3, 0, 0, 0 }; // ACCENT_ENABLE_BLURBEHIND=3...
            WINCOMPATTRDATA data = { 19, &policy, sizeof(ACCENTPOLICY) }; // WCA_ACCENT_POLICY=19
            func_SetWindowCompositionAttribute(window, &data);

            FreeLibrary(user32_module);
        } 
    }//setBlur
    void setUnBlur(HWND window)
    { 
        // HWND window = ::GetAncestor(registrar1->GetView()->GetNativeWindow(), GA_ROOT);
        HMODULE user32_module = LoadLibraryA("User32.dll");

        using SetWindowCompositionAttribute = int __stdcall(HWND hwnd, WINCOMPATTRDATA* pCompData);
        auto func_SetWindowCompositionAttribute =
            reinterpret_cast<SetWindowCompositionAttribute*>(
                GetProcAddress(user32_module, "SetWindowCompositionAttribute"));
        if (func_SetWindowCompositionAttribute != nullptr)
        { 
            // Acrylic  blur windows data
            ACCENTPOLICY policy = { 0, 0, 0, 0 }; // ACCENT_ENABLE_BLURBEHIND=3...
            WINCOMPATTRDATA data = { 19, &policy, sizeof(ACCENTPOLICY) }; // WCA_ACCENT_POLICY=19
            func_SetWindowCompositionAttribute(window, &data);

            FreeLibrary(user32_module);
        } 
    }//setBlur 
}


