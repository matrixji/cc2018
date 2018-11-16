// common helpers
template <typename T> T lexical_cast(const std::string &str) {
  T var;
  std::istringstream iss;
  iss.str(str);
  iss >> var;
  return var;
};

// filter start
void ut();
// filter end

// all code contest have one file input parameter
void paramCheck(int argc, const char *argv[]) {
  if (argc != 2) {
    std::cout << "usage: " << std::string(argv[0]) << " <input>" << std::endl;
    std::exit(-1);
  }

  // filter start
  if (std::string(argv[1]) == "ut") {
    ut();
    exit(0);
  }
  // filter end
}
