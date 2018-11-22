// code contest 03
#pragma GCC optimize("-O2")

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <functional>
#include <iostream>
#include <list>
#include <sched.h>
#include <sstream>
#include <sys/types.h>
#include <sys/wait.h>
#include <thread>
#include <unordered_map>
#include <vector>

namespace code
{

#include "../common/DataLoader.hpp"

#include "../common/helper.hpp"

class Solution
{
    // using bit to store test line combinations
    // as total <=20, 32bit is ok
    // 0x0003 -> means test line: 1 & 2 are selected.
    using Combinations = std::unordered_map<uint32_t, size_t>;

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
        size_t total = 0;
        const size_t usingThreadCountLimit = 1000;
        const size_t stackSize = 1024;

        if(data.size() == 1)
        {
            return data.front().size();
        }

        merge();

        size_t lastCombsSize = data.back().size();
        if(lastCombsSize < usingThreadCountLimit)
        {
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
        }
        else
        {
            // using threaded for accelerate last counting if have many.
            auto threadCount = std::thread::hardware_concurrency();
            std::vector<int> pids;
            std::vector<std::unique_ptr<Arg>> args;
            std::vector<std::unique_ptr<char[]>> stacks;

            pids.reserve(threadCount);

            auto it = data.back().begin();
            auto itEnd = it;
            size_t eachSize = lastCombsSize / threadCount;
            for(size_t i = 0; i < threadCount; ++i)
            {
                if(i == threadCount - 1)
                {
                    itEnd = data.back().end();
                }
                else
                {
                    std::advance(itEnd, eachSize);
                }
                // -lpthread not provided, using clone.
                auto arg = std::make_unique<Arg>(Arg{it, itEnd, data.front(), 0});
                std::unique_ptr<char[]> stack(new char[stackSize]);
                int pid = ::clone(&Solution::countingFunc, static_cast<void*>(stack.get() + stackSize), CLONE_VM | SIGCHLD,
                                  static_cast<void*>(arg.get()));
                pids.emplace_back(pid);
                args.emplace_back(std::move(arg));
                stacks.emplace_back(std::move(stack));
                it = itEnd;
            }
            for(auto& pid : pids)
            {
                waitpid(pid, nullptr, WUNTRACED | WCONTINUED);
            }
            for(const auto& arg : args)
            {
                total += arg->sum;
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

    // merge data to size() == 2
    void merge()
    {
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
    }

    using Arg = struct
    {
        Combinations::iterator curr;
        Combinations::iterator end;
        Combinations& cbs;
        size_t sum;
    };

    static int countingFunc(void* data)
    {
        Arg* arg = (static_cast<Arg*>(data));

        while(arg->curr != arg->end)
        {
            for(const auto& cb : arg->cbs)
            {
                if((cb.first & arg->curr->first) == 0)
                {
                    arg->sum += cb.second * arg->curr->second;
                }
            }
            ++arg->curr;
        }
        return 0;
    }
};

} // namespace code

int main(int argc, const char* argv[])
{
    code::paramCheck(argc, argv);

    const std::string filename(argv[1]);
    code::DataLoader loader(filename);
    code::Solution solution;
    loader.skipLine();
    loader.load(',', [&solution](const std::vector<std::string>& words) { solution.load(words); });
    std::fprintf(stdout, "%lu\n", solution.calculate());
    return 0;
}
