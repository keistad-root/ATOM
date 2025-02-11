#ifndef __TEXPERIMENTINFO__
#define __TEXPERIMENTINFO__

#include<string>
#include<array>

class TExperimentInfo {
public:
	TExperimentInfo();
	~TExperimentInfo();
private:
	std::string mTag;
	int mLength;
	double mCollimatorLength;
	double mCollimatorArea;
	int mMinute;
	std::array<double, 60> mEntry;
	std::array<double, 60> mError;
public:
	void setTag(const std::string& tag);
	void setLength(int length);
	void setCollimatorLength(double collimatorLength);
	void setCollimatorArea(double collimatorArea);
	void setMinute(int minute);
	void setEntry(const std::array<double, 60>& entry);
	void setError(const std::array<double, 60>& error);
	const std::string& getTag() const;
	int getLength() const;
	double getCollimatorLength() const;
	double getCollimatorArea() const;
	int getMinute() const;
	const std::array<double, 60>& getEntry() const;
	const std::array<double, 60>& getError() const;
	const std::array<double, 2> getSubEntry(int start, int end) const;
};

#endif