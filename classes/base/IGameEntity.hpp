#ifndef IGAMEENTITY_HPP_HNSS
#define IGAMEENTITY_HPP_HNSS

class IGameEntity {
public:
    IGameEntity() = default;
    virtual void update() = 0;
    virtual void render() = 0;
    virtual ~IGameEntity() = default;
};

#endif