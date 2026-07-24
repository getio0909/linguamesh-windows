#include "pch.h"
#include "MainWindow.xaml.h"
#include "MainWindow.g.cpp"

#include <winrt/Microsoft.UI.Dispatching.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/base.h>

namespace winrt::LinguaMesh::implementation {

MainWindow::MainWindow() {
    InitializeComponent();
    model_ = std::make_shared<linguamesh::windows::AppModel>(
        std::make_shared<linguamesh::windows::FakeCoreBridge>(),
        std::make_shared<linguamesh::windows::WindowsCredentialStore>());
    for (const auto& model : model_->discover_models()) {
        ModelSelector().Items().Append(winrt::box_value(winrt::to_hstring(model)));
    }
    ModelSelector().SelectedIndex(0);
    timer_ = DispatcherQueue().CreateTimer();
    timer_.Interval(std::chrono::milliseconds(50));
    timer_.Tick([this](auto const&, auto const&) { UpdateView(); });
}

void MainWindow::TranslateButton_Click(winrt::Windows::Foundation::IInspectable const&,
                                        Microsoft::UI::Xaml::RoutedEventArgs const&) {
    if (ModelSelector().SelectedItem()) {
        model_->select_model(winrt::to_string(winrt::unbox_value<winrt::hstring>(ModelSelector().SelectedItem())));
    }
    model_->start_translation(winrt::to_string(SourceText().Text()));
    timer_.Start();
    TranslateButton().IsEnabled(false);
    CancelButton().IsEnabled(true);
}

void MainWindow::CancelButton_Click(winrt::Windows::Foundation::IInspectable const&,
                                     Microsoft::UI::Xaml::RoutedEventArgs const&) {
    model_->cancel_translation();
}

void MainWindow::ChineseLocaleButton_Click(winrt::Windows::Foundation::IInspectable const&,
                                            Microsoft::UI::Xaml::RoutedEventArgs const&) {
    model_->set_locale("zh-CN");
    UpdateView();
}

void MainWindow::EnglishLocaleButton_Click(winrt::Windows::Foundation::IInspectable const&,
                                            Microsoft::UI::Xaml::RoutedEventArgs const&) {
    model_->set_locale("en-US");
    UpdateView();
}

void MainWindow::UpdateView() {
    const auto snapshot = model_->snapshot();
    OutputText().Text(winrt::to_hstring(snapshot.output));
    if (!snapshot.error_key.empty()) {
        StatusText().Text(winrt::to_hstring(snapshot.error_key));
    } else if (snapshot.status == linguamesh::windows::TranslationStatus::Completed) {
        StatusText().Text(L"Completed");
    } else if (snapshot.status == linguamesh::windows::TranslationStatus::Cancelled) {
        StatusText().Text(L"Cancelled");
    } else {
        StatusText().Text(L"Streaming");
    }
    const bool terminal = snapshot.status == linguamesh::windows::TranslationStatus::Completed ||
                          snapshot.status == linguamesh::windows::TranslationStatus::Cancelled ||
                          snapshot.status == linguamesh::windows::TranslationStatus::Failed;
    TranslateButton().IsEnabled(terminal || snapshot.status == linguamesh::windows::TranslationStatus::Idle);
    CancelButton().IsEnabled(!terminal && snapshot.status == linguamesh::windows::TranslationStatus::Streaming);
    if (terminal) {
        timer_.Stop();
    }
}

}
