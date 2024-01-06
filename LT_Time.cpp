#include "LT_Time.h"
#include <cmath>

LT_TimePoint::LT_TimePoint()
{
	this->time = time_t(0);
}

LT_TimePoint::LT_TimePoint(time_t raw_epoch)
{
	this->time = raw_epoch;
}

//--------------------------------------------------

LT_TimestampFormat::LT_TimestampFormat(std::string formatString)
{
	this->formatString = formatString;

	const char* char_array = formatString.c_str();

	int i = 0;
	while (i < formatString.length() - 1) // it's -1 because you need both the '%' and the character after it
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
				break;
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
	LT_TimePoint::LT_SeparatedTime separatedTime{};

	int runningCharIndex = 0;

	const char* char_array = timestamp.c_str();

	for (int i = 0; i < tokens.size(); i++)
	{
		runningCharIndex += tokens[i].tokenIndex - (i == 0 ? 0 : tokens[i - 1].tokenIndex);

		if (runningCharIndex >= timestamp.length())
		{
			throw std::runtime_error("Token index " + std::to_string(runningCharIndex) + " is out of bounds.");
		}

		//looking at char_array starting at index runningCharIndex
		
		switch (tokens[i].tokenType)
		{
		case LT_TimestampToken::LT_TIMESTAMP_NUMBERED_MONTH:
			if (!char_is_int(char_array[runningCharIndex]))
				throw std::runtime_error("Token " + std::string(char_array[runningCharIndex], 1) + " is not a number.");
			else if (timestamp.length() > i + 1 && char_is_int(char_array[runningCharIndex + 1]))
			{
				int tens_place = char_to_int(char_array[runningCharIndex]);
				int ones_place = char_to_int(char_array[runningCharIndex + 1]);

				if (tens_place > 1)
					throw std::runtime_error("First digit " + std::string(char_array[runningCharIndex], 1) + " must be 0 or 1.");

				separatedTime.month = tens_place * 10 + ones_place;
				runningCharIndex += 2;
			}
			else
			{
				separatedTime.month = char_to_int(char_array[i]);
				runningCharIndex += 1;
			}

			break;
		case LT_TimestampToken::LT_TIMESTAMP_LEADING_ZERO_NUMBERED_MONTH:
			if (timestamp.length() <= runningCharIndex + 1)
				throw std::runtime_error("Token index " + std::to_string(runningCharIndex) + " is out of bounds.");
			separatedTime.month = (char_to_int(char_array[runningCharIndex]) * 10) + char_to_int(char_array[runningCharIndex + 1]);
			runningCharIndex += 2;
			break;
		case LT_TimestampToken::LT_TIMESTAMP_WRITTEN_MONTH:
			int finalLetterIndexOffset = 0;

			while (char_is_letter(char_array[finalLetterIndexOffset + runningCharIndex]))
			{
				finalLetterIndexOffset += 1;
			}

			separatedTime.month = writtenMonthToInt(std::string(char_array + runningCharIndex, finalLetterIndexOffset));
			runningCharIndex += finalLetterIndexOffset;

			break;
		case LT_TimestampToken::LT_TIMESTAMP_NUMBERED_DAY:
			if (!char_is_int(char_array[runningCharIndex]))
				throw std::runtime_error("Token " + std::string(char_array[runningCharIndex], 1) + " is not a number.");
			else if (timestamp.length() > i + 1 && char_is_int(char_array[runningCharIndex + 1]))
			{
				int tens_place = char_to_int(char_array[runningCharIndex]);
				int ones_place = char_to_int(char_array[runningCharIndex + 1]);

				if (tens_place > 3)
					throw std::runtime_error("First digit " + std::string(char_array[runningCharIndex], 1) + " must be 0, 1, 2, or 3.");

				separatedTime.day = tens_place * 10 + ones_place;
				runningCharIndex += 2;
			}
			else
			{
				separatedTime.day = char_to_int(char_array[i]);
				runningCharIndex += 1;
			}
			break;
		case LT_TimestampToken::LT_TIMESTAMP_LEADING_ZERO_NUMBERED_DAY:
			if (timestamp.length() <= runningCharIndex + 1)
				throw std::runtime_error("Token index " + std::to_string(runningCharIndex) + " is out of bounds.");
			separatedTime.day = (char_to_int(char_array[runningCharIndex]) * 10) + char_to_int(char_array[runningCharIndex + 1]);
			runningCharIndex += 2;
			break;
		case LT_TimestampToken::LT_TIMESTAMP_TWENTY_FOUR_HOUR:
			if (!char_is_int(char_array[runningCharIndex]))
				throw std::runtime_error("Token " + std::string(char_array[runningCharIndex], 1) + " is not a number.");
			else if (timestamp.length() > i + 1 && char_is_int(char_array[runningCharIndex + 1]))
			{
				int tens_place = char_to_int(char_array[runningCharIndex]);
				int ones_place = char_to_int(char_array[runningCharIndex + 1]);

				if (tens_place > 2)
					throw std::runtime_error("First digit " + std::string(char_array[runningCharIndex], 1) + " must be 0, 1, or 2.");

				separatedTime.hour = tens_place * 10 + ones_place;
				runningCharIndex += 2;
			}
			else
			{
				separatedTime.hour = char_to_int(char_array[i]);
				runningCharIndex += 1;
			}
			break;
		case LT_TimestampToken::LT_TIMESTAMP_LEADING_ZERO_TWENTY_FOUR_HOUR:
			if (timestamp.length() <= runningCharIndex + 1)
				throw std::runtime_error("Token index " + std::to_string(runningCharIndex) + " is out of bounds.");
			separatedTime.hour = (char_to_int(char_array[runningCharIndex]) * 10) + char_to_int(char_array[runningCharIndex + 1]);
			runningCharIndex += 2;
			break;
		case LT_TimestampToken::LT_TIMESTAMP_TWO_DIGIT_YEAR:
			if (timestamp.length() <= runningCharIndex + 1)
				throw std::runtime_error("Token index " + std::to_string(runningCharIndex) + " is out of bounds.");
			separatedTime.year = (char_to_int(char_array[runningCharIndex]) * 10) + char_to_int(char_array[runningCharIndex + 1]) + 2000;
			runningCharIndex += 2;
			break;
		case LT_TimestampToken::LT_TIMESTAMP_FOUR_DIGIT_YEAR:
			if (timestamp.length() <= runningCharIndex + 3)
				throw std::runtime_error("Token index " + std::to_string(runningCharIndex) + " is out of bounds.");
			separatedTime.year =	(char_to_int(char_array[runningCharIndex]) * 1000) + 
									(char_to_int(char_array[runningCharIndex + 1]) * 100) + 
									(char_to_int(char_array[runningCharIndex + 2]) * 10) +
									char_to_int(char_array[runningCharIndex + 3]);
			runningCharIndex += 4;
			break;
		case LT_TimestampToken::LT_TIMESTAMP_MINUTE:
			if (!char_is_int(char_array[runningCharIndex]))
				throw std::runtime_error("Token " + std::string(char_array[runningCharIndex], 1) + " is not a number.");
			else if (timestamp.length() > i + 1 && char_is_int(char_array[runningCharIndex + 1]))
			{
				int tens_place = char_to_int(char_array[runningCharIndex]);
				int ones_place = char_to_int(char_array[runningCharIndex + 1]);

				if (tens_place > 5)
					throw std::runtime_error("First digit " + std::string(char_array[runningCharIndex], 1) + " must be 0, 1, 2, 3, 4, or 5.");

				separatedTime.minute = tens_place * 10 + ones_place;
				runningCharIndex += 2;
			}
			else
			{
				separatedTime.minute = char_to_int(char_array[i]);
				runningCharIndex += 1;
			}
			break;
		case LT_TimestampToken::LT_TIMESTAMP_LEADING_ZERO_MINUTE:
			if (timestamp.length() <= runningCharIndex + 1)
				throw std::runtime_error("Token index " + std::to_string(runningCharIndex) + " is out of bounds.");
			separatedTime.minute = (char_to_int(char_array[runningCharIndex]) * 10) + char_to_int(char_array[runningCharIndex + 1]);
			runningCharIndex += 2;
			break;
		case LT_TimestampToken::LT_TIMESTAMP_SECOND:
			if (!char_is_int(char_array[runningCharIndex]))
				throw std::runtime_error("Token " + std::string(char_array[runningCharIndex], 1) + " is not a number.");
			else if (timestamp.length() > i + 1 && char_is_int(char_array[runningCharIndex + 1]))
			{
				int tens_place = char_to_int(char_array[runningCharIndex]);
				int ones_place = char_to_int(char_array[runningCharIndex + 1]);

				if (tens_place > 5)
					throw std::runtime_error("First digit " + std::string(char_array[runningCharIndex], 1) + " must be 0, 1, 2, 3, 4, or 5.");

				separatedTime.second = tens_place * 10 + ones_place;
				runningCharIndex += 2;
			}
			else
			{
				separatedTime.second = char_to_int(char_array[i]);
				runningCharIndex += 1;
			}
			break;
		case LT_TimestampToken::LT_TIMESTAMP_LEADING_ZERO_SECOND:
			if (timestamp.length() <= runningCharIndex + 1)
				throw std::runtime_error("Token index " + std::to_string(runningCharIndex) + " is out of bounds.");
			separatedTime.second = (char_to_int(char_array[runningCharIndex]) * 10) + char_to_int(char_array[runningCharIndex + 1]);
			runningCharIndex += 2;
			break;
		case LT_TimestampToken::LT_TIMESTAMP_MILLISECOND:
			int finalNumberIndexOffset = 0;

			while (char_is_int(char_array[finalLetterIndexOffset + runningCharIndex]))
			{
				finalNumberIndexOffset += 1;
			}

			separatedTime.millisecond = 0;

			for (int j = 0; j < finalNumberIndexOffset; j++)
			{

				separatedTime.millisecond += char_to_int(char_array[runningCharIndex + j]) * (std::pow(10, finalNumberIndexOffset - j - 1));
			}
			runningCharIndex += finalNumberIndexOffset;
			break;
		default:
			throw std::runtime_error("Unknown token format " + (std::to_string((int)(tokens[i].tokenType))));
		}
	}
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


bool LT_TimestampFormat::char_is_int(char c)
{
	return c >= 48 && c <= 57;
}

bool LT_TimestampFormat::char_is_letter(char c)
{
	return (c >= 65 && c <= 90) || (c >= 97 && c <= 122);
}

int LT_TimestampFormat::char_to_int(char c)
{
	if (!char_is_int(c))
		return -1;
	return ((int)c - 48); // '0' is 48
}
