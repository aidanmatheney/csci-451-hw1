#include "../include/hw1.h"

#include "../include/util/result.h"
#include "../include/util/list.h"
#include "../include/util/guard.h"
#include "../include/util/file.h"
#include "../include/util/wget.h"

#include "../include/USSenateContactWebPageParser.h"

#include <stdlib.h>
#include <stdio.h>

static void printContactAddressForEachCallback(void *state, size_t index, char *contactAddress);
static void freeContactAddressForEachCallback(void *state, size_t index, char *contactAddress);

DEFINE_VOID_RESULT(HW1Result, int)

/**
 * Run CSCI 451 HW1. This downloads the US Senate Contact web page, parses the senator contact web addresses, and then
 * prints them to stdout.
 *
 * @param contactPageUrl The US Senate Contact web page URL.
 *
 * @returns A result where failure indicates a wget error. The failure error contains the wget exit code. The caller is
 *          is responsible for freeing the result.
 */
HW1Result hw1(char const * const contactPageUrl) {
    guardNotNull(contactPageUrl, "contactPageUrl", "hw1");

    WgetGetStringResult wgetResult = wgetGetString(contactPageUrl);
    if (!WgetGetStringResult_isSuccess(wgetResult)) {
        return HW1Result_failure(WgetGetStringResult_getErrorAndDestroy(wgetResult));
    }
    char * const contactPageHtml = WgetGetStringResult_getValueAndDestroy(wgetResult);

    USSenateContactWebPageParser const contactPageParser = USSenateContactWebPageParser_create();
    StringList const contactAddresses = USSenateContactWebPageParser_parseContactAddresses(
        contactPageParser,
        contactPageHtml
    );
    USSenateContactWebPageParser_destroy(contactPageParser);

    free(contactPageHtml);

    StringList_forEach(contactAddresses, NULL, printContactAddressForEachCallback);
    StringList_forEach(contactAddresses, NULL, freeContactAddressForEachCallback);

    StringList_destroy(contactAddresses);

    return HW1Result_success();
}

static void printContactAddressForEachCallback(void * const state, size_t const index, char * const contactAddress) {
    puts(contactAddress);
}

static void freeContactAddressForEachCallback(void * const state, size_t const index, char * const contactAddress) {
    free(contactAddress);
}
