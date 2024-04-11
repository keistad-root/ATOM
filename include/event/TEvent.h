#ifndef __TEVENT__
#define __TEVENT__

#include "Headers.h"

class TEvent
	{
	private:
		int iEvent;

	public:
		TEvent();
		virtual ~TEvent();
		void setEvent(const int event);
		const int getEvent() const;
	};

class TALPIDEEvent : public TEvent
	{
	private:
		long int iTime;
		std::vector<std::pair<int, int>> data;

	public:
		TALPIDEEvent();
		TALPIDEEvent(const TALPIDEEvent& copy);
		TALPIDEEvent& operator=(const TALPIDEEvent& copy);
		TALPIDEEvent(TALPIDEEvent&& move);
		TALPIDEEvent& operator=(TALPIDEEvent&& move);

		// Setter
		void setTime(const long int time);

		// Getter
		const long int getTime() const;
		const std::vector<std::pair<int, int>>& getData() const;

		void removePixel(const std::pair<int, int>& coordinate);
		void pushData(const std::pair<int, int>& coordinate);
		void removeDuplication();
		void sortPixel();
	};

class TAPTSEvent : public TEvent
	{
	private:
		int iFrame;
		std::array<int, 16> data{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	public:
		TAPTSEvent();

		// Setter
		void setFrame(int frame);

		// Getter
		int getFrame();
		std::array<int, 16>& getData();
	};

#endif