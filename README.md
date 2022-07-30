<img align="left" alt="Logo" style="max-width:100%; margin-right: 10px;" src="https://avatars.githubusercontent.com/u/78804706?s=100&v=4"/>

<a href="https://github.com/hikogui/hikogui/blob/main/LICENSE_1_0.txt">
<img align="right" style="margin: 5px" src="https://img.shields.io/github/license/hikogui/hikogui.svg"/>
</a>
<a href="https://github.com/hikogui/hikogui/releases/latest">
<img align="right" style="margin: 5px" src="https://img.shields.io/badge/dynamic/json?url=https://raw.githubusercontent.com/hikogui/hikogui/main/vcpkg.json&label=Latest%20Version&query=$[%27version%27]&color=blue"/>
</a>

&nbsp;

# HikoGUI: a modern accelerated GUI

[Website](https://hikogui.org/) •
[Documentation](https://hikogui.org/docs/hikogui/main/index.html) •
[Examples](https://github.com/hikogui/hikogui_hello_world/blob/main/src/main.cpp) •
[Bug tracker](https://github.com/hikogui/hikogui/issues) •
[Git repository](https://github.com/hikogui/hikogui)

A portable, low latency, retained-mode GUI framework written in C++
-------------------------------------------------------------------

I started this library to make a portable, low latency and modern looking
UI framework, which may also be used in proprietary (closed source) applications.

It is specifically designed to display information with low-latency,
and at the screen's refresh rate. Special care is taken for making
it easy for GUI element to observe and modify data external to the GUI.

You can find more information,
[documentation](https://hikogui.org/docs/hikogui/main/index.html),
[example code](https://github.com/hikogui/hikogui_hello_world/blob/main/src/main.cpp),
news and blog posts on the main web site: <https://hikogui.org/>

Features
--------

 - High level API to make simple desktop applications.
 - Modern C++20 library.
 - Retained-mode GUI.
 - GUI will dynamically track the state of the application.
 - Localization and translation.
 - Animation at the screen's refresh rate.
 - Themes; including light/dark support.
 - Editable key-bindings.

![Themes with dark and light mode](docs/media/screenshots/demo_dark_and_light.png)

 - Most or all drawing is GPU accelerated with Vulkan.
 - Text is drawn using kerning, perceptional correct blending and subpixel anti-aliasing.
 - High dynamic range and high gamut color handling.

![Subpixel anti-aliasing](docs/media/screenshots/subpixel_glyphs.png)

 - Automatic application preferences storage.
 - Many support systems:
   + logging,
   + statistics,
   + text handling,
   + text template language,
   + expression language,
   + dynamic type system.

Example
-------
Here is some example code for an application with three radio buttons,
who form a set by sharing a single `value` observable.

```
int hi_main(int argc, char *argv[])
{
    observable<int> value = 0;

    auto gui = hi::gui_system::make_unique();
    auto &window = gui.make_window(tr("Radio button example"));
    window.content().make_widget<label_widget>("A1", tr("radio buttons:"));
    window.content().make_widget<radio_button_widget>("B1", tr("one"), value, 1);
    window.content().make_widget<radio_button_widget>("B2", tr("two"), value, 2);
    window.content().make_widget<radio_button_widget>("B3", tr("three"), value, 3);

    return gui->loop();
}
```

Platform Support
----------------

| Platform |  Architectures  | Compiler | Version |   Build    | Coverage |
|:--------:|:---------------:|:--------:|:-------:|:----------:|:--------:|
| Windows 10 | x86-64-v2.5 | MSVC       | VC17 (VS2022) | [![Build on Windows][hikogui-ci-badge-main]](hikogui-wf-build-on-windows) | [![codecov.io Code Coverage][hikogui-codecov-badge-main]](hikogui-codecov-main) |
| Linux      | x64         | GCC, Clang | *platform not supported, yet* |
| MacOS 11   | ARM64       | AppleClang | *platform not supported, yet* |

HikoGUI makes use of bleeding edge C++20 language features.

It will compile on Windows 10 x64 using MSVC 17 (VS2022).

Other compilers, including Clang-14 and GCC-12, are still missing these new C++20 language features and are currently unable to compile HikoGUI.

Referencing: [C++ Compiler Support][cpp-compiler-support].

For hardware support see: [hardware\_support](docs/hardware_support.md)

Installation
------------

If you like to help with the development or want to modify hikogui you can
find instruction how to install the dependencies and how to build hikogui in the
[CONTRIBUTING](docs/CONTRIBUTING.md) document.

If you want to use hikogui as a library for your own application you can
find instructions in the [hikogui_hello_world](https://github.com/hikogui/hikogui_hello_world)
example application's [README](https://github.com/hikogui/hikogui_hello_world/blob/main/README.md).

Sponsors
--------

The following people and companies are platinum sponsors:

_There are currently no platinum sponsors._

for more sponsors please see [SPONSORS](SPONSORS.md).

<!-- Section for Reference Links -->

[hikogui-latest-version-badge]:
https://img.shields.io/badge/dynamic/json?url=https://raw.githubusercontent.com/hikogui/hikogui/main/vcpkg.json&label=Latest%20Version&query=$[%27version%27]&color=blue
[hikogui-latest-release]:
https://github.com/hikogui/hikogui/releases/latest

[hikogui-license-badge]:
https://img.shields.io/github/license/hikogui/hikogui.svg
[hikogui-license-text]:
https://github.com/hikogui/hikogui/blob/main/LICENSE_1_0.txt

[hikogui-ci-badge-main]:
https://github.com/hikogui/hikogui/actions/workflows/build-on-windows.yml/badge.svg?branch=main
[hikogui-wf-build-on-windows]:
https://github.com/hikogui/hikogui/actions/workflows/build-on-windows.yml

[hikogui-codecov-badge-main]:
https://img.shields.io/codecov/c/github/hikogui/hikogui.svg
[hikogui-codecov-main]:
https://codecov.io/github/hikogui/hikogui?branch=main

[cpp-compiler-support]:
https://en.cppreference.com/w/cpp/compiler_support
