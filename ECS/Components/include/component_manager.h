
#ifndef DATASTRUCTURES_COMPONENT_MANAGER_H
#define DATASTRUCTURES_COMPONENT_MANAGER_H

#include "component_manager_interface.h" // ComponentManagerInterface

namespace ECS::Components {
    template <class ComponentType>
    class ComponentManager : public ComponentManagerInterface {
        public:
            /**
             * Ensures desired component type for the ComponentManager is valid before any initialization happens.
             * Creates the back-end functionality and allocates space for components.
             */
            ComponentManager();

            /**
             * Safely cleans up back-end functionality and allocated component memory space.
             */
            ~ComponentManager() override;

            /**
             * Retrieves and default-constructs a component to return to the user.
             * @return Pointer to new default-constructed component.
             */
            _NODISCARD_ ComponentType* CreateComponent();

            /**
             * Return the memory associated with a component back to the ComponentManager to be reused. Automatically
             * calls component destructor before returning memory back.
             * @param component - Pointer to component to return.
             */
            void DeleteComponent(ComponentType* component);

        private:
            // Storage for ComponentManager data, back-end functionality, and helper functions.
            class ComponentManagerData;
            ComponentManagerData* _data;
    };
}

#include "../src/component_manager.tpp" // Template function includes.

#endif //DATASTRUCTURES_COMPONENT_MANAGER_H

