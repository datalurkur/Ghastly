#ifndef SCENENODE_H
#define SCENENODE_H

#include <Base/Vector2.h>
#include <Base/AABB3.h>
#include <Engine/Frustum.h>
#include <Render/Renderable.h>

class SceneManager;

template <typename T>
class SceneNode {
public:
    typedef std::map<std::string, SceneNode<T>*> NodeMap;
    typedef std::list<SceneNode<T>*> NodeList;

public:
    static const std::string NodeType;

    enum DirtyPropagation { Upward, Downward };

public:
    SceneNode(const std::string &name);
    virtual ~SceneNode();

    // Positions
    Vector3<T> getAbsolutePosition() const;
    Vector3<T> getLocalPosition() const;

    void setPosition(const Vector3<T> &pos);
    void moveRelative(const Vector3<T> &pos);
    
    // Dimensions
    Vector3<T> getDimensions() const;
    void setDimensions(const Vector3<T> &dim);

    // AABB Information
    const AABB3<T>& getAbsoluteBounds() const;

    // Identifying information
    const std::string &getName() const;
    const std::string &getType() const;

    // Scene heirarchy
    void addChild(SceneNode *child);
    void deleteChild(const std::string &childName);

    // Adds this scene node and its children to the list
    // If frustum is non-null, frustum culling will be performed
    void getNodes(NodeList &list, Frustum *frustum = 0);

    // Adds this scene node and its children to the list
    // Cull nodes based on bounding box intersection
    void getNodes(NodeList &list, const AABB3<T> &bounds);

    // Adds this scene node and its children to the list
    // Culls nodes based on the filters supplied
    void getDifference(NodeList &listA, NodeList &listB, const AABB3<T> &boundsA, const AABB3<T> &boundsB);

    // Adds the renderables to the provided list
    virtual void getRenderables(RenderableList &list);

    // Adds a renderable to the scenenode's internal renderable list
    void addRenderable(Renderable *renderable);

    // Clear the renderable list and delete the renderables if flag is set
    void clearRenderables(bool deleteOnClear = true);

    // Regenerate the renderables automatically (on a resize, for example)
    virtual void recreateRenderables() {}

protected:
    SceneNode(const std::string &name, const std::string &type);

    // Updates any values that are dependent on local state or parent state
    void updateCachedValues();

    // Indicate that cached values should be updated before used
    void flagDirty(DirtyPropagation direction);

protected:
    std::string _name;
    std::string _type;

    Vector3<T> _position;
    Vector3<T> _absolutePosition;
    
    Vector3<T> _dimensions;
    AABB3<T> _absoluteBounds;

    Matrix4 _affine;
    Matrix4 _absoluteAffine;

    SceneNode *_parent;
    NodeMap _children;

    bool _dirty;

    RenderableList _renderables;

    friend class SceneManager;
    friend class UIManager;
};

template <typename T>
const std::string SceneNode<T>::NodeType = "SceneNode";

template <typename T>
SceneNode<T>::SceneNode(const std::string &name):
    _name(name), _type(NodeType), _parent(0), _dirty(true)
{}

template <typename T>
SceneNode<T>::SceneNode(const std::string &name, const std::string &type):
    _name(name), _type(type), _parent(0), _dirty(true)
{}

template <typename T>
SceneNode<T>::~SceneNode() {
    clearRenderables(true);
    typename NodeMap::iterator itr = _children.begin();
    for(; itr != _children.end(); itr++) {
        delete (itr->second);
    }
    _children.clear();
}

template <typename T>
Vector3<T> SceneNode<T>::getAbsolutePosition() const {
    ASSERT(!_dirty);

    return _absolutePosition;

}

template <typename T>
Vector3<T> SceneNode<T>::getLocalPosition() const {
    ASSERT(!_dirty);

    return _position;
}

template <typename T>
void SceneNode<T>::setPosition(const Vector3<T> &pos) {
    _position = pos;

    flagDirty(Downward);

    // This will change the position of the AABB, so the parent may need to adjust the size of its AABB
    flagDirty(Upward);
}

template <typename T>
Vector3<T> SceneNode<T>::getDimensions() const {
    return _dimensions;
}

template <typename T>
void SceneNode<T>::setDimensions(const Vector3<T> &dim) {
    _dimensions = dim;

    // Parents will need to update their AABBs
    flagDirty(Upward);
    recreateRenderables();
}

template <typename T>
const AABB3<T>& SceneNode<T>::getAbsoluteBounds() const {
    ASSERT(!_dirty);
    return _absoluteBounds;
}

template <typename T>
void SceneNode<T>::moveRelative(const Vector3<T> &pos) {
    setPosition(_position + pos);
}

template <typename T>
const std::string &SceneNode<T>::getName() const { return _name; }

