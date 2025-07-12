/*
    Copyright 2023 zeromake
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

#include <windows.h>
#include <winrt/Windows.ApplicationModel.Core.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.System.h>
#include <winrt/Windows.UI.Composition.h>
#include <winrt/Windows.UI.Core.h>
#include <winrt/Windows.UI.Input.h>
#include <winrt/Windows.UI.Text.Core.h>

#include <borealis/core/application.hpp>
#include <borealis/core/event.hpp>
#include <borealis/core/logger.hpp>
#include <borealis/platforms/winrt/winrt_ime.hpp>
#include <borealis/views/edit_text_dialog.hpp>

namespace brls
{
WinRTImeManager::WinRTImeManager(winrt::Windows::UI::Core::CoreWindow coreWindow):coreWindow(coreWindow)
{
}

void WinRTImeManager::initCoreText(winrt::Windows::UI::Text::Core::CoreTextInputScope scope)
{
    auto manager      = winrt::Windows::UI::Text::Core::CoreTextServicesManager::GetForCurrentView();
    this->editContext = manager.CreateEditContext();
    editContext.InputScope(scope);
    editContext.InputPaneDisplayPolicy(winrt::Windows::UI::Text::Core::CoreTextInputPaneDisplayPolicy::Automatic);

    // register composition events
    editContext.CompositionStarted({ this, &WinRTImeManager::onCompositionStarted });
    editContext.CompositionCompleted({ this, &WinRTImeManager::onCompositionCompleted });
    editContext.SelectionRequested({ this, &WinRTImeManager::onSelectionRequested });
    editContext.SelectionUpdating({ this, &WinRTImeManager::onSelectionUpdating });

    // new event handlers
    editContext.TextRequested({ this, &WinRTImeManager::onTextRequested });
    editContext.TextUpdating({ this, &WinRTImeManager::onTextUpdating });

    editContext.LayoutRequested({ this, &WinRTImeManager::onLayoutRequested2 });
    editContext.FormatUpdating({ this, &WinRTImeManager::onFormatUpdating });
}

void WinRTImeManager::onLayoutRequested2(
    winrt::Windows::UI::Text::Core::CoreTextEditContext const& /*context*/,
    winrt::Windows::UI::Text::Core::CoreTextLayoutRequestedEventArgs const& args)
{
    if (rect.X > 0 && rect.Y > 0)
    {
        auto windowBounds                      = coreWindow.Bounds();

        float scale        = Application::windowScale;
        double scaleFactor = brls::Application::getPlatform()->getVideoContext()->getScaleFactor();

        winrt::Windows::Foundation::Rect rectC;
        rectC.X = rect.X * Application::windowScale + windowBounds.X * scaleFactor;
        rectC.Y = rect.Y * Application::windowScale + windowBounds.Y * scaleFactor;

        args.Request().LayoutBounds().ControlBounds(rectC);
        args.Request().LayoutBounds().TextBounds(rectC);
    }
}

void WinRTImeManager::onFormatUpdating(
    winrt::Windows::UI::Text::Core::CoreTextEditContext const& /*context*/,
    winrt::Windows::UI::Text::Core::CoreTextFormatUpdatingEventArgs const& args)
{
}

void WinRTImeManager::onCompositionStarted(
    winrt::Windows::UI::Text::Core::CoreTextEditContext const& /*context*/,
    winrt::Windows::UI::Text::Core::CoreTextCompositionStartedEventArgs const& args)
{
    isEditing = true;
}

void WinRTImeManager::onCompositionCompleted(
    winrt::Windows::UI::Text::Core::CoreTextEditContext const& /*context*/,
    winrt::Windows::UI::Text::Core::CoreTextCompositionCompletedEventArgs const& args)
{
    isEditing = false;
}

void WinRTImeManager::onSelectionRequested(
    winrt::Windows::UI::Text::Core::CoreTextEditContext const& /*context*/,
    winrt::Windows::UI::Text::Core::CoreTextSelectionRequestedEventArgs const& args)
{
    brls::Logger().debug("---->onSelectionRequested");
    args.Request().Selection(this->selection);
}

