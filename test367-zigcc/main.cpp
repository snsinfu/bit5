#include <iostream>
#include <filesystem>
//#include <ranges> // none


int main()
{
    std::filesystem::directory_iterator dir_iterator{"."};

    for (auto const& entry : dir_iterator) {
        std::cout
            << entry.file_size()
            << '\t'
            << entry.path().string()
            << '\n';
    }
}
