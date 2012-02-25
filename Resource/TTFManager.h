#ifndef TTFMANAGER_H
#define TTFMANAGER_H

#include <Render/Font.h>
#include <Resource/ResourceManager.h>

class TTFManager;

class TTFManager: public ResourceManager<Font, TTFManager> {
protected:
    static void DoLoad(const std::string &name, Font *font);
    static const std::string LoadDirectory;
	friend class ResourceManager<Font, TTFManager>;
};

#endif
