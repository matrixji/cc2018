
// filter begin: 01 03 04 05
// all code contest have one file input parameter
void paramCheck(int argc, const char* argv[])
{
    if(argc <= 1)
    {
        std::fprintf(stdout, "usage %s <input>", argv[0]);
        std::exit(-1);
    }
}
// filter end
