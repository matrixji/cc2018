// code contest 01
#pragma GCC optimize("-O2")

#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <numeric>
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

    // load with pattern matching, cb on each line, all match line + sub matches
    // in vector.
    void load(const std::string& pattern, const std::function<void(std::vector<std::string>&)>& cb)
    {
        std::vector<std::string> ret;
        load([&ret, &pattern](const std::string& line) {
            std::regex reg(pattern);
            std::smatch matches;
            std::regex_match(line, matches, reg);
            for(const auto& match : matches)
            {
                ret.emplace_back(match);
            }
        });
        cb(ret);
    }

private:
    std::ifstream fs;
};

// all code contest have one file input parameter
void paramCheck(int argc, const char* argv[])
{
    if(argc <= 1)
    {
        std::fprintf(stdout, "usage %s <input>", argv[0]);
        std::exit(-1);
    }
}

class Expression
{
public:
    using CharToNumberMapping = std::map<char, uint64_t>;
    using CharToNumberOptions = std::map<char, std::set<uint64_t>>;
    Expression(std::string a, std::string b, std::string r) : strA(std::move(a)), strB(std::move(b)), strR(std::move(r)), resolvedDeepth(0)
    {
        init();
    }

    Expression(std::string a, std::string b, std::string r, size_t resolvedDeepth, CharToNumberMapping& resolved, CharToNumberOptions& options)
    : strA(std::move(a)), strB(std::move(b)), strR(std::move(r)), resolvedDeepth(resolvedDeepth), resolved(resolved), options(options)
    {
    }

    uint64_t a() const
    {
        return toNumber(strA);
    }

    uint64_t b() const
    {
        return toNumber(strB);
    }

    uint64_t r() const
    {
        return toNumber(strR);
    }

    // validate current resolve if possible to complete the expression
    bool isValid() const
    {
        uint64_t numMod = 10;
        for(size_t i = 1; i < resolvedDeepth; ++i)
        {
            numMod *= 10;
        }
        return ((a() * b()) % numMod) == (r() % numMod);
    }

    bool isFinished() const
    {
        return options.empty();
    }

    // go with next
    std::vector<std::unique_ptr<Expression>> next()
    {
        std::vector<std::unique_ptr<Expression>> ret;
        resolvedDeepth++;
        std::set<char> toResolved = nextResolved();

        if(toResolved.empty() && (!isFinished()))
        {
            // no new character to be resolved, to next
            auto nextExp = std::make_unique<Expression>(strA, strB, strR, resolvedDeepth, resolved, options);
            ret.emplace_back(std::move(nextExp));
        }

        std::set<std::map<uint64_t, char>> tempResolves = getTempResolves(toResolved);

        for(auto& tempResolve : tempResolves)
        {
            // no new character to be resolved, to next
            auto nextExp = std::make_unique<Expression>(strA, strB, strR, resolvedDeepth, resolved, options);
            for(auto& pair : tempResolve)
            {
                nextExp->bind(pair.second, pair.first);
            }
            ret.emplace_back(std::move(nextExp));
        }

        return ret;
    }

private:
    // bind ch -> num
    void bind(const char ch, uint64_t num)
    {
        resolved.emplace(ch, num);
        options.erase(ch);
        std::for_each(options.begin(), options.end(), [&num](auto& opt) { opt.second.erase(num); });
    }

    // if ch resolved
    bool isResolved(const char ch)
    {
        return resolved.find(ch) != resolved.end();
    }

