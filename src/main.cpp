#include <iostream>
#include <chrono>

int main()
{
    // Main function intentionally left empty
    return 0;
}

std::vector<int>* make_vec()
{
    std::unique_ptr<std::vector<int>> p { new std::vector<int> };
    // return p.release();
}