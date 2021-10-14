#include "data_generator.hpp"

RandomGenerator::RandomGenerator(int seed)
{
    this->seed = seed;
    this->engine.seed(this->seed);
    srand(this->seed);
    this->dist_left = std::uniform_int_distribution<int>(KEY_BOTTOM, KEY_MIDDLE_LEFT);
    this->dist_right = std::uniform_int_distribution<int>(KEY_MIDDLE_RIGHT, KEY_DOMAIN);
}


RandomGenerator::RandomGenerator()
{
    RandomGenerator(0);
}


std::string RandomGenerator::generate_rnd()
{
    if (std::rand() % 2)
    {
        return std::to_string(this->dist_left(this->engine));
    }
    else
    {
        return std::to_string(this->dist_right(this->engine));
    }
}


std::pair<std::string, std::string> DataGenerator::generate_kv_pair(size_t kv_size)
{
    return this->generate_kv_pair(kv_size, "", "");
}


std::pair<std::string, std::string> DataGenerator::generate_kv_pair(
    size_t kv_size,
    const std::string key_prefix,
    const std::string value_prefix)
{
    std::string key = this->generate_key(key_prefix);
    assert(key.size() < kv_size && "Requires larger key size");
    size_t value_size = kv_size - key.size();
    std::string value = this->generate_val(value_size, value_prefix);

    return std::pair<std::string, std::string>(key, value);
}


std::string RandomGenerator::generate_key(const std::string key_prefix)
{
    std::string rand = this->generate_rnd();
    std::string key = key_prefix + rand;

    return key;
}


std::string RandomGenerator::generate_val(size_t value_size, const std::string value_prefix)
{
    unsigned long random_size = value_size - value_prefix.size();
    std::string value = value_prefix + std::string(random_size, 'a');

    return value;
}


KeyFileGenerator::KeyFileGenerator(std::string key_file, int num_keys, int seed)
{
    this->engine.seed(seed);
    spdlog::debug("Reading {}", key_file);
    std::ifstream fid(key_file, std::ios::in);
    if (!fid)
    {
        spdlog::warn("Error opening key file {}", key_file);
    }

    std::string k;
    int keys_loaded = 0;
    spdlog::debug("Loading in keys");
    while ((std::getline(fid, k)) && (keys_loaded < num_keys))
    {
        this->keys.push_back(k);
        keys_loaded++;
    }
 
    this->key_gen = this->keys.begin();
    fid.close();
}


KeyFileGenerator::KeyFileGenerator(std::string key_file, int start_idx, int num_keys, int seed)
{
    this->engine.seed(seed);
    spdlog::debug("Reading {}", key_file);
    std::ifstream fid(key_file, std::ios::in);
    if (!fid)
    {
        spdlog::warn("Error opening key file {}", key_file);
    }

    std::string k;
    int keys_loaded = 0;
    spdlog::debug("Loading in keys");
    while ((std::getline(fid, k)) && (keys_loaded < start_idx))
    {
        keys_loaded++;
    }

    keys_loaded = 0;
    while ((std::getline(fid, k)) && (keys_loaded < num_keys))
    {
        this->keys.push_back(k);
        keys_loaded++;
    }
 
    this->key_gen = this->keys.begin();
    fid.close();
}


std::string KeyFileGenerator::generate_key(const std::string key_prefix)
{
    std::string key = key_prefix + *this->key_gen;
    this->key_gen++;

    return key;
}


std::string KeyFileGenerator::generate_val(size_t value_size, const std::string value_prefix)
{
    unsigned long random_size = value_size - value_prefix.size();
    std::string value = value_prefix + std::string(random_size, 'a');

    return value;
} 