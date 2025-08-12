/*
    Copyright 2021 natinusala
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
#ifdef __WINRT_NEW__
#include <borealis/core/application.hpp>
#include <borealis/core/logger.hpp>
#include <borealis/core/thread.hpp>
#include <borealis/platforms/winrt/winrt_video.hpp>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <windows.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.ApplicationModel.Core.h>
#include <winrt/Windows.UI.Core.h>
#include <winrt/Windows.UI.ViewManagement.h>
#include <winrt/Windows.UI.Composition.h>
#include <winrt/Windows.UI.Input.h>
#include <winrt/Windows.System.h>
#include <winrt/Windows.System.Profile.h>
#include <winrt/Windows.Graphics.Display.Core.h>
#include <gamingdeviceinformation.h>

#include <nanovg_d3d11.h>

#include <borealis/platforms/driver/d3d11.hpp>
std::unique_ptr<brls::D3D11Context> D3D11_CONTEXT;


namespace brls
{

    static double scaleFactor = 1.0;

   static winrt::Windows::Foundation::Rect getWindowRawSize (winrt::Windows::Foundation::Rect coreWindowSize, double scaleFactor)
    {
        winrt::Windows::Foundation::Rect rect{};
        bool isXbox = winrt::Windows::System::Profile::AnalyticsInfo::VersionInfo ().DeviceFamily () == L"Windows.Xbox";
        if (isXbox)
        {
            GAMING_DEVICE_MODEL_INFORMATION infos;
            if (SUCCEEDED (GetGamingDeviceModelInformation (&infos)))
            {
                if (infos.deviceId == GAMING_DEVICE_DEVICE_ID_XBOX_SERIES_X 
                    || infos.deviceId == GAMING_DEVICE_DEVICE_ID_XBOX_SERIES_X_DEVKIT
                    || infos.deviceId == GAMING_DEVICE_DEVICE_ID_XBOX_ONE_X
                    || infos.deviceId == GAMING_DEVICE_DEVICE_ID_XBOX_ONE_X_DEVKIT
                    || infos.deviceId == GAMING_DEVICE_DEVICE_ID_XBOX_ONE_S
                    )
                {
                    auto hdmiDisplayInformation = winrt::Windows::Graphics::Display::Core::HdmiDisplayInformation::GetForCurrentView ();
                    auto currentDisplayMode = hdmiDisplayInformation.GetCurrentDisplayMode ();
                    rect.Width = currentDisplayMode.ResolutionWidthInRawPixels ();
                    rect.Height = currentDisplayMode.ResolutionHeightInRawPixels ();
                    rect.X = 0;
                    rect.Y = 0;
                    return rect;
                }
            }
        } 
      
        rect.Width = coreWindowSize.Width * scaleFactor;
        rect.Height = coreWindowSize.Height * scaleFactor;
        rect.X = coreWindowSize.X * scaleFactor;
        rect.Y = coreWindowSize.Y * scaleFactor;
        return rect;
    }

    static void updateWindowSize (winrt::Windows::Foundation::Rect coreWindowSize)
    {
        scaleFactor = D3D11_CONTEXT->getScaleFactor ();
        auto windowRawSize = getWindowRawSize (coreWindowSize, scaleFactor);

        D3D11_CONTEXT->onFramebufferSize (windowRawSize.Width, windowRawSize.Height);
        Application::onWindowResized (windowRawSize.Width, windowRawSize.Height);
        if (!VideoContext::FULLSCREEN)
        {
            VideoContext::sizeW = windowRawSize.Width;
            VideoContext::sizeH = windowRawSize.Height;
            if (windowRawSize.X >= 0)
            {
                VideoContext::posX = (float)windowRawSize.X;
            }
            if (windowRawSize.Y >= 0)
            {
                VideoContext::posY = (float)windowRawSize.Y;
            }
        }
    }


    WinRTVideoContext::WinRTVideoContext (winrt::Windows::UI::Core::CoreWindow coreWindow, std::string windowTitle, uint32_t windowWidth, uint32_t windowHeight, float windowXPos, float windowYPos)
    {
        if (VideoContext::FULLSCREEN)
        {
            bool result = winrt::Windows::UI::ViewManagement::ApplicationView::GetForCurrentView ().TryEnterFullScreenMode ();
            VideoContext::FULLSCREEN = result;
        }

        using namespace  winrt::Windows::UI::Core;

        coreWindow.SizeChanged ([&](winrt::Windows::Foundation::IInspectable const&, winrt::Windows::UI::Core::WindowSizeChangedEventArgs const& args) {
            winrt::Windows::Foundation::Rect rect{ -1,-1,args.Size ().Width,args.Size ().Height };
            updateWindowSize (rect);
            });

        this->window = reinterpret_cast<ABI::Windows::UI::Core::ICoreWindow*>(winrt::get_abi (coreWindow));

        if (!this->window)
        {
            fatal ("winrt: failed to create window");
        }

        Logger::info ("winrt: USE_D3D11");
        D3D11_CONTEXT = std::make_unique<D3D11Context> (this->window, windowWidth, windowHeight);
        D3D11_CONTEXT->setSwapInterval (VideoContext::swapInterval);

        this->nvgContext = nvgCreateD3D11 (D3D11_CONTEXT->getDevice (), NVG_ANTIALIAS | NVG_STENCIL_STROKES);
        if (!this->nvgContext)
        {
            brls::fatal ("winrt: unable to init nanovg");
        }

        scaleFactor = D3D11_CONTEXT->getScaleFactor ();
        auto windowRawSize = getWindowRawSize (coreWindow.Bounds (), scaleFactor);

        Application::setWindowSize (windowRawSize.Width, windowRawSize.Height);
        D3D11_CONTEXT->onFramebufferSize (windowRawSize.Width, windowRawSize.Height);

        if (!VideoContext::FULLSCREEN)
        {
            VideoContext::sizeW = windowRawSize.Width;
            VideoContext::sizeH = windowRawSize.Height;
            VideoContext::posX = (float)windowRawSize.X;
            VideoContext::posY = (float)windowRawSize.Y;
        }
    }

    void WinRTVideoContext::beginFrame ()
    {
        D3D11_CONTEXT->beginFrame ();
    }

    void WinRTVideoContext::endFrame ()
    {
        D3D11_CONTEXT->endFrame ();
    }

    void WinRTVideoContext::setSwapInterval (int interval)
    {
        VideoContext::swapInterval = interval;
        D3D11_CONTEXT->setSwapInterval (interval);
    }

    void WinRTVideoContext::clear (NVGcolor color)
    {
        D3D11_CONTEXT->clear (nvgRGBAf (
            color.r,
            color.g,
            color.b,
            color.a));
    }

    void WinRTVideoContext::resetState ()
    {

    }

    double WinRTVideoContext::getScaleFactor ()
    {
        return scaleFactor;
    }

    WinRTVideoContext::~WinRTVideoContext ()
    {
        try
        {
            if (this->nvgContext)
            {

                nvgDeleteD3D11 (this->nvgContext);
                D3D11_CONTEXT = nullptr;
            }
        } catch (...)
        {
            Logger::error ("Cannot delete nvg Context");
        }
    }

    NVGcontext* WinRTVideoContext::getNVGContext ()
    {
        return this->nvgContext;
    }

    ABI::Windows::UI::Core::ICoreWindow* WinRTVideoContext::getWindow ()
    {
        return this->window;
    }

    void WinRTVideoContext::fullScreen (bool fs)
    {
        if (fs)
        {
            winrt::Windows::UI::ViewManagement::ApplicationView::GetForCurrentView ().TryEnterFullScreenMode ();
        } else {
            winrt::Windows::UI::ViewManagement::ApplicationView::GetForCurrentView ().ExitFullScreenMode ();
        }
    }

} // namespace brls
#endif