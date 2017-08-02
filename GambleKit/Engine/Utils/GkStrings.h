#pragma once

#include "..\GkDefinitions.h"

#include <string>

struct DateTime;

class GKDLLCLASS GkStrings
{
public:
	GkStrings::GkStrings(void);
	GkStrings::~GkStrings(void);

	static std::wstring GkStrings::FromShort(const char *chars);
	static std::wstring GkStrings::FromShort(const char *chars, int start, int length);
	static std::string GkStrings::ToShort(const wchar_t *string);
	static std::wstring GkStrings::Format(const wchar_t *formatString, ...);
	static std::wstring GkStrings::FormatArray(const wchar_t *formatString, wchar_t **parameters);
	static std::wstring GkStrings::IntToString(int value, int base = 10, int digits = 0, const wchar_t defaultDigit = L'0');
	static int GkStrings::StringToInt(const wchar_t *string);
	static std::wstring GkStrings::FloatToString(float value, int fracDigits, int intDigits = 0, const wchar_t defaultDigit = L'0');
	static wchar_t *GkStrings::Copy(const wchar_t *string);
	static char *GkStrings::Copy(const char *string);
	static void GkStrings::Dispose(wchar_t **string);
	static int GkStrings::GetLength(const wchar_t *string);
	static int GkStrings::GetLength(const char *string);
	static int GkStrings::SubChar(const wchar_t *string, const wchar_t subChar);
	static int GkStrings::Compare(const char *string1, const char *string2);
	static int GkStrings::Compare(const wchar_t *string1, const wchar_t *string2);

	static std::wstring GkStrings::FormatDateTime(const wchar_t *formatString, DateTime &dateTime);
	static std::wstring GkStrings::IntToDecimalString(int value, int numFractionDigits = 2);
};

