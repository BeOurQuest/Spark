
#include "entity_manager_data.h" // EntityManagerData

namespace ECS::Entities {
    // Constructor.
    EntityManagerData::EntityManagerData() {
        UtilityBox::Logger::LogMessage message {};
        message.Supply("Constructing EntityManager.");
        _loggingSystem.Log(message);
    }

    // Cleans up any resources associated with the Entity Manager helper class.
    EntityManagerData::~EntityManagerData() {
        UtilityBox::Logger::LogMessage message {};
        message.Supply("Destroying EntityManager.");
        _loggingSystem.Log(message);

        // Clear entity components.
        for (auto entityComponentList : _entityComponents) {
            entityComponentList.second.clear();
        }
        _entityComponents.clear();

        // Clear entity names.
        _entityNames.clear();

        // Clear callback functions.
        _entityCreateCallbackFunctions.clear();
        _entityDestroyCallbackFunctions.clear();
        _componentAddCallbackFunctions.clear();
        _componentRemoveCallbackFunctions.clear();
    }

    // Create an entity. Throws error if the provided entity name matches any of the the build-in component type names
    // or any pre-existing entity names. Automatically notifies all fully registered component systems that a new entity has been created.
    void EntityManagerData::CreateEntity(std::string name) {
        UtilityBox::Logger::LogMessage message {};
        message.Supply("Entering function CreateEntity with entity name: '%s.'", name.c_str());

        ConvertToLowercase(name);
        message.Supply("Entity name converted to lowercase: '%s.'", name.c_str());

        // Entity name cannot be a name of any component.
        message.Supply("Checking name against the names of all build-in component names.");

        if (CheckEntityName<ALL_COMPONENTS>(name)) {
            message.SetMessageSeverity(UtilityBox::Logger::LogMessageSeverity::SEVERE);
            message.Supply("Exception thrown: Provided entity name: '%s' cannot match a built-in component name.", name.c_str());
            _loggingSystem.Log(message);

            throw std::invalid_argument("In function CreateEntity: Provided entity name cannot match a built-in component name.");
        }

        EntityID hashedID = STRINGHASH(name.c_str());
        message.Supply("Checking hashed entity ID against all other entities.");

        // Newly created entity cannot have a matching ID to an already existing entity.
        if (_entityComponents.find(hashedID) != _entityComponents.end()) {
            message.SetMessageSeverity(UtilityBox::Logger::LogMessageSeverity::SEVERE);
            message.Supply("Exception thrown: Hashed entity ID matches with another entity: there already exists an entity with the same hashed ID under entity name: '%s'.", _entityNames.find(hashedID)->second.c_str());
            _loggingSystem.Log(message);

            throw std::invalid_argument("In function CreateEntity: Provided entity name matches an already existing entity name.");
        }

        message.Supply("Entity creation successful. Creating entries for entity component list and name.");
        // Index operator creates a new default constructed entry.
        _entityComponents[hashedID];
        _entityNames[hashedID] = name;

        // Notify systems of created entity.
        message.Supply("Component system 'OnEntityCreate' callback functions called.");
        _loggingSystem.Log(message);

        for (auto &entityCreateCallbackFunction : _entityCreateCallbackFunctions) {
            entityCreateCallbackFunction(hashedID);
        }
    }

    // Destroy an entity from the Entity Manager with the provided entity ID, given that it exists. Automatically notifies
    // all fully registered component systems that an entity has been deleted.
    void EntityManagerData::DestroyEntity(EntityID ID) {
        UtilityBox::Logger::LogMessage message {};
        message.Supply("Entering function DestroyEntity with entity ID: %i.", ID);

        auto entityIterator = _entityComponents.find(ID);
        if (entityIterator != _entityComponents.end()) {
            _entityComponents.erase(entityIterator);
            message.Supply("Entity location was found and removed from Entity Manager.");

            // Notify systems of deleted entity.
            message.Supply("Component system 'OnEntityDestroy' callback functions called.");
            _loggingSystem.Log(message);

            for (auto &entityDestroyCallbackFunction : _entityDestroyCallbackFunctions) {
                entityDestroyCallbackFunction(ID);
            }
        }
        else {
            message.SetMessageSeverity(UtilityBox::Logger::LogMessageSeverity::WARNING);
            message.Supply("Entity location was not found in entity manager - no entity exists at ID: %i.", ID);
            _loggingSystem.Log(message);
        }
    }

    // Destroy an entity from the Entity Manager with the provided entity name, given that it exists. Automatically notifies
    // all fully registered component systems that an entity has been deleted.
    void EntityManagerData::DestroyEntity(std::string name) {
        UtilityBox::Logger::LogMessage message {};
        message.Supply("Entering function DestroyEntity with entity name: '%s.'", name.c_str());

        ConvertToLowercase(name);
        message.Supply("Entity name converted to lowercase: '%s.'", name.c_str());

        // Hand off responsibility to overloaded function.
        message.Supply("Calling function DestroyEntity with hashed entity ID.");
        _loggingSystem.Log(message);

        DestroyEntity(STRINGHASH(name.c_str()));
    }

    // Retrieve the list of components that are attached to an entity with the provided ID, given that such an entity
    // exists in the Entity Manager.
    const std::unordered_map<ComponentTypeID, Components::BaseComponent*>& EntityManagerData::GetComponents(EntityID ID) const {
        UtilityBox::Logger::LogMessage message {};
        message.Supply("Entering function GetComponents with entity ID: %i.", ID);

        if (_entityComponents.find(ID) == _entityComponents.end()) {
            message.SetMessageSeverity(UtilityBox::Logger::LogMessageSeverity::SEVERE);
            message.Supply("Exception thrown: Entity location was not found in entity manager - no entity exists at ID: %i.", ID);
            _loggingSystem.Log(message);

            throw std::out_of_range("In function GetComponents: Entity ID is invalid - no entity exists at the provided ID.");
        }

        message.Supply("Entity found, components accessed.");
        _loggingSystem.Log(message);

        return _entityComponents.at(ID);
    }

    // Retrieve the list of components that are attached to an entity with the provided name, given that such an entity
    // exists in the Entity Manager.
    const std::unordered_map<ComponentTypeID, Components::BaseComponent*>& EntityManagerData::GetComponents(std::string name) const {
        UtilityBox::Logger::LogMessage message {};
        message.Supply("Entering function GetComponents with entity name: '%s.'", name.c_str());

        ConvertToLowercase(name);
        message.Supply("Entity name converted to lowercase: '%s.'", name.c_str());

        // Hand off responsibility to overloaded function.
        message.Supply("Calling function GetComponents with hashed entity ID.");
        _loggingSystem.Log(message);

        return GetComponents(STRINGHASH(name.c_str()));
    }

    // Convert a given string to lowercase.
    void EntityManagerData::ConvertToLowercase(std::string& string) const {
        // Character by character traversal + conversion.
        for (char& character : string) {
            character = static_cast<char>(tolower(character));
        }
    }

    // Convert an instance of an EntityManager::CallbackType object to a string to use in debug messages.
    const char* EntityManagerData::CallbackTypeToString(CallbackType callbackType) {
        switch (callbackType) {
            case CallbackType::ENTITY_CREATE:
                return "OnEntityCreate";
            case CallbackType::ENTITY_DELETE:
                return "OnEntityDestroy";
            case CallbackType::COMPONENT_ADD:
                return "OnEntityComponentAdd";
            case CallbackType::COMPONENT_REMOVE:
                return "OnEntityComponentRemove";
            default:
                return "Unknown type";
        }
    }
}
