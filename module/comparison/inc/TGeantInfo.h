#ifndef __TGEANTINFO__
#define __TGEANTINFO__

#include<string>
#include<array>

class TGeantInfo {
public:
	TGeantInfo();
	~TGeantInfo();
private:
	std::string mTag;
	int mLength, mPhi;
	double mCollimatorLength;
	double mCollimatorArea;
	int mNEvent;
	std::array<double, 4> mEntry;
	std::array<double, 4> mError;
public:
	void setTag(const std::string& tag);
	void setLength(int length);
	void setPhi(int phi);
	void setCollimatorLength(double collimatorLength);
	void setCollimatorArea(double collimatorArea);
	void setNEvent(int nEvent);
	void setEntry(const std::array<double, 4>& entry);
	void setError(const std::array<double, 4>& error);
	const std::string& getTag() const;
	int getLength() const;
	int getPhi() const;
	double getCollimatorLength() const;
	double getCollimatorArea() const;
	int getNEvent() const;
	const std::array<double, 2> getEEM() const;
	const std::array<double, 2> getEAM() const;
	const std::array<double, 2> getEAE() const;
	const std::array<double, 2> getDouble() const;
};
#endif