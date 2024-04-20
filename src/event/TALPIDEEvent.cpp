#include "TALPIDEEvent.h"

TALPIDEEvent::TALPIDEEvent() : TEvent(), fBits(kNotDeleted) { }

TALPIDEEvent::TALPIDEEvent(const TALPIDEEvent& copy) : iTime(copy.iTime), fBits(copy.fBits) {
	setEvent(copy.getEvent());
	data.assign(copy.data.begin(), copy.data.end());
}

TALPIDEEvent& TALPIDEEvent::operator=(const TALPIDEEvent& copy) {
	setEvent(copy.getEvent());
	fBits = copy.fBits;
	iTime = copy.iTime;
	data.assign(copy.data.begin(), copy.data.end());
	return *this;
}

TALPIDEEvent::TALPIDEEvent(TALPIDEEvent&& move) : iTime(move.iTime), fBits(move.fBits) {
	setEvent(move.getEvent());
	data.assign(move.data.begin(), move.data.end());
	move.fBits = 0;
	move.setEvent(0);
	move.iTime = 0;
	move.data.clear();
}

TALPIDEEvent& TALPIDEEvent::operator=(TALPIDEEvent&& move) {
	setEvent(move.getEvent());
	fBits = move.fBits;
	iTime = move.iTime;
	data.assign(move.data.begin(), move.data.end());
	move.setEvent(0);
	move.fBits = 0;
	move.iTime = 0;
	move.data.clear();
	return *this;
}

void TALPIDEEvent::setTime(const long int time) {
	iTime = time;
}

void TALPIDEEvent::pushData(const std::pair<int, int>& coordinate) {
	data.push_back(std::move(coordinate));
}

const long int TALPIDEEvent::getTime() const {
	return iTime;
}

const std::vector<std::pair<int, int>>& TALPIDEEvent::getData() const {
	return data;
}

void TALPIDEEvent::removePixel(const std::pair<int, int>& coordinate) {
	data.erase(std::find(data.begin(), data.end(), coordinate));
}

void TALPIDEEvent::removeDuplication() {
	std::vector<std::pair<int, int>> uniqueData;

	auto arrayHash = [ ](const std::pair<int, int>& arr) {
		return std::hash<int>()(arr.first) ^ (std::hash<int>()(arr.second) << 1);
		};

	std::unordered_set<std::pair<int, int>, decltype(arrayHash)> uniqueSet(data.size() * 2, arrayHash);

	for ( const auto& arr : data ) {
		if ( uniqueSet.find(arr) == uniqueSet.end() ) {
			uniqueData.push_back(arr);
			uniqueSet.insert(arr);
		}
	}
	data = std::move(uniqueData);
}

void TALPIDEEvent::sortPixel() {
	std::sort(data.begin(), data.end(), [ ](const std::pair<int, int>& a, const std::pair<int, int>& b) {
		if ( a.second != b.second ) {
			return a.second < b.second;  // Sort by first element in ascending order
		}
		return a.first < b.first;  // If the first elements are equal, sort by second element in ascending order
			  });
}