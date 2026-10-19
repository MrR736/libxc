#include "xstring_view"

#include <iostream>
#if CPLUSPLUS_GE(STDC17PP)
#include <cassert>
#include <cstring>
#include <string>
#include <vector>
#include <list>

static void test_default() {
	xstd::xstring_view v;
	assert(v.empty());
	assert(v.size() == 0);
	assert(v.data() == nullptr);
}

static void test_char_string() {
	const char* text = "hello";
	xstd::xstring_view v(text);
	assert(!v.empty());
	assert(v.size() == 5);
	assert(v == "hello");
	assert(v.c_cstr() != nullptr);
	assert(std::strcmp(v.c_cstr(), "hello") == 0);
}

static void test_std_string() {
	std::string s = "hello world";
	xstd::xstring_view v(s);
	assert(v.size() == s.size());
	assert(v == s);
}

static void test_xstring() {
	xstd::xstring s = "hello";
	xstd::xstring_view v(s);
	assert(v == "hello");
	assert(v.size() == 5);
}

static void test_wide_view() {
	const wchar_t* text = L"hello";
	xstd::xwstring_view v(text);
	assert(!v.empty());
	assert(v.size() == 5);
	assert(v == L"hello");
	assert(std::wcscmp(v.c_wstr(), L"hello") == 0);
}

static void test_std_wstring() {
	std::wstring s = L"hello world";
	xstd::xwstring_view v(s);
	assert(v.size() == s.size());
	assert(v == s);
}

static void test_nullptr() {
	xstd::xstring_view a(nullptr);
	xstd::xwstring_view b(nullptr);
	assert(a.empty());
	assert(b.empty());
}

static void test_empty() {
	xstd::xstring_view a("");
	xstd::xwstring_view b(L"");
	assert(a.empty());
	assert(b.empty());
}

static void test_str() {
	xstd::xstring_view v("hello");
	xstd::xstring s = v.str();
	assert(s == "hello");
}

static void test_wstr() {
	xstd::xwstring_view v(L"hello");
	xstd::xwstring s = v.wstr();
	assert(s == L"hello");
}

static void test_cross_conversion() {
	xstd::xstring_view a("hello");
	xstd::xwstring ws = a.wstr();
	assert(ws == L"hello");
	xstd::xwstring_view b(ws);
	xstd::xstring s = b.str();
	assert(s == "hello");
}

static void test_equals_any_pointer_array() {
	xstd::xstring_view v("hello");
	const char* values[] = {"foo","bar","hello","world"};
	assert(v.equals_any(values, 4));
}

static void test_equals_any_vector() {
	xstd::xstring_view v("hello");
	std::vector<const char*> values = {"foo","hello"};
	assert(v.equals_any(values));
}

static void test_equals_any_list() {
	xstd::xwstring_view v(L"hello");
	std::list<const wchar_t*> values = {L"foo",L"bar",L"hello"};
	assert(v.equals_any(values));
}

static void test_equals_any_not_found() {
	xstd::xstring_view v("hello");
	std::vector<const char*> values = {"foo","bar","world"};
	assert(!v.equals_any(values));
}

static void test_equals_any_null() {
	xstd::xstring_view v("");
	const char* values[] = {"hello",nullptr,"world"};
	assert(v.equals_any(values, 3));
}

static void test_equals_any_null_non_empty() {
	xstd::xstring_view v("hello");
	const char* values[] = {nullptr,"world"};
	assert(!v.equals_any(values, 2));
}

static void test_zero_count() {
	xstd::xstring_view v("hello");
	const char* values[] = {"hello"};
	assert(!v.equals_any(values, 0));
}

static void test_null_array() {
	xstd::xstring_view v("hello");
	assert(!v.equals_any(nullptr, 0));
}

static void test_cout() {
	std::string_view sv("hello char");
	std::cout << "string_view cout: " << sv << std::endl;
	std::wcout << L"string_view wcout: " << sv << std::endl << std::endl;

	std::cout << "string_view to xstring_view.wview() cout: " << xstd::xstring_view(sv).wview() << std::endl;
	std::wcout << L"string_view to xstring_view.wview() wcout: " << xstd::xstring_view(sv).wview() << std::endl << std::endl;

	std::wstring_view wwv(L"hello wchar_t");
	std::cout << std::endl << "wstring_view cout: " << wwv << std::endl;
	std::wcout << L"wstring_view wcout: " << wwv << std::endl << std::endl;

	std::cout << "wstring_view to xwstring_view.cview() cout: " << xstd::xwstring_view(wwv).cview() << std::endl;
	std::wcout << L"wstring_view to xwstring_view.cview() wcout: " << xstd::xwstring_view(wwv).cview() << std::endl << std::endl;

	xstd::xstring_view v(L"hello char");
	std::cout << "xstring_view cout: " << v << std::endl;
	std::wcout << L"xstring_view wcout: " << v << std::endl << std::endl;

	std::cout << "xstring_view.view() cout: " << v.view() << std::endl;
	std::cout << "xstring_view.wview() cout: " << v.wview() << std::endl << std::endl;

	std::wcout << L"xstring_view.view() wcout: " << v.view() << std::endl;
	std::wcout << L"xstring_view.wview() wcout: " << v.wview() << std::endl << std::endl;

	std::wcout << L"xstring_view.str() wcout: " << v.str() << std::endl;
	std::wcout << L"xstring_view.cstr() wcout: " << v.wstr() << std::endl << std::endl;

	xstd::xwstring_view wv("hello wchar_t");
	std::cout << "xwstring_view cout: " << wv << std::endl;
	std::wcout << L"xwstring_view wcout: " << wv << std::endl << std::endl;

	std::cout << "xwstring_view.view() cout: " << wv.view() << std::endl;
	std::cout << "xwstring_view.cview() cout: " << wv.cview() << std::endl << std::endl;

	std::wcout << L"xwstring_view.view() wcout: " << wv.view() << std::endl;
	std::wcout << L"xwstring_view.cview() wcout: " << wv.cview() << std::endl << std::endl;

	std::wcout << L"xwstring_view.str() wcout: " << wv.str() << std::endl;
	std::wcout << L"xwstring_view.cstr() wcout: " << wv.cstr() << std::endl;
}
#endif

int main() {
#if CPLUSPLUS_GE(STDC17PP)
	test_default();
	test_char_string();
	test_std_string();
	test_xstring();

	test_wide_view();
	test_std_wstring();

	test_nullptr();
	test_empty();

	test_str();
	test_wstr();
	test_cross_conversion();

	test_equals_any_pointer_array();
	test_equals_any_vector();
	test_equals_any_list();
	test_equals_any_not_found();

	test_equals_any_null();
	test_equals_any_null_non_empty();
	test_zero_count();
	test_null_array();
	test_cout();
#else
	std::cout << "C++17 or newer is required" << std::endl;
#endif
	return 0;
}