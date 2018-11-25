// code contest 01
#pragma GCC optimize("-O2")

#include <fstream>
#include <functional>
#include <iostream>
#include <list>
#include <regex>
#include <string>
#include <vector>

namespace code
{

#include "../common/DataLoader.hpp"

#include "../common/helper.hpp"

const uint8_t invalid = 0xff;
const uint8_t atozNums = 26;
const uint8_t firstChar = 'A';
const uint8_t ten = 10;

class Solution
{
public:
    class Cache
    {
    public:
        Cache()
        {
            mapping.fill(invalid);
            nums.fill(0);
        }
        size_t a{0};
        size_t b{0};
        size_t r{0};
        std::array<uint8_t, atozNums> mapping;
        std::array<uint8_t, ten> nums;
        size_t depth{0};
        size_t modNum{1};
    };

    void processA(const std::string& strA, size_t lenA, const Cache& cache, std::vector<Cache>& newCaches)
    {
        std::vector<Cache> newTempCaches{};
        newCaches.emplace_back(cache);
        for(const auto& cacheA : newCaches)
        {
            if(cacheA.depth < lenA)
            {
                auto ch = strA[lenA - cacheA.depth - 1] - firstChar;
                auto valA = cacheA.mapping[ch];
                if(valA != invalid)
                {
                    Cache newCacheA = cacheA;
                    auto added = valA * cacheA.modNum;
                    newCacheA.a += added;
                    newTempCaches.emplace_back(newCacheA);
                }
                else
                {
                    size_t start = 0;
                    if(lenA - cacheA.depth == 1)
                    {
                        start = 1;
                    }
                    for(size_t i = start; i < ten; ++i)
                    {
                        if(cacheA.nums[i] == 0)
                        {
                            Cache newCacheA = cacheA;
                            newCacheA.mapping[ch] = i;
                            newCacheA.nums[i] = 1;
                            auto added = i * cacheA.modNum;
                            newCacheA.a += added;
                            newTempCaches.emplace_back(newCacheA);
                        }
                    }
                }
            }
            else
            {
                newTempCaches.emplace_back(cacheA);
            }
        }
        newCaches.swap(newTempCaches);
    }

    void processB(const std::string& strB, size_t lenB, std::vector<Cache>& newCaches)
    {

        std::vector<Cache> newTempCaches{};
        for(const auto& cacheB : newCaches)
        {
            if(cacheB.depth < lenB)
            {
                auto ch = strB[lenB - cacheB.depth - 1] - firstChar;
                auto valB = cacheB.mapping[ch];
                if(valB != invalid)
                {
                    Cache newCacheB = cacheB;
                    auto added = valB * cacheB.modNum;
                    newCacheB.b += added;
                    newTempCaches.emplace_back(newCacheB);
                }
                else
                {
                    size_t start = 0;
                    if(lenB - cacheB.depth == 1)
                    {
                        start = 1;
                    }
                    for(size_t i = start; i < ten; ++i)
                    {
                        if(cacheB.nums[i] == 0)
                        {
                            Cache newCacheB = cacheB;
                            newCacheB.mapping[ch] = i;
                            newCacheB.nums[i] = 1;
                            auto added = i * cacheB.modNum;
                            newCacheB.b += added;
                            newTempCaches.emplace_back(newCacheB);
                        }
                    }
                }
            }
            else
            {
                newTempCaches.emplace_back(cacheB);
            }
        }
        newCaches.swap(newTempCaches);
    }

    void process(const std::vector<std::string>& words)
    {
        auto it = words.begin();
        ++it;
        auto& strA = *it;
        ++it;
        auto& strB = *it;
        ++it;
        auto& strR = *it;
        ++it;
        auto lenA = strA.size();
        auto lenB = strB.size();
        auto lenR = strR.size();

        std::list<Cache> caches{};
        caches.emplace_back(Cache());

        while(caches.size() > 0)
        {
            Cache cache = caches.front();
            caches.pop_front();

            std::vector<Cache> newCaches{};
            processA(strA, lenA, cache, newCaches);
            processB(strB, lenB, newCaches);
            for(const auto& cacheR : newCaches)
            {
                if(cacheR.depth < lenR)
                {
                    auto ch = strR[lenR - cacheR.depth - 1] - firstChar;
                    auto r = cacheR.a * cacheR.b % (cacheR.modNum * 10);
                    auto rHead = r / cacheR.modNum;

                    if(rHead != 0 || lenR - cacheR.depth > 1)
                    {

                        auto valR = cacheR.mapping[ch];
                        if(valR == invalid && cacheR.nums[rHead] == 0)
                        {
                            auto newCacheR = cacheR;
                            newCacheR.mapping[ch] = rHead;
                            newCacheR.nums[rHead] = 1;
                            newCacheR.r = r;
                            newCacheR.depth++;
                            newCacheR.modNum *= 10;
                            caches.emplace_back(newCacheR);
                        }
                        else
                        {
                            if(cacheR.mapping[ch] == rHead)
                            {
                                auto newCacheR = cacheR;
                                newCacheR.r = r;
                                newCacheR.depth++;
                                newCacheR.modNum *= 10;
                                caches.emplace_back(newCacheR);
                            }
                        }
                    }
                }
                else
                {
                    if(cacheR.a * cacheR.b == cacheR.r)
                    {

                        std::fprintf(stdout, "%lu * %lu = %lu\n", cacheR.a, cacheR.b, cacheR.r);
                    }
                }
            }
        }
    }
};

} // namespace code

int main(int argc, const char* argv[])
{
    code::paramCheck(argc, argv);

    const std::string filename(argv[1]);
    auto loader = std::make_unique<code::DataLoader>(filename);
    code::Solution solution;
    loader->load(std::string("([A-Z]+) \\* ([A-Z]+) = ([A-Z]+)\n?"),
                 [&solution](const std::vector<std::string>& words) { solution.process(words); });
    return 0;
}
