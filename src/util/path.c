#include "../include/util/path.h"

#include "../include/util/memory.h"
#include "../include/util/error.h"

#include <stdio.h>
#include <string.h>
#include <errno.h>

/**
 * Generates a unique file path that does not name a currently existing file.
 *
 * @returns The generated file path. The caller is responsible for freeing this memory.
 */
char *generateTempFilePath(void) {
    char * const tempPath = safeMalloc(sizeof *tempPath * L_tmpnam, "generateTempFilePath");
    char * const tmpnamResult = tmpnam(tempPath);
    if (tmpnamResult == NULL) {
        free(tempPath);

        int const tmpnamErrorCode = errno;
        char const * const tmpnamErrorMessage = strerror(tmpnamErrorCode);

        abortWithErrorFmt(
            "generateTempFilePath: Failed to generate a temporary file path using tmpnam (error code: %d; error message: \"%s\")",
            tmpnamErrorCode,
            tmpnamErrorMessage
        );
        return NULL;
    }

    return tempPath;
}
