#include <map>
#include <memory>
#include <sstream>
#include <fpdfview.h>
#include <fstream>
#include <windows.h>
#include <atlstr.h>

using namespace std;
namespace 
{ 
    string openFileDialog()
    {

        OPENFILENAME l={sizeof(l),};
        TCHAR buf[1024];
        // l.hwndOwner = hwndDlg;
        l.lpstrFilter = L"PDF Files\0*.pdf\0All Files\0*.*\0";
        l.lpstrFile = buf;
        l.nMaxFile = 1023;
        l.lpstrTitle = L"Open PDF File";
        l.lpstrDefExt = L"pdf";
        l.lpstrInitialDir = NULL;
        l.Flags = OFN_HIDEREADONLY|OFN_EXPLORER|OFN_PATHMUSTEXIST;
        buf[0]=0;
        if (GetOpenFileName(&l))
        {
            // MessageBox ( NULL , l.lpstrFile , L"File Name" , MB_OK);
            //   SetZip(hwndDlg,buf);
            // stringstream filename;
            // filename << l.lpstrFile;
            fprintf(stderr, "File name is : "); 
            string fff = CW2A(l.lpstrFile);
            fprintf(stderr, fff.c_str()); 
            return fff;
        }
        else{
            return "";
        }




        // // GetOpenFileName(NULL);
        // OPENFILENAME open;
        // ZeroMemory(&open, sizeof(open));

        // open.lStructSize = sizeof(open);
        // open.lpstrFilter = L"Képek\0*.jpg;*.jpeg;*.gif;*.png;*.bmp\0\0";
        // open.nFileOffset = 1;
        // open.lpstrFile[0] = '\0';
        // open.nMaxFile = 2048;
        // open.lpstrTitle = L"Képek kiválasztása..";
        // open.Flags = OFN_ALLOWMULTISELECT | OFN_FILEMUSTEXIST;

        // if(GetOpenFileName(&open))
        // {

        // }



            // open a file name
// Gobal Variables and declarations.
// 
// OPENFILENAME ofn ;


// // a another memory buffer to contain the file name
// // char szFile[100] ;
// 	ZeroMemory( &ofn , sizeof( ofn));
// 	ofn.lStructSize = sizeof ( ofn );
// 	ofn.hwndOwner = NULL  ;
// 	ofn.lpstrFile = L"" ;
// 	ofn.lpstrFile[0] = '\0';
// 	ofn.nMaxFile = 0;
// 	ofn.lpstrFilter = L"All\0*.*\0Text\0*.TXT\0";
// 	ofn.nFilterIndex =1;
// 	ofn.lpstrFileTitle = NULL ;
// 	ofn.nMaxFileTitle = 0 ;
// 	ofn.lpstrInitialDir=NULL ;
// 	ofn.Flags = OFN_PATHMUSTEXIST|OFN_FILEMUSTEXIST ;

// 	GetOpenFileName( &ofn );
	
// 	// Now simpley display the file name 
// 	MessageBox ( NULL , ofn.lpstrFile , L"File Name" , MB_OK);
    }
     

}


