#include <Base/PropertyMap.h>
#include <Base/Vector4.h>

PropertyMap::PropertyMap() {
}

PropertyMap::PropertyMap(const char *data) {
    std::list<std::string> lines;
    std::list<std::string>::iterator itr;
    std::string key, value;
    size_t key_end, value_begin;
    
    tokenize_string(std::string(data), "\n", lines);

    for(itr = lines.begin(); itr != lines.end(); itr++) {
        // Separate the key from the value(s)
        key_end     = (*itr).find_first_of(" ");
        value_begin = (*itr).find_first_not_of(" ", key_end);

        key = (*itr).substr(0, key_end);
        value = (*itr).substr(value_begin);
        
        // Add this pair to the map
        _mapData[key] = value;
    }
}

void PropertyMap::getKeys(std::list<std::string> &keys) {
    std::map<std::string,std::string>::iterator itr;
    for(itr = _mapData.begin(); itr != _mapData.end(); itr++) {
        keys.push_back(itr->first);
    }
}

template <>
void PropertyMap::getValue(const std::string &key, std::string &value) {
    value = _mapData[key];
}

template <>
void PropertyMap::getValue(const std::string &key, Vector4 &value) {
    std::string strValue;
    std::vector<std::string> arrValue;
    int i;
    
    strValue = _mapData[key];
    tokenize_string(strValue, " ", arrValue);
    ASSERT(arrValue.size() == 4);
    
    for(i = 0; i < 4; i++) {
        string_to_decimal(arrValue[i], value[i]);
    }
}