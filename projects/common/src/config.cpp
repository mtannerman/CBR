#include "common/config.h"
#include "common/exceptions.h"
#include "rapidjson/document.h"
#include <fstream>
#include <sstream>

#define PARSE_BOOL_LINE(entry, propertyName, configVariable)\
    if (entry.HasMember(propertyName)) {\
        configVariable = entry[propertyName].GetBool();\
    }\

namespace cbr
{


bool is_line_commented(const std::string& line)
{
    return line.find("//") != std::string::npos;
}

std::string read_json_document(const std::string& fileName)
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
    LOG("reading tests:");
    if (doc.HasMember("tests")) {
        const auto& testArray = doc["tests"];
        for (const auto& test : testArray.GetArray()) {
            PARSE_BOOL_LINE(test, "squareOverlap", config.testImgProcSquareOverlap);
        }
    }
}

void read_visualization_options(Config& config, const rapidjson::Document& doc)
{
    LOG("reading visualization options:");
    if (doc.HasMember("visualize")) {
        const auto& visualizationArray = doc["visualize"].GetArray();
        for (const auto& v : visualizationArray) {
            PARSE_BOOL_LINE(v, "squareFiltering", config.visualizeSquareFiltering);
            PARSE_BOOL_LINE(v, "dominantEdgeDirections", config.visualizeDominantEdgeDirections);
        }
    }
}

void Config::ParseFile(const std::string& path)
{
    LOG(DESC(path));
    ASSERT(path != "", "configPath must be set.");
    const auto configFileContent = read_json_document(path);
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