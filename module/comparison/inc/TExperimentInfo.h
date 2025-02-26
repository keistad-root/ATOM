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
	int mLength, mPhi;
	double mCollimatorLength;
	double mCollimatorArea;
	int mMinute;
	std::array<double, 3> mEntry;
	std::array<double, 3> mError;
public:
	void setTag(const std::string& tag);
	void setLength(int length);
	void setPhi(int phi);
	void setCollimatorLength(double collimatorLength);
	void setCollimatorArea(double collimatorArea);
	void setMinute(int minute);
	void setEntry(const std::array<double, 3>& entry);
	void setError(const std::array<double, 3>& error);
	const std::string& getTag() const;
	int getLength() const;
	int getPhi() const;
	double getCollimatorLength() const;
	double getCollimatorArea() const;
	int getMinute() const;
	const std::array<double, 2> getCS1() const;
	const std::array<double, 2> getCS4TO32() const;
	const std::array<double, 2> getCS33Over() const;
};

#endif