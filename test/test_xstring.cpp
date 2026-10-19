#include <iostream>
#include <string>
#include <list>
#include <vector>

#include <xstring>

static void teststring(const std::string& r) {
	std::cout << "Func String: " << r << std::endl;
}

static void testwstring(const std::wstring& r) {
	std::wcout << L"Func WString: " << r << std::endl;
}

int main() {
	bool ok = true;

	std::cout << "*************************************" << std::endl;
	std::cout << "        libxcxx xstring Test" << std::endl;
	std::cout << "*************************************" << std::endl << std::endl;

	std::cout << "=== string tests ===" << std::endl;

	std::string str("hello char");

	std::cout << "String cout: " << str << std::endl;
	std::wcout << "String wcout: " << str << std::endl << std::endl;

	std::cout << "=== wstring tests ===" << std::endl;

	std::wstring wstr(L"hello wchar_t");

	std::cout << "WString cout: " << wstr << std::endl;
	std::wcout << "WString wcout: " << wstr << std::endl << std::endl;

	std::cout << "=== xstring tests ===" << std::endl;

	xstd::xstring s(L"ll LL lkk");

	teststring(s);

	std::cout << std::endl << "String cout: " << s << std::endl;
	std::wcout << L"to String wcout: " << xstd::to_xstring(999) << std::endl;
	std::wcout << L"String Wstr wcout: " << s << std::endl;
	std::cout << "String Upper cout: " << s.toupper() << std::endl;
	std::cout << "String Lower cout: " << s.tolower() << std::endl;

	if (s != "ll LL lkk") {
		std::cout << "FAIL: initial xstring value" << std::endl;
		ok = false;
	}

	const char *ss[] {"ll LL","ll LL lkk",nullptr};
	if (s.compares(ss, xsizeof(ss))) {
		std::cout << "FAIL: compares" << std::endl;
		ok = false;
	}

	std::vector<const char*> vss = {"ll LL","ll LL lkk",nullptr};
	if (s.compares(vss)) {
		std::cout << "FAIL: compares vector" << std::endl;
		ok = false;
	}

	std::list<const char*> lss = {"ll LL","ll LL lkk",nullptr};
	if (s.compares(lss)) {
		std::cout << "FAIL: compares list" << std::endl;
		ok = false;
	}

	s.append(" 8ss");

	std::cout << std::endl << "Append: \" 8ss\"" << std::endl;

	teststring(s);

	std::cout << std::endl << "String cout: " << s << std::endl;
	std::wcout << L"String Wstr wcout: " << s << std::endl;
	std::cout << "String Upper cout: " << s.toupper() << std::endl;
	std::cout << "String Lower cout: " << s.tolower() << std::endl;

	if (s != "ll LL lkk 8ss") {
		std::cout << "FAIL: xstring append" << std::endl;
		ok = false;
	}

	std::cout << std::endl << "=== xwstring tests ===" << std::endl;

	xstd::xwstring ws("ll LL lkk");

	testwstring(ws);

	std::wcout << std::endl << L"WString wcout: " << ws << std::endl;
	std::wcout << L"to WString wcout: " << xstd::to_xwstring(999) << std::endl;
	std::cout << "WString Cstr cout: " << ws << std::endl;
	std::wcout << L"WString Upper wcout: " << ws.toupper() << std::endl;
	std::wcout << L"WString Lower wcout: " << ws.tolower() << std::endl;

	if (ws != L"ll LL lkk") {
		std::cout << "FAIL: initial xwstring value" << std::endl;
		ok = false;
	}

	const wchar_t *wss[] {L"ll LL",L"ll LL lkk",nullptr};
    if (ws.compares(wss,xsizeof(wss))) {
		std::cout << "FAIL: compares" << std::endl;
		ok = false;
	}

	std::vector<const wchar_t*> vwss = {L"ll LL",L"ll LL lkk",nullptr};
	if (ws.compares(vwss)) {
		std::cout << "FAIL: compares vector" << std::endl;
		ok = false;
	}

	std::list<const wchar_t*> lwss = {L"ll LL",L"ll LL lkk",nullptr};
	if (ws.compares(lwss)) {
		std::cout << "FAIL: compares list" << std::endl;
		ok = false;
	}

	ws.append(L" 8ss");

	std::wcout << std::endl << L"Append: \" 8ss\"" << std::endl;

	testwstring(ws);

	std::wcout << std::endl << L"WString wcout: " << ws << std::endl;
	std::cout << "WString Cstr cout: " << ws << std::endl;
	std::wcout << L"WString Upper wcout: " << ws.toupper() << std::endl;
	std::wcout << L"WString Lower wcout: " << ws.tolower() << std::endl;

	if (ws != L"ll LL lkk 8ss") {
		std::cout << "FAIL: xwstring append" << std::endl;
		ok = false;
	}

	std::cout << std::endl;

	if (ok) {
		std::cout << "xstring: PASS" << std::endl;
		return 0;
	}

	std::cout << "xstring: FAIL" << std::endl;
	return 1;
}