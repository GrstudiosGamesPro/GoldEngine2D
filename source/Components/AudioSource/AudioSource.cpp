#include "AudioSource.h"
#include "nlohmann/json.hpp"
#include "../SaveSystem/CheckVar.h"

using namespace nlohmann;
using namespace std;

std::string AudioSource::serialize () {
    json componentData;
    componentData["pan"] = pan;
    componentData["volumen"] = Volumen;
    componentData["pathfile"] = AudioPath;
    componentData["minv"] = minDistance;
    componentData["maxv"] = maxDistance;

    return componentData.dump();
}

void AudioSource::deserialize(std::string g, std::string path) {
    json componentData = json::parse(g);
    if (CheckVar::Has (componentData, "pan"))
    SetPan  (componentData["pan"]);

    if (CheckVar::Has(componentData, "volumen"))
    Volumen = componentData["volumen"];

    if (CheckVar::Has(componentData, "minv"))
        minDistance = componentData["minv"];

    if (CheckVar::Has(componentData, "maxv"))
        maxDistance = componentData["maxv"];

    if (CheckVar::Has(componentData, "pathfile"))
    AudioPath = (string)componentData["pathfile"];

    Reset();
}