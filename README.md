# QuantIO

QuantIO provides a GUI implementation of the QuantLib project. It allows non c++ developers to use the QuantLib project for their Quant needs.

# Example

The project currently supports the following functionalities:

- Calendars (incl. holidays, weekends, special holidays)
- Currencies
- Rounding
- Day conventions

# Development

To use the project you must clone the repo and open the solution in Visual Studio 2022 or later. The dependencies have been excluded from this repo, and can be downloaded from the links below and placed in their respective directories in `./vendor/`

For now the app uses local storage for the Db solution, this change later.

# Dependencies

- `./vendor/boost/` (Boost https://www.boost.org/ you must build it first to get the libraries)
- `./vendor/glfw/` (GLFW https://www.glfw.org/)
- `./vendor/imgui/` (ImGui https://github.com/ocornut/imgui)
- `./vendor/lua/` (Lua https://www.lua.org/)
- `./vendor/quantlib/` (QuantLib https://www.quantlib.org/ you must build it first to get the libraries)
- `./vendor/sqlite/` (SQLite https://www.sqlite.org/)

# Project

https://github.com/sthabene/QuantIO

# Screenshots

<a>
    <img src="misc/screenshots/screen1.png">
</a>
<a>
    <img src="misc/screenshots/screen2.png">
</a>
