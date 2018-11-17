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

// data loader for code contest
class DataLoader
{
public:
    explicit DataLoader(const std::string& filename) : fs(filename)
    {
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

auto& console()
{
    return std::cout; // NOSONAR
}

// all code contest have one file input parameter
void paramCheck(int argc, const char* argv[])
{
    if(argc <= 1)
    {
        console() << "usage: " << std::string(argv[0]) << " <input>" << std::endl;
        std::exit(-1);
    }
}

class MaxPathSolution
{
public:
    explicit MaxPathSolution(const size_t n) : line(n + 1, 0), pos(1), width(n)
    {
    }

    void put(uint32_t num)
    {
        line.at(pos) = num + std::max(line.at(pos), line.at(pos - 1));
        if(++pos > width)
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

int main(int argc, const char* argv[])
{
    paramCheck(argc, argv);

    const std::string filename(argv[1]);
    auto loader = std::make_unique<DataLoader>(filename);
    std::unique_ptr<MaxPathSolution> solution{nullptr};
    loader->load(',', [&solution](const std::vector<std::string>& words) { handle(words, solution); });
    console() << solution->value() << std::endl;
    return 0;
}
