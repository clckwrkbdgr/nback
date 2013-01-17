#pragma once

class Statistics {
public:
	Statistics();
	virtual ~Statistics();

	void add(int n, int size, int errors, int missed);
	int currentTryCount() const;
	double currentErrorRatio() const;
private:
	int sessionId;
};
