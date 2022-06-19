#include "include/windows_plugin/windows_plugin.h"
// This must be included before many other Windows headers.
#include <windows.h>
// For getPlatformVersion; remove unless needed for your plugin implementation.
#include <VersionHelpers.h>
#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>
#include <flutter/standard_method_codec.h>
#include <map>
#include <memory>
#include <sstream>
#include <fpdfview.h>
#include <fstream>
 
#include "include/windows_plugin/select_file_dialog.cpp" 
#include "include/windows_plugin/print_dialog.cpp"
#include "include/windows_plugin/print_text.cpp"
#include "include/windows_plugin/print_pdf.cpp"
#include "include/windows_plugin/window_blur.cpp"
#include "include/windows_plugin/window_titlebar_color.cpp" 
using namespace std;

namespace {

  class WindowsPlugin : public flutter::Plugin 
  {
    public:
      static void RegisterWithRegistrar(flutter::PluginRegistrarWindows *registrar);

      WindowsPlugin();

      virtual ~WindowsPlugin();

    private:
      // Called when a method is called on this plugin's channel from Dart.
      void HandleMethodCall(
          const flutter::MethodCall<flutter::EncodableValue> &method_call,
          std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
  };

  flutter::PluginRegistrarWindows* registrar1;
  HWND window; 
  void WindowsPlugin::RegisterWithRegistrar(
      flutter::PluginRegistrarWindows *registrar) 
  {
    auto channel =
        std::make_unique<flutter::MethodChannel<flutter::EncodableValue>>(
            registrar->messenger(), "windows_plugin",
            &flutter::StandardMethodCodec::GetInstance());

    auto plugin = std::make_unique<WindowsPlugin>();

    channel->SetMethodCallHandler(
        [plugin_pointer = plugin.get()](const auto &call, auto result) {
          plugin_pointer->HandleMethodCall(call, std::move(result));
        });

    registrar->AddPlugin(std::move(plugin));
    registrar1 = registrar;
  }

  WindowsPlugin::WindowsPlugin() {}

  WindowsPlugin::~WindowsPlugin() {}
 
  
  void WindowsPlugin::HandleMethodCall(
      const flutter::MethodCall<flutter::EncodableValue> &method_call,
      std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
    if (method_call.method_name().compare("getPlatformVersion") == 0) {
      std::ostringstream version_stream;
      version_stream << "Windows ";
      if (IsWindows10OrGreater()) {
        version_stream << "10+";
      } else if (IsWindows8OrGreater()) {
        version_stream << "8";
      } else if (IsWindows7OrGreater()) {
        version_stream << "7";
      }
      result->Success(flutter::EncodableValue(version_stream.str()));
    } else if (method_call.method_name().compare("getPrinterList") == 0) {
      result->Success(flutter::EncodableValue(getPrinterList()));
    } else if (method_call.method_name().compare("chooseColor") == 0) { 
      if(window == NULL)
      {
        fprintf(stderr, "Window found\n");
        window = ::GetAncestor(registrar1->GetView()->GetNativeWindow(), GA_ROOT);
      }
      result->Success(flutter::EncodableValue(chooseColor(window)));
    } else if (method_call.method_name().compare("setTitleBarColor") == 0) { 
      const flutter::EncodableMap& args =
          std::get<flutter::EncodableMap>(*method_call.arguments());
      window = ::GetAncestor(registrar1->GetView()->GetNativeWindow(), GA_ROOT);
      setTitleBarColor(std::get<int>(args.at(flutter::EncodableValue("color"))), window);
      result->Success(flutter::EncodableValue("Done"));
    } else if (method_call.method_name().compare("setBlur") == 0) {  
      window = ::GetAncestor(registrar1->GetView()->GetNativeWindow(), GA_ROOT);
      setBlur(window);
      result->Success(flutter::EncodableValue("Done"));
    } else if (method_call.method_name().compare("setUnBlur") == 0) {  
      window = ::GetAncestor(registrar1->GetView()->GetNativeWindow(), GA_ROOT);
      setUnBlur(window);
      result->Success(flutter::EncodableValue("Done"));
		}
		else if (method_call.method_name().compare("printPdf") == 0)
		{
			if (!method_call.arguments())
			{
				result->Error("Bad arguments", "Expected String");
				return;
			}
			std::string filePrint, printerName, pageNumber, orientation;
			const auto* arguments = std::get_if<flutter::EncodableMap>(method_call.arguments());
			auto vl = arguments->find(flutter::EncodableValue("path"));
			auto vp = arguments->find(flutter::EncodableValue("printer"));
			auto vn = arguments->find(flutter::EncodableValue("number"));
			auto vo = arguments->find(flutter::EncodableValue("orientation"));
			if (vl != arguments->end()) {
				filePrint = std::get<std::string>(vl->second);
			}
			if (vp != arguments->end()) {
				printerName = std::get<std::string>(vp->second);
			}
			if (vn != arguments->end()) {
				pageNumber = std::get<std::string>(vn->second);
			}
			if (vo != arguments->end()) {
				orientation = std::get<std::string>(vo->second);
			}
			std::string st = printPdfFile(filePrint, printerName, pageNumber, orientation);
			flutter::EncodableValue response(st.c_str());
			result->Success(&response);
    } else if (method_call.method_name().compare("printText") == 0) {  
      const flutter::EncodableMap& args =
          std::get<flutter::EncodableMap>(*method_call.arguments());

      string printerName = get<string>(args.at(flutter::EncodableValue("printer")));
      string texttoprint = get<string>(args.at(flutter::EncodableValue("texttoprint")));
      int fontsize = get<int>(args.at(flutter::EncodableValue("fontSize")));
      string fontface = get<string>(args.at(flutter::EncodableValue("fontFace")));
      int textcolor = get<int>(args.at(flutter::EncodableValue("color")));

      printText(printerName, texttoprint, fontface, fontsize, textcolor);
      result->Success(flutter::EncodableValue("Done"));
    } else if (method_call.method_name().compare("createNewWindow") == 0) {   
      // const wchar_t* window_class =
      //     registrar1->GetWindowClass();
      // HWND window1 = 

      // flutter::PluginRegistrarManager::GetInstance()
      // CreateWindow(L"WindowsPlugin", L"test new window", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 0, 0, 600, 800, nullptr, nullptr, GetModuleHandle(nullptr), this);
      result->Success(flutter::EncodableValue("Done"));
    } else {
      result->NotImplemented();
    }
  }
}  // namespace

void WindowsPluginRegisterWithRegistrar(
    FlutterDesktopPluginRegistrarRef registrar) {
  WindowsPlugin::RegisterWithRegistrar(
      flutter::PluginRegistrarManager::GetInstance()
          ->GetRegistrar<flutter::PluginRegistrarWindows>(registrar));
}