template <typename T>
const std::string &SceneNode<T>::getType() const { return NodeType; }

template <typename T>
void SceneNode<T>::addChild(SceneNode<T> *child) {
    _children[child->getName()] = child;
    child->_parent = this;

    // Bounding boxes need to be recomputed
    flagDirty(Upward);
}

template <typename T>
void SceneNode<T>::deleteChild(const std::string &childName) {
    typename NodeMap::iterator itr;
    itr = _children.find(childName);
    if(itr != _children.end()) {
        delete itr->second;
        _children.erase(itr);
    }
}

template <typename T>
void SceneNode<T>::getNodes(NodeList &list, Frustum *frustum) {
    ASSERT(!_dirty);

    if(frustum) {
        // Do bounds checking and return early if this node is not within the view frustum
        // FIXME - Actually implement frustum culling
    }
    list.push_back(this);
    typename NodeMap::iterator itr = _children.begin();
    for(; itr != _children.end(); itr++) {
        itr->second->getNodes(list, frustum);
    }
}

template <typename T>
void SceneNode<T>::getNodes(NodeList &list, const AABB3<T> &bounds) {
    ASSERT(!_dirty);

    if(_absoluteBounds.overlaps(bounds)) {
        list.push_back(this);
        typename NodeMap::iterator itr = _children.begin();
        for(; itr != _children.end(); itr++) {
            itr->second->getNodes(list, bounds);
        }
    }
}

template <typename T>
void SceneNode<T>::getDifference(NodeList &listA, NodeList &listB, const AABB3<T> &boundsA, const AABB3<T> &boundsB) {
    ASSERT(!_dirty);

    bool groupA, groupB;

    groupA = _absoluteBounds.overlaps(boundsA);
    groupB = _absoluteBounds.overlaps(boundsB);

    if(groupA && !groupB) {
        listA.push_back(this);
    } else if(!groupA && groupB) {
        listB.push_back(this);
    }

    if(groupA || groupB) {
        typename NodeMap::iterator itr = _children.begin();
        for(; itr != _children.end(); itr++) {
            itr->second->getDifference(listA, listB, boundsA, boundsB);
        }
    }
}

template <typename T>
void SceneNode<T>::getRenderables(RenderableList &list) {
    ASSERT(!_dirty);

    //Info("SceneNode " << _name << " adding " << _renderables.size() << " renderables to list.");
    list.insert(list.end(), _renderables.begin(), _renderables.end());
}

template <typename T>
void SceneNode<T>::addRenderable(Renderable *renderable) {
    _renderables.push_back(renderable);
}

template <typename T>
void SceneNode<T>::clearRenderables(bool deleteOnClear) {
    if(deleteOnClear) {
        RenderableList::iterator itr = _renderables.begin();
        for(; itr != _renderables.end(); itr++) {
            delete *itr;
        }
    }
    _renderables.clear();
}

template <typename T>
void SceneNode<T>::updateCachedValues() {
    bool needsUpdate = _dirty;

    if(needsUpdate) {
        _dirty = false;

        // Update any values dependent on the parent state
        if(_parent) {
            _absolutePosition = _position + _parent->getAbsolutePosition();
        } else {
            _absolutePosition = _position;
        }

        // Update any values dependent on other local values
        // FIXME - Eventually, these need to be full affine transformations, to deal with rotation
        _affine = Matrix4::MakeTranslation((float)_position.x, (float)_position.y, (float)_position.z);
        _absoluteAffine = Matrix4::MakeTranslation((float)_absolutePosition.x, (float)_absolutePosition.y, 0.0f);

        // Update renderable view matrices
        RenderableList::iterator rItr = _renderables.begin();
        for(; rItr != _renderables.end(); rItr++) {
            (*rItr)->setViewMatrix(_absoluteAffine);
        }
    }

    typename NodeMap::iterator itr = _children.begin();
    for(; itr != _children.end(); itr++) {
        itr->second->updateCachedValues();
    }
    
    if(needsUpdate) {
        // Update any values dependend on child states

        // Update the absolute AABB
        _absoluteBounds = AABB3<T>(
            _absolutePosition - (_dimensions / 2),
            _absolutePosition + (_dimensions / 2)
        );

        // Expand the AABB with the children's bounds
        typename NodeMap::iterator itr = _children.begin();
        for(; itr != _children.end(); itr++) {
            _absoluteBounds.expand(itr->second->getAbsoluteBounds());
        }
    }
}

template <typename T>
void SceneNode<T>::flagDirty(DirtyPropagation direction) {
    _dirty = true;
    if(direction == Upward) {
        if(_parent) { _parent->flagDirty(direction); }
    } else {
        typename NodeMap::iterator itr = _children.begin();
        for(; itr != _children.end(); itr++) {
            itr->second->flagDirty(direction);
        }
    }
}

#endif
