// data loader for code contest
class DataLoader
{
public:
    explicit DataLoader(const std::string& filename) : fs(filename)
    {
    }

    // filter begin: 01 02
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
    // filter end

    // filter begin: 02
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
    // filter end

    // filter begin: 01
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
    // filter end

private:
    std::ifstream fs;
};
