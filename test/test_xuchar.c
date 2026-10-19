#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include "xmemory.h"
#include "xuchar.h"

#define XU_JOIN2_I(a, b) a##b
#define XU_JOIN2(a, b) XU_JOIN2_I(a, b)

#define XU8(s)  s
#define XU16(s) u##s
#define XU32(s) U##s

#define XC8(c)  c
#define XC16(c) u##c
#define XC32(c) U##c

static unsigned long tests_run = 0;

#define CHECK(expr) do { \
++tests_run; \
if (!(expr)) { \
	printf("FAIL: %s:%d: %s\n",__FILE__,__LINE__,#expr); \
	abort(); \
} \
} while (0)

#define TEST_CHAR_API(PFX, TYPE, LIT, CH, ZERO) \
static void test_##PFX() { \
	typedef TYPE T; \
	/* Length */ \
	{ \
		const T empty[] = LIT(""); \
		const T abc[] = LIT("abc"); \
		CHECK(XU_JOIN2(PFX, len)(empty) == 0); \
		CHECK(XU_JOIN2(PFX, len)(abc) == 3); \
		CHECK(XU_JOIN2(PFX, nlen)(abc, 0) == 0); \
		CHECK(XU_JOIN2(PFX, nlen)(abc, 2) == 2); \
		CHECK(XU_JOIN2(PFX, nlen)(abc, 99) == 3); \
		CHECK(XU_JOIN2(PFX, len)(0) == 0); \
		CHECK(XU_JOIN2(PFX, nlen)(0, 99) == 0); \
	} \
	/* Copy */ \
	{ \
		T dst[16] = {}; \
		const T src[] = LIT("hello"); \
		CHECK(XU_JOIN2(PFX, cpy)(dst, src) == dst); \
		CHECK(XU_JOIN2(PFX, cmp)(dst, src) == 0); \
		T out[8] = {}; \
		CHECK(XU_JOIN2(PFX, ncpy)(out, src, 2) == out); \
		CHECK(out[0] == CH('h') && out[1] == CH('e')); \
		for (size_t i = 0; i < 7; ++i) out[i] = CH('x'); \
			XU_JOIN2(PFX, ncpy)(out, LIT("a"), 4); \
			CHECK(out[0] == CH('a')); \
			CHECK(out[1] == ZERO && out[2] == ZERO && out[3] == ZERO); \
			T unchanged[4] = LIT("abc"); \
			XU_JOIN2(PFX, ncpy)(unchanged, LIT("xyz"), 0); \
			CHECK(XU_JOIN2(PFX, cmp)(unchanged, LIT("abc")) == 0); \
	} \
	/* Concatenation */ \
	{ \
		T dst[16] = LIT("ab"); \
		CHECK(XU_JOIN2(PFX, cat)(dst, LIT("cd")) == dst); \
		CHECK(XU_JOIN2(PFX, cmp)(dst, LIT("abcd")) == 0); \
		T out[16] = LIT("ab"); \
		CHECK(XU_JOIN2(PFX, ncat)(out, LIT("cdef"), 2) == out); \
		CHECK(XU_JOIN2(PFX, cmp)(out, LIT("abcd")) == 0); \
		XU_JOIN2(PFX, ncat)(out, LIT("xyz"), 0); \
		CHECK(XU_JOIN2(PFX, cmp)(out, LIT("abcd")) == 0); \
	} \
	/* Comparisons and transformation */ \
	{ \
		CHECK(XU_JOIN2(PFX, cmp)(LIT("abc"), LIT("abc")) == 0); \
		CHECK(XU_JOIN2(PFX, cmp)(LIT("abc"), LIT("abd")) < 0); \
		CHECK(XU_JOIN2(PFX, cmp)(LIT("abd"), LIT("abc")) > 0); \
		CHECK(XU_JOIN2(PFX, ncmp)(LIT("abcX"), LIT("abcY"), 3) == 0); \
		CHECK(XU_JOIN2(PFX, ncmp)(LIT("abc"), LIT("abd"), 3) < 0); \
		CHECK(XU_JOIN2(PFX, coll)(LIT("abc"), LIT("abc")) == 0); \
		{ \
			T out[8] = {}; \
			CHECK(XU_JOIN2(PFX, xfrm)(out, LIT("hello"), 8) == 5); \
			CHECK(XU_JOIN2(PFX, cmp)(out, LIT("hello")) == 0); \
		} \
		{ \
			T out[4] = {}; \
			CHECK(XU_JOIN2(PFX, xfrm)(out, LIT("hello"), 4) == 5); \
			CHECK(out[0] == CH('h') && out[1] == CH('e')); \
			CHECK(out[2] == CH('l') && out[3] == ZERO); \
		} \
		CHECK(XU_JOIN2(PFX, xfrm)(0, LIT("hello"), 0) == 5); \
	} \
	/* Duplicate and bounded duplicate */ \
	{ \
		T *p = XU_JOIN2(PFX, dup)(LIT("hello")); \
		CHECK(p != 0); \
		CHECK(XU_JOIN2(PFX, cmp)(p, LIT("hello")) == 0); \
		xfree(p); \
		p = XU_JOIN2(PFX, dup)(LIT("")); \
		CHECK(p != 0 && p[0] == ZERO); \
		xfree(p); \
		p = XU_JOIN2(PFX, ndup)(LIT("hello"), 3); \
		CHECK(p != 0); \
		CHECK(XU_JOIN2(PFX, cmp)(p, LIT("hel")) == 0); \
		xfree(p); \
		p = XU_JOIN2(PFX, ndup)(LIT("hello"), 0); \
		CHECK(p != 0 && p[0] == ZERO); \
		xfree(p); \
		CHECK(XU_JOIN2(PFX, dup)(0) == 0); \
		CHECK(XU_JOIN2(PFX, ndup)(0, 3) == 0); \
	} \
	/* Character search */ \
	{ \
		const T s[] = LIT("banana"); \
		CHECK(XU_JOIN2(PFX, chr)(s, CH('n')) == s + 2); \
		CHECK(XU_JOIN2(PFX, chr)(s, CH('z')) == 0); \
		CHECK(XU_JOIN2(PFX, chr)(s, ZERO) == s + 6); \
		CHECK(XU_JOIN2(PFX, chrnul)(s, CH('n')) == s + 2); \
		CHECK(XU_JOIN2(PFX, chrnul)(s, CH('z')) == s + 6); \
	} \
	/* Spans and character-set search */ \
	{ \
		CHECK(XU_JOIN2(PFX, spn)(LIT("aaabxyz"), LIT("ab")) == 4); \
		CHECK(XU_JOIN2(PFX, cspn)(LIT("abc123"), LIT("0123456789")) == 3); \
		CHECK(XU_JOIN2(PFX, spn)(LIT("xyz"), LIT("abc")) == 0); \
		CHECK(XU_JOIN2(PFX, cspn)(LIT("abc"), LIT("")) == 3); \
		{ \
			const T s[] = LIT("hello"); \
			const T *p = XU_JOIN2(PFX, pbrk)(s, LIT("xyzol")); \
			CHECK(p == s + 2); \
			CHECK(XU_JOIN2(PFX, pbrk)(s, LIT("xyz")) == 0); \
		} \
	} \
	/* Substring search */ \
	{ \
		const T s[] = LIT("one two two"); \
		CHECK(XU_JOIN2(PFX, str)(s, LIT("two")) == s + 4); \
		CHECK(XU_JOIN2(PFX, str)(s, LIT("missing")) == 0); \
		CHECK(XU_JOIN2(PFX, str)(s, LIT("")) == s); \
		{ \
			const T mixed[] = LIT("AbCdEf"); \
			CHECK(XU_JOIN2(PFX, casestr)(mixed, LIT("cDe")) == mixed + 2); \
			CHECK(XU_JOIN2(PFX, casestr)(mixed, LIT("xyz")) == 0); \
		} \
	} \
	/* ASCII case-insensitive comparison */ \
	{ \
		CHECK(XU_JOIN2(PFX, casecmp)(LIT("AbC"), LIT("aBc")) == 0); \
		CHECK(XU_JOIN2(PFX, casecmp)(LIT("abc"), LIT("abd")) < 0); \
		CHECK(XU_JOIN2(PFX, ncasecmp)(LIT("AbCX"), LIT("aBcY"), 3) == 0); \
		CHECK(XU_JOIN2(PFX, ncasecmp)(LIT("AbC"), LIT("aBd"), 3) < 0); \
		CHECK(XU_JOIN2(PFX, ncasecmp)(LIT("abc"), LIT("xyz"), 0) == 0); \
	} \
	/* Reentrant tokenizer */ \
	{ \
		T input[] = LIT("red,,green;blue"); \
		const T delim[] = LIT(",;"); \
		T *save = 0; \
		T *p = XU_JOIN2(PFX, tok_r)(input, delim, &save); \
		CHECK(p != 0 && XU_JOIN2(PFX, cmp)(p, LIT("red")) == 0); \
		p = XU_JOIN2(PFX, tok_r)(0, delim, &save); \
		CHECK(p != 0 && XU_JOIN2(PFX, cmp)(p, LIT("green")) == 0); \
		p = XU_JOIN2(PFX, tok_r)(0, delim, &save); \
		CHECK(p != 0 && XU_JOIN2(PFX, cmp)(p, LIT("blue")) == 0); \
		p = XU_JOIN2(PFX, tok_r)(0, delim, &save); \
		CHECK(p == 0); \
	} \
	/* Non-reentrant tokenizer */ \
	{ \
		T input[] = LIT("a:b::c"); \
		T *p = XU_JOIN2(PFX, tok)(input, LIT(":")); \
		CHECK(p != 0 && XU_JOIN2(PFX, cmp)(p, LIT("a")) == 0); \
		p = XU_JOIN2(PFX, tok)(0, LIT(":")); \
		CHECK(p != 0 && XU_JOIN2(PFX, cmp)(p, LIT("b")) == 0); \
		p = XU_JOIN2(PFX, tok)(0, LIT(":")); \
		CHECK(p != 0 && XU_JOIN2(PFX, cmp)(p, LIT("c")) == 0); \
		p = XU_JOIN2(PFX, tok)(0, LIT(":")); \
		CHECK(p == 0); \
	} \
}

