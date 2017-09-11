#pragma once

#include <cstring>
#include <cstdarg>
#include <cassert>
#include <vector>
#include <sstream>

#pragma warning( disable : 4996 )

namespace u8
{
	struct Char
	{
		Char()
		{
			unicode = 0;
		}

		Char(char c)
		{
			unicode = c;
		}

		static Char FromUnicode(int code)
		{
			Char c;
			c.unicode = (unsigned int)code;
			return c;
		}

		static Char FromUnicode(unsigned int code)
		{
			Char c;
			c.unicode = code;
			return c;
		}
		
		bool IsNull()
		{
			return unicode == 0;
		}

		bool IsNotNull()
		{
			return unicode != 0;
		}

		bool IsWhiteSpace()
		{
			if (unicode == ' ') return true;
			if (unicode == '\r') return true;
			if (unicode == '\n') return true;
			if (unicode == '\t') return true;
			return false;
		}

		bool IsAscii()
		{
			return unicode <= 127;
		}

		bool operator==(const Char& other)
		{
			return unicode == other.unicode;
		}

		bool operator!=(const Char& other)
		{
			return unicode != other.unicode;
		}

		unsigned int unicode;
	};

	namespace Util
	{
		static Char GetCharacter(const char* str)
		{
			unsigned int letter;
			letter = (unsigned int)str[0];
			if ((letter & 0x80) != 0x00)
			{
				if ((letter & 0xe0) == 0xc0)
					letter = ((letter & 0x1f) << 6) | (unsigned int)(str[1] & 0x3f);
				else if ((letter & 0xf0) == 0xe0)
					letter = ((letter & 0x0f) << 12) | ((unsigned int)(str[1] & 0x3f) << 6) | (unsigned int)(str[2] & 0x3f);
				else if ((letter & 0xf8) == 0xf0)
					letter = ((letter & 0x07) << 18) | ((unsigned int)(str[1] & 0x3f) << 12) | ((unsigned int)(str[2] & 0x3f) << 6) | (unsigned int)(str[3] & 0x3f);
			}
			return Char::FromUnicode(letter);
		}

		static const char* SkipCharacter(const char* str)
		{
			unsigned char c = *str;
			if ((c & 0x80) == 0x00)
				str++;
			else if ((c & 0xe0) == 0xc0)
				str += 2;
			else if ((c & 0xf0) == 0xe0)
				str += 3;
			else if ((c & 0xf8) == 0xf0)
				str += 4;
			return str;
		}

		static int GetCharacterWidth(Char letter)
		{
			if (letter.unicode <= 0x7f) return 1;
			if ((letter.unicode >= 0x80) && (letter.unicode <= 0x7ff)) return 2;
			if ((letter.unicode >= 0x800) && (letter.unicode <= 0xffff)) return 3;
			if ((letter.unicode >= 0x10000) && (letter.unicode <= 0x10ffff)) return 4;
			return 0;
		}

		static int GetCharacterWidth(const char* str)
		{
			return GetCharacterWidth(GetCharacter(str));
		}

		static int PeekCharacterWidth(const char* str)
		{
			int c = *str;
			int mask = 128;
			for (int i = 0; i < 7; i++)
			{
				if (!(c & mask))
					return i;
			}
			return 1;
		}

