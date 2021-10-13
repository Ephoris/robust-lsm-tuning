#include <algorithm>
#include <iostream>
#include <fstream>
#include <random>
#include <vector>

#include "clipp.h"
#include "spdlog/spdlog.h"

typedef struct environment
{
    std::string key_file;

    size_t num_keys = 1000000;
} environment;


environment parse_args(int argc, char * argv[])
{
    using namespace clipp;
    using std::to_string;

    environment env;
    bool help = false;

    auto cli = "general options" % (
        (option("-h", "--help").set(help, true)) % "prints this message",
        (value("key_file", env.key_file)) % "path to the db",
        (option("-n", "--num_keys") & integer("num", env.num_keys))
            % ("number of sessions, [default: " + to_string(env.num_keys) + "]")
    );

    if (!parse(argc, argv, cli) || help)
    {
        auto fmt = doc_formatting{}.doc_column(42);
        std::cout << make_man_page(cli, "exp_robust", fmt);
        exit(EXIT_FAILURE);
    }

    return env;
}


int main(int argc, char * argv[])
{
    spdlog::set_pattern("[%T.%e]%^[%l]%$ %v");
    environment env = parse_args(argc, argv);

    spdlog::info("Welcome to key generator");
    spdlog::info("Generating {} keys", env.num_keys);

    std::random_device rd;
    std::mt19937 gen(rd());

    std::vector<int> vec(env.num_keys);
    std::iota(vec.begin(), vec.end(), 0);
    std::shuffle(vec.begin(), vec.end(), gen);

    spdlog::info("Writing keys to {}", env.key_file);
    std::ofstream fid(env.key_file, std::ios::out);
    if (!fid)
    {
        spdlog::warn("Cannot create file");
        return 1;
    }

    for (auto key : vec)
    {
        fid << key << std::endl;
    }
    fid.close();

    if (!fid.good())
    {
        spdlog::warn("Error occured during writing");
        return 1;
    }
    
    return 0;
}