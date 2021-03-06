
#ifndef DATASTRUCTURES_ECS_TYPEDEFS_H
#define DATASTRUCTURES_ECS_TYPEDEFS_H

#include <cstdint> // std::uint32_t

namespace ECS {
    typedef unsigned ComponentTypeID; // Component type gets hashed into an unsigned value.
    typedef std::uint32_t EntityID;   // EntityID gets created by STRINGHASHing the entity name.
}

#endif //DATASTRUCTURES_ECS_TYPEDEFS_H
