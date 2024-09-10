#pragma once 

// std
#include <filesystem>
#include <fstream>
#include <iostream>

// json
#include <nlohmann/json.hpp>



namespace settings {

extern nlohmann::json global_settings;

const std::filesystem::path settings_path = "settings.conf";

inline nlohmann::json ReadSettings()
{
    nlohmann::json settings_json = ParseJsonFromFile(settings_path);

    if (settings_json.empty())
    {
        std::cout << "settings_json is empty" << std::endl;
        settings_json["read_latency_ms"] = 0;
        settings_json["write_latency_ms"] = 0;
        settings_json["move_latency_ms"] = 0;
        settings_json["mem_usage_percentages"] = 50; // 50% default
    }

    return settings_json;
}

inline nlohmann::json ParseJsonFromFile(std::filesystem::path path)
{
    nlohmann::json read_json;
    try {
        std::ifstream file(path);
        read_json = nlohmann::json::parse(file);
        
    } catch (const nlohmann::detail::parse_error &err) {
        return nlohmann::json::object();
    }
    return read_json;
}

} // namespace settings