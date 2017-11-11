#include "common/playground/json_playground.h"
#include "common/config.h"
#include "rapidjson/document.h"
#include "common/logging.h"

namespace cbr {
namespace playground {
void StartJsonPlayground()
{
    const std::string fileName = "../input/JsonPlayground/1.json";
    const auto fileContent = read_json_document(fileName);
    rapidjson::Document doc;
    doc.Parse(fileContent.c_str());
    if (doc.HasMember("collection1")) {
        const auto& coll1Array = doc["collection1"];
        for (const auto& entry : coll1Array.GetArray()) {
            LOG(entry.GetInt());
        }
    }
    if (doc.HasMember("collection2")) {
        const auto& coll2 = doc["collection2"];
        if (coll2.HasMember("option1")) {
            LOG(DESC(coll2["option1"].GetBool()));
        }

        if (coll2.HasMember("option2")) {
            LOG(DESC(coll2["option2"].GetBool()));
        }
        // for (const auto& entry : coll2Array.GetArray()) {
        //     if (entry.HasMember("option1")) {
        //         LOG(DESC(entry["option1"].GetBool()));
        //     }
        //     if (entry.HasMember("option2")) {
        //         LOG(DESC(entry["option2"].GetBool()));
        //     }
        // }
    } 
}
}
}