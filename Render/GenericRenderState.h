#ifndef GENERICRENDERSTATE_H
#define GENERICRENDERSTATE_H

class GenericRenderState {
public:
    virtual void preRender() = 0;
    virtual void postRender() = 0;
};

#endif