static void test_ustr8() {
	typedef char8_t T;

	static const T empty[] = {0};
	static const T abc[] = {'a', 'b', 'c', 0};
	static const T hello[] = {'h', 'e', 'l', 'l', 'o', 0};
	static const T a[] = {'a', 0};
	static const T xyz[] = {'x', 'y', 'z', 0};
	static const T banana[] = {'b', 'a', 'n', 'a', 'n', 'a', 0};

	CHECK(ustr8len(empty) == 0);
	CHECK(ustr8len(abc) == 3);
	CHECK(ustr8nlen(abc, 2) == 2);
	CHECK(ustr8nlen(abc, 99) == 3);

	T dst[16] = {};
	CHECK(ustr8cpy(dst, hello) == dst);
	CHECK(ustr8cmp(dst, hello) == 0);

	T out[8] = {};
	CHECK(ustr8ncpy(out, hello, 2) == out);
	CHECK(out[0] == 'h' && out[1] == 'e');

	for (size_t i = 0; i < 7; ++i) out[i] = 'x';

	CHECK(ustr8ncpy(out, a, 4) == out);
	CHECK(out[0] == 'a');
	CHECK(out[1] == 0 && out[2] == 0 && out[3] == 0);

	T unchanged[4] = {'a', 'b', 'c', 0};
	CHECK(ustr8ncpy(unchanged, xyz, 0) == unchanged);
	CHECK(ustr8cmp(unchanged, abc) == 0);

	CHECK(ustr8cmp(abc, abc) == 0);
	CHECK(ustr8cmp(abc, hello) < 0);

	CHECK(ustr8chr(banana, 'n') == banana + 2);
	CHECK(ustr8chr(banana, 'z') == 0);
	CHECK(ustr8chr(banana, 0) == banana + 6);
}

