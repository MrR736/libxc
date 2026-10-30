#include <iostream>
#include <atomic>
#include <chrono>
#include <thread>
#include <system_error>
#include <cstdlib>
#include <utility>

#include "xthread"

static int passed = 0;
static int failed = 0;

#define CHECK(condition, description) \
do { \
	if (condition) { \
		++passed; \
		std::cout << "[PASS] " << description << '\n'; \
	} else { \
		++failed; \
		std::cerr << "[FAIL] " << description << '\n'; \
	} \
} while (0)

template<class F>
bool throws_system_error(F&& fn)
{
	try {
		fn();
	} catch (const std::system_error&) {
		return true;
	}
	return false;
}

static void wait_until(std::atomic<bool>& flag)
{
	while (!flag.load()) {
		std::this_thread::yield();
	}
}

int main()
{
	typedef std::chrono::milliseconds milliseconds;

	std::cout << "=== xstd::xthread tests ===\n\n";

	// 1. Default construction
	{
		xstd::xthread t;

		CHECK(!t.joinable(), "Default thread is not joinable");
		CHECK(t.get_id() == xstd::xthread::id(),
			  "Default thread has an invalid ID");
	}

	// 2. Thread creation and execution
	{
		std::atomic<bool> executed(false);

		xstd::xthread t([&executed]() {
			executed.store(true);
		});

		CHECK(t.joinable(), "New thread is joinable");

		t.join();

		CHECK(executed.load(), "Thread function executes");
		CHECK(!t.joinable(), "Thread is not joinable after join");
		CHECK(t.get_id() == xstd::xthread::id(),
			  "Thread ID is invalid after join");
	}

	// 3. Argument passing
	{
		std::atomic<int> result(0);

		xstd::xthread t(
			[](std::atomic<int>* output, int a, int b) {
				output->store(a + b);
			},
			&result, 20, 22
		);

		t.join();

		CHECK(result.load() == 42,
			  "Arguments are passed correctly");
	}

	// 4. Thread ID
	{
		std::atomic<bool> started(false);
		std::atomic<bool> release(false);

		xstd::xthread t([&started, &release]() {
			started.store(true);

			while (!release.load()) {
				std::this_thread::yield();
			}
		});

		wait_until(started);

		xstd::xthread::id id = t.get_id();

		CHECK(id != xstd::xthread::id(),
			  "Running thread has a valid ID");
		CHECK(id == t.get_id(),
			  "Thread ID remains stable");

		release.store(true);
		t.join();
	}

	// 5. Joining a non-joinable thread
	{
		xstd::xthread t([]() {});
		t.join();

		CHECK(
			throws_system_error([&t]() { t.join(); }),
			  "Joining a non-joinable thread throws"
		);
	}

	// 6. Detach
	{
		std::atomic<bool> finished(false);

		xstd::xthread t([&finished]() {
			std::this_thread::sleep_for(milliseconds(10));
			finished.store(true);
		});

		t.detach();

		CHECK(!t.joinable(),
			  "Thread is not joinable after detach");

		CHECK(
			throws_system_error([&t]() { t.detach(); }),
			  "Detaching a non-joinable thread throws"
		);

		for (int i = 0; i < 1000 && !finished.load(); ++i) {
			std::this_thread::sleep_for(milliseconds(1));
		}

		CHECK(finished.load(),
			  "Detached thread completes");
	}

	// 7. Move construction
	{
		std::atomic<bool> executed(false);

		xstd::xthread original([&executed]() {
			executed.store(true);
		});

		xstd::xthread moved(std::move(original));

		CHECK(!original.joinable(),
			  "Moved-from thread is not joinable");
		CHECK(moved.joinable(),
			  "Move-constructed thread is joinable");

		moved.join();

		CHECK(executed.load(),
			  "Move-constructed thread executes correctly");
	}

	// 8. Move assignment
	{
		std::atomic<bool> first_done(false);
		std::atomic<bool> second_done(false);

		xstd::xthread first([&first_done]() {
			std::this_thread::sleep_for(milliseconds(10));
			first_done.store(true);
		});

		xstd::xthread second([&second_done]() {
			second_done.store(true);
		});

		first = std::move(second);

		CHECK(!second.joinable(),
			  "Move-assigned source is not joinable");
		CHECK(first.joinable(),
			  "Move-assigned destination is joinable");

		first.join();

		for (int i = 0; i < 1000 && !first_done.load(); ++i) {
			std::this_thread::sleep_for(milliseconds(1));
		}

		CHECK(first_done.load(),
			  "Previously detached thread completes");
		CHECK(second_done.load(),
			  "Move-assigned thread executes");
	}

	// 9. Member swap
	{
		std::atomic<int> value(0);

		xstd::xthread a([&value]() {
			value.fetch_add(1);
		});

		xstd::xthread b([&value]() {
			value.fetch_add(10);
		});

		a.swap(b);

		CHECK(a.joinable() && b.joinable(),
			  "Both threads remain joinable after swap");

		a.join();
		b.join();

		CHECK(value.load() == 11,
			  "Both swapped threads execute correctly");
	}

	// 10. Free swap
	{
		xstd::xthread a;
		xstd::xthread b([]() {});

		swap(a, b);

		CHECK(a.joinable(),
			  "Free swap transfers thread ownership");
		CHECK(!b.joinable(),
			  "Free swap leaves source non-joinable");

		a.join();
	}

	// 11. Hardware concurrency
	{
		unsigned count = xstd::xthread::hardware_concurrency();

		std::cout << "[INFO] Hardware concurrency: "
		<< count << '\n';

		// Zero is valid if the value cannot be determined.
		CHECK(true, "hardware_concurrency() is callable");
	}

	// Summary
	std::cout << "\n=== Test summary ===\n";
	std::cout << "Passed: " << passed << '\n';
	std::cout << "Failed: " << failed << '\n';

	return failed == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
