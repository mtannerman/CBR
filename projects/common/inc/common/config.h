#pragma once
#include <string>
#include "common/logging.h"
#include <map>

namespace cbr
{

class Config
{
public:
    static Config& GetInstance();
    void ParseFile(const std::string& path);

    // tests
    bool testImgProcSquareOverlap = false;

    // visualization
    bool visualizeSquareFiltering = false;
    bool visualizeDominantEdgeDirections = false;
private:
    Config() = default;
};

}