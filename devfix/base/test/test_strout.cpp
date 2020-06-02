//
// Created by core on 6/2/20.
//

#if ENABLE_GOOGLETEST == 1

#include <gtest/gtest.h>
#include "../strout.h"

using namespace devfix::base;

TEST(TestStrOut, BufferBehavior)
{
	// prolog
	std::stringstream buf;
	strout<char> str_out(buf);
	std::ostream os(&str_out);
	str_out.set_prefix("[prefix] ");

	os << "test message.";
	buf.flush();
	ASSERT_STREQ(buf.str().c_str(), "");

	os << std::flush;
	buf.flush();
	ASSERT_STREQ(buf.str().c_str(), "[prefix] test message.");

	buf.str(""); // clear buffer
	os << " more content, without prefix now and newline" << std::endl;
	ASSERT_STREQ(buf.str().c_str(), " more content, without prefix now and newline\n");

	buf.str(""); // clear buffer
	os << "oneline message with differend newline\n";
	ASSERT_STREQ(buf.str().c_str(), "[prefix] oneline message with differend newline\n");
}

TEST(TestStrOut, BufferBehaviorWchar)
{
	// prolog
	std::wstringstream buf;
	strout<wchar_t> str_out(buf);
	std::wostream os(&str_out);
	str_out.set_prefix(L"[prefix] ");

	os << L"test message.";
	buf.flush();
	ASSERT_STREQ(buf.str().c_str(), L"");

	os << std::flush;
	buf.flush();
	ASSERT_STREQ(buf.str().c_str(), L"[prefix] test message.");

	buf.str(L""); // clear buffer
	os << " more content, without prefix now and newline" << std::endl;
	ASSERT_STREQ(buf.str().c_str(), L" more content, without prefix now and newline\n");

	buf.str(L""); // clear buffer
	os << L"oneline message with differend newline\n";
	ASSERT_STREQ(buf.str().c_str(), L"[prefix] oneline message with differend newline\n");
}

TEST(TestStrOut, EnableDisable)
{
	// prolog
	std::stringstream buf;
	strout<char> str_out(buf);
	std::ostream os(&str_out);
	str_out.set_enabled(false);

	os << "test message.";
	os << std::flush;
	os << " more content, without prefix now and newline" << std::endl;
	os << "oneline message with differend newline\n";
	ASSERT_STREQ(buf.str().c_str(), ""); // should be empty since disabled


	str_out.set_enabled(true);
	os << "new content, now enabled" << std::flush;
	ASSERT_STREQ(buf.str().c_str(), "new content, now enabled"); // now new content
}

TEST(TestStrOut, EnableDisableWchar)
{
	// prolog
	std::wstringstream buf;
	strout<wchar_t> str_out(buf);
	std::wostream os(&str_out);
	str_out.set_enabled(false);

	os << L"test message.";
	os << std::flush;
	os << L" more content, without prefix now and newline" << std::endl;
	os << L"oneline message with differend newline\n";
	ASSERT_STREQ(buf.str().c_str(), L""); // should be empty since disabled


	str_out.set_enabled(true);
	os << L"new content, now enabled" << std::flush;
	ASSERT_STREQ(buf.str().c_str(), L"new content, now enabled"); // now new content
}

TEST(TestStrOut, StartOfText)
{
	// prolog
	std::stringstream buf;
	strout<char> str_out(buf);
	std::ostream os(&str_out);

	os << "first";
	os << decltype(str_out)::STX << "second";
	os << std::flush;
	ASSERT_STREQ(buf.str().c_str(), "\033[2K\rsecond"); // first message got dropped

	buf.str(""); // clear buffer
	os << "first";
	os << std::flush;
	os << decltype(str_out)::STX << "second";
	os << std::flush;
	ASSERT_STREQ(buf.str().c_str(), "first\033[2K\rsecond"); // first message gets clear by escape sequences
}

TEST(TestStrOut, StartOfTextWchar)
{
	// prolog
	std::wstringstream buf;
	strout<wchar_t> str_out(buf);
	std::wostream os(&str_out);

	os << L"first";
	os << decltype(str_out)::STX << L"second";
	os << std::flush;
	ASSERT_STREQ(buf.str().c_str(), L"\033[2K\rsecond"); // first message got dropped

	buf.str(L""); // clear buffer
	os << L"first";
	os << std::flush;
	os << decltype(str_out)::STX << L"second";
	os << std::flush;
	ASSERT_STREQ(buf.str().c_str(), L"first\033[2K\rsecond"); // first message gets clear by escape sequences
}


#endif