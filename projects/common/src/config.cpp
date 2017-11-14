#include "common/config.h"
#include "common/exceptions.h"
#include "rapidjson/document.h"
#include "common/file_operation.h"
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
    void ParseFile(std::string path);
    void ReadTests(const rapidjson::Document& doc);
    void ReadVisualizationOptions(const rapidjson::Document& doc);
    void ReadImages(const rapidjson::Document& doc);

    std::map<std::string, bool> mBools;
    std::map<std::string, std::vector<std::string>> mStringLists;
};


bool is_line_commented(const std::string& line)
{
    return line.find("//") != std::string::npos;
}

std::string read_json_document(const std::string& fileName)
{
    std::ifstream s(fileName);
    ASSERT(s.is_open(), STR("Cannot open config file: " << fileName));
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

void Config::Impl::ReadImages(const rapidjson::Document& doc)
{
    LOG("reading images:");
    if (doc.HasMember("images")) {
        const auto& imageArray = doc["images"].GetArray();
        for (const auto& entry : imageArray) {
            mStringLists["images"].push_back(GetInputDirectory() + "/" + entry.GetString());
        }
    }
}

void Config::ParseFile(const std::string& path)
{
    mImpl->ParseFile(path);
}

void Config::Impl::ParseFile(std::string path)
{
    ASSERT(path != "", "configPath must be set.");
    if (!IsDirectoryOrFileExist(path)) {
        path = GetWorkingDirectory() + "/" + path;
    }
    const auto configFileContent = read_json_document(path);
    rapidjson::Document doc;
    LOG("Parsing config file.");
    doc.Parse(configFileContent.c_str());
    ASSERT(doc.IsObject(), "invalid config file");
    ReadTests(doc);
    ReadVisualizationOptions(doc);
    ReadImages(doc);
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

template<typename T>
const T& GetVariable(const std::map<std::string, T>& container, const std::string& variableName)
{
    const auto foundVariable = container.find(variableName);
    ASSERT(foundVariable != container.end(), "Can't find variable " + variableName);
    return foundVariable->second;
} 

bool Config::GetBool(const std::string& variableName)
{
    return GetVariable<bool>(mImpl->mBools, variableName);
}

std::vector<std::string> Config::GetStringList(const std::string& variableName)
{
    return GetVariable<std::vector<std::string>>(mImpl->mStringLists, variableName);
}

Config::~Config()
{

}


}