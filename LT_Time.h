#pragma once

#include <chrono>
#include <ctime>
#include <string>
#include <cstring>
#include <stdexcept>
#include <vector>

struct LT_TimePoint;

struct LT_TimestampFormat {
	
	LT_TimestampFormat(std::string formatString);

	LT_TimePoint getTimePointFromTimestamp(std::string timestamp);

private:
	
	enum class LT_TimestampToken {
		LT_TIMESTAMP_LEADING_ZERO_NUMBERED_MONTH,
		LT_TIMESTAMP_NUMBERED_MONTH,
		LT_TIMESTAMP_WRITTEN_MONTH,
		LT_TIMESTAMP_LEADING_ZERO_NUMBERED_DAY,
		LT_TIMESTAMP_NUMBERED_DAY,
		LT_TIMESTAMP_FOUR_DIGIT_YEAR,
		LT_TIMESTAMP_TWO_DIGIT_YEAR,
		LT_TIMESTAMP_LEADING_ZERO_TWENTY_FOUR_HOUR,
		LT_TIMESTAMP_TWENTY_FOUR_HOUR,
		LT_TIMESTAMP_LEADING_ZERO_MINUTE,
		LT_TIMESTAMP_MINUTE,
		LT_TIMESTAMP_LEADING_ZERO_SECOND,
		LT_TIMESTAMP_SECOND,
		LT_TIMESTAMP_LEADING_ZERO_MILLISECOND,
		LT_TIMESTAMP_MILLISECOND
	};

	struct LT_TimestampFormatToken {
		LT_TimestampToken tokenType;
		int tokenIndex;
	};

	int writtenMonthToInt(std::string month);


	std::string formatString;
	std::vector<LT_TimestampFormatToken> tokens;
};

struct LT_TimePoint {

	LT_TimePoint();
	LT_TimePoint(time_t raw_epoch);
	LT_TimePoint(std::string timestamp, LT_TimestampFormat format);
	LT_TimePoint(const LT_TimePoint &timepoint); //copy constructor

	time_t epoch() {
		return time;
	}
	
private:
	time_t time;

};