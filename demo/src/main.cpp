/*
    Copyright 2020-2021 natinusala
    Copyright 2019 p-sam

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

#if defined(ANDROID) || defined(IOS)
#include <SDL2/SDL_main.h>
#endif

#include <borealis.hpp>
#include <cstdlib>
#include <string>

#include "view/captioned_image.hpp"
#include "view/pokemon_view.hpp"
#include "tab/components_tab.hpp"
#include "tab/transform_tab.hpp"
#include "tab/recycling_list_tab.hpp"
#include "tab/settings_tab.hpp"
#include "tab/text_test_tab.hpp"
#include "activity/main_activity.hpp"

#ifdef __WINRT__
#include <windows.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.UI.Core.h>
#include <winrt/Windows.ApplicationModel.Core.h>
#include <winrt/Windows.Storage.Streams.h>
#include <winrt/Windows.UI.ViewManagement.h>
#endif

#ifdef __WINRT_NEW__
#include <borealis/platforms/winrt/winrt_app.hpp>
#endif

#if defined(__PSV__) && defined(BOREALIS_USE_OPENGL)
// Needed for the OpenGL driver to work
extern "C" unsigned int sceLibcHeapSize = 2 * 1024 * 1024;
#endif

using namespace brls::literals; // for _i18n

int main(int argc, char* argv[])
{
#ifdef __WINRT__
    setlocale (LC_ALL, ".utf8");

    //TODO use config @ikas
    brls::Logger::setLogLevel (brls::LogLevel::LOG_DEBUG);
    brls::Application::enableDebuggingView (false);

    auto appLocal = winrt::Windows::Storage::AppDataPaths::GetDefault ().LocalAppData ();
    auto const time = std::chrono::current_zone ()->to_local (std::chrono::system_clock::now ());
    auto logFile = std::format ("{}\\borealis-demo.{:%Y-%m-%d-%H-%M-%S}.log", winrt::to_string (appLocal), time);
    brls::Logger::setLogOutput (std::fopen (logFile.c_str (), "w+"));

    if (IsDebuggerPresent ()) {
        brls::Logger::getLogEvent ()->subscribe ([](brls::Logger::TimePoint now, brls::LogLevel level, const std::string& log) {
            auto message = std::format (L"[{}] {}\n", (int)level, winrt::to_hstring (log));
            OutputDebugString (message.c_str ());
            });
    }

    auto cmdline = winrt::to_string (GetCommandLine ());
    brls::Logger::debug ("app start,cmdline:{}", cmdline);

    //for xbox 
    winrt::Windows::UI::Core::SystemNavigationManager::GetForCurrentView ().BackRequested ([] (
        winrt::Windows::Foundation::IInspectable const,
        winrt::Windows::UI::Core::BackRequestedEventArgs const& args
        ) {
            args.Handled (true);
        });

#else
    // We recommend to use INFO for real apps
    for (int i = 1; i < argc; i++) {
        if (std::strcmp(argv[i], "-d") == 0) { // Set log level
            brls::Logger::setLogLevel(brls::LogLevel::LOG_DEBUG);
        } else if (std::strcmp(argv[i], "-o") == 0) {
            const char* path = (i + 1 < argc) ? argv[++i] : "borealis.log";
            brls::Logger::setLogOutput(std::fopen(path, "w+"));
        } else if (std::strcmp(argv[i], "-v") == 0) {
            brls::Application::enableDebuggingView(true);
        }
    }
#endif
    brls::Platform::APP_LOCALE_DEFAULT = brls::LOCALE_AUTO;

    // Init the app and i18n
    if (!brls::Application::init())
    {
        brls::Logger::error("Unable to init Borealis application");
        return EXIT_FAILURE;
    }

    brls::Application::createWindow("demo/title"_i18n);

    brls::Application::getPlatform()->setThemeVariant(brls::ThemeVariant::DARK);

    // Have the application register an action on every activity that will quit when you press BUTTON_START
    brls::Application::setGlobalQuit(false);

    // Register custom views (including tabs, which are views)
    brls::Application::registerXMLView("CaptionedImage", CaptionedImage::create);
    brls::Application::registerXMLView("RecyclingListTab", RecyclingListTab::create);
    brls::Application::registerXMLView("ComponentsTab", ComponentsTab::create);
    brls::Application::registerXMLView("TransformTab", TransformTab::create);
    brls::Application::registerXMLView("TransformBox", TransformBox::create);
    brls::Application::registerXMLView("PokemonView", PokemonView::create);
    brls::Application::registerXMLView("SettingsTab", SettingsTab::create);
    brls::Application::registerXMLView("TextTestTab", TextTestTab::create);

    // Add custom values to the theme
    brls::Theme::getLightTheme().addColor("captioned_image/caption", nvgRGB(2, 176, 183));
    brls::Theme::getDarkTheme().addColor("captioned_image/caption", nvgRGB(51, 186, 227));

    // Add custom values to the style
    brls::getStyle().addMetric("about/padding_top_bottom", 50);
    brls::getStyle().addMetric("about/padding_sides", 75);
    brls::getStyle().addMetric("about/description_margin", 50);

    // Create and push the main activity to the stack
    brls::Application::pushActivity(new MainActivity());

    // Run the app
    while (brls::Application::mainLoop())
        ;

    // Exit
    return EXIT_SUCCESS;
}

#ifdef __WINRT_NEW__
int __stdcall wWinMain (HINSTANCE, HINSTANCE, PWSTR szCmdLine, int)
{
    (void)szCmdLine;
    return WinrtApp::RunApp (main);
}
#elif __WINRT__
#include <borealis/core/main.hpp>
#endif
