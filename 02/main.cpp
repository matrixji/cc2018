// code contest 02
#pragma GCC optimize("-O2")

#include <cstdlib>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <regex>
#include <set>
#include <vector>

namespace code
{

#include "../common/DataLoader.hpp"

#include "../common/helper.hpp"

class MaxPathSolution
{
public:
    explicit MaxPathSolution(const size_t n) : line(n + 1, 0), pos(1), width(n)
    {
    }

    void put(uint32_t num)
    {
        line.at(pos) = num + std::max(line.at(pos), line.at(pos - 1));
        ++pos;
        if(pos > width)
        {
            pos = 1;
        }
    }
    uint32_t value()
    {
        return line.back();
    }

private:
    std::vector<uint32_t> line;
    size_t pos;
    size_t width;
};

void handle(const std::vector<std::string>& words, std::unique_ptr<MaxPathSolution>& solution)
{
    if(!solution)
    {
        // first line, create solution with m count.
        solution = std::make_unique<MaxPathSolution>(static_cast<size_t>(::strtoul(words.front().c_str(), nullptr, 10)));
    }
    else
    {
        for(const auto& word : words)
        {
            solution->put(static_cast<size_t>(::strtoul(word.c_str(), nullptr, 10)));
        }
    }
}
} // namespace code

int main(int argc, const char* argv[])
{
    code::paramCheck(argc, argv);

    const std::string filename(argv[1]);
    auto loader = std::make_unique<code::DataLoader>(filename);
    std::unique_ptr<code::MaxPathSolution> solution{nullptr};
    loader->load(',', [&solution](const std::vector<std::string>& words) { handle(words, solution); });
    CONSOLE << solution->value() << std::endl;
    return 0;
}
