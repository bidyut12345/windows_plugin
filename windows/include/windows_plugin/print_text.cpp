#include <map>
#include <memory>
#include <sstream>
#include <fpdfview.h>
#include <fstream>
#include <math.h>
#include <windows.h> 

using namespace std;
namespace 
{  

    void printText(string printerName, string texttoprint, string fontface, int fontSize, int color)
    {
        HDC hDC;
         
        if(printerName == "")
        {
            hDC = openPrinterDialog();
        }
        else
        { 
            hDC = CreateDC(L"WINSPOOL", strToLPSTR(printerName), NULL, NULL);
        } 
        if (hDC == NULL) { 
            fprintf(stderr, "Printer error\n");
            return;
        }

        DOCINFO doc_info = { sizeof(DOCINFO) }; 
        StartDoc(hDC, &doc_info);
        StartPage(hDC);
        long lfHeight; 
        lfHeight = -MulDiv(fontSize, GetDeviceCaps(hDC, LOGPIXELSY), 72); 
        HFONT hf = CreateFont(lfHeight, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, strToLPSTR(fontface));
        SelectObject(hDC, hf);
        // COLORREF colorRed = RGB(255, 0, 0);
        // COLORREF oldTextColor = SetTextColor(hDC, colorRed); 
        SetTextColor(hDC, getColorRef(color));       
        int topValue = 10;
        for(int i = 0; i<=100; i++)
        {
            TextOut (hDC, 10, topValue, strToLPSTR(texttoprint), (int)strlen(texttoprint.c_str()));  
            topValue += (int)(fontSize * 9.5);
        }
        EndPage(hDC);
        EndDoc(hDC);
        DeleteDC(hDC);
        fprintf(stderr, "TEXT PRINTED");  
    }
}


