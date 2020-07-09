//
// Created by core on 7/7/20.
//
#pragma once

#include <functional>
#include <tuple>
#include <ostream>
#include <variant>
#include <sstream>
#include <mutex>
#include <atomic>

namespace devfix::base
{
	struct stx_t {};
	static constexpr stx_t stx{};

	struct etx_t {};
	static constexpr etx_t etx{};

	template<typename CharT>
	struct slog
	{
		using ostream_t = std::basic_ostream<CharT>;
		using write_t = std::function<void(const std::basic_string<CharT>& str)>;
		using flush_t = std::function<void()>;

		explicit slog(slog* sl) { add_sink(sl); }

		explicit slog(ostream_t* ostream) { add_sink(ostream); }

		explicit slog(const write_t& write, const flush_t& flush) { add_sink(write, flush); }

		void add_sink(slog* sl) { sinks_.push_back({ sl }); }

		void add_sink(ostream_t* ostream)
		{
			sinks_.push_back({ std::make_pair<write_t, flush_t>(
				[ostream](const std::basic_string<CharT>& str) { ostream->write(str.data(), str.length()); },
				[ostream] { ostream->flush(); }) });
		}

		void add_sink(const write_t& write, const flush_t& flush) { sinks_.push_back({ std::make_pair(write, flush) }); }

		slog(const slog& sl) = delete;
		slog(slog&& sl) = delete;
		slog& operator=(const slog& sl) = delete;

		slog& operator<<(const stx_t&)
		{
			std::lock_guard<std::mutex> guard(mutex_);
			start_text();
			return *this;
		}

		slog& operator<<(const std::basic_string<CharT>& str)
		{
			std::lock_guard<std::mutex> guard(mutex_);
			write(str);
			return *this;
		}

		slog& operator<<(const etx_t&)
		{
			std::lock_guard<std::mutex> guard(mutex_);
			end_text();
			return *this;
		}

		void set_prefix(const std::basic_string<CharT>& prefix)
		{
			std::lock_guard<std::mutex> guard(mutex_);
			prefix_ = prefix;
		}

		[[nodiscard]] std::basic_string<CharT> get_prefix()
		{
			std::lock_guard<std::mutex> guard(mutex_);
			return prefix_;
		}

		void set_enabled(bool enabled) { enabled_ = enabled; }

		[[nodiscard]] bool is_enabled() { return enabled_; }

	protected:
		void start_text()
		{
			if (enabled_)
			{
				for (auto& sink : sinks_)
				{
					if (std::holds_alternative<slog*>(sink))
					{
						std::get<slog*>(sink)->start_text();
					}
					else if (std::holds_alternative<ostream_t*>(sink))
					{
						std::get<ostream_t*>(sink)->write(CLEAR_LINE, sizeof(CLEAR_LINE) / sizeof(CharT));
					}
					else
					{
						std::get<0>(std::get<std::pair<write_t, flush_t>>(sink))(CLEAR_LINE);
					}
				}
			}
		}

		void write(const std::basic_string<CharT>& str)
		{
			if (enabled_)
			{
				std::basic_istringstream<CharT> tokenStream(str);
				std::basic_string<CharT> line;
				std::size_t passed = 0;
				while (std::getline(tokenStream, line, CharT('\n')))
				{
					passed += line.length();
					line.insert(line.begin(), prefix_.begin(), prefix_.end());
					if (passed < str.length()) { line += CharT('\n'); }
					for (auto& sink : sinks_)
					{
						if (std::holds_alternative<slog*>(sink))
						{
							std::get<slog*>(sink)->write(line);
						}
						else if (std::holds_alternative<ostream_t*>(sink))
						{
							std::get<ostream_t*>(sink)->write(line.c_str(), line.length());
						}
						else
						{
							std::get<0>(std::get<std::pair<write_t, flush_t>>(sink))(line);
						}
					}
					passed++;
				}
			}
		}

		void end_text()
		{
			if (enabled_)
			{
				for (auto& sink : sinks_)
				{
					if (std::holds_alternative<slog*>(sink))
					{
						std::get<slog*>(sink)->end_text();
					}
					else if (std::holds_alternative<ostream_t*>(sink))
					{
						std::get<ostream_t*>(sink)->flush();
					}
					else
					{
						std::get<1>(std::get<std::pair<write_t, flush_t>>(sink))();
					}
				}
			}
		}

		static constexpr CharT CLEAR_LINE[] = { '\033', '[', '2', 'K', '\r' };

		static inline std::mutex mutex_;

		std::vector<std::variant<slog*, std::pair<write_t, flush_t>, ostream_t*>> sinks_;
		std::basic_string<CharT> prefix_;
		std::atomic_bool enabled_ = true;
	};

}