// code contest 02
#pragma GCC optimize("-O3")

#include <cstdio>
#include <cstdlib>
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>

#define BUFF_SIZE (4096)
#define LEFT_MOVE1 (3)
#define LEFT_MOVE2 (1)

int main(int argc, const char* argv[])
{
    (void)argc;
    char buff[BUFF_SIZE];
    size_t m = 0;
    register uint32_t num = 0;
    register bool prevIsNum = false;
    register size_t pos = 0;
    std::vector<uint32_t> line;

    auto fd = open(argv[1], O_RDONLY);
    while(auto readBytes = ::read(fd, buff, BUFF_SIZE))
    {
        for(register int i = 0; i < readBytes; ++i)
        {
            register auto ch = buff[i];
            // '\r', '\n', ' ', ',' are all < '0'
            if(ch >= '0')
            {
                if(num > 0)
                {
                    // num x= 10
                    num = (num << LEFT_MOVE1) + (num << LEFT_MOVE2);
                }
                num += static_cast<uint32_t>(ch - '0');
                prevIsNum = true;
            }
            else
            {
                if(prevIsNum)
                {
                    if(pos == 0)
                    {
                        if(m == 0)
                        {
                            // first num in first line
                            m = num;
                            line = std::vector<uint32_t>(m + 1, 0);
                            num = 0;
                            prevIsNum = false;
                        }
                        else
                        {
                            // second num in first line
                            pos++;
                            num = 0;
                            prevIsNum = false;
                        }
                    }
                    else
                    {
                        // other nums
                        line[pos] = num + std::max(line[pos], line[pos - 1]);
                        if(pos == m)
                        {
                            pos = 1;
                        }
                        else
                        {
                            ++pos;
                        }
                        num = 0;
                        prevIsNum = false;
                    }
                }
            }
        }
    }
    if(prevIsNum)
    {
        line[pos] = num + std::max(line[pos], line[pos - 1]);
    }
    std::fprintf(stdout, "%u\n", line[m]);
    std::fflush(stdout);
    _exit(0);
    return 0;
}