    // get a temp ch -> num mapping for new chars to be resolved
    std::set<std::map<uint64_t, char>> getTempResolves(const std::set<char>& toResolved)
    {
        std::set<std::map<uint64_t, char>> tempResolves;
        for(const auto& ch : toResolved)
        {
            std::set<std::map<uint64_t, char>> tempResolvesLocal;
            for(const auto& num : options.at(ch))
            {

                if(tempResolves.empty())
                {
                    std::map<uint64_t, char> newItem;
                    newItem.emplace(num, ch);
                    tempResolvesLocal.emplace(newItem);
                }
                else
                {
                    for(const auto& resolve : tempResolves)
                    {
                        if(!isResolved(ch))
                        {
                            std::map<uint64_t, char> newItem = resolve;
                            if(newItem.find(num) == newItem.end())
                            {
                                newItem.emplace(num, ch);
                                tempResolvesLocal.emplace(newItem);
                            }
                        }
                    }
                }
            }
            tempResolves = tempResolvesLocal;
        }
        return tempResolves;
    }

    // return next round tobe resolved chars
    std::set<char> nextResolved()
    {
        std::set<char> ret;
        for(const auto& str : {strA, strB, strR})
        {
            auto strLen = str.size();
            if(strLen >= resolvedDeepth)
            {
                const auto ch = str.at(strLen - resolvedDeepth);
                if(!isResolved(ch))
                {
                    ret.emplace(ch);
                }
            }
        }
        return std::move(ret);
    }

    // init internal data
    void init()
    {
        const uint64_t singleNumberLimit = 10;
        for(const auto& str : {strA, strB, strR})
        {
            auto it = options.emplace(str.front(), std::set<uint64_t>{});
            if(it.second)
            {
                for(uint64_t i = 1; i < singleNumberLimit; ++i)
                {
                    it.first->second.emplace(i);
                }
            }
        }
        for(const auto& str : {strA, strB, strR})
        {
            for(const auto ch : str)
            {
                auto it = options.emplace(ch, std::set<uint64_t>{});
                if(it.second)
                {
                    for(uint64_t i = 0; i < singleNumberLimit; ++i)
                    {
                        it.first->second.emplace(i);
                    }
                }
            }
        }
    }

    uint64_t toNumber(const std::string& str) const
    {
        uint64_t ret = 0;
        auto it = str.rbegin();
        auto itResolve = resolved.find(*it);
        uint64_t base = 1;
        while(itResolve != resolved.end())
        {
            ret += itResolve->second * base;
            base *= 10;
            ++it;
            itResolve = resolved.find(*it);
        }
        return ret;
    }

    const std::string strA;
    const std::string strB;
    const std::string strR;
    size_t resolvedDeepth;
    CharToNumberMapping resolved;
    CharToNumberOptions options;
};

void handle(const std::vector<std::string>& words)
{
    const size_t PatternCount = 4;
    const size_t PatternAPosition = 1;
    const size_t PatternBPosition = 2;
    const size_t PatternRPosition = 3;

    if(words.size() >= PatternCount)
    {
        auto expInit = std::make_unique<Expression>(words[PatternAPosition], words[PatternBPosition], words[PatternRPosition]);
        std::vector<std::unique_ptr<Expression>> exps;
        exps.emplace_back(std::move(expInit));

        while(!exps.empty())
        {
            std::vector<std::unique_ptr<Expression>> nextExps;
            for(const auto& exp : exps)
            {
                for(auto& nextExp : exp->next())
                {
                    if(nextExp->isValid())
                    {
                        if(nextExp->a() * nextExp->b() == nextExp->r() && nextExp->isFinished())
                        {
                            std::fprintf(stdout, "%lu * %lu = %lu\n", nextExp->a(), nextExp->b(), nextExp->r());
                        }
                        else
                        {
                            nextExps.emplace_back(std::move(nextExp));
                        }
                    }
                }
            }
            exps.clear();
            exps.swap(nextExps);
        }
    }
}
}

int main(int argc, const char* argv[])
{
    code::paramCheck(argc, argv);

    const std::string filename(argv[1]);
    auto loader = std::make_unique<code::DataLoader>(filename);
    loader->load(std::string("([A-Z]+) \\* ([A-Z]+) = ([A-Z]+)\n?"), [](const std::vector<std::string>& words) { code::handle(words); });

    return 0;
}
