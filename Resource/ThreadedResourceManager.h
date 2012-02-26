#ifndef THREADEDRESOURCEMANAGER_H
#define THREADEDRESOURCEMANAGER_H

#include <Resource/ResourceManager.h>

#include <SDL/SDL_mutex.h>
#include <SDL/SDL_thread.h>

#define LOCK_MUTEX \
    do { \
        ASSERT(F::Lock); \
        SDL_mutexP(F::Lock); \
    } while(false)

#define UNLOCK_MUTEX \
    do { \
        SDL_mutexV(F::Lock); \
    } while(false)

struct ThreadInfo {
    SDL_Thread *thread;
    float progress;
    std::string status;
    
    ThreadInfo();
    ThreadInfo(SDL_Thread *nThread);
};

struct ResourceThreadParams {
	std::string name;
	void *ptr;
    SDL_mutex *mutex;

	ResourceThreadParams(const std::string &nName, void* nPtr, SDL_mutex *nMutex);
};

template <typename T, typename F>
class ThreadedResourceManager: public ResourceManager<T,F> {
public:
    static void Setup();
    static void Teardown();

    static T* Get(const std::string &name);
	static T* Load(const std::string &name);
    static T* Load(const std::string &name, T* t);
    static void Unload(T* t);
	static void Unload(const std::string &name);
	static void Reload(T* t);
	static void Reload(const std::string &name);

	static void ReloadAll();

    static bool Register(const std::string &name, T* t);
    static T* Unregister(const std::string &name);

    static float Progress(T* t);
    static std::string Status(T* t);
    static bool IsDone(T* t);
    static bool IsWorking();
    
protected:
    static int ThreadedLoad(void* data);

    static void UpdateProgress(T* t, float progress);
    static void UpdateStatus(T* t, const std::string &status);
    static void Finish(T* t);
    
    static ThreadInfo* GetThreadInfo(T* t);

private:
	static void Report();

protected:
    typedef std::map<T*,ThreadInfo> ThreadMap;
    typedef typename ThreadMap::iterator ThreadMapIterator;

	static SDL_mutex* Lock;
    static ThreadMap Threads;
};

template <typename T, typename F>
SDL_mutex* ThreadedResourceManager<T,F>::Lock;

template <typename T, typename F>
typename ThreadedResourceManager<T,F>::ThreadMap ThreadedResourceManager<T,F>::Threads;

template <typename T, typename F>
void ThreadedResourceManager<T,F>::Setup() {
    F::Lock = SDL_CreateMutex();
	ResourceManager<T,F>::Setup();
}

template <typename T, typename F>
void ThreadedResourceManager<T,F>::Teardown() {
    if(F::Lock) {
        SDL_mutexP(F::Lock);

        ThreadMapIterator itr = F::Threads.begin();
        for(; itr != F::Threads.end(); itr++) {
            SDL_KillThread(itr->second.thread);
        }
        F::Threads.clear();

		ResourceManager<T,F>::Teardown();

        SDL_mutexV(F::Lock);
        SDL_DestroyMutex(F::Lock);
        F::Lock = 0;
    }
}

template <typename T, typename F>
T* ThreadedResourceManager<T,F>::Get(const std::string &name) {
    T* t = 0;

	LOCK_MUTEX;
	t = ResourceManager<T,F>::Get(name);
    UNLOCK_MUTEX;

    return t;
}

template <typename T, typename F>
T* ThreadedResourceManager<T,F>::Load(const std::string &name) {
	T *t;

	LOCK_MUTEX;
	t = new T();
	ResourceThreadParams params(name, (void*)t, F::Lock);
    SDL_Thread *thread = SDL_CreateThread(F::ThreadedLoad, (void*)&params);
    F::Threads[t] = ThreadInfo(thread);
    F::Resources[name] = t;
    UNLOCK_MUTEX;

    return t;
}

template <typename T, typename F>
T* ThreadedResourceManager<T,F>::Load(const std::string &name, T* t = 0) {
	LOCK_MUTEX;
	if(!t) { t = new T(); }
	ResourceThreadParams params(name, (void*)t, F::Lock);
    SDL_Thread *thread = SDL_CreateThread(F::ThreadedLoad, (void*)&params);
    F::Threads[t] = ThreadInfo(thread);
    F::Resources[name] = t;
    UNLOCK_MUTEX;

    return t;
}

template <typename T, typename F>
void ThreadedResourceManager<T,F>::Unload(T* t) {
    LOCK_MUTEX;
	ResourceManager<T,F>::Unload(t);
	UNLOCK_MUTEX;
}