		static const char* PutCharacter(const char* str, Char letter, int space)
		{
			int width = GetCharacterWidth(letter);
			if (width <= space)
			{
				char* dst = (char*)str;
				switch (width)
				{
				case 1:
				{
					dst[0] = (unsigned char)(letter.unicode & 0x7f);
					dst[1] = (unsigned char)0;
					str += 1;
					break;
				}
				case 2:
				{
					letter.unicode &= 0x7ff;
					dst[0] = (unsigned char)(0xc0 | (letter.unicode >> 6));
					dst[1] = (unsigned char)(0x80 | (letter.unicode & 0x3f));
					dst[2] = 0;
					str += 2;
					break;
				}
				case 3:
				{
					letter.unicode &= 0xffff;
					dst[0] = (unsigned char)(0xe0 | (letter.unicode >> 12));
					dst[1] = (unsigned char)(0x80 | ((letter.unicode >> 6) & 0x3f));
					dst[2] = (unsigned char)(0x80 | (letter.unicode & 0x3f));
					dst[3] = 0;
					str += 3;
					break;
				}
				case 4:
				{
					letter.unicode &= 0xfffff;
					dst[0] = (unsigned char)(0xf0 | (letter.unicode >> 18));
					dst[1] = (unsigned char)(0x80 | ((letter.unicode >> 12) & 0x3f));
					dst[2] = (unsigned char)(0x80 | ((letter.unicode >> 6) & 0x3f));
					dst[3] = (unsigned char)(0x80 | (letter.unicode & 0x3f));
					dst[4] = 0;
					str += 4;
					break;
				}
				}
			}

			return str;
		}

		static int StrnCpy(const char* dstStr, const char* srcStr, int space)
		{
			int copiedLetters = 0;

			const char* ss = srcStr;

			int byteCount = 0;
			while (true)
			{
				Char letter = GetCharacter(ss);
				if (!letter.unicode) break;
				int width = GetCharacterWidth(letter);
				if (width + byteCount > space) break;
				byteCount += width;
				ss = SkipCharacter(ss);
				copiedLetters++;
			}

			if (byteCount > 0) memcpy((void*)dstStr, srcStr, byteCount);
			((char*)dstStr)[byteCount] = 0;

			return copiedLetters;
		}

		static int GetCharacterCount(const char* str)
		{
			int letterCount = 0;
			while (true)
			{
				Char letter = GetCharacter(str);
				if (!letter.unicode) break;
				str = SkipCharacter(str);
				letterCount++;
			}
			return letterCount;
		}

		static int GetByteCount(const char* str)
		{
			return (int)strlen(str);
		}

		static bool StrCmp(const char* s0, const char* s1)
		{
			if (strcmp(s0, s1)) return false;
			return true;
		}

		static bool ContainsCharacter(const char* str, Char letter)
		{
			while (true) 
			{
				Char character = GetCharacter(str);
				if (character.IsNull()) break;
				if (character == letter) return true;
				str = SkipCharacter(str);
			}
			return false;
		}
	};

	class Str
	{
	public:

		static void Append(std::string& stdString, Char character)
		{
			char b[16];
			Util::PutCharacter(b, character, 16);
			stdString += (const char*)b;
		}

		static int GetByteLength(const std::string& stdString)
		{
			return (int)stdString.length();
		}

		static int GetLength(const std::string& stdString)
		{
			if (stdString.empty()) return 0;

			int count = 0;
			const char* s = stdString.c_str();
			while (true)
			{
				Char l = Util::GetCharacter(s);
				if (l.IsNull()) break;
				s = Util::SkipCharacter(s);
				count++;
			}

			return count;
		}

		static std::string Trim(const std::string& stdString, const std::string& whitespaces = " \r\n\t")
		{
			int frontWhiteSpaceCount = 0, backWhitespaceCount = 0;

			int reader = 0;
			while (true)
			{
				Char c;
				reader = GetCharacter(stdString, reader, c);
				if (Contains(whitespaces, c))
					frontWhiteSpaceCount++;
				else
					break;
			}

			reader = 0;
			while (true)
			{
				Char c;
				reader = GetCharacter(stdString, reader, c);
				if (c.IsNull()) break;
				if (Contains(whitespaces, c))
					backWhitespaceCount++;
				else
					backWhitespaceCount = 0;
			}

			return GetSubstring(stdString, frontWhiteSpaceCount, GetLength(stdString) - frontWhiteSpaceCount - backWhitespaceCount);
		}

		static int GetCount(const std::string& stdString, Char character)
		{
			if (stdString.empty()) return 0;

			int count = 0;
			const char* s = stdString.c_str();
			while (true)
			{
				Char l = Util::GetCharacter(s);
				if (!l.unicode)
					break;
				s = Util::SkipCharacter(s);
				if (l == character)
					count++;
			}

			return count;
		}