void WinRTImeManager::onSelectionUpdating(
    winrt::Windows::UI::Text::Core::CoreTextEditContext const& /*context*/,
    winrt::Windows::UI::Text::Core::CoreTextSelectionUpdatingEventArgs const& args)
{
    brls::Logger().debug("---->onSelectionUpdating");
    this->selection.StartCaretPosition = args.Selection().StartCaretPosition;
    this->selection.EndCaretPosition   = args.Selection().EndCaretPosition;
    updateTextCursor();
}

void WinRTImeManager::onTextRequested(
    winrt::Windows::UI::Text::Core::CoreTextEditContext const& /*context*/,
    winrt::Windows::UI::Text::Core::CoreTextTextRequestedEventArgs const& args)
{
    brls::Logger().debug("---->onTextRequested");
    auto request = args.Request();

    auto range = request.Range();
    request.Text(this->inputBuffer.substr(
        range.StartCaretPosition,
        std::min(range.EndCaretPosition, (int)this->inputBuffer.length()) - range.StartCaretPosition));
}

void WinRTImeManager::onTextUpdating(
    winrt::Windows::UI::Text::Core::CoreTextEditContext const& /*context*/,
    winrt::Windows::UI::Text::Core::CoreTextTextUpdatingEventArgs const& args)
{
    auto range        = args.Range();
    auto newText      = args.Text();
    auto newSelection = args.NewSelection();

    brls::Logger().debug("---->onTextUpdating:newText:{},range:{}-{}", winrt::to_string(newText), range.StartCaretPosition, range.EndCaretPosition);

    this->inputBuffer = inputBuffer.substr(0, range.StartCaretPosition) + newText + inputBuffer.substr(std::min((int)this->inputBuffer.length(), range.EndCaretPosition));

    brls::Logger().debug("---->onTextUpdating:inputBuffer:{}", winrt::to_string(inputBuffer));

    newSelection.EndCaretPosition      = newSelection.StartCaretPosition;
    this->selection.StartCaretPosition = newSelection.StartCaretPosition;
    this->selection.EndCaretPosition   = newSelection.EndCaretPosition;
    brls::Logger().debug("---->onTextUpdating:n newselection:{}", this->selection.StartCaretPosition);
    updateTextCursor();
    updateText();
}

void WinRTImeManager::updateText()
{
    dialog->setText(winrt::to_string(this->inputBuffer));
    dialog->setCountText(fmt::format("{}/{}", this->inputBuffer.length(), maxStringLength));
};
void WinRTImeManager::updateTextCursor()
{
    brls::Logger().debug("---->updateTextCursor:cursor:{}", selection.StartCaretPosition);
    dialog->setCursor(selection.StartCaretPosition);
};

