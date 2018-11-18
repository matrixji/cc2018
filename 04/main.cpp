// code contest 04
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

namespace code
{

#include "../common/DataLoader.hpp"

#include "../common/helper.hpp"

class Feature
{
public:
    uint32_t index{0};
    int32_t value{0};
    std::set<uint32_t> deps;
};

using FeatureGroup = std::map<uint32_t, Feature>;

//
// find best solution and output
void solution(FeatureGroup& features)
{
    int32_t bestResult = 0;
    size_t targetNumber = 1;
    FeatureGroup selectedFeatures;

    while(features.size() >= targetNumber)
    {
        bool resolved = false;
        for(const auto& pair : features)
        {
            if(pair.second.value > 0)
            {
                auto sum = pair.second.value;
                std::set<uint32_t> selectedTempIndexes{pair.first};
                std::set<uint32_t> unResolvedTempDeps{pair.second.deps};

                while(unResolvedTempDeps.size() > 0 && sum > 0 && selectedTempIndexes.size() <= targetNumber)
                {
                    std::set<uint32_t> nextUnResolvedTempDeps;
                    for(const auto& depIndex : unResolvedTempDeps)
                    {
                        if(selectedFeatures.find(depIndex) == selectedFeatures.end())
                        {
                            auto ret = selectedTempIndexes.emplace(depIndex);
                            if(ret.second)
                            {
                                auto it = features.find(depIndex);
                                sum += it->second.value;
                                for(const auto nextDepIndex : it->second.deps)
                                {
                                    nextUnResolvedTempDeps.emplace(nextDepIndex);
                                }
                            }
                        }
                    }
                    unResolvedTempDeps = nextUnResolvedTempDeps;
                }

                if(unResolvedTempDeps.size() == 0 && sum > 0 && selectedTempIndexes.size() <= targetNumber)
                {
                    bestResult += sum;
                    for(const auto& index : selectedTempIndexes)
                    {
                        auto it = features.find(index);
                        selectedFeatures.emplace(index, std::move(it->second));
                        features.erase(index);
                    }
                    resolved = true;
                    break;
                }
            }
        }

        if(!resolved)
        {
            targetNumber++;
        }
    }

    CONSOLE << bestResult << std::endl;
    for(const auto& feature : selectedFeatures)
    {
        CONSOLE << feature.first + 1 << std::endl;
    }
}

void handle(const std::vector<std::string>& words, FeatureGroup& features, size_t& featureIndex)
{
    if(features.empty())
    {
        auto count = static_cast<size_t>(::strtoul(words.front().c_str(), nullptr, 10));
        for(uint32_t i = 0; i < count; ++i)
        {
            Feature f;
            f.index = i;
            features.emplace(i, f);
        }
    }
    else
    {
        if(!words.empty())
        {
            auto itFeature = features.find(featureIndex);
            auto itWord = words.begin();
            itFeature->second.value = static_cast<int32_t>(::strtol(itWord->c_str(), nullptr, 10));
            ++itWord;
            while(itWord != words.end())
            {
                auto depIndex = static_cast<uint32_t>(::strtoul(itWord->c_str(), nullptr, 10)) - 1;
                itFeature->second.deps.emplace(depIndex);
                ++itWord;
            }
            ++featureIndex;
        }
    }
}
} // namespace code

int main(int argc, const char* argv[])
{
    code::paramCheck(argc, argv);

    const std::string filename(argv[1]);
    auto loader = std::make_unique<code::DataLoader>(filename);
    code::FeatureGroup features;
    size_t featureIndex = 0;
    loader->load(' ', [&features, &featureIndex](std::vector<std::string>& words) { code::handle(words, features, featureIndex); });
    code::solution(features);
    return 0;
}