		static std::string RemoveRight(const std::string& stdString, int characterCount)
		{
			int length = GetLength(stdString);

			if (length <= characterCount) return "";

			int copyCount = length - characterCount;

			std::string ret;

			const char* s = stdString.c_str();
			for (int i = 0; i < copyCount; i++)
			{
				Append(ret, Util::GetCharacter(s));
				s = Util::SkipCharacter(s);
			}

			return ret;
		}

		static std::string RemoveLeft(const std::string& stdString, int characterCount)
		{
			int length = GetLength(stdString);

			if (length <= characterCount) return "";

			const char* s = stdString.c_str();
			for (int i = 0; i < characterCount; i++)
				s = Util::SkipCharacter(s);

			return s;
		}

		static std::string GetRight(const std::string& stdString, int characterCount)
		{
			int length = GetLength(stdString);
			if (characterCount >= length) return stdString;
			return RemoveLeft(stdString, length - characterCount);
		}

		static std::string GetLeft(const std::string& stdString, int characterCount)
		{
			int length = GetLength(stdString);
			if (characterCount >= length) return stdString;
			return RemoveRight(stdString, length - characterCount);
		}

		static Char GetRight(const std::string& stdString)
		{
			int byteOffset = 0;
			return GetCharacter(GetRight(stdString, 1), byteOffset);
		}

		static Char GetLeft(const std::string& stdString)
		{
			int byteOffset = 0;
			return GetCharacter(stdString, byteOffset);
		}

		static std::string GetSubstring(const std::string& stdString, int firstCharacter, int characterCount)
		{
			int length = GetLength(stdString);

			if (firstCharacter >= length) return "";

			std::string ret = GetRight(stdString, length - firstCharacter);
			return GetLeft(ret, characterCount);
		}

		static std::string Replace(const std::string& stdString, Char character, Char with)
		{
			std::string temp;
			const char* s = stdString.c_str();

			while (true)
			{
				Char l = Util::GetCharacter(s);
				if (!l.unicode) break;
				if (l == character) Str::Append(temp, with); else Str::Append(temp, l);
				s = Util::SkipCharacter(s);
			}

			return temp;
		}

		static std::string Replace(const std::string& stdString, const std::string& replace, const std::string& to)
		{
			std::string ret = stdString;

			if (replace == "") return ret;

			int replaceLength = GetLength(replace);
			int atIndex;
			while (Contains(ret, replace, &atIndex))
			{
				std::string left = GetLeft(ret, atIndex);
				std::string right = GetRight(ret, GetLength(ret) - atIndex - replaceLength);
				ret = left + to + right;
			}

			return ret;
		}

		static std::string Remove(const std::string& stdString, Char character)
		{
			std::string temp;
			const char* s = stdString.c_str();

			while (true)
			{
				Char l = Util::GetCharacter(s);
				if (!l.unicode) break;
				if (l != character) Str::Append(temp, l);
				s = Util::SkipCharacter(s);
			}

			return temp;
		}

		static std::string SimplifyWhitespaces(const std::string& stdString, const std::string& whitespaces = " \r\n\t", Char simpleWhitespace = ' ')
		{
			std::string simplified;
			std::string trimmed = Trim(stdString);

			int reader = 0;
			bool inWhite = false;
			while (true)
			{//TODO
				Char c;
				reader = GetCharacter(trimmed, reader, c);
				if (c.IsNull()) break;

				if (Contains(whitespaces, c))
				{ // this is a whitespace
					if (!inWhite)
						Append(simplified, simpleWhitespace);
					inWhite = true;
				}
				else
				{ // not whitespace
					inWhite = false;
					Append(simplified, c);
				}
			}

			return simplified;
		}

		static int GetSegmentCount(const std::string& stdString, Char separator)
		{
			return GetCount(stdString, separator) + 1;
		}

		static bool HasSegment(const std::string& stdString, Char separator, const std::string& segmentText)
		{
			int cnt = GetSegmentCount(stdString, separator);
			for (int i = 0; i < cnt; i++)
			{
				if (GetSegment(stdString, separator, i) == segmentText)
					return true;
			}
			return false;
		}

