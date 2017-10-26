#include "common/config.h"
#include "common/exceptions.h"
#include "rapidjson/document.h"
#include <fstream>
#include <sstream>

namespace cbr
{

bool is_line_commented(const std::string& line)
{
    return line.find("//") != std::string::npos;
}

std::string read_config_file(const std::string& fileName)
{
    std::ifstream s(fileName);
    ASSERT(s.is_open(), "Cannot open config file.");
    std::string line;
    std::string ret;
    while (std::getline(s, line)) {
        if (!is_line_commented(line)) {
            ret += line + "\n";
        }
    }
    
    s.close();

    return ret;
}


void read_tests(Config& config, const rapidjson::Document& doc)
{
    int nTests = 0;
    LOG("reading tests:");
    if (doc.HasMember("tests")) {
        const auto& testArray = doc["tests"];
        for (const auto& test : testArray.GetArray()) {
            const auto testName = std::string(test.GetString());
            LOG("\t" << testName);
            if (testName == "imgProcSquareOverlapTest") {
                config.testImgProcSquareOverlap = true;
                ++nTests;
            }
            else {
                LOG("Unknown test: " << testName);
            }
        }
    }

    if (nTests == 0) {
        LOG("No tests given.");
    }
}

void read_visualization_options(Config& config, const rapidjson::Document& doc)
{
    int nVisOptions = 0;
    LOG("reading visualization options:");
    if (doc.HasMember("visualize")) {
        for (const auto& v : doc["visualize"].GetArray()) {
            const auto name = std::string(v.GetString());
            LOG("\t" << name);
            if (name == "squareOverlap") {
                config.visualizeSquareFiltering = true;
                ++nVisOptions;
            }
            else {
                LOG("Unknown option: " << name);
            }
        }
    }
    if (nVisOptions == 0) {
        LOG("No visualization options given.");
    }
}

void Config::ParseFile(const std::string& path)
{
    LOG(DESC(path));
    ASSERT(path != "", "configPath must be set.");
    const auto configFileContent = read_config_file(path);
    rapidjson::Document doc;
    LOG("Parsing config file.");
    doc.Parse(configFileContent.c_str());
    ASSERT(doc.IsObject(), "invalid config file");
    read_tests(*this, doc);
    read_visualization_options(*this, doc);
    LOG("Parsing finished.");
}


Config& Config::GetInstance()
{
    static Config config;
    return config;
}

}