TEST_CHAR_API(ustr16, char16_t, XU16, XC16, u'\0')
TEST_CHAR_API(ustr32, char32_t, XU32, XC32, U'\0')


/* ---------------------------------------------------------
 * UTF-8 conversion tests
 * --------------------------------------------------------- */

static void test_utf8(void)
{
	puts("[UTF-8]");

	{
		const char8_t input[] = u8"Hello";
		const char expected[] = "Hello";
		size_t len = 999;

		char *result = xmbrtoc8_len(input, &len);

		CHECK(result != NULL);
		if (result) {
			CHECK(strcmp(result, expected) == 0);
			CHECK(len == strlen(expected));
			xfree(result);
		}
	}

	{
		const char8_t input[] = u8"Hello";
		const char expected[] = "Hello";

		char *result = xmbrtoc8(input);

		CHECK(result != NULL);
		if (result) {
			CHECK(strcmp(result, expected) == 0);
			xfree(result);
		}
	}

	{
		const char input[] = "Hello";
		const char8_t expected[] = u8"Hello";
		size_t len = 999;

		char8_t *result = xc8rtomb_len(input, &len);

		CHECK(result != NULL);
		if (result) {
			CHECK(ustr8cmp(result, expected) == 0);
			CHECK(len == ustr8len(expected));
			xfree(result);
		}
	}

	{
		const char input[] = "Hello";
		const char8_t expected[] = u8"Hello";

		char8_t *result = xc8rtomb(input);

		CHECK(result != NULL);
		if (result) {
			CHECK(ustr8cmp(result, expected) == 0);
			xfree(result);
		}
	}

	{
		const char8_t input[] = u8"Café 😀";
		const char expected[] =
		"Caf\xc3\xa9 \xf0\x9f\x98\x80";
		size_t len = 0;

		char *result = xmbrtoc8_len(input, &len);

		CHECK(result != NULL);
		if (result) {
			CHECK(strcmp(result, expected) == 0);
			CHECK(len == strlen(expected));
			xfree(result);
		}
	}

	{
		const char input[] = "Caf\xc3\xa9 \xf0\x9f\x98\x80";
		const char8_t expected[] = u8"Café 😀";
		size_t len = 0;

		char8_t *result = xc8rtomb_len(input, &len);

		CHECK(result != NULL);
		if (result) {
			CHECK(ustr8cmp(result, expected) == 0);
			CHECK(len == ustr8len(expected));
			xfree(result);
		}
	}
}

