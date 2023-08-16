#include "LT_Time.h"

LT_TimePoint::LT_TimePoint()
{
	this->time = time_t(0);
}

//--------------------------------------------------

LT_TimestampFormat::LT_TimestampFormat(std::string formatString)
{
	this->formatString = formatString;

	const char* char_array = formatString.c_str();

	int i = 0;
	while (i < formatString.length())
	{
		if (char_array[i] == '%')
		{
			LT_TimestampFormatToken token = LT_TimestampFormatToken();
			token.tokenIndex = i;

			++i;

			switch (char_array[i])
			{
			case 'm':
				token.tokenType = LT_TimestampToken::LT_TIMESTAMP_NUMBERED_MONTH;
				break;
			case 'M':
				token.tokenType = LT_TimestampToken::LT_TIMESTAMP_LEADING_ZERO_NUMBERED_MONTH;
				break;
			case 'w':
				token.tokenType = LT_TimestampToken::LT_TIMESTAMP_WRITTEN_MONTH;
			case 'd':
				token.tokenType = LT_TimestampToken::LT_TIMESTAMP_NUMBERED_DAY;
				break;
			case 'D':
				token.tokenType = LT_TimestampToken::LT_TIMESTAMP_LEADING_ZERO_NUMBERED_DAY;
				break;
			case 'h':
				token.tokenType = LT_TimestampToken::LT_TIMESTAMP_TWENTY_FOUR_HOUR;
				break;
			case 'H':
				token.tokenType = LT_TimestampToken::LT_TIMESTAMP_LEADING_ZERO_TWENTY_FOUR_HOUR;
				break;
			case 'y':
				token.tokenType = LT_TimestampToken::LT_TIMESTAMP_TWO_DIGIT_YEAR;
				break;
			case 'Y':
				token.tokenType = LT_TimestampToken::LT_TIMESTAMP_FOUR_DIGIT_YEAR;
				break;
			case 's':
				token.tokenType = LT_TimestampToken::LT_TIMESTAMP_SECOND;
				break;
			case 'S':
				token.tokenType = LT_TimestampToken::LT_TIMESTAMP_LEADING_ZERO_SECOND;
				break;
			case 'i':
				token.tokenType = LT_TimestampToken::LT_TIMESTAMP_MILLISECOND;
				break;
			case 'I':
				token.tokenType = LT_TimestampToken::LT_TIMESTAMP_LEADING_ZERO_MILLISECOND;
				break;
			default:
				throw std::runtime_error("Unknown token format %" + char_array[i]);
			}

			tokens.push_back(token);
		}
		else
		{
			++i;
		}
	}

	delete char_array;
}

LT_TimePoint LT_TimestampFormat::getTimePointFromTimestamp(std::string timestamp)
{
	return LT_TimePoint();
}

int LT_TimestampFormat::writtenMonthToInt(std::string month)
{
	if (month == "Jan" || month == "jan" || month == "JAN" || month == "January" || month == "JANUARY" || month == "january")
	{
		return 1;
	}
	else if (month == "Feb" || month == "feb" || month == "FEB" || month == "February" || month == "FEBRUARY" || month == "february")
	{
		return 2;
	}
	else if (month == "Mar" || month == "mar" || month == "MAR" || month == "March" || month == "MARCH" || month == "march")
	{
		return 3;
	}
	else if (month == "Apr" || month == "apr" || month == "APR" || month == "April" || month == "APRIL" || month == "april")
	{
		return 4;
	}
	else if (month == "May" || month == "may" || month == "MAY")
	{
		return 5;
	}
	else if (month == "Jun" || month == "jun" || month == "JUN" || month == "June" || month == "JUNE" || month == "june")
	{
		return 6;
	}
	else if (month == "Jul" || month == "jul" || month == "JUL" || month == "JULY" || month == "July" || month == "july")
	{
		return 7;
	}
	else if (month == "Aug" || month == "aug" || month == "AUG" || month == "August" || month == "AUGUST" || month == "august")
	{
		return 8;
	}
	else if (month == "Sep" || month == "sep" || month == "SEP" || month == "September" || month == "SEPTEMBER" || month == "september")
	{
		return 9;
	}
	else if (month == "Oct" || month == "oct" || month == "OCT" || month == "October" || month == "OCTOBER" || month == "october")
	{
		return 10;
	}
	else if (month == "Nov" || month == "nov" || month == "NOV" || month == "November" || month == "NOVEMBER" || month == "november")
	{
		return 11;
	}
	else if (month == "Dec" || month == "dec" || month == "DEC" || month == "December" || month == "DECEMBER" || month == "december")
	{
		return 12;
	}
	else
	{
		throw std::runtime_error("Unknown month " + month);
	}
}