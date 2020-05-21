#ifndef DATASTRUCTURES_TIMESTAMP_H
#define DATASTRUCTURES_TIMESTAMP_H

#include <memory>
#include <chrono>

#define _NODISCARD_ [[nodiscard]]

namespace UtilityBox::TimeStamp {
    class TimeStamp {
        public:
            TimeStamp();
            explicit TimeStamp(const std::chrono::time_point<std::chrono::high_resolution_clock>& timestamp);
            TimeStamp(const TimeStamp& other);
            TimeStamp(TimeStamp&& other) noexcept;
            ~TimeStamp();
            _NODISCARD_ bool operator<(const TimeStamp& other) const;
            _NODISCARD_ bool operator==(const TimeStamp& other) const;

            _NODISCARD_ unsigned ConvertToMillis() const;
            _NODISCARD_ unsigned GetMillis() const;
            _NODISCARD_ unsigned GetSeconds() const;
            _NODISCARD_ unsigned GetMinutes() const;

        private:
            struct TimeStampData;
            std::unique_ptr<TimeStampData> _data;
    };
}



#endif //DATASTRUCTURES_TIMESTAMP_H
