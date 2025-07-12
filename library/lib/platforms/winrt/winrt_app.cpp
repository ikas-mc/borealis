/*
    Copyright 2025 ikas-mc

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#include <borealis/platforms/winrt/winrt_app.hpp>

using namespace winrt;

using namespace Windows;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::Foundation::Numerics;
using namespace Windows::UI;
using namespace Windows::UI::Core;
using namespace Windows::UI::Composition;

WinrtApp::WinrtApp(std::function<int(int,char*[])> callback)
{
    this->callback = callback;
}

IFrameworkView WinrtApp::CreateView()
{
    return *this;
}

void WinrtApp::Initialize(CoreApplicationView const&)
{
}

void WinrtApp::Load(hstring const&)
{
}

void WinrtApp::Uninitialize()
{
}

void WinrtApp::Run()
{
    CoreWindow window = CoreWindow::GetForCurrentThread();
    window.Activate();
    const char* argv[] = { "winrt" };//TODO 
    callback(0, const_cast<char**>(argv));
}

void WinrtApp::SetWindow(CoreWindow const& window)
{
}

void WinrtApp::OnPointerPressed(IInspectable const&, PointerEventArgs const& args)
{
}

void WinrtApp::PointerReleased(IInspectable const&, PointerEventArgs const& args)
{
}
