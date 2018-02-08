#pragma once

#include "common/logging.h"
#include "common/debugbreak.h"

#define THROW(exceptionType, message) \
	LOG("\n\t" << exceptionType::Type() << "\n" <<\
		'\t' << "File: " << __FILE__ << "\n" <<\
		'\t' << "Line: " << __LINE__ << "\n" <<\
		'\t' << "Function: " << __FUNCTION__ << "\n" <<\
		'\t' << "Message: " << message);\
	debug_break();\
	throw exceptionType(message, __FUNCTION__, __FILE__, __LINE__); 

#define ASSERT(expression, message) if (!(expression)) { THROW(Assertion, message); }
#define TEST_ASSERT(expression, message) if (!(expression)) { THROW(TestAssertion, message); }
#define THROW_IF(expression, exceptionType, message) if ((expression)) { THROW(exceptionType, message); }

#if _WIN32
	#define NOEXCEPT
#elif __GNUG__
	#define NOEXCEPT noexcept
#endif

namespace cbr
{

class RuntimeException
{
public:
	RuntimeException(const std::string& message, const std::string& function,
		const std::string& file, int line)
		:message(message)
		, function(function)
		, file(file)
		, line(line)
	{}
	virtual ~RuntimeException() NOEXCEPT {}
protected:
	int line;
	std::string file;
	std::string message;
	std::string function;
};

class Assertion : public RuntimeException
{
public:
	Assertion(const std::string& message, const std::string& function,
		const std::string& file, int line) : RuntimeException(message, function, file, line) {}

	static std::string Type() NOEXCEPT {
		return "cbr::Assertion";
	}
};

class TestAssertion : public RuntimeException
{
public:
	TestAssertion(const std::string& message, const std::string& function,
		const std::string& file, int line) : RuntimeException(message, function, file, line) {}

	static std::string Type() NOEXCEPT {
		return "cbr::TestAssertion";
	}
};

class BadFunctionInput : public RuntimeException
{
public:
	BadFunctionInput(const std::string& message, const std::string& function,
		const std::string& file, int line) : RuntimeException(message, function, file, line) {}

	static std::string Type() NOEXCEPT {
		return "cbr::BadFunctionInput";
	}
};

class BadProgramInput : public RuntimeException
{
public:
	BadProgramInput(const std::string& message, const std::string& function,
		const std::string& file, int line) : RuntimeException(message, function, file, line) {}

	static std::string Type() NOEXCEPT {
		return "cbr::BadProgramInput";
	}
};

class UnreachableCode : public RuntimeException
{
public:
	UnreachableCode(const std::string& message, const std::string& function,
		const std::string& file, int line) : RuntimeException(message, function, file, line) {}

	static std::string Type() NOEXCEPT {
		return "cbr::UnreachableCode";
	}
};

class IndexError : public RuntimeException
{
public:
	IndexError(const std::string& message, const std::string& function,
		const std::string& file, int line) : RuntimeException(message, function, file, line) {}

	static std::string Type() NOEXCEPT {
		return "cbr::IndexError";
	}
};

class SizeMismatch : public RuntimeException
{
public:
	SizeMismatch(const std::string& message, const std::string& function,
		const std::string& file, int line) : RuntimeException(message, function, file, line) {}

	static std::string Type() NOEXCEPT {
		return "cbr::SizeMismatch";
	}
};

class FileOperationFailure : public RuntimeException
{
public:
	FileOperationFailure(const std::string& message, const std::string& function,
		const std::string& file, int line) : RuntimeException(message, function, file, line) {}

	static std::string Type() NOEXCEPT {
		return "cbr::FileOperationFailure";
	}
};

class UnImplementedFeature : public RuntimeException
{
public:
	UnImplementedFeature(const std::string& message, const std::string& function,
		const std::string& file, int line) : RuntimeException(message, function, file, line) {}

	static std::string Type() NOEXCEPT {
		return "cbr::UnImplementedFeature";
	}
};


}   // namespace cbr