template <typename T, typename F>
void ThreadedResourceManager<T,F>::Unload(const std::string &name) {
    LOCK_MUTEX;
	ResourceManager<T,F>::Unload(name);
	UNLOCK_MUTEX;
}

template <typename T, typename F>
void ThreadedResourceManager<T,F>::Reload(T* t) {
    LOCK_MUTEX;
	ResourceManager<T,F>::Reload(t);
	UNLOCK_MUTEX;
}

template <typename T, typename F>
void ThreadedResourceManager<T,F>::Reload(const std::string &name) {
    LOCK_MUTEX;
	ResourceManager<T,F>::Reload(name);
	UNLOCK_MUTEX;
}

template <typename T, typename F>
void ThreadedResourceManager<T,F>::ReloadAll() {
    LOCK_MUTEX;
	ResourceManager<T,F>::ReloadAll();
	UNLOCK_MUTEX;
}

template <typename T, typename F>
bool ThreadedResourceManager<T,F>::Register(const std::string &name, T* t) {
	bool ret;
	LOCK_MUTEX;
	ret = ResourceManager<T,F>::Register(name, t);
	UNLOCK_MUTEX;
	return ret;
}

template <typename T, typename F>
T* ThreadedResourceManager<T,F>::Unregister(const std::string &name) {
    T* t = 0;

    LOCK_MUTEX;
	t = ResourceManager<T,F>::Unregister(name);
    UNLOCK_MUTEX;

    return t;
}

template <typename T, typename F>
void ThreadedResourceManager<T,F>::Finish(T *t) {
    LOCK_MUTEX;
    F::Threads.erase(t);
    UNLOCK_MUTEX;
}

template <typename T, typename F>
float ThreadedResourceManager<T,F>::Progress(T *t) {
    float progress;
    ThreadInfo* threadInfo;

    LOCK_MUTEX;
    if((threadInfo = F::GetThreadInfo(t))) {
        progress = threadInfo->progress;
    }
    UNLOCK_MUTEX;
    
    return progress;
}

template <typename T, typename F>
std::string ThreadedResourceManager<T,F>::Status(T *t) {
    std::string status;
    ThreadInfo* threadInfo; 

    LOCK_MUTEX;
    if((threadInfo = F::GetThreadInfo(t))) {
        status = threadInfo->status;
    }
    UNLOCK_MUTEX;
    
    return status;
}

template <typename T, typename F>
bool ThreadedResourceManager<T,F>::IsDone(T *t) {
    bool done;

    LOCK_MUTEX;
    done = !F::GetThreadInfo(t);
    UNLOCK_MUTEX;
    
    return done;
}

template <typename T, typename F>
bool ThreadedResourceManager<T,F>::IsWorking() {
    bool working;
    
    LOCK_MUTEX;
    working = !F::Threads.empty();
    UNLOCK_MUTEX;
    
    return working;
}

template <typename T, typename F>
int ThreadedResourceManager<T,F>::ThreadedLoad(void* data) {
    ResourceThreadParams *params = (ResourceThreadParams*)data;
	T* t = (T*)params->ptr;
	F::DoLoad(params->name, t);
    F::Finish(t);

    return 0;
}

template <typename T, typename F>
void ThreadedResourceManager<T,F>::UpdateProgress(T* t, float progress) {
    LOCK_MUTEX;
    F::Threads[t].progress = progress;
    UNLOCK_MUTEX;
}

template <typename T, typename F>
void ThreadedResourceManager<T,F>::UpdateStatus(T* t, const std::string &status) {
    LOCK_MUTEX;
    F::Threads[t].status = status;
    UNLOCK_MUTEX;
}

// WARNING: This function is not threadsafe, and is designed to be called
//  from within functions that make use of mutex locking
template <typename T, typename F>
ThreadInfo* ThreadedResourceManager<T,F>::GetThreadInfo(T *t) {
    ThreadMapIterator itr = F::Threads.find(t);
    if(itr != F::Threads.end()) {
        return &(itr->second);
    } else {
        return 0;
    }
}

template <typename T, typename F>
void ThreadedResourceManager<T,F>::Report() {
	Info("ThreadedResourceManager Threads contains:");
	ThreadMapIterator itr = Threads.begin();
	for(; itr != Threads.end(); itr++) {
		Info(" - " << itr->first);
	}
	Info("F::Threads contains:");
	itr = F::Threads.begin();
	for(; itr != F::Threads.end(); itr++) {
		Info(" - " << itr->first);
	}
}

#undef LOCK_MUTEX
#undef UNLOCK_MUTEX

#endif
