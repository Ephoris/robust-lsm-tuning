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
    bool plain_text = false;
} environment;


environment parse_args(int argc, char * argv[])
{
    using namespace clipp;
    using std::to_string;

    environment env;
    bool help = false;

    auto cli = "general options" % (
        (value("key_file", env.key_file)) % "path to keyfile",
        (option("-h", "--help").set(help, true)) % "prints this message",
        (option("-n", "--num_keys") & integer("num", env.num_keys))
            % ("number of sessions, [default: " + to_string(env.num_keys) + "]"),
        (option("-p", "--plain").set(env.plain_text))
            % ("writes out to binary [default: " + to_string(env.plain_text) + "]")
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
    std::fstream fid;

    if (env.plain_text)
    {
        fid.open(env.key_file, std::ios::out);
        for (auto key : vec)
        {
            fid << key << std::endl;
        }
    }
    else
    {
        fid.open(env.key_file, std::ios::out | std::ios::binary);
        fid.write(reinterpret_cast<char *>(&vec[0]), env.num_keys * sizeof(vec[0]));
    }
    fid.close();

    if (!fid.good())
    {
        spdlog::warn("Error occured during writing");
        return 1;
    }
    
    return 0;
}