#include <map>
#include <memory>
#include <sstream>
#include <fpdfview.h>
#include <fstream>
#include <windows.h>

using namespace std;
namespace 
{ 
    HDC openPrinterDialog()
    {
        PRINTDLG pdlg;
        // PDEVMODE returnedDevmode;

        /*
            * XXX - can this be done without a Windows print dialog?
            *
            * "CreateDC()" creates a device context, and you can
            * apparently specify WINSPL16 as the driver name on
            * Windows OT, or the name of a "print provider", such as
            * "WINSPOOL" on Windows NT, to get a context for a printer.
            *
            * The device name would be the printer name as shown by the
            * Print Manager; is there a way to enumerate those?
            */

        /* Initialize the PRINTDLG structure. */
        memset(&pdlg, 0, sizeof(PRINTDLG));
        pdlg.lStructSize = sizeof(PRINTDLG);
        /* Set the flag to return printer DC. */
        pdlg.Flags =
            PD_RETURNDC |           /* return the device context we need */
            PD_NOPAGENUMS |         /* disable the "Pages" radio button */
            PD_NOSELECTION |        /* disable the "Selection" radio button */
            PD_USEDEVMODECOPIESANDCOLLATE;  /* let device print multiple pages */

        /* Invoke the printer dialog box. */
        if (PrintDlg(&pdlg)) {
            return pdlg.hDC;
        }
        return (HDC)0;
    }
    

    wchar_t* strToLPSTR(string str)
    {
        wchar_t* wString = new wchar_t[4096];
        MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, wString, 4096);
        return wString;
    }

    COLORREF getColorRef(int color)
    {
        int b = (int)floor(color / 65536);
        color -= b * 65536;
        int g = (int)floor(color / 256);
        color -= g * 256;
        int r = color; 
        return RGB(r,g,b);
    }
    std::string getPrinterList() 
    {
        std::string listPrinter = "";
        PRINTER_INFO_2* list;
        DWORD            cnt = 0;
        DWORD            sz = 0;
        DWORD Level = 2;
        int            i;
        int            sl;

        EnumPrinters(PRINTER_ENUM_LOCAL | PRINTER_ENUM_CONNECTIONS, NULL, Level, NULL, 0, &sz, &cnt);

        if ((list = (PRINTER_INFO_2*)malloc(sz)) == 0)    return 0;

        if (!EnumPrinters(PRINTER_ENUM_LOCAL | PRINTER_ENUM_CONNECTIONS, NULL, Level, (LPBYTE)list, sz, &sz, &cnt))
        {
        free(list);
        return 0;
        }

        for (i = 0, sl = 0; i < (int)cnt; i++)
        {

        std::string strTo;
        LPWSTR printerName = list[i].pPrinterName;
        char* p = 0;
        int bsz;
        UINT codepage = CP_ACP;
        bsz = WideCharToMultiByte(codepage, 0, printerName, -1, 0, 0, 0, 0);
        if (bsz > 0) {
            p = new char[bsz];
            int rc = WideCharToMultiByte(codepage, 0, printerName, -1, p, bsz, 0, 0);
            if (rc != 0) {
            p[bsz - 1] = 0;
            strTo = p;
            }
        }
        delete[] p;
        if (listPrinter == "") {
            listPrinter.append(strTo);
        }
        else {
            listPrinter.append(";").append(strTo);
        }

        }
        return listPrinter;
    }//getPrinterList 

    std::string chooseColor(HWND window)
    {
        // window = ::GetAncestor(registrar1->GetView()->GetNativeWindow(), GA_ROOT);
        CHOOSECOLOR cc;                 // common dialog box structure 
        static COLORREF acrCustClr[16]; // array of custom colors 
        HWND hwnd = window;                     // owner window
        HBRUSH hbrush;                  // brush handle
        static DWORD rgbCurrent;        // initial color selection

        // Initialize CHOOSECOLOR 
        ZeroMemory(&cc, sizeof(cc));
        cc.lStructSize = sizeof(cc);
        cc.hwndOwner = hwnd; 
        cc.lpCustColors = (LPDWORD) acrCustClr;
        cc.rgbResult = rgbCurrent;
        cc.Flags = CC_FULLOPEN | CC_RGBINIT;
        
        if (ChooseColor(&cc)==TRUE) 
        {
            hbrush = CreateSolidBrush(cc.rgbResult);
            rgbCurrent = cc.rgbResult; 
        } 

        std::ostringstream color;

        color << rgbCurrent;

        return color.str();
    }//chooseColor


}