/* ---------------------------------------------------------
 * UTF-16 conversion tests
 * --------------------------------------------------------- */

static void test_utf16(void)
{
	puts("[UTF-16]");

	{
		const char16_t input[] = u"Hello";
		const char expected[] = "Hello";
		size_t len = 0;

		char *result = xmbrtoc16_len(input, &len);

		CHECK(result != NULL);
		if (result) {
			CHECK(strcmp(result, expected) == 0);
			CHECK(len == strlen(expected));
			xfree(result);
		}
	}

	{
		const char16_t input[] = u"Hello";
		const char expected[] = "Hello";

		char *result = xmbrtoc16(input);

		CHECK(result != NULL);
		if (result) {
			CHECK(strcmp(result, expected) == 0);
			xfree(result);
		}
	}

	{
		const char input[] = "Hello";
		const char16_t expected[] = u"Hello";
		size_t len = 0;

		char16_t *result = xc16rtomb_len(input, &len);

		CHECK(result != NULL);
		if (result) {
			CHECK(ustr16cmp(result, expected) == 0);
			CHECK(len == ustr16len(expected));
			xfree(result);
		}
	}

	{
		const char input[] = "Hello";
		const char16_t expected[] = u"Hello";

		char16_t *result = xc16rtomb(input);

		CHECK(result != NULL);
		if (result) {
			CHECK(ustr16cmp(result, expected) == 0);
			xfree(result);
		}
	}

	{
		const char16_t input[] = u"Hello \U0001F600";
		const char expected[] =
		"Hello \xf0\x9f\x98\x80";
		size_t len = 0;

		char *result = xmbrtoc16_len(input, &len);

		CHECK(result != NULL);
		if (result) {
			CHECK(strcmp(result, expected) == 0);
			CHECK(len == strlen(expected));
			xfree(result);
		}
	}

	{
		const char input[] = "Hello \xf0\x9f\x98\x80";
		const char16_t expected[] = u"Hello \U0001F600";
		size_t len = 0;

		char16_t *result = xc16rtomb_len(input, &len);

		CHECK(result != NULL);
		if (result) {
			CHECK(ustr16cmp(result, expected) == 0);
			CHECK(len == ustr16len(expected));
			xfree(result);
		}
	}
}

