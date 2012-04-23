#ifndef PROPERTYMAP_H
#define PROPERTYMAP_H

#include <Base/Base.h>

class PropertyMap {
public:
    PropertyMap();
    PropertyMap(const char *data);

    void getKeys(std::list<std::string> &keys) const;
    bool hasKey(const std::string &key) const;
    
    const std::string &getValue(const std::string &key) const;
    
    template <typename T>
    void getValue(const std::string &key, T &value);

public:
    static void SeparateTypedValue(const std::string &source, std::string &type, std::string &value);
    static bool ExtractScope(const std::string &source, std::string &parent, std::string &child);
    
    template <typename T>
    static void ExtractValue(const std::string &source, T &value);

private:
    std::map<std::string,std::string> _mapData;
};

template <typename T>
void PropertyMap::getValue(const std::string &key, T &value) {
    ExtractValue(_mapData[key], value);
}

#endif