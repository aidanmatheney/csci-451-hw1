#include "../../include/util/regex.h"

#include "../../include/util/memory.h"
#include "../../include/util/error.h"

#include <regex.h>

/**
 * Compile the given regular expression pattern and flags into a regex object using regcomp. If the compilation fails,
 * abort the program with an error message.
 *
 * @param pattern The regular expression pattern.
 * @param flags The regular expression flags.
 * @param callerDescription A description of the caller to be included in the error message. This could be the name of
 * the calling function, plus extra information if useful.
 *
 * @returns The compiled regex object. The caller is responsible for freeing this memory.
 */
regex_t *safeRegcomp(char const * const pattern, int const flags, char const * const callerDescription) {
    regex_t * const regex = safeMalloc(sizeof *regex, callerDescription);
    int const regcompResult = regcomp(regex, pattern, flags);
    if (regcompResult != 0) {
        abortWithErrorFmt(
            "%s: Failed to compile regular expression /%s/%d using regcomp (error: %d)",
            callerDescription,
            pattern,
            flags,
            regcompResult
        );
        return NULL;
    }

    return regex;
}