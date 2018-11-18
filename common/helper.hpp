
#define CONSOLE std::cout // NOSONAR

// filter begin: 01 03 04 05
// all code contest have one file input parameter
void paramCheck(int argc, const char* argv[])
{
    if(argc <= 1)
    {
        CONSOLE << "usage: " << std::string(argv[0]) << " <input>" << std::endl;
        std::exit(-1);
    }
}
// filter end
