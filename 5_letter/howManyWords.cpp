#include <iostream>
#include <fstream>
#include <string>

int main()
{
    std::ifstream file("WORDS.txt");
    std::string line;
    int count = 0;
    while (getline(file, line))
    {
        count++;
    }
    std::cout<<count;
    return 0;
}
