// code contest 01

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

class Expression
{
public:
    using CharToNumberMapping = std::map<char, uint64_t>;
    using CharToNumberOptions = std::map<char, std::set<uint64_t>>;
    Expression(const std::string a, const std::string b, const std::string r)
    : strA(std::move(a)), strB(std::move(b)), strR(std::move(r)), resolvedDeepth(0)
    {
        init();
    }

    Expression(const std::string& a, const std::string& b, const std::string& r, size_t resolvedDeepth, CharToNumberMapping& resolved,
               CharToNumberOptions& options)
    : strA(a), strB(b), strR(r), resolvedDeepth(resolvedDeepth), resolved(resolved), options(options)
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
        for(const std::string& str : {strA, strB, strR})
        {
            auto strLen = str.length();
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
        options.emplace(*strA.begin(), std::set<uint64_t>{1, 2, 3, 4, 5, 6, 7, 8, 9});
        options.emplace(*strB.begin(), std::set<uint64_t>{1, 2, 3, 4, 5, 6, 7, 8, 9});
        options.emplace(*strR.begin(), std::set<uint64_t>{1, 2, 3, 4, 5, 6, 7, 8, 9});
        for(const char& ch : strA)
        {
            options.emplace(ch, std::set<uint64_t>{0, 1, 2, 3, 4, 5, 6, 7, 8, 9});
        }
        for(const char& ch : strB)
        {
            options.emplace(ch, std::set<uint64_t>{0, 1, 2, 3, 4, 5, 6, 7, 8, 9});
        }
        for(const char& ch : strR)
        {
            options.emplace(ch, std::set<uint64_t>{0, 1, 2, 3, 4, 5, 6, 7, 8, 9});
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

    std::string strA;
    std::string strB;
    std::string strR;
    size_t resolvedDeepth;
    CharToNumberMapping resolved;
    CharToNumberOptions options;
};

// filter start
void ut()
{
}
// filter end

void handle(const std::vector<std::string>& words)
{
    if(words.size() >= 4)
    {
        auto expInit = std::make_unique<Expression>(words[1], words[2], words[3]);
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
                            std::cout << nextExp->a() << " * " << nextExp->b() << " = " << nextExp->r() << std::endl;
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

int main(int argc, const char* argv[])
{
    paramCheck(argc, argv);

    const std::string filename(argv[1]);
    auto loader = std::make_unique<DataLoader>(filename);
    loader->load(std::string("([A-Z]+) \\* ([A-Z]+) = ([A-Z]+)\n?"), [&](const std::vector<std::string>& words) { handle(words); });

    return 0;
}
