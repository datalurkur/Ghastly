#include <Base/PropertyMap.h>
#include <Base/Vector4.h>
#include <Base/Base.h>

PropertyMap::PropertyMap() {
}

PropertyMap::PropertyMap(const char *data) {
    std::list<std::string> lines;
    std::list<std::string>::iterator itr;
    std::string key, value;
    
    tokenize_string(std::string(data), "\n", lines);

    for(itr = lines.begin(); itr != lines.end(); itr++) {
        SeparateTypedValue(*itr, key, value);
        _mapData[key] = value;
    }
}

void PropertyMap::getKeys(std::list<std::string> &keys) const {
    std::map<std::string,std::string>::const_iterator itr;
    for(itr = _mapData.begin(); itr != _mapData.end(); itr++) {
        keys.push_back(itr->first);
    }
}

bool PropertyMap::hasKey(const std::string &key) const {
    std::map<std::string,std::string>::const_iterator itr;
    itr = _mapData.find(key);
    return(itr != _mapData.end());
}

const std::string &PropertyMap::getValue(const std::string &key) const {
    std::map<std::string,std::string>::const_iterator itr;
    itr = _mapData.find(key);
    ASSERT(itr != _mapData.end());
    return itr->second;
}

void PropertyMap::SeparateTypedValue(const std::string &source, std::string &type, std::string &value) {    
    size_t type_end, value_begin;
    
    // Separate the type from the value
    type_end = source.find_first_of(" ");
    if(type_end == std::string::npos) {
        type  = "";
        value = "";
        return;
    }
    value_begin  = source.find_first_not_of(" ", type_end);
    
    type  = source.substr(0, type_end);
    value = source.substr(value_begin);
}

bool PropertyMap::ExtractScope(const std::string &source, std::string &parent, std::string &child) {
    size_t scope_end;

    scope_end = source.find_first_of(".");
    if(scope_end == std::string::npos) {
        return false;
    } else {
        parent = source.substr(0, scope_end);
        child = source.substr(scope_end + 1);
        return true;
    }
}

template <>
void PropertyMap::ExtractValue(const std::string &source, std::string &value) {
    value = source;
}

template <>
void PropertyMap::ExtractValue(const std::string &source, int &value) {
    string_to_decimal(source, value);
}

template <>
void PropertyMap::ExtractValue(const std::string &source, Vector4 &value) {
    std::vector<std::string> arrValue;
    int i;
    
    tokenize_string(source, " ", arrValue);
    ASSERT(arrValue.size() == 4);
    
    for(i = 0; i < 4; i++) {
        string_to_decimal(arrValue[i], value[i]);
    }
}