		static std::string GetSegment(const std::string& stdString, Char separator, int segmentIndex)
		{
			std::string temp;
			const char* s = stdString.c_str();

			for (int i = 0; i < segmentIndex; i++)
			{
				while (true)
				{
					Char l = Util::GetCharacter(s);
					s = Util::SkipCharacter(s);
					if (!l.unicode) return temp;
					if (l == separator) break;
				}
			}

			while (true)
			{
				Char l = Util::GetCharacter(s);
				if (!l.unicode) break;
				if (l == separator) break;
				Str::Append(temp, l);
				s = Util::SkipCharacter(s);
			}
			return temp;
		}

		static std::vector<std::string> GetSegments(const std::string& stdString, Char separator, bool discardEmptySegments)
		{
			std::vector<std::string> segments;
			int segmentCount = GetSegmentCount(stdString, separator);
			for (int i = 0; i < segmentCount; i++)
			{
				std::string segment = GetSegment(stdString, separator, i);
				if (!discardEmptySegments || !segment.empty())
					segments.push_back(segment);
			}
			return segments;
		}

		static std::vector<std::string> Split(const std::string& stdString, Char separator, bool discardEmptySegments)
		{
			return GetSegments(stdString, separator, discardEmptySegments);
		}

		static int GetCharacter(const std::string& stdString, int byteOffset, Char& code)
		{
			const char* s = stdString.c_str();
			s += byteOffset;
			code = Util::GetCharacter(s);
			const char* s1 = Util::SkipCharacter(s);
			return byteOffset + (int)(s1 - s);
		}

		static Char GetCharacter(const std::string& stdString, int& inAndOutByteOffset)
		{
			Char c;
			inAndOutByteOffset = GetCharacter(stdString, inAndOutByteOffset, c);
			return c;
		}

		static std::vector<Char> Split(const std::string& stdString)
		{
			std::vector<Char> ret;
			int offset = 0;
			while (true)
			{
				Char c = GetCharacter(stdString, offset);
				if (c.IsNull()) break;
				ret.push_back(c);
			}
			return ret;
		}

		static bool BeginsWith(const std::string& stdString, const std::string& with)
		{
			if (GetLength(with) > GetLength(stdString)) return false;
			
			int reader0 = 0, reader1 = 0;
			while (true)
			{
				Char c0, c1;
				reader0 = GetCharacter(stdString, reader0, c0);
				reader1 = GetCharacter(with, reader1, c1);
				if (c1.IsNull()) break;
				if (c0 != c1) return false;
			}

			return true;
		}

		static bool EndsWith(const std::string& stdString, const std::string& with)
		{
			int withLength = GetLength(with);

			if (withLength > GetLength(stdString)) return false;

			return GetRight(stdString, withLength) == with;
		}

		static bool BeginsWith(const std::string& stdString, Char character)
		{
			return GetLeft(stdString) == character;
		}

		static bool EndsWith(const std::string& stdString, Char character)
		{
			return GetRight(stdString) == character;
		}

		static bool Contains(const std::string& stdString, Char character)
		{
			const char* s = stdString.c_str();
			while (true)
			{
				Char l = Util::GetCharacter(s);
				if (!l.unicode) break;
				if (l == character)
					return true;
				s = Util::SkipCharacter(s);
			}
			return false;
		}


		static bool Contains(const std::string& stdString, const std::string& what, int* foundAtCharacterIndex = nullptr)
		{
			int strLength = GetLength(stdString);
			int whatLength = GetLength(what);
			if (whatLength > strLength) return false;
			if (strLength == whatLength) return stdString == what;

			for (int i = 0; i <= strLength - whatLength; i++)
			{
				if (GetSubstring(stdString, i, whatLength) == what)
				{
					if (foundAtCharacterIndex)
						*foundAtCharacterIndex = i;
					return true;
				}
			}

			return false;
		}

