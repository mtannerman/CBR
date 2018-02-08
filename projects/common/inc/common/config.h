#pragma once
#include <string>

#include "common/pimpl.h"
#include <vector>

namespace cbr
{


bool is_line_commented(const std::string& line);
std::string read_json_document(const std::string& fileName);

class Config
{
public:
    static Config& GetInstance();
    void ParseFile(const std::string& path);
	bool GetBool(const std::string& variableName);
	std::string GetString(const std::string& variableName);
    std::vector<std::string> GetStringList(const std::string& variableName);
    
    ~Config();

private:
    Config();
    struct Impl;
    impl_ptr<Impl> mImpl;
};

}