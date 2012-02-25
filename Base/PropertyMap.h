#ifndef PROPERTYMAP_H
#define PROPERTYMAP_H

#include <Base/Base.h>

class PropertyMap {
public:
    PropertyMap();
    PropertyMap(const char *data);

    void getKeys(std::list<std::string> &keys);
    
    template <typename T>
    void getValue(const std::string &key, T &value);

private:
    std::map<std::string,std::string> _mapData;
};

#endif