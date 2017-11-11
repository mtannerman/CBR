#include "common/config.h"
#include "common/exceptions.h"
#include "rapidjson/document.h"
#include <fstream>
#include <sstream>
#include <map>

#define PARSE_BOOL_LINE(entry, propertyName, configVariable)\
    if (entry.HasMember(propertyName)) {\
        configVariable = entry[propertyName].GetBool();\
    }\

namespace cbr
{

struct Config::Impl
{
    void ParseFile(const std::string& path);
    void ReadTests(const rapidjson::Document& doc);
    void ReadVisualizationOptions(const rapidjson::Document& doc);

    std::map<std::string, bool> mBools;
};


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


void Config::Impl::ReadTests(const rapidjson::Document& doc)
{
    LOG("reading tests:");
    if (doc.HasMember("tests")) {
        const auto& testArray = doc["tests"];
        PARSE_BOOL_LINE(testArray, "squareOverlap", mBools["testImgProcSquareOverlap"]);
        PARSE_BOOL_LINE(testArray, "simplexOptimizer", mBools["testSimplexOptimizer"]);
    }
}

void Config::Impl::ReadVisualizationOptions(const rapidjson::Document& doc)
{
    LOG("reading visualization options:");
    if (doc.HasMember("visualize")) {
        const auto& visualizationArray = doc["visualize"];
        PARSE_BOOL_LINE(visualizationArray, "squareFiltering", mBools["visualizeSquareFiltering"]);
        PARSE_BOOL_LINE(visualizationArray, "dominantEdgeDirections", mBools["visualizeDominantEdgeDirections"]);

    }
}

void Config::ParseFile(const std::string& path)
{
    mImpl->ParseFile(path);
}

void Config::Impl::ParseFile(const std::string& path)
{
    LOG(DESC(path));
    ASSERT(path != "", "configPath must be set.");
    const auto configFileContent = read_json_document(path);
    rapidjson::Document doc;
    LOG("Parsing config file.");
    doc.Parse(configFileContent.c_str());
    ASSERT(doc.IsObject(), "invalid config file");
    ReadTests(doc);
    ReadVisualizationOptions(doc);
    LOG("Parsing finished.");
}

Config::Config()
    : mImpl(new Impl())
{

}

Config& Config::GetInstance()
{
    static Config config;
    return config;
}

bool Config::GetBool(const std::string& variableName)
{
    const auto foundVariable = mImpl->mBools.find(variableName);
    ASSERT(foundVariable != mImpl->mBools.end(), "Can't find specified variable.");
    return foundVariable->second;
}

Config::~Config()
{

}


}