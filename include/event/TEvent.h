#ifndef __TEVENT__
#define __TEVENT__

class TEvent {
private:
	int iEvent;

public:
	TEvent();
	virtual ~TEvent();
	void setEvent(const int event);
	const int getEvent() const;
};

#endif