// code contest 02
#pragma GCC optimize("-O3")

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

    void inline input(std::stringstream& ss)
    {
        uint32_t num = 0;
        bool prevIsNum = false;
        for(char ch; ss.get(ch);)
        {
            if(ch >= '0') // '\r', '\n', ' ', ',' are all < '0'
            {
                num *= 10;
                num += static_cast<uint32_t>(ch - '0');
                prevIsNum = true;
            }
            else
            {
                if(prevIsNum)
                {
                    line[pos] = num + std::max(line[pos], line[pos - 1]);
                    ++pos;
                    if(pos > width)
                    {
                        pos = 1;
                    }
                    num = 0;
                    prevIsNum = false;
                }
            }
        }
        if(prevIsNum)
        {
            line[pos] = num + std::max(line[pos], line[pos - 1]);
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
} // namespace code

int main(int argc, const char* argv[])
{
    (void)argc;
    const std::string filename(argv[1]);
    code::DataLoader loader(filename);
    std::stringstream paylaod;
    auto numM = loader.getM();
    loader.read(paylaod);
    code::MaxPathSolution solution{numM};
    solution.input(paylaod);
    std::fprintf(stdout, "%u\n", solution.value());
    return 0;
}
