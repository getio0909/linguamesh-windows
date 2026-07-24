#pragma once

#include "MainWindow.xaml.g.h"
#include "linguamesh/windows/app_model.hpp"

namespace winrt::LinguaMesh::implementation {

struct MainWindow final : MainWindowT<MainWindow> {
    MainWindow();

    void TranslateButton_Click(winrt::Windows::Foundation::IInspectable const& sender,
                               Microsoft::UI::Xaml::RoutedEventArgs const& arguments);
    void CancelButton_Click(winrt::Windows::Foundation::IInspectable const& sender,
                            Microsoft::UI::Xaml::RoutedEventArgs const& arguments);
    void ChineseLocaleButton_Click(winrt::Windows::Foundation::IInspectable const& sender,
                                   Microsoft::UI::Xaml::RoutedEventArgs const& arguments);
    void EnglishLocaleButton_Click(winrt::Windows::Foundation::IInspectable const& sender,
                                   Microsoft::UI::Xaml::RoutedEventArgs const& arguments);

private:
    void UpdateView();

    std::shared_ptr<linguamesh::windows::AppModel> model_;
    winrt::Microsoft::UI::Dispatching::DispatcherQueueTimer timer_{nullptr};
};

}

namespace winrt::LinguaMesh::factory_implementation {

struct MainWindow : MainWindowT<MainWindow, implementation::MainWindow> {};

}
