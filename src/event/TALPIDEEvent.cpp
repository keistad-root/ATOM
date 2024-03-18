#include "TALPIDEEvent.h"

TALPIDEEvent::TALPIDEEvent() : TEvent() {}

TALPIDEEvent::TALPIDEEvent(const TALPIDEEvent& copy) : iTime(copy.iTime) {
    setEvent(copy.getEvent());
    data.assign(copy.data.begin(),copy.data.end());
}

void TALPIDEEvent::setTime(const long int time) {
    iTime = time;
}

void TALPIDEEvent::pushData(const std::array<int,2>& coordinate) {
    data.push_back(coordinate);
}

long int TALPIDEEvent::getTime() {
    return iTime;
}

std::vector<std::array<int,2>>& TALPIDEEvent::getData() {
    return data;
}

void TALPIDEEvent::removeDuplication() {
    std::vector<std::array<int,2>> uniqueData;

    auto arrayHash = [](const std::array<int, 2>& arr) {
        return std::hash<int>()(arr[0]) ^ (std::hash<int>()(arr[1]) << 1);
    };

    std::unordered_set<std::array<int,2>, decltype(arrayHash)> uniqueSet(data.size() * 2, arrayHash);

    for (const auto& arr : data) {
        if (uniqueSet.find(arr) == uniqueSet.end()) {
            uniqueData.push_back(arr);
            uniqueSet.insert(arr);
        }
    }

    data = std::move(uniqueData);
}

void TALPIDEEvent::sortPixel() {
    std::sort(data.begin(), data.end(), [](const std::array<int, 2>& a, const std::array<int, 2>& b) {
        if (a[1] != b[1]) {
            return a[1] < b[1];  // Sort by first element in ascending order
        }
        return a[0] < b[0];  // If the first elements are equal, sort by second element in ascending order
    });
}