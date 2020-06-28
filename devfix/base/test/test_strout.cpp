//
// Created by core on 6/2/20.
//

#if CPPLIBS_ENABLE_TESTS == 1

#include <catch/catch.hpp>
#include "../strout.h"

using namespace devfix::base;

TEST_CASE("StrOut - BufferBehavior")
{
	// prolog
	std::stringstream buf;
	strout<char> str_out(&buf);
	std::ostream os(&str_out);
	str_out.set_prefix("[prefix] ");

	os << "test message.";
	buf.flush();
	REQUIRE(buf.str().empty());

	os << std::flush;
	buf.flush();
	REQUIRE(buf.str() == "[prefix] test message.");

	buf.str(""); // clear buffer
	os << " more content, without prefix now and newline" << std::endl;
	REQUIRE(buf.str() == " more content, without prefix now and newline\n");

	buf.str(""); // clear buffer
	os << "oneline message with differend newline\n";
	REQUIRE(buf.str() == "[prefix] oneline message with differend newline\n");
}

TEST_CASE("StrOut - BufferBehaviorWchar")
{
	// prolog
	std::wstringstream buf;
	strout<wchar_t> str_out(&buf);
	std::wostream os(&str_out);
	str_out.set_prefix(L"[prefix] ");

	os << L"test message.";
	buf.flush();
	REQUIRE(buf.str().empty());

	os << std::flush;
	buf.flush();
	REQUIRE(buf.str() == L"[prefix] test message.");

	buf.str(L""); // clear buffer
	os << " more content, without prefix now and newline" << std::endl;
	REQUIRE(buf.str() == L" more content, without prefix now and newline\n");

	buf.str(L""); // clear buffer
	os << L"oneline message with differend newline\n";
	REQUIRE(buf.str() == L"[prefix] oneline message with differend newline\n");
}

TEST_CASE("StrOut - EnableDisable")
{
	// prolog
	std::stringstream buf;
	strout<char> str_out(&buf);
	std::ostream os(&str_out);
	str_out.set_enabled(false);

	os << "test message.";
	os << std::flush;
	os << " more content, without prefix now and newline" << std::endl;
	os << "oneline message with differend newline\n";
	REQUIRE(buf.str().empty()); // should be empty since disabled


	str_out.set_enabled(true);
	os << "new content, now enabled" << std::flush;
	REQUIRE(buf.str() == "new content, now enabled"); // now new content
}

TEST_CASE("StrOut - EnableDisableWchar")
{
	// prolog
	std::wstringstream buf;
	strout<wchar_t> str_out(&buf);
	std::wostream os(&str_out);
	str_out.set_enabled(false);

	os << L"test message.";
	os << std::flush;
	os << L" more content, without prefix now and newline" << std::endl;
	os << L"oneline message with differend newline\n";
	REQUIRE(buf.str().empty()); // should be empty since disabled


	str_out.set_enabled(true);
	os << L"new content, now enabled" << std::flush;
	REQUIRE(buf.str() == L"new content, now enabled"); // now new content
}

TEST_CASE("StrOut - StartOfText")
{
	// prolog
	std::stringstream buf;
	strout<char> str_out(&buf);
	std::ostream os(&str_out);

	os << "first";
	os << decltype(str_out)::STX << "second";
	os << std::flush;
	REQUIRE(buf.str() == "\033[2K\rsecond"); // first message got dropped

	buf.str(""); // clear buffer
	os << "first";
	os << std::flush;
	os << decltype(str_out)::STX << "second";
	os << std::flush;
	REQUIRE(buf.str() == "first\033[2K\rsecond"); // first message gets clear by escape sequences
}

TEST_CASE("StrOut - StartOfTextWchar")
{
	// prolog
	std::wstringstream buf;
	strout<wchar_t> str_out(&buf);
	std::wostream os(&str_out);

	os << L"first";
	os << decltype(str_out)::STX << L"second";
	os << std::flush;
	REQUIRE(buf.str() == L"\033[2K\rsecond"); // first message got dropped

	buf.str(L""); // clear buffer
	os << L"first";
	os << std::flush;
	os << decltype(str_out)::STX << L"second";
	os << std::flush;
	REQUIRE(buf.str() == L"first\033[2K\rsecond"); // first message gets clear by escape sequences
}


#endif