/*
Copyright 2023 zeromake

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

#pragma once

#include <borealis/core/ime.hpp>
#include <borealis/core/event.hpp>
#include <winrt/Windows.UI.Text.Core.h>
#include <borealis/views/edit_text_dialog.hpp>

namespace brls
{
class WinRTImeManager : public ImeManager
{
  public:
    WinRTImeManager(winrt::Windows::UI::Core::CoreWindow coreWindow);

    bool openForText(std::function<void(std::string)> f, std::string headerText = "",
        std::string subText = "", int maxStringLength = 32, std::string initialText = "",
        int kbdDisableBitmask = KeyboardKeyDisableBitmask::KEYBOARD_DISABLE_NONE) override;

    bool openForNumber(std::function<void(long)> f, std::string headerText = "",
        std::string subText = "", int maxStringLength = 18, std::string initialText = "",
        std::string leftButton = "", std::string rightButton = "",
        int kbdDisableBitmask = KeyboardKeyDisableBitmask::KEYBOARD_DISABLE_NONE) override;

    void openInputDialog(std::function<void(std::string)> cb, std::string headerText,
        std::string subText, size_t maxStringLength = 50, std::string initialText = "");
  private:
    winrt::Windows::UI::Core::CoreWindow coreWindow = nullptr;

    EditTextDialog* dialog = nullptr;
    winrt::Windows::UI::Text::Core::CoreTextEditContext editContext = nullptr;

    size_t maxStringLength = 100;
    std::wstring inputBuffer;

    winrt::Windows::Foundation::Rect rect;//���뷨λ��
    winrt::Windows::UI::Text::Core::CoreTextRange selection;//��ǰѡ��λ��
    bool isEditing; 
    
    // helper to initialize the context
    void initCoreText(winrt::Windows::UI::Text::Core::CoreTextInputScope scope);

    void onLayoutRequested2(
        winrt::Windows::UI::Text::Core::CoreTextEditContext const& context,
        winrt::Windows::UI::Text::Core::CoreTextLayoutRequestedEventArgs const& args);

    void onFormatUpdating(winrt::Windows::UI::Text::Core::CoreTextEditContext const&, winrt::Windows::UI::Text::Core::CoreTextFormatUpdatingEventArgs const& args);

    // event handlers
    void onCompositionStarted(
        winrt::Windows::UI::Text::Core::CoreTextEditContext const& context,
        winrt::Windows::UI::Text::Core::CoreTextCompositionStartedEventArgs const& args);

    void onSelectionUpdating(
        winrt::Windows::UI::Text::Core::CoreTextEditContext const& /*context*/,
        winrt::Windows::UI::Text::Core::CoreTextSelectionUpdatingEventArgs const& args);

    void onCompositionCompleted(
        winrt::Windows::UI::Text::Core::CoreTextEditContext const& context,
        winrt::Windows::UI::Text::Core::CoreTextCompositionCompletedEventArgs const& args);

   void onSelectionRequested(
        winrt::Windows::UI::Text::Core::CoreTextEditContext const& /*context*/,
        winrt::Windows::UI::Text::Core::CoreTextSelectionRequestedEventArgs const& args);

    // new handlers
    void onTextRequested(
        winrt::Windows::UI::Text::Core::CoreTextEditContext const& context,
        winrt::Windows::UI::Text::Core::CoreTextTextRequestedEventArgs const& args);

    void onTextUpdating(
        winrt::Windows::UI::Text::Core::CoreTextEditContext const& context,
        winrt::Windows::UI::Text::Core::CoreTextTextUpdatingEventArgs const& args);

    void updateText();
    void updateTextCursor();

};
}