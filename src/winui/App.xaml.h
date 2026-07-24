#pragma once

#include "App.xaml.g.h"

namespace winrt::LinguaMesh::implementation {

struct App : AppT<App> {
    App() = default;
    void OnLaunched(Microsoft::UI::Xaml::LaunchActivatedEventArgs const& arguments);
};

}

namespace winrt::LinguaMesh::factory_implementation {

struct App : AppT<App, implementation::App> {};

}
