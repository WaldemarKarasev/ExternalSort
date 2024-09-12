#pragma once 

// std
#include <filesystem>
#include <fstream>
#include <iostream>

// json
#include <nlohmann/json.hpp>



namespace settings {

extern nlohmann::json global;

const std::filesystem::path settings_path = "settings.json";


inline nlohmann::json ParseJsonFromFile(std::filesystem::path path)
{
    nlohmann::json read_json;
    try {
        std::cout << "AAA" << std::endl;
        std::ifstream file(path);
        read_json = nlohmann::json::parse(file);
        
    } catch (const nlohmann::detail::parse_error &err) {
        std::cout << "BBB" << std::endl;
        return nlohmann::json::object();
    }
    return read_json;
}

inline nlohmann::json ReadSettings()
{
    nlohmann::json settings_json = ParseJsonFromFile(settings_path);

    if (settings_json.empty())
    {
        std::cerr << "settings_json is empty" << std::endl;
        settings_json["read_latency_ms"] = 0;
        settings_json["write_latency_ms"] = 0;
        settings_json["move_latency_ms"] = 0;
        settings_json["mem_usage_percentages"] = 50; // 50% default
    }

    return settings_json;
}

} // namespace settings