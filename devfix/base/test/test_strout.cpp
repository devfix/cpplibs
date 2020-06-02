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

#endif