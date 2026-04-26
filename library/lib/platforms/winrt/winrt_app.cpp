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
#include <winrt/Windows.Storage.h>
#include <winrt/windows.storage.accesscache.h>

#include <borealis/platforms/winrt/winrt_app.hpp>

using namespace winrt;

using namespace Windows;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::Storage::AccessCache;

WinrtApp::WinrtApp(std::function<int(int, char*[])> callback)
    : commandLine("")
{
    this->callback = callback;
}

IFrameworkView WinrtApp::CreateView()
{
    return *this;
}

void WinrtApp::Initialize(CoreApplicationView const& view)
{
    view.Activated({ this, &WinrtApp::OnActivated });
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
    // TODO split
    std::vector<const char*> argv;
    argv.push_back("winrt");
    if (!commandLine.empty())
    {
        argv.push_back(commandLine.c_str());
    }
    callback(static_cast<int>(argv.size()), const_cast<char**>(argv.data()));
}

void WinrtApp::SetWindow(CoreWindow const& window)
{
}

void WinrtApp::OnActivated(CoreApplicationView const&, IActivatedEventArgs const& args)
{
    // file
    if (args.Kind() == ActivationKind::File)
    {
        auto fileArgs = args.as<FileActivatedEventArgs>();
        auto files    = fileArgs.Files();
        if (files.Size() > 0)
        {
            auto file   = files.First().Current();
            auto token  = StorageApplicationPermissions::MostRecentlyUsedList().Add(file);
            commandLine = std::format("winrt://{}?path={}", winrt::to_string(token), winrt::to_string(file.Path()));
        }
    }
    // ExecutionAlias
    else if (args.Kind() == ActivationKind::CommandLineLaunch)
    {
        auto cmdArgs   = args.as<CommandLineActivatedEventArgs>();
        auto operation = cmdArgs.Operation();
        commandLine    = winrt::to_string(operation.Arguments());
    }
}
