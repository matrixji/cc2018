
auto& console()
{
    return std::cout; // NOSONAR
}

// all code contest have one file input parameter
void paramCheck(int argc, const char* argv[])
{
    if(argc <= 1)
    {
        console() << "usage: " << std::string(argv[0]) << " <input>" << std::endl;
        std::exit(-1);
    }

    // filter begin
    if(std::string(argv[1]) == "ut")
    {
        ut();
        exit(0);
    }
    // filter end
}
