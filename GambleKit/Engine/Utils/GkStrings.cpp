//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//	GambleKit Copyright (C) 2014 Ivan Donev
//
//	This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 2.1 of the License, or (at your option) any later version.
//
//	This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.
//
//	You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
//
//	contact: gamblekit@gmail.com
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#include "GkStrings.h"

#include <vector>
#include "..\OSSpecific\GkOSUtils.h"

GkStrings::GkStrings(void)
{
}


GkStrings::~GkStrings(void)
{
}

struct FormatStringChunk
{
	bool isFormatter;
	int start, length;
};

struct FormatStringChunks
{
	std::vector<FormatStringChunk> chunks;
	int numFormatterChunks;
};

FormatStringChunks GetFormatStringChunks(const wchar_t *formatString)
{
	FormatStringChunks result;
	result.numFormatterChunks = 0;

	FormatStringChunk chunk;
	chunk.isFormatter = false;
	chunk.start = 0;
	
	for (int i = 0; ; i++)
	{
		if (formatString[i] == L'{')
		{
			chunk.length = i - chunk.start;

			if (chunk.length > 0)
				result.chunks.push_back(chunk);

			chunk.isFormatter = true;
			chunk.start = i + 1;
		}
		else if (formatString[i] == L'}')
		{
			chunk.length = i - chunk.start;

			if (chunk.length > 0)
			{
				result.chunks.push_back(chunk);
				
				if (chunk.isFormatter)
					result.numFormatterChunks++;
			}

			chunk.isFormatter = false;
			chunk.start = i + 1;
		}
		else if (formatString[i] == 0)
		{
			chunk.length = i - chunk.start;

			if (chunk.length > 0)
				result.chunks.push_back(chunk);

			break;
		}
	}

	return result;
}

std::wstring GkStrings::Format(const wchar_t *formatString, ...)
{
	wchar_t **parameters = (wchar_t**)((int)&formatString + 4);

	return GkStrings::FormatArray(formatString, parameters);
}

std::wstring GkStrings::FormatArray(const wchar_t *formatString, wchar_t **parameters)
{
	FormatStringChunks chunks = GetFormatStringChunks(formatString);

	std::wstring result;

	int numChunks = chunks.chunks.size();
	for (int i = 0; i < numChunks; i++)
	{
		FormatStringChunk chunk = chunks.chunks[i];

		if (chunk.isFormatter)
		{
			result.append(parameters[GkStrings::StringToInt(std::wstring(&formatString[chunk.start], chunk.length).c_str())]);
		}
		else
		{
			result.append(std::wstring(&formatString[chunk.start], chunk.length));
		}
	}

	return result;
}

std::wstring GkStrings::IntToString(int value, int base, int digits, const wchar_t defaultDigit)
{
	wchar_t s[256], result[256];
	_itow_s(value, s, base);
	int intLength = wcslen(s);
	int extraDigits = digits > intLength ? digits - intLength : 0;
	int resultLength = extraDigits > 0 ? digits : intLength;

	for (int i = 0; i < resultLength; i++)
	{
		if (i < extraDigits)
			result[i] = defaultDigit;
		else
			result[i] = s[i - extraDigits];
	}

	result[resultLength] = L'\0';

	return std::wstring(result);
}

std::wstring GkStrings::FloatToString(float value, int fracDigits, int intDigits, const wchar_t defaultDigit)
{
	int intPart = (int)value;
	int fracPart = (int)((value - intPart) * pow(10.0f, fracDigits));

	return GkStrings::Format(L"{0}.{1}", GkStrings::IntToString(intPart, 10, intDigits, defaultDigit).c_str(), GkStrings::IntToString(fracPart, 10, fracDigits, L'0').c_str());
}

int GkStrings::StringToInt(const wchar_t *string)
{
	return _wtoi(string);
}

wchar_t *GkStrings::Copy(const wchar_t *string)
{
	if (string == 0)
		return 0;
	
	int length = wcslen(string);
	
	wchar_t *result = new wchar_t[length + 1];
	memcpy(result, string, (length + 1) * 2);

	return result;
}

char *GkStrings::Copy(const char *string)
{
	if (string == 0)
		return 0;
	
	int length = strlen(string);
	
	char *result = new char[length + 1];
	memcpy(result, string, (length + 1));

	return result;
}

void GkStrings::Dispose(wchar_t **string)
{
	if (*string != 0)
	{
		delete[] *string;
		string = 0;
	}
}

int GkStrings::GetLength(const wchar_t *string)
{
	return wcslen(string);
}

int GkStrings::GetLength(const char *string)
{
	return strlen(string);
}

int GkStrings::SubChar(const wchar_t *string, const wchar_t subChar)
{
	if (string == 0)
		return -1;

	for (int i = 0; *string != 0; i++)
	{
		if (*string++ == subChar)
		{
			return i;
		}
	}

	return -1;
}

int GkStrings::Compare(const char *string1, const char *string2)
{
	return strcmp(string1, string2);
}

int GkStrings::Compare(const wchar_t *string1, const wchar_t *string2)
{
	return wcscmp(string1, string2);
}

std::wstring GkStrings::FormatDateTime(const wchar_t *formatString, DateTime &dateTime)
{
	return std::wstring(GkStrings::Format(formatString, GkStrings::IntToString(dateTime.day, 10, 2).c_str(), GkStrings::IntToString(dateTime.month, 10, 2).c_str(), GkStrings::IntToString(dateTime.year, 10, 4).c_str(), GkStrings::IntToString(dateTime.hour, 10, 2).c_str(), GkStrings::IntToString(dateTime.minute, 10, 2).c_str(), GkStrings::IntToString(dateTime.second, 10, 2).c_str()));
}

std::wstring GkStrings::IntToDecimalString(int value, int numFractionDigits)
{
	int fractionalFactor = 1;
	for (int i = 0; i < numFractionDigits; i++)
		fractionalFactor *= 10;

	return GkStrings::Format(L"{0}.{1}", GkStrings::IntToString(value / fractionalFactor).c_str(), GkStrings::IntToString(value % fractionalFactor, 10, numFractionDigits).c_str());
}

std::wstring GkStrings::FromShort(const char *chars)
{
	int length = GkStrings::GetLength(chars);

	wchar_t *wide = new wchar_t[length + 1];
	for (int i = 0; i < length; i++)
		wide[i] = chars[i];
	wide[length] = 0;
	
	std::wstring result(wide);

	delete[] wide;
	
	return result;
}

std::wstring GkStrings::FromShort(const char *chars, int start, int length)
{
	wchar_t *wide = new wchar_t[length + 1];
	for (int i = 0; i < length; i++)
		wide[i] = chars[start + i];
	wide[length] = 0;
	
	std::wstring result(wide);

	delete[] wide;
	
	return result;
}

std::string GkStrings::ToShort(const wchar_t *string)
{
	int length = GkStrings::GetLength(string);

	char *shortString = new char[length + 1];
	for (int i = 0; i < length; i++)
		shortString[i] = (char)string[i];
	shortString[length] = 0;
	
	std::string result(shortString);

	delete[] shortString;

	return result;
}
