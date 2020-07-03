#include <iostream>
#include "Utilities/Logger/logger.h"
#include "Utilities/Logger/adapter.h"
#include "Utilities/assert_dev.h"
#include "Utilities/Memory/segmented_pool_allocator.h"
#include "Utilities/Memory/contiguous_pool_allocator.h"
#include "Utilities/DataStructures/Array/array.h"
#include "ECS/Components/component_manager.h"
#include "ECS/Components/base_component.h"
#include "ECS/Components/component_manager_collection.h"
#include "ECS/Systems/base_component_system.h"

using namespace UtilityBox::Logger;

class myAdapter : public Adapter {
    public:
        myAdapter();
        void ProcessMessage(void* messageAddress) override;
        void OutputMessage() override;
    private:
};

myAdapter::myAdapter() : Adapter("myAdapter") {
}

void myAdapter::ProcessMessage(void *messageAddress) {
    messageAddress = static_cast<int*>(messageAddress) + 3;
    const LogMessageSeverity& messageSeverity = LoggingHub::GetInstance().GetMessageSeverity(messageAddress);

    if (messageSeverity >= _config.GetMessageSeverityCutoff()) {
        // format header
        ConstructMessageHeader(messageAddress);

        // format messages
        ConstructMessageBody(messageAddress);
    }
}

void myAdapter::OutputMessage() {
    while (!_formattedMessages.empty()) {
        std::cout << _formattedMessages.front();
        _formattedMessages.pop();
    }
}

class Data : public ECS::Components::BaseComponent {
    public:
        static constexpr unsigned ID = 0;
        int a;
        int b;
        float c;
        char h[16];
};


class Data2 : public ECS::Components::BaseComponent {
    public:
        static constexpr unsigned ID = 1;
        int a;
        int b;
        float c;
        char h[16];
};

class Data3 : public ECS::Components::BaseComponent {
    public:
        static constexpr unsigned ID = 2;
        int a;
        int b;
        float c;
        char h[16];
};

class mySystem : ECS::Components::BaseComponentSystem<Data, Data2, Data3> {
    public:
        explicit mySystem(std::string&& name);

        void Initialize() override;
        void Update(float dt) override;
        void Shutdown() override;
    private:
};

mySystem::mySystem(std::string &&name) : ECS::Components::BaseComponentSystem<Data, Data2, Data3>(std::move(name)) {
}

void mySystem::Update(float dt) {
}

void mySystem::Initialize() {
    ComponentTuple a;
    Data2* ad = new Data2();
    std::get<1>(a) = ad;
    auto* data2ptr = GetComponent<Data2>(a);
    LogMessage message = LogMessage();
    message.Supply("Logging from mySystem.");
    this->_loggingSystem.Log(message);
}

void mySystem::Shutdown() {
}


int main() {
    UtilityBox::Logger::LoggingHub::Initialize();

    auto* mysystem = new mySystem("name");
    mysystem->Initialize();

    return 0;
}