void WinRTImeManager::openInputDialog(
    std::function<void(std::string)> cb,
    std::string headerText,
    std::string subText,
    size_t maxStringLength1,
    std::string initialText)
{
    dialog                = new EditTextDialog();
    this->inputBuffer     = winrt::to_hstring(initialText);
    this->maxStringLength = maxStringLength1;

    dialog->setHeaderText(headerText);
    dialog->setHintText(subText);

    // cancel
    dialog->getCancelEvent()->subscribe([this]()
        { this->editContext = nullptr; });

    // submit
    dialog->getSubmitEvent()->subscribe([this, cb]()
        {
            cb(winrt::to_string(this->inputBuffer));
            editContext.NotifyFocusLeave();
            return true; });

    dialog->getFocusEvent()->subscribe([this](View* v)
        { editContext.NotifyFocusEnter(); });

    dialog->getFocusLostEvent()->subscribe([this](View* v)
        { editContext.NotifyFocusLeave(); });

    dialog->registerAction(
        "hints/left"_i18n, BUTTON_LEFT, [this](...)
        {
                if (this->isEditing) return true;

                int cursor = selection.StartCaretPosition;
                if (cursor == (int)CursorPosition::END)
                {
                    cursor = this->inputBuffer.length() - 1;
                    if(cursor < 0) cursor = 0;
                  
                } else if (cursor > (int)CursorPosition::START) {
                    cursor--;
              
                }
                if (cursor != selection.StartCaretPosition)
                {
                    selection.StartCaretPosition = cursor;
                    selection.EndCaretPosition   = cursor;
                    updateTextCursor();
                    editContext.NotifySelectionChanged(selection);
                }

                return true; }, true, true);
    dialog->registerAction(
        "hints/right"_i18n, BUTTON_RIGHT, [this](...)
        {
              int cursor = selection.StartCaretPosition;
                if (this->isEditing) return true;
                if (cursor >= (int)CursorPosition::START) {
                    if (cursor < this->inputBuffer.length())
                    {
                        cursor++;
                    }
                }

                if (cursor != selection.StartCaretPosition)
                {
                    selection.StartCaretPosition = cursor;
                    selection.EndCaretPosition   = cursor;
                    updateTextCursor();
                    editContext.NotifySelectionChanged(selection);
                }

                return true; }, true, true);

    // delete text
    dialog->getBackspaceEvent()->subscribe([this](...)
        {
            int cursor = selection.StartCaretPosition;
            auto length=inputBuffer.length();
            if(inputBuffer.empty()) return true;
            if (cursor <=(int)CursorPosition::START || cursor > length)
                return true;
          
             winrt::Windows::UI::Text::Core::CoreTextRange changed;
             changed.StartCaretPosition = cursor-1;
             changed.EndCaretPosition   = cursor;
             inputBuffer.erase(changed.StartCaretPosition, 1);

            selection.StartCaretPosition = changed.StartCaretPosition;
                selection.EndCaretPosition   = changed.StartCaretPosition;
                editContext.NotifyTextChanged(changed, inputBuffer.length(), selection);
            updateTextCursor();
            updateText();
            return true; });

    // 更新输入法条位置
    dialog->getLayoutEvent()->subscribe([this](Point p)
        {
            if(std::abs(p.x-rect.X) >2 || std::abs(p.y-rect.Y) >2) {
                rect.X = p.x;
                rect.Y = p.y;
                if (editContext)
                {
                    editContext.NotifyLayoutChanged();
                }
            } });

    this->selection.StartCaretPosition = this->inputBuffer.length();
    this->selection.EndCaretPosition   = this->selection.StartCaretPosition;
    updateText();
    updateTextCursor();
    dialog->open();
}

bool WinRTImeManager::openForText(std::function<void(std::string)> f, std::string headerText,
    std::string subText, int maxStringLength, std::string initialText,
    int kbdDisableBitmask)
{
    initCoreText(winrt::Windows::UI::Text::Core::CoreTextInputScope::Text);
    this->openInputDialog([f](const std::string& text)
        { f(text); },
        headerText, subText, maxStringLength, initialText);
    return true;
}

bool WinRTImeManager::openForNumber(std::function<void(long)> f, std::string headerText,
    std::string subText, int maxStringLength, std::string initialText,
    std::string leftButton, std::string rightButton,
    int kbdDisableBitmask)
{

    initCoreText(winrt::Windows::UI::Text::Core::CoreTextInputScope::Number);
    this->openInputDialog([f](const std::string& text)
        {
            if(text.empty()) return ;
            try
            {
                f(stoll(text));
            }
            catch (const std::invalid_argument& e)
            {
                Logger::error("Could not parse input, did you enter a valid integer? {}", e.what());
            }
            catch (const std::out_of_range& e) {
                Logger::error("Out of range: {}", e.what());
            }
            catch (const std::exception& e)
            {
                Logger::error("Unexpected error occurred: {}", e.what());
            } }, headerText, subText, maxStringLength, initialText);
    return true;
}
}