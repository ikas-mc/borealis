#pragma once
#include <functional>

#include <windows.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.ApplicationModel.Core.h>
#include <winrt/Windows.UI.Core.h>
#include <winrt/Windows.UI.Composition.h>
#include <winrt/Windows.UI.ViewManagement.h>
using namespace winrt;

using namespace Windows;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::Foundation::Numerics;
using namespace Windows::UI;
using namespace Windows::UI::Core;
using namespace Windows::UI::Composition;

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

    void OnPointerPressed(IInspectable const&, PointerEventArgs const& args);

    void PointerReleased(IInspectable const&, PointerEventArgs const& args);

    static int RunApp(std::function<int(int,char*[])> callback){
        winrt::init_apartment(winrt::apartment_type::multi_threaded);
        bool result = winrt::Windows::UI::ViewManagement::ApplicationViewScaling::TrySetDisableLayoutScaling(true);
        winrt::Windows::ApplicationModel::Core::CoreApplication::Run(winrt::make<WinrtApp>(callback));
        return 0;
    }

private:
    std::function<int(int,char*[])> callback;
 
};



