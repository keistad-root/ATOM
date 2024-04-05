#include "TALPIDEEvent.h"

TALPIDEEvent::TALPIDEEvent() : TEvent() {}

TALPIDEEvent::TALPIDEEvent(const TALPIDEEvent& copy) : iTime(copy.iTime) {
    setEvent(copy.getEvent());
    data.assign(copy.data.begin(),copy.data.end());
}

TALPIDEEvent& TALPIDEEvent::operator=(const TALPIDEEvent& copy) {
    setEvent(copy.getEvent());
    iTime = copy.iTime;
    data.assign(copy.data.begin(), copy.data.end());
    return *this;
}

TALPIDEEvent::TALPIDEEvent(TALPIDEEvent&& move) : iTime(move.iTime) {
    setEvent(move.getEvent());
    data.assign(move.data.begin(), move.data.end());
    move.setEvent(0);
    move.iTime = 0;
    move.data.clear();
}

TALPIDEEvent& TALPIDEEvent::operator=(TALPIDEEvent&& move) {
    setEvent(move.getEvent());
    iTime = move.iTime;
    data.assign(move.data.begin(), move.data.end());
    move.setEvent(0);
    move.iTime = 0;
    move.data.clear();
    return *this;
}

void TALPIDEEvent::setTime(const long int time) {
    iTime = time;
}

void TALPIDEEvent::pushData(const std::array<int,2>& coordinate) {
    data.push_back(coordinate);
}

const long int TALPIDEEvent::getTime() const {
    return iTime;
}

const std::vector<std::array<int,2>>& TALPIDEEvent::getData() const {
    return data;
}

void TALPIDEEvent::removePixel(const std::array<int, 2>& coordinate) {
    data.erase(std::find(data.begin(), data.end(), coordinate));
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