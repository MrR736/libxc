#include <iostream>
#include <string>
#include <cstdlib>
#include "xmap.h"

void test_xmap_basic_operations() {
	xmap_t xm;
	xmap_init(&xm);

	// Insert narrow strings
	xxmap_strinsert(&xm, std::string("Hello, world!"));
	xxmap_strinsert(&xm, std::string("Goodbye, world!"));

	std::cout << "Retrieved string 0: " << xxmap_strget(&xm, 0) << "\n";
	std::cout << "Retrieved string 1: " << xxmap_strget(&xm, 1) << "\n";

	// Insert wide strings
	xxmap_wcsinsert(&xm, std::wstring(L"Wide Hello!"));
	xxmap_wcsinsert(&xm, std::wstring(L"Wide Goodbye!"));

	std::wcout << L"Retrieved wide 0: " << xxmap_wcsget(&xm, 0) << L"\n";
	std::wcout << L"Retrieved wide 1: " << xxmap_wcsget(&xm, 1) << L"\n";

	// Erase first narrow string
	xmap_strerase(&xm, 0);

	// After erase: narrow indices shift
	std::cout << "After erase, string[0] (was string[1]): "
			  << xxmap_strget(&xm, 0) << "\n";

	// Erase first wide string
	xmap_wcserase(&xm, 0);

	// After erase: wide indices shift
	std::wcout << L"After erase, wcs[0] (was wcs[1]): "
			   << xxmap_wcsget(&xm, 0) << L"\n";

	// No-shift erases
	xmap_strerase_no_shift(&xm, 0);
	xmap_wcserase_no_shift(&xm, 0);

	std::cout << "After no-shift erase: count=" << xm.count
			  << " cstr=" << xm.cstr
			  << " cwstr=" << xm.cwstr << "\n";

	xmap_destroy(&xm);
}

void test_memory_allocation_failure() {
	xmap_t xm;
	xmap_init(&xm);

	const int num_entries = 10000000;

	for (int i = 0; i < num_entries; i++) {

		char* buf = (char*)malloc(64);
		if (!buf) {
			std::cout << "Memory allocation failed at iteration " << i << "\n";
			break;
		}

		snprintf(buf, 64, "String %d", i);

		// Your map takes ownership → DO NOT free buf after insert
		if (!xxmap_strinsert_safe(&xm, buf)) {
			// insert failed, so free the buffer
			free(buf);
			std::cout << "xmap_strinsert failed at entry " << i << "\n";
			break;
		}

		// On success, ownership transferred → buffer must NOT be freed here
	}

	std::cout << "After memory failure, total stored entries: "
			  << xm.count << "\n";

	xmap_destroy(&xm);
}

int main() {
	std::cout << "Running basic xmap tests...\n";
	test_xmap_basic_operations();

	std::cout << "\nRunning memory allocation failure test...\n";
	test_memory_allocation_failure();

	return 0;
}
