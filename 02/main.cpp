// code contest 02

#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <regex>
#include <set>
#include <vector>

#include "../common/DataLoader.hpp"

#include "../common/helper.hpp"

struct MaxPathSolution
{
    std::vector<std::vector<uint32_t>> data;

    MaxPathSolution(const size_t m, const size_t n)
    {
        data.reserve(m);
        for(size_t i = 0; i < m; i++)
        {
            data.emplace_back(std::vector<uint32_t>(n));
        }
    }

    uint32_t get(size_t i, size_t j)
    {
        return data[i][j];
    }

    void set(size_t i, size_t j, uint32_t value)
    {
        data[i][j] = value;
    }
};

void handle(const std::vector<std::string>& words, std::unique_ptr<MaxPathSolution>& solution)
{
    if(words.size() == 2 && (!solution))
    {
    }
}

int main(int argc, const char* argv[])
{
    paramCheck(argc, argv);

    const std::string filename(argv[1]);
    auto loader = std::make_unique<DataLoader>(filename);
    std::unique_ptr<MaxPathSolution> solution{nullptr};
    loader->load(',', [&](const std::vector<std::string>& words) { handle(words, solution); });

    return 0;
}
