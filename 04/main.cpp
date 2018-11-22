// code contest 05
#pragma GCC optimize("-O2")

#include <algorithm>
#include <array>
#include <ctime>
#include <fstream>
#include <functional>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <vector>

namespace code
{
static const size_t infinity = SIZE_MAX;
static const size_t invalid = SIZE_MAX;
static const size_t extraNodeCount = 2;

#include "../common/DataLoader.hpp"

#include "../common/helper.hpp"

class Solution
{
public:
    explicit Solution(size_t n)
    : nodeCount{n + extraNodeCount}
    , linkLimit{nodeCount * nodeCount}
    , linkHeads(nodeCount, invalid)
    , deps(nodeCount, invalid)
    , links(linkLimit, {0, 0, 0, 0})
    , tIndex{n + 1}
    {
    }

    // save graph to s-graph-t, s:0 t:n+1
    void load(const std::vector<std::string>& words)
    {
        if(words.size() > 0)
        {
            auto it = words.begin();
            ++tempIndex;
            auto value = static_cast<int32_t>(std::strtol(it->c_str(), nullptr, 10));
            it++;

            if(value > 0)
            {
                link(sIndex, tempIndex, static_cast<size_t>(value));
                valueSum += value;
            }
            else
            {
                link(tempIndex, tIndex, static_cast<size_t>(-value));
            }
            while(it != words.end())
            {
                auto depIndex = static_cast<size_t>(std::strtoul(it->c_str(), nullptr, 10));
                link(tempIndex, depIndex, infinity);
                ++it;
            }
        }
    }

    void process()
    {
        auto minimalCut = resolveMinimalCut();
        if(valueSum <= minimalCut)
        {
            std::fprintf(stdout, "0\n");
            return;
        }
        std::fprintf(stdout, "%lu\n", valueSum - minimalCut);

        std::set<size_t> selected;
        enumerateSelectedNodes(sIndex, selected);
        std::for_each(selected.begin(), selected.end(), [this](const auto& v) {
            if(v != this->sIndex)
            {
                std::fprintf(stdout, "%lu\n", v);
            }
        });
    }

private:
    using Link = struct
    {
        size_t begin;
        size_t end;
        size_t capacity;
        size_t next;
    };

    void link(size_t from, size_t to, size_t weight)
    {
        links[linkCount] = {from, to, weight, linkHeads.at(from)};
        linkHeads[from] = linkCount++;
        links[linkCount] = {to, from, 0, linkHeads.at(to)};
        linkHeads[to] = linkCount++;
    }

    bool search()
    {
        std::vector<size_t> queue(nodeCount, invalid);
        size_t front{0};
        size_t back{1};

        std::fill(deps.begin(), deps.end(), invalid);
        queue[0] = sIndex;
        deps[sIndex] = 0;

        while(front != back)
        {
            auto tmpBegin = queue[front++];
            front %= nodeCount;
            auto index = linkHeads[tmpBegin];
            while(index != invalid)
            {
                auto& link = links[index];
                auto tmpEnd = link.end;
                if(link.capacity > 0 && deps[tmpEnd] == invalid)
                {
                    deps[tmpEnd] = deps[tmpBegin] + 1;
                    queue[back] = tmpEnd;
                    back++;
                    back %= nodeCount;
                    if(tmpEnd == tIndex)
                    {
                        return true;
                    }
                }
                index = links[index].next;
            }
        }
        return false;
    }

    size_t resolveMinimalCut()
    {
        size_t result{0};
        size_t top{0};
        std::vector<size_t> tmpHeads(linkHeads);
        std::vector<size_t> cache(nodeCount, 0);
        while(search())
        {
            std::copy(linkHeads.begin(), linkHeads.end(), tmpHeads.begin());
            auto currIndex = sIndex;
            top = 0;
            while(true)
            {
                size_t tempTop{0};
                auto minimalCapacity = infinity;
                if(currIndex == tIndex)
                {
                    for(size_t i = 0; i < top; ++i)
                    {
                        auto cap = links[cache[i]].capacity;
                        if(minimalCapacity > cap)
                        {
                            minimalCapacity = cap;
                            tempTop = i;
                        }
                    }
                    for(size_t i = 0; i < top; ++i)
                    {
                        auto index = cache[i];
                        links[index].capacity -= minimalCapacity;
                        links[index ^ 1].capacity += minimalCapacity;
                    }
                    result += minimalCapacity;
                    currIndex = links[cache[tempTop]].begin;
                    top = tempTop;
                }
                auto index = tmpHeads[currIndex];
                while(index != invalid)
                {
                    auto& link = links[index];
                    if(link.capacity > 0 && deps[currIndex] + 1 == deps[link.end])
                    {
                        break;
                    }
                    index = link.next;
                    tmpHeads[currIndex] = index;
                }
                if(tmpHeads[currIndex] == invalid)
                {
                    if(top != 0)
                    {
                        top--;
                        deps[currIndex] = invalid;
                        currIndex = links[cache[top]].begin;
                    }
                    else
                    {
                        break;
                    }
                }
                else
                {
                    cache[top] = tmpHeads[currIndex];
                    currIndex = links[tmpHeads[currIndex]].end;
                    top++;
                }
            }
        }
        return result;
    }

    void enumerateSelectedNodes(size_t curr, std::set<size_t>& selected)
    {
        selected.emplace(curr);
        auto index = linkHeads[curr];
        while(index != invalid)
        {
            auto linked = links[index].end;
            if(selected.find(linked) == selected.end() && links[index].capacity > 0)
            {
                enumerateSelectedNodes(linked, selected);
            }
            index = links[index].next;
        }
    }

    size_t nodeCount;
    size_t linkLimit;
    size_t linkCount{0};
    std::vector<size_t> linkHeads;
    std::vector<size_t> deps;
    std::vector<Link> links;

    size_t tempIndex{0};
    size_t sIndex{0};
    size_t tIndex;
    size_t valueSum{0};
};
} // namespace code

int main(int argc, const char* argv[])
{
    code::paramCheck(argc, argv);

    const std::string filename(argv[1]);
    code::DataLoader loader(filename);

    code::Solution solution{loader.getCount()};
    loader.load(' ', [&solution](const std::vector<std::string>& words) { solution.load(words); });
    solution.process();
    return 0;
}
