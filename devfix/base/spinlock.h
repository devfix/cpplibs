//
// Created by Tristan Krause on 10.07.2020.
//

#pragma once

#include <atomic>

namespace devfix::base
{
	struct spinlock
	{
		void lock() { while (locked.test_and_set(std::memory_order_acquire)) {}}

		void unlock() { locked.clear(std::memory_order_release); }

	private:
		std::atomic_flag locked = ATOMIC_FLAG_INIT;
	};

	struct spinlock_guard
	{
		spinlock_guard(spinlock& sl) : spinlock_(sl) { spinlock_.lock(); }

		~spinlock_guard() { spinlock_.unlock(); }

	private:
		spinlock& spinlock_;

	};
}
