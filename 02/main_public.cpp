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

// data loader for code contest
class DataLoader
{
public:
    explicit DataLoader(const std::string& filename) : fs(filename)
    {
    }

    size_t getM()
    {
        size_t ret = 0;
        std::string token;
        std::getline(fs, token, ',');
        ret = static_cast<size_t>(std::strtoul(token.c_str(), nullptr, 10));
        std::getline(fs, token);
        return ret;
    }

    void read(std::stringstream& stream)
    {
        stream << fs.rdbuf();
    }

private:
    std::ifstream fs;
};

#define CONSOLE std::cout // NOSONAR

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
            if(ch >= '0' && ch <= '9')
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
}

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
    CONSOLE << solution.value() << std::endl;
    return 0;
}