		static bool ContainsOnly(const std::string& stdString, const std::string& characters)
		{
			int offset = 0;
			while (true)
			{
				Char c = GetCharacter(stdString, offset);
				if (c.IsNull()) break;
				if (Contains(characters, c) == false)
					return false;
			}
			return true;
		}

		static int GetFirstIndexOf(const std::string& stdString, Char character)
		{
			int step = 0;
			int reader = 0;
			while (true)
			{
				Char c;
				reader = GetCharacter(stdString, reader, c);
				if (c.IsNull()) break;
				if (c == character) return step;
				step++;
			}

			return -1;
		}

		static int GetLastIndexOf(const std::string& stdString, Char character)
		{
			int ret = -1;
			int step = 0;
			int reader = 0;
			while (true)
			{
				Char c;
				reader = GetCharacter(stdString, reader, c);
				if (c.IsNull()) break;
				if (c == character) ret = step;
				step++;
			}

			return ret;
		}

		template <typename T>
		static T Parse(const std::string stdString, bool* parsedSuccefully = nullptr)
		{
			if (stdString.empty())
			{
				if (parsedSuccefully) *parsedSuccefully = false;
				return 0;
			}

			try
			{
				std::istringstream iss(stdString);
				T value;
				iss >> std::noskipws >> value;
				if (parsedSuccefully) *parsedSuccefully = iss.eof() && !iss.fail();
				return value;
			}
			catch (...)
			{
				if (parsedSuccefully) *parsedSuccefully = false;
				return 0;
			}
		}

		template <typename T>
		static bool TryParse(const std::string stdString, T* value = nullptr)
		{
			if (stdString.empty())
				return false;

			bool succeeded;
			T pv = Parse<T>(stdString, &succeeded);
			if (value) *value = pv;
			return succeeded;
		}

		static std::string ToLower(const std::string stdString)
		{
			std::string ret;

			int offset = 0;
			while (true)
			{
				Char c = GetCharacter(stdString, offset);
				if (c.IsNull()) break;
				if (c.unicode >= 'A' && c.unicode <= 'Z')
					c.unicode = 'a' + (c.unicode - 'A');
				Append(ret, c);
			}

			return ret;
		}

		static std::wstring ToWideString(const std::string& utf8String)
		{
			std::wstring value;
			int read = 0;

			while (true)
			{
				u8::Char letter = u8::Str::GetCharacter(utf8String, read);
				if (letter.IsNull()) break;
				value += (wchar_t)letter.unicode;
			}

			return value;
		}

		static std::string FromWideString(const std::wstring& wString)
		{
			std::string value;
			for (auto a : wString)
			{
				u8::Char letter = u8::Char::FromUnicode(a);
				u8::Str::Append(value, letter);
			}
			return value;
		}

		static std::string Format(const char* fmt, ...)
		{
			char buffer[4096 + 1];
			va_list args;
			va_start(args, fmt);
			vsnprintf(buffer, 4096, fmt, args);
			va_end(args);
			buffer[4096] = 0;
			return buffer;
		}

		static void TestStringOperations()
		{
			assert(
				Trim("\t \n \r One two, three. \t\n ") == "One two, three."
				);

			assert(
				RemoveRight(RemoveLeft("\t \n \r One two, three. \t\n ", 6), 4) == "One two, three."
				);

			assert(
				GetSubstring("\t \n \r One two, three. \t\n ", 6, 15) == "One two, three."
				);

			assert(
				Trim(ToLower("\t \n \r One two, three. \t\n ")) == "one two, three."
				);
		}
	};
}

namespace u8
{
	class Text
	{
	public:

		template<typename ARG>
		void Append(ARG arg)
		{
			mContent << arg;
		}

		template<typename ARG, typename... ARGS>
		void Append(ARG arg, ARGS... args)
		{
			Append(arg);
			Append(args...);
		}

		template<typename... ARGS>
		Text(ARGS... args)
		{
			Append(args...);
		}

		operator std::string() const
		{
			return mContent.str();
		}

	private:

		std::stringstream mContent;

	};
}

namespace u8
{
	class TextPool
	{
	public:

		struct Resource { Resource(int h) { handle = h; } int handle = 0; };

