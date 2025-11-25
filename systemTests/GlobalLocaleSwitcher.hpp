#pragma once

#include <locale>

namespace test {

class GlobalLocaleSwitcher {
public:
    GlobalLocaleSwitcher(char const* newLocale) : previousLocale(std::locale::global(std::locale(newLocale))) {}

    ~GlobalLocaleSwitcher() {
        std::locale::global(previousLocale);
    }

private:
    std::locale const previousLocale;
};

} // namespace test