/* ---------------------------------------------------------
 * UTF-32 conversion tests
 * --------------------------------------------------------- */

static void test_utf32(void)
{
	puts("[UTF-32]");

	{
		const char32_t input[] = U"Hello";
		const char expected[] = "Hello";
		size_t len = 0;

		char *result = xmbrtoc32_len(input, &len);

		CHECK(result != NULL);
		if (result) {
			CHECK(strcmp(result, expected) == 0);
			CHECK(len == strlen(expected));
			xfree(result);
		}
	}

	{
		const char32_t input[] = U"Hello";
		const char expected[] = "Hello";

		char *result = xmbrtoc32(input);

		CHECK(result != NULL);
		if (result) {
			CHECK(strcmp(result, expected) == 0);
			xfree(result);
		}
	}

	{
		const char input[] = "Hello";
		const char32_t expected[] = U"Hello";
		size_t len = 0;

		char32_t *result = xc32rtomb_len(input, &len);

		CHECK(result != NULL);
		if (result) {
			CHECK(ustr32cmp(result, expected) == 0);
			CHECK(len == ustr32len(expected));
			xfree(result);
		}
	}

	{
		const char input[] = "Hello";
		const char32_t expected[] = U"Hello";

		char32_t *result = xc32rtomb(input);

		CHECK(result != NULL);
		if (result) {
			CHECK(ustr32cmp(result, expected) == 0);
			xfree(result);
		}
	}

	{
		const char32_t input[] = U"Hello \U0001F600";
		const char expected[] =
		"Hello \xf0\x9f\x98\x80";
		size_t len = 0;

		char *result = xmbrtoc32_len(input, &len);

		CHECK(result != NULL);
		if (result) {
			CHECK(strcmp(result, expected) == 0);
			CHECK(len == strlen(expected));
			xfree(result);
		}
	}

	{
		const char input[] = "Hello \xf0\x9f\x98\x80";
		const char32_t expected[] = U"Hello \U0001F600";
		size_t len = 0;

		char32_t *result = xc32rtomb_len(input, &len);

		CHECK(result != NULL);
		if (result) {
			CHECK(ustr32cmp(result, expected) == 0);
			CHECK(len == ustr32len(expected));
			xfree(result);
		}
	}
}

static void test_unicode_lengths() {
	/* UTF-8 bytes for U+00E9 LATIN SMALL LETTER E WITH ACUTE. */
	const char8_t utf8[] = "\xC3\xA9";
	/* U+1F600 GRINNING FACE: UTF-16 surrogate pair, one UTF-32 unit. */
	const char16_t utf16[] = u"\U0001F600";
	const char32_t utf32[] = U"\U0001F600";

	CHECK(ustr8len(utf8) == 2);
	CHECK(ustr16len(utf16) == 2);
	CHECK(ustr32len(utf32) == 1);
}

int main() {
	test_ustr8();
	test_utf8();
	test_ustr16();
	test_utf16();
	test_ustr32();
	test_utf32();
	test_unicode_lengths();

	printf("xuchar tests passed: %lu checks\n", tests_run);
	return 0;
}