		TextPool::Resource Set(const std::string& text)
		{
			return Set(text.c_str());
		}

		TextPool::Resource Set(const char* text = nullptr)
		{
			TextPool::Resource resource(0);
			return Set(resource, text);
		}

		TextPool::Resource Set(TextPool::Resource resource, const char* text = "")
		{
			if (resource.handle == 0)
			{
				int byteLenght = 1 + (int)strlen(text);

				for (int i = 0; i < mSlots.size(); i++)
				{
					if (mSlots[i].used == false && mSlots[i].size >= byteLenght)
					{
						mSlots[i].used = true;
						strcpy((char*)mBuffer.data() + mSlots[i].offset, text);
						return TextPool::Resource(i + 1);
					}
				}

				EnsureFreeSpace(byteLenght);

				int currentUsed = 0;
				if (mSlots.size() > 0)
					currentUsed = mSlots[mSlots.size() - 1].offset + mSlots[mSlots.size() - 1].size;

				Slot slot;
				slot.offset = currentUsed;
				slot.size = byteLenght;
				strcpy((char*)mBuffer.data() + currentUsed, text);
				slot.used = true;

				int index = (int)mSlots.size();

				mSlots.push_back(slot);

				return TextPool::Resource(index + 1);
			}
			else
			{
				int byteLenght = 1 + (int)strlen(text);
				int index = resource.handle - 1;

				if (mSlots[index].size >= byteLenght)
				{
					strcpy((char*)mBuffer.data() + mSlots[index].offset, text);
					return resource;
				}

				TextPool::Resource newResource = Set(text);
				mSlots[index].used = false;

				return newResource;
			}
		}
		
		void Delete(TextPool::Resource resource)
		{
			int index = resource.handle - 1;

			mSlots[index].used = false;
		}

		bool Contains(TextPool::Resource resource)
		{
			int index = resource.handle - 1;
			if (mSlots.size() <= index) return false;
			return mSlots[index].used;
		}

		void Get(TextPool::Resource resource, std::string& str)
		{
			int index = resource.handle - 1;
			str = (const char*)mBuffer.data() + mSlots[index].offset;
		}

		std::string Get(TextPool::Resource resource)
		{
			int index = resource.handle - 1;
			return std::string((const char*)mBuffer.data() + mSlots[index].offset);
		}

		const char* GetAsPointer(TextPool::Resource resource)
		{
			int index = resource.handle - 1;

			return (const char*)mBuffer.data() + mSlots[index].offset;
		}

		void Clear()
		{
			for (int i = 0; i < mSlots.size(); i++)
				mSlots[i].used = false;
		}

		std::vector<TextPool::Resource> GetAll()
		{
			int index = 0;
			std::vector<TextPool::Resource> all;
			for (const auto& a : mSlots)
			{
				if (a.used) all.push_back(TextPool::Resource(index + 1));
				index++;
			}
			return all;
		}

		std::vector<std::string> GetAllAsString()
		{
			int index = 0;
			std::vector<std::string> all;
			for (const auto& a : mSlots)
			{
				if (a.used) all.push_back(std::string(GetAsPointer(index + 1)));
				index++;
			}
			return all;
		}

		std::vector<const char*> GetAllAsPointer()
		{
			int index = 0;
			std::vector<const char*> all;
			for (const auto& a : mSlots)
			{
				if (a.used) all.push_back(GetAsPointer(index + 1));
				index++;
			}
			return all;
		}

	private:
		struct Slot { int offset, size; bool used; };

		void EnsureFreeSpace(int freeBytes)
		{
			int currentUsed = 0;
			if (mSlots.size() > 0)
				currentUsed = mSlots[mSlots.size() - 1].offset + mSlots[mSlots.size() - 1].size;

			if (mBuffer.size() - currentUsed >= freeBytes) return;

			int size = 1024;
			while (size < currentUsed + freeBytes)
				size *= 2;

			mBuffer.resize(size);
		}

		std::vector<unsigned char> mBuffer;
		std::vector<Slot> mSlots;
	};
}
