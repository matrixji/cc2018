// code contest 05
#pragma GCC optimize("-O2")

#include <cmath>
#include <cstdlib>
#include <fstream>
#include <functional>
#include <iostream>
#include <list>
#include <regex>
#include <set>
#include <vector>

namespace code
{

#include "../common/DataLoader.hpp"

#include "../common/helper.hpp"

class Solution
{
public:
    void train(const std::string& name, bool isMale)
    {
        if(isMale)
        {
            for(const auto& pattern : getPatterns(name))
            {
                patternCountMale_.emplace(pattern, 0);
                patternCountMale_[pattern]++;
            }
            countMale_++;
        }
        else
        {
            for(const auto& pattern : getPatterns(name))
            {
                patternCountFemale_.emplace(pattern, 0);
                patternCountFemale_[pattern]++;
            }
            countFemale_++;
        }
    }

    void trainComplete()
    {
        double freqSum = 0.0;
        double baseFreqSum = 0.0;
        for(const auto& pair : patternCountMale_)
        {
            auto freq = 1.0 * pair.second / countMale_;
            freqMale_[pair.first] = freq;
            freqSum += freq;
            baseFreqSum += log(1 - freq);
        }
        baseFreqMale_ = log(1.0 * countMale_ / (countMale_ + countFemale_)) + baseFreqSum;

        freqSum = 0.0;
        baseFreqSum = 0.0;
        for(const auto& pair : patternCountFemale_)
        {
            auto freq = 1.0 * pair.second / countFemale_;
            freqFemale_[pair.first] = freq;
            freqSum += freq;
            baseFreqSum += log(1 - freq);
        }
        baseFreqFemale_ = log(1.0 * countFemale_ / (countFemale_ + countMale_)) + baseFreqSum;
    }

    bool test(const std::string& name)
    {
        auto freqMale = baseFreqMale_;
        auto freqFemale = baseFreqFemale_;
        auto patterns = getPatterns(name);
        for(size_t i = 0; i < patterns.size(); ++i)
        {
            const auto& pattern = patterns[i];
            freqMale += logP(pattern, freqMale_, countMale_);
            freqFemale += logP(pattern, freqFemale_, countFemale_);
        }
        return freqMale > freqFemale;
    }

private:
    std::vector<std::string> getPatterns(const std::string& name)
    {
        std::vector<std::string> ret;
        char buff[64] = {0};
        size_t buffLen = 0;
        std::stringstream ss;

        if(name.length() >= sizeof(buff))
        {
            return ret;
        }
        for(const char& ch : name)
        {
            if(buffLen > 1 && isVowel(ch) && (!isVowel(buff[buffLen - 1])))
            {
                char temp = buff[buffLen - 1];
                buff[buffLen - 1] = 0;
                ret.emplace_back(std::string(buff));
                buff[buffLen - 1] = 0;
                ret.emplace_back(std::string(buff));
                for(size_t i = 1; i < buffLen; ++i)
                {
                    buff[i] = 0;
                }
                buffLen = 1;
                buff[0] = temp;
            }
            buff[buffLen++] = ch;
        }
        if(buffLen > 0)
        {
            // if last add a $ to identify this is a suffix
            ret.emplace_back(std::string(buff) + '$');
        }
        // first & last character as pattern
        ret.emplace_back(name.substr(name.size() - 1, 1) + '$');
        ret.emplace_back(name);
        return ret;
    }

    bool inline isVowel(const char ch)
    {
        // w & y alwasy sound like some Vowels, w -> o|u; y -> i
        switch(ch)
        {
        case 'w':
        case 'y':
        case 'a':
        case 'e':
        case 'i':
        case 'o':
        case 'u':
            return true;
        default:
            return false;
        }
    }

    double logP(const std::string& pattern, std::map<std::string, double>& freq, uint64_t total)
    {
        const double adjust = 1.0;
        auto it = freq.emplace(pattern, 0.0);
        auto count = it.first->second * total;
        auto freqSmooth = (count + adjust) / (total + freq.size() * adjust);
        return log(freqSmooth) - log(1 - freqSmooth);
    }

    uint64_t countMale_ = 0;
    uint64_t countFemale_ = 0;
    std::map<std::string, uint64_t> patternCountMale_;
    std::map<std::string, uint64_t> patternCountFemale_;
    std::map<std::string, double> freqMale_;
    std::map<std::string, double> freqFemale_;
    double baseFreqMale_;
    double baseFreqFemale_;
};

void handleTrain(const std::vector<std::string>& words, Solution& solution)
{
    solution.train(words.front(), words.back()[0] == 'm');
}

void handleTesting(const std::string& name, Solution& solution)
{
    if(solution.test(name))
    {
        std::fprintf(stdout, "%s,male\n", name.c_str());
    }
    else
    {
        std::fprintf(stdout, "%s,female\n", name.c_str());
    }
}
} // namespace code

int main(int argc, const char* argv[])
{
    code::paramCheck(argc, argv);

    const std::string training = "training_dataset.txt";
    const std::string testing(argv[1]);

    code::DataLoader loader(training);
    code::DataLoader tester(testing);

    code::Solution solution;
    loader.load(',', [&solution](std::vector<std::string>& words) { code::handleTrain(words, solution); });
    solution.trainComplete();

    tester.load([&solution](const std::string& name) { code::handleTesting(name, solution); });
    return 0;
}
