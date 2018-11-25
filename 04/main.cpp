// code contest 04
#pragma GCC optimize("-O2")

#include <algorithm>
#include <array>
#include <ctime>
#include <fstream>
#include <functional>
#include <iostream>
#include <list>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
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
    : nodeCount{n + extraNodeCount}, linkLimit{(nodeCount * nodeCount) << 1}, linkHeads(nodeCount, invalid), links(linkLimit, {0, 0, 0, 0}), tIndex{n + 1}
    {
    }

    // save graph to s-graph-t, s:0 t:n+1
    void load(const std::vector<std::string>& words)
    {
        if(words.size() > 0)
        {
            auto it = words.begin();
            ++tempIndex;
            auto value = static_cast<ssize_t>(std::strtol(it->c_str(), nullptr, 10));
            it++;
            std::vector<size_t> deps;
            deps.reserve(words.size() - 1);

            while(it != words.end())
            {
                auto depIndex = static_cast<size_t>(std::strtoul(it->c_str(), nullptr, 10));
                deps.emplace_back(depIndex);
                ++it;
            }

            bool merged = false;
            for(auto& n : nodes)
            {
                if(tryMerge(n.second, tempIndex, value, deps))
                {
                    merged = true;
                    break;
                }
            }
            if(!merged)
            {
                Node node;
                node.value = value;
                node.features.emplace(tempIndex);
                for(const auto dep : deps)
                {
                    node.deps.emplace(dep);
                }
                nodes.emplace(tempIndex, node);
            }
        }
    }

    void prepareLink()
    {
        for(const auto& node : nodes)
        {
            if(node.second.value > 0)
            {
                link(sIndex, node.first, static_cast<size_t>(node.second.value));
                valueSum += node.second.value;
            }
            else
            {
                link(node.first, tIndex, static_cast<size_t>(-node.second.value));
            }
            for(const auto dep : node.second.deps)
            {
                if(nodes.find(dep) != nodes.end())
                {
                    link(node.first, dep, infinity);
                }
            }
        }
    }

    void process()
    {
        prepareLink();
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
    using Node = struct
    {
        ssize_t value;
        std::unordered_set<size_t> features;
        std::unordered_set<size_t> deps;
    };
    using Link = struct
    {
        size_t begin;
        size_t end;
        size_t capacity;
        size_t next;
    };

    bool tryMerge(Node& a, size_t newFeature, ssize_t value, std::vector<size_t>& deps)
    {
        bool match = false;
        for(const auto feature : a.features)
        {
            for(const auto dep : deps)
            {
                if(feature == dep)
                {
                    match = true;
                    break;
                }
            }
            if(match)
            {
                break;
            }
        }
        if(match && (a.deps.find(newFeature) != a.deps.end()))
        {
            a.features.emplace(newFeature);
            a.value += value;
            if(a.deps.size() < nodeCount - extraNodeCount)
            {
                std::for_each(deps.begin(), deps.end(), [&a](const auto& dep) { a.deps.emplace(dep); });
            }
            return true;
        }
        return false;
    }

    void link(size_t from, size_t to, size_t weight)
    {
        links[linkCount] = {from, to, weight, linkHeads.at(from)};
        linkHeads[from] = linkCount++;
        links[linkCount] = {to, from, 0, linkHeads.at(to)};
        linkHeads[to] = linkCount++;
    }

    bool search()
    {
        std::list<size_t> queue{sIndex};

        nodeDepthes.clear();
        nodeDepthes.emplace(sIndex, 0);

        while(!queue.empty())
        {
            auto tempBegin = queue.front();
            queue.pop_front();
            auto index = linkHeads[tempBegin];
            while(index != invalid)
            {
                auto& lnk = links[index];
                auto tempEnd = lnk.end;
                if(lnk.capacity > 0 && nodeDepthes.find(tempEnd) == nodeDepthes.end())
                {
                    nodeDepthes.emplace(tempEnd, nodeDepthes[tempBegin] + 1);
                    queue.emplace_back(tempEnd);
                    if(tempEnd == tIndex)
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
        std::vector<size_t> tempHeads(linkHeads);
        std::vector<size_t> cache(nodeCount, 0);
        while(search())
        {
            std::copy(linkHeads.begin(), linkHeads.end(), tempHeads.begin());
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
                auto index = tempHeads[currIndex];
                while(index != invalid)
                {
                    auto& lnk = links[index];
                    if(lnk.capacity > 0 && nodeDepthes[currIndex] + 1 == nodeDepthes[lnk.end])
                    {
                        break;
                    }
                    index = lnk.next;
                    tempHeads[currIndex] = index;
                }
                if(tempHeads[currIndex] == invalid)
                {
                    if(top != 0)
                    {
                        top--;
                        nodeDepthes[currIndex] = invalid;
                        currIndex = links[cache[top]].begin;
                    }
                    else
                    {
                        break;
                    }
                }
                else
                {
                    cache[top] = tempHeads[currIndex];
                    currIndex = links[tempHeads[currIndex]].end;
                    top++;
                }
            }
        }
        return result;
    }

    void enumerateSelectedNodes(size_t curr, std::set<size_t>& selected)
    {
        for(const auto f : nodes[curr].features)
        {
            selected.emplace(f);
        }
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
    std::unordered_map<size_t, size_t> nodeDepthes;
    std::vector<Link> links;
    std::unordered_map<size_t, Node> nodes;

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
