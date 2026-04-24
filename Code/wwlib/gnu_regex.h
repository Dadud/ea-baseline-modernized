/*
** gnu_regex.h - GNU regex compatibility stubs for Windows/MSVC
**
** This provides minimal GNU regex API stubs so regexpr.cpp compiles on Windows.
** Regex functionality is stubbed out - regex operations will always fail/match nothing.
*/

#ifndef GNU_REGEX_H
#define GNU_REGEX_H

#include <stddef.h>

/* Syntax options (only what's used by regexpr.cpp) */
#define RE_CHAR_CLASSES           (1 << 0)
#define RE_CONTEXT_INDEP_ANCHORS  (1 << 1)
#define RE_CONTEXT_INDEP_OPS      (1 << 2)
#define RE_CONTEXT_INVALID_OPS    (1 << 3)
#define RE_INTERVALS              (1 << 4)
#define RE_NO_BK_BRACES           (1 << 5)
#define RE_NO_BK_PARENS           (1 << 6)
#define RE_NO_BK_VBAR             (1 << 7)
#define RE_NO_EMPTY_RANGES        (1 << 8)

typedef int reg_syntax_t;

/* Forward declarations */
struct re_pattern_buffer
{
	char padding[256];  // Stub - size doesn't matter, we zero it
};

typedef struct re_pattern_buffer regex_t;

struct re_registers
{
	unsigned start[10];
	unsigned end[10];
};

/* Stub function declarations */
#ifdef __cplusplus
extern "C" {
#endif

reg_syntax_t re_set_syntax(reg_syntax_t syntax);
const char *re_compile_pattern(const char *pattern, size_t length, struct re_pattern_buffer *buffer);
int re_match(const struct re_pattern_buffer *buffer, const char *string, size_t size, size_t match_start, struct re_registers *regs);
void regfree(struct re_pattern_buffer *buffer);

#ifdef __cplusplus
}
#endif

#endif // GNU_REGEX_H
