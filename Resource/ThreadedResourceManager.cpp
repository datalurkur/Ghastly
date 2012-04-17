#include <Resource/ThreadedResourceManager.h>

ThreadInfo::ThreadInfo(): thread(0), progress(0.0), status("Default"), threadShouldDie(false) {}
ThreadInfo::ThreadInfo(SDL_Thread *nThread): thread(nThread), progress(0.0), status("Default"), threadShouldDie(false) {}

ResourceThreadParams::ResourceThreadParams(const std::string &nName, void* nPtr, SDL_mutex *nMutex): name(nName), ptr(nPtr), mutex(nMutex) {}
