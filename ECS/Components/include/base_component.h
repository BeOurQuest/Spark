
#ifndef DATASTRUCTURES_BASE_COMPONENT_H
#define DATASTRUCTURES_BASE_COMPONENT_H

#include "../../../UtilityBox/Tools/compile_time_hash.h" // STRINGHASH

namespace ECS::Components {
    struct BaseComponent {
        static constexpr unsigned ID = STRINGHASH("BaseComponent");
        static constexpr const char* Name = "Base";

        virtual ~BaseComponent() = default;
        bool _enabled = true;
    };
}


#endif //DATASTRUCTURES_BASE_COMPONENT_H
