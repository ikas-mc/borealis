/*
    Copyright 2021 natinusala

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

#include "settings_tab.hpp"

#include <nanovg.h>

using namespace brls::literals;  // for _i18n

bool radioSelected = false;

SettingsTab::SettingsTab()
{
    // Inflate the tab from the XML file
    this->inflateFromXMLRes("xml/tabs/settings.xml");

    radio->title->setText("Radio cell");
    radio->setSelected(radioSelected);
    radio->registerClickAction([this](brls::View* view) {
        radioSelected = !radioSelected;
        this->radio->setSelected(radioSelected);
        return true;
    });

    boolean->title->setText("Switcher");

    debug->init("Debug Layer", brls::Application::isDebuggingViewEnabled(), [](bool value){
        brls::Application::enableDebuggingView(value);
        brls::sync([value](){
            brls::Logger::info((value ? "Open" : "Close") + std::string{" the debug layer"});
        });
    });

    bottomBar->init("Bottom Bar", !brls::AppletFrame::HIDE_BOTTOM_BAR, [](bool value){
        brls::AppletFrame::HIDE_BOTTOM_BAR = !value;
        auto stack = brls::Application::getActivitiesStack();
        for (auto& activity : stack) {
            auto* frame = dynamic_cast<brls::AppletFrame*>(
                activity->getContentView());
            if (!frame) continue;
            frame->setFooterVisibility(!value ? brls::Visibility::GONE
                                             : brls::Visibility::VISIBLE);
        }
    });

    fps->init("FPS", brls::Application::getFPSStatus(), [](bool value){
        brls::Application::setFPSStatus(value);
    });

    selector->init("Selector", { "Test 1", "Test 2", "Test 3", "Test 4", "Test 5", "Test 6", "Test 7", "Test 8", "Test 9", "Test 10", "Test 11", "Test 12", "Test 13" }, 0, [](int selected) {
    }, [](int selected) {
        auto dialog = new brls::Dialog(fmt::format("selected {}", selected));
        dialog->addButton("hints/ok"_i18n, []() {});
        dialog->open();
    });

    input->init(
        "Input text", "https://github.com", [](std::string text) {

        },
        "Placeholder", "Hint");

    inputNumeric->init(
        "Input number", 2448, [](int number) {

        },
        "Hint");

    ipAddress->setDetailText(brls::Application::getPlatform()->getIpAddress());
    dnsServer->setDetailText(brls::Application::getPlatform()->getDnsServer());

    texture->setDetailText("alloc 1000 textures");
    texture->registerClickAction([](...){
            std::string path = std::string(BRLS_RESOURCES) + "img/borealis_256.png";
            auto nvg = brls::Application::getNVGContext();
            for(int i = 0; i < 1000; i++) {
                int tex = nvgCreateImage(nvg, path.c_str(), 0);
                if (tex == 0) {
                    brls::Logger::error("Failed to alloc texture {}");
                } else {
                    brls::Logger::info("Alloc texture success, handle: {}", tex);
                }
            }
            return true;
    });

    input->registerAction("hints/open"_i18n, brls::BUTTON_X, [](brls::View* view) {
        brls::DetailCell *cell = dynamic_cast<brls::DetailCell *>(view);
        brls::Application::getPlatform()->openBrowser(cell->detail->getFullText());
        return true;
    }, false, false, brls::SOUND_CLICK);
}

brls::View* SettingsTab::create()
{
    return new SettingsTab();
}
