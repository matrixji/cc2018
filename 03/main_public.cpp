// code contest 03
#pragma GCC optimize("-O2")

#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <list>
#include <map>
#include <set>
#include <sstream>
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

    void skipLine()
    {
        std::string token;
        std::getline(fs, token);
    }

    // load content from file, callback on each line content
    // ignore \r for windows
    void load(const std::function<void(const std::string& line)>& cb)
    {
        for(std::string line; std::getline(fs, line);)
        {
            if(line.length() > 0 && *line.rbegin() == '\r')
            {
                *line.rbegin() = '\0';
            }
            cb(line);
        }
    }

    // load with delimiter, cb on each line, all words in vector.
    void load(const char delimiter, const std::function<void(std::vector<std::string>&)>& cb)
    {
        load([&delimiter, &cb](const std::string& line) {
            std::vector<std::string> ret;
            std::istringstream iss(line);
            for(std::string token; std::getline(iss, token, delimiter);)
            {
                ret.push_back(std::move(token));
            }
            cb(ret);
        });
    }

private:
    std::ifstream fs;
};

#define CONSOLE std::cout // NOSONAR

// all code contest have one file input parameter
void paramCheck(int argc, const char* argv[])
{
    if(argc <= 1)
    {
        CONSOLE << "usage: " << std::string(argv[0]) << " <input>" << std::endl;
        std::exit(-1);
    }
}

class Solution
{
    // using bit to store test line combinations
    // as total <=20, 32bit is ok
    // 0x0003 -> means test line: 1 & 2 are selected.
    using Combinations = std::map<uint32_t, size_t>;

public:
    void load(const std::vector<std::string>& words)
    {
        size_t len = words.size();
        if(len > 0)
        {
            Combinations cbs;
            for(const auto& word : words)
            {
                cbs.emplace(static_cast<uint32_t>(1 << (strtoul(word.c_str(), nullptr, 10))), size_t{1});
            }
            data.emplace_back(cbs);
        }
    }

    size_t calculate()
    {
        if(data.size() == 1)
        {
            return data.front().size();
        }
        const size_t mergeExitLimit = 2;
        while(data.size() > mergeExitLimit)
        {
            data.sort(DataSorter);
            auto it = data.begin();
            auto& tempA = *it;
            ++it;
            auto& tempB = *it;
            Combinations tempNew;
            for(const auto& a : tempA)
            {
                for(const auto& b : tempB)
                {
                    if((a.first & b.first) == 0)
                    {
                        auto itAB = tempNew.emplace(a.first | b.first, 0);
                        itAB.first->second += a.second * b.second;
                    }
                }
            }
            data.pop_front();
            data.pop_front();
            data.emplace_back(tempNew);
        }

        size_t total = 0;
        for(const auto& a : data.front())
        {
            for(const auto& b : data.back())
            {
                if((a.first & b.first) == 0)
                {
                    total += a.second * b.second;
                }
            }
        }

        return total;
    }

private:
    struct
    {
        template <typename T>
        bool operator()(const T& a, const T& b)
        {
            return a.size() < b.size();
        }
    } DataSorter;
    std::list<Combinations> data;
};

}

int main(int argc, const char* argv[])
{
    code::paramCheck(argc, argv);

    const std::string filename(argv[1]);
    code::DataLoader loader(filename);
    code::Solution solution;
    loader.skipLine();
    loader.load(',', [&solution](const std::vector<std::string>& words) { solution.load(words); });
    CONSOLE << solution.calculate() << std::endl;
    return 0;
}
