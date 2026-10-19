#include <iostream>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <chrono>
#include <iomanip>
#include <cstddef>

#include "xmemory.h"
#include "xmap.h"

using xmap_clock = std::chrono::steady_clock;

class xmap_timer {
public:
	explicit xmap_timer(const char* name)
	: name_(name), start_(xmap_clock::now()) {}

	~xmap_timer() {
		const auto end = xmap_clock::now();
		const auto elapsed = end - start_;

		std::cout << "[TIMER] " << name_ << ": "
		<< std::fixed << std::setprecision(6)
		<< std::chrono::duration<double>(elapsed).count()
		<< " seconds ("
		<< std::chrono::duration_cast<
		std::chrono::milliseconds>(elapsed).count()
		<< " ms)\n";
	}

	xmap_timer(const xmap_timer&) = delete;
	xmap_timer& operator=(const xmap_timer&) = delete;

private:
	const char* name_;
	xmap_clock::time_point start_;
};

/*
 * Insert a string and detect failure by checking whether
 * the number of map entries increased.
 *
 * This assumes no other thread modifies xm concurrently.
 * xmap_strinsert duplicates the string, so the caller
 * retains ownership of the original buffer.
 */
static bool insert_string_checked(xmap_t* xm, const char* str) {
	const size_t before = xm->count;

	xmap_strinsert(xm, str);

	return xm->count == before + 1;
}

void test_xmap_basic_operations() {
	xmap_timer timer(__func__);

	xmap_t xm;
	xmap_init_mutex(&xm, XMAP_MUTEX_FALSE);

	std::cout << "\n--- Basic operations (mutex disabled) ---\n";

	xxmap_strinsert(&xm, std::string("Hello, world!"));
	xxmap_strinsert(&xm, std::string("Goodbye, world!"));

	std::cout << "Retrieved string 0: "
	<< xxmap_strget(&xm, 0) << '\n';
	std::cout << "Retrieved string 1: "
	<< xxmap_strget(&xm, 1) << '\n';

	xxmap_wcsinsert(&xm, std::wstring(L"Wide Hello!"));
	xxmap_wcsinsert(&xm, std::wstring(L"Wide Goodbye!"));

	std::wcout << L"Retrieved wide 0: "
	<< xxmap_wcsget(&xm, 0) << L'\n';
	std::wcout << L"Retrieved wide 1: "
	<< xxmap_wcsget(&xm, 1) << L'\n';

	xmap_strerase(&xm, 0);

	std::cout << "After narrow erase: "
	<< xxmap_strget(&xm, 0) << '\n';

	xmap_wcserase(&xm, 0);

	std::wcout << L"After wide erase: "
	<< xxmap_wcsget(&xm, 0) << L'\n';

	xmap_strerase_no_shift(&xm, 0);
	xmap_wcserase_no_shift(&xm, 0);

	std::cout << "After no-shift erase: count=" << xm.count
	<< " cstr=" << xm.cstr
	<< " cwstr=" << xm.cwstr << '\n';

	xmap_destroy(&xm);
}

void test_xmap_basic_operations_mutex() {
	xmap_timer timer(__func__);

	xmap_t xm;
	xmap_init_mutex(&xm, XMAP_MUTEX_TRUE);

	std::cout << "\n--- Basic operations (mutex enabled) ---\n";

	xxmap_strinsert(&xm, std::string("Hello, world!"));
	xxmap_strinsert(&xm, std::string("Goodbye, world!"));

	std::cout << "Retrieved string 0: "
	<< xxmap_strget(&xm, 0) << '\n';
	std::cout << "Retrieved string 1: "
	<< xxmap_strget(&xm, 1) << '\n';

	xxmap_wcsinsert(&xm, std::wstring(L"Wide Hello!"));
	xxmap_wcsinsert(&xm, std::wstring(L"Wide Goodbye!"));

	std::wcout << L"Retrieved wide 0: "
	<< xxmap_wcsget(&xm, 0) << L'\n';
	std::wcout << L"Retrieved wide 1: "
	<< xxmap_wcsget(&xm, 1) << L'\n';

	xmap_strerase(&xm, 0);

	std::cout << "After narrow erase: "
	<< xxmap_strget(&xm, 0) << '\n';

	xmap_wcserase(&xm, 0);

	std::wcout << L"After wide erase: "
	<< xxmap_wcsget(&xm, 0) << L'\n';

	xmap_strerase_no_shift(&xm, 0);
	xmap_wcserase_no_shift(&xm, 0);

	std::cout << "After no-shift erase: count=" << xm.count
	<< " cstr=" << xm.cstr
	<< " cwstr=" << xm.cwstr << '\n';

	xmap_destroy(&xm);
}

/*
 * Tests insertion under memory pressure without deliberately
 * requesting millions of entries by default.
 */
void test_memory_allocation_failure() {
	xmap_timer timer(__func__);

	xmap_t xm;
	xmap_init_mutex(&xm, XMAP_MUTEX_FALSE);

	constexpr size_t num_entries = 100000;

	std::cout << "\n--- Allocation test (mutex disabled) ---\n";

	size_t inserted = 0;

	for (size_t i = 0; i < num_entries; ++i) {
		char* buf = static_cast<char*>(std::malloc(64));

		if (!buf) {
			std::cout << "malloc failed at iteration "
			<< i << '\n';
			break;
		}

		std::snprintf(buf, 64, "String %zu", i);

		const bool success = insert_string_checked(&xm, buf);

		// xmap_strinsert duplicates the input string.
		// The original buffer remains ours to free.
		std::free(buf);

		if (!success) {
			std::cout << "xmap_strinsert failed at entry "
			<< i << '\n';
			break;
		}

		++inserted;
	}

	std::cout << "Successfully inserted: " << inserted << '\n';
	std::cout << "Map entry count: " << xm.count << '\n';

	xmap_destroy(&xm);
}

void test_memory_allocation_failure_mutex() {
	xmap_timer timer(__func__);

	xmap_t xm;
	xmap_init_mutex(&xm, XMAP_MUTEX_TRUE);

	constexpr size_t num_entries = 100000;

	std::cout << "\n--- Allocation test (mutex enabled) ---\n";

	size_t inserted = 0;

	for (size_t i = 0; i < num_entries; ++i) {
		char* buf = static_cast<char*>(std::malloc(64));

		if (!buf) {
			std::cout << "malloc failed at iteration "
			<< i << '\n';
			break;
		}

		std::snprintf(buf, 64, "String %zu", i);

		const bool success = insert_string_checked(&xm, buf);

		std::free(buf);

		if (!success) {
			std::cout << "xmap_strinsert failed at entry "
			<< i << '\n';
			break;
		}

		++inserted;
	}

	std::cout << "Successfully inserted: " << inserted << '\n';
	std::cout << "Map entry count: " << xm.count << '\n';

	xmap_destroy(&xm);
}

int main() {
	std::cout << "Running xmap tests...\n";

	test_xmap_basic_operations_mutex();
	test_xmap_basic_operations();

	test_memory_allocation_failure_mutex();
	test_memory_allocation_failure();

	std::cout << "\nAll tests completed.\n";

	return 0;
}