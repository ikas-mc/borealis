#pragma once
#include <functional>

#include <windows.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.ApplicationModel.Core.h>
#include <winrt/Windows.UI.Core.h>
#include <winrt/Windows.UI.ViewManagement.h>
#include <winrt/Windows.ApplicationModel.Activation.h>
#include <winrt/Windows.Foundation.Collections.h>
using namespace winrt;

using namespace Windows;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::UI::ViewManagement;
using namespace Windows::UI::Core;
struct WinrtApp : implements<WinrtApp, IFrameworkViewSource, IFrameworkView>
{
public:
    WinrtApp(std::function<int(int,char*[])> callback);

    IFrameworkView CreateView();

    void Initialize(CoreApplicationView const&);

    void Load(hstring const&);

    void Uninitialize();

    void Run();

    void SetWindow(CoreWindow const& window);

    void OnActivated (CoreApplicationView const&, IActivatedEventArgs const& args);

    static int RunApp(std::function<int(int,char*[])> callback){
        winrt::init_apartment(winrt::apartment_type::multi_threaded);
        bool result = ::ApplicationViewScaling::TrySetDisableLayoutScaling(true);
        CoreApplication::Run(winrt::make<WinrtApp>(callback));
        return 0;
    }

private:
    std::function<int(int,char*[])> callback;
    std::string commandLine;
};



