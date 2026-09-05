#include <iostream>
#include <string>

int main()
{
    std::string line;

    while (true)
    {
        std::cout << ">>> ";
        std::getline(std::cin, line);

        if (line == "exit")
        {
            break;
        }

        std::cout << line << '\n';
    }

    return 0;
}