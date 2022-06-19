
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
    std::string printPdfFile(std::string fileprint, std::string printerName, std::string pageNumber, std::string orientation) 
    { 
        if(fileprint == "")
        {
            fileprint = openFileDialog();
        }
        FPDF_LIBRARY_CONFIG config;
        config.version = 2;
        config.m_pUserFontPaths = NULL;
        config.m_pIsolate = NULL;
        config.m_v8EmbedderSlot = 0;
        double page_width, page_height;
        int size_x, size_y, logpixelsx, logpixelsy, _orientation;
        _orientation = orientation == "LANDSCAPE_PAGE" ? 1 : 0;
        FPDF_InitLibraryWithConfig(&config);

        FPDF_DOCUMENT doc;
        ifstream ifile;
        ifile.open(fileprint);
        if (ifile) { 
            fprintf(stderr, "file exists\n");
        }
        else { 
            fprintf(stderr, "file doesn't exist"); 
            return "-1";
        }
        try
        {
            doc = FPDF_LoadDocument(fileprint.c_str(), NULL);
        }
        catch (const std::exception&)
        {
            fprintf(stderr, "DOC OPEN ERROR\n"); 
            return "-2";
        }

        if (doc == NULL) {
            unsigned long err = FPDF_GetLastError();
            fprintf(stderr, "Load pdf docs unsuccessful: ");
            switch (err) {
                case FPDF_ERR_SUCCESS:
                    fprintf(stderr, "Success");
                    break;
                case FPDF_ERR_UNKNOWN:
                    fprintf(stderr, "Unknown error");
                    break;
                case FPDF_ERR_FILE:
                    fprintf(stderr, "File not found or could not be opened");
                    break;
                case FPDF_ERR_FORMAT:
                    fprintf(stderr, "File not in PDF format or corrupted");
                    break;
                case FPDF_ERR_PASSWORD:
                    fprintf(stderr, "Password required or incorrect password");
                    break;
                case FPDF_ERR_SECURITY:
                    fprintf(stderr, "Unsupported security scheme");
                    break;
                case FPDF_ERR_PAGE:
                    fprintf(stderr, "Page not found or content error");
                    break;
                default:
                    fprintf(stderr, "Unknown error %ld", err);
            }
            fprintf(stderr, ".\n");
            // fileprint.append(" DOC NULL")
            return "-3";
        } 
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
            return "-4";
        }
        DOCINFO doc_info = { sizeof(DOCINFO) };
        StartDoc(hDC, &doc_info);

        int pageCount = FPDF_GetPageCount(doc);
        if (pageNumber == "ALL_PAGES") {
            for (int i = 0; i < pageCount; i++) {
                StartPage(hDC);
                FPDF_PAGE pdf_page = FPDF_LoadPage(doc, i);
                if (pdf_page == NULL) return "-5";
                page_width = FPDF_GetPageWidth(pdf_page);;
                page_height = FPDF_GetPageHeight(pdf_page);
                logpixelsx = GetDeviceCaps(hDC, LOGPIXELSX);
                logpixelsy = GetDeviceCaps(hDC, LOGPIXELSY);
                size_x = (int)page_width / 72 * logpixelsx;
                size_y = (int)page_height / 72 * logpixelsy;
                //int start_y = i * size_y;
                FPDF_RenderPage(hDC, pdf_page, 0, 0, size_x, size_y, _orientation, 0);
                FPDF_ClosePage(pdf_page);
                EndPage(hDC);
            }
        }
        else {
            StartPage(hDC);
            FPDF_PAGE pdf_page = FPDF_LoadPage(doc, std::stoi(pageNumber.c_str()));
            if (pdf_page == NULL) return "-5";
            page_width = FPDF_GetPageWidth(pdf_page);;
            page_height = FPDF_GetPageHeight(pdf_page);
            logpixelsx = GetDeviceCaps(hDC, LOGPIXELSX);
            logpixelsy = GetDeviceCaps(hDC, LOGPIXELSY);
            size_x = (int)page_width / 72 * logpixelsx;
            size_y = (int)page_height / 72 * logpixelsy;
            FPDF_RenderPage(hDC, pdf_page, 0, 0, size_x, size_y, _orientation, 0);
            FPDF_ClosePage(pdf_page);
            EndPage(hDC);
        }

        EndDoc(hDC);
        DeleteDC(hDC);
        FPDF_CloseDocument(doc);
        // wprintf(L"PDF Success\n");
        fprintf(stderr, "PDF Success\n");

        FPDF_DestroyLibrary();
        return "1";

    } 
}