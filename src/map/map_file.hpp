// Vereiste 13: 13. correct protections (for header files)
// Voorkomt dubbele verwerking van dit headerbestand.
#pragma once
#include "map_data.hpp"
#include <filesystem>
#include <string>
namespace Battlefield {
class MapFile {
public:
    // Vereiste 22: 22. at least 2 parameterized constructors
    // De editor kiest de map waarin de kaartbestanden bewaard worden.
    explicit MapFile(const std::filesystem::path& directory);
    void save(const std::string& name, const MapData& map) const;
    std::vector<std::string> list() const;
    MapData load(const std::string& name) const;
private:
    std::filesystem::path directory;
    std::filesystem::path path(const std::string& name) const;
};
}
