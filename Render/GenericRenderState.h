#ifndef GENERICRENDERSTATE_H
#define GENERICRENDERSTATE_H

class GenericRenderState {
public:
    GenericRenderState();
    virtual ~GenericRenderState();

    virtual void preRender() = 0;
    virtual void postRender() = 0;
};

#endif