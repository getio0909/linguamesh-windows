#include "pch.h"
#include "App.xaml.h"
#include "MainWindow.xaml.h"

namespace winrt::LinguaMesh::implementation {

void App::OnLaunched(Microsoft::UI::Xaml::LaunchActivatedEventArgs const&) {
    auto window = make<MainWindow>();
    window.Activate();
}

}
