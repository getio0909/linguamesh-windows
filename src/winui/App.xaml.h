#pragma once

#include "App.xaml.g.h"

namespace winrt::LinguaMesh::implementation {

struct App : ::winrt::Microsoft::UI::Xaml::ApplicationT<App> {
    App() = default;
    void OnLaunched(Microsoft::UI::Xaml::LaunchActivatedEventArgs const& arguments);
};

}
