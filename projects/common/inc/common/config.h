#pragma once
#include <string>

#define CBR_INIT_CONFIG(configPath_) ::cbr::SetConfigPath(configPath_); ::cbr::Config::GetInstance();

namespace cbr
{

std::string configPath = "";
void SetConfigPath(const std::string& path) { configPath = path; }

class Config
{
public:
    static const Config& GetInstance();

    // tests
    bool testImgProcSquareOverlap = false;

    // visualization
    bool visualizeSquareFiltering = false;
private:
    Config();
};

}