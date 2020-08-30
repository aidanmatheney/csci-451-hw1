#include "../include/USSenateContactWebPageParser.h"

#include "../include/util/string.h"
#include "../include/util/lists.h"
#include "../include/util/regex.h"
#include "../include/util/memory.h"
#include "../include/util/guard.h"

#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <regex.h>

/**
 * Represents an object capable of parsing the US Senate Contact web page
 * (https://www.senate.gov/general/contact_information/senators_cfm.cfm).
 */
struct USSenateContactWebPageParser {
    regex_t *contactAddressRegex;
};


/**
 * Create a USSenateContactWebPageParser.
 *
 * @returns The newly allocated USSenateContactWebPageParser. The caller is responsible for freeing this memory.
 */
USSenateContactWebPageParser USSenateContactWebPageParser_create(void) {
    USSenateContactWebPageParser const parser = safeMalloc(sizeof *parser, "USSenateContactWebPageParser_create");

    parser->contactAddressRegex = safeRegcomp(
        // Contact:\s*<a[^>]*\shref\s*=\s*"\s*([^"]+?)\s*"
        "Contact:\\s*<a[^>]*\\shref\\s*=\\s*\"\\s*([^\"]+?)\\s*\"",
        REG_EXTENDED,
        "USSenateContactWebPageParser_create"
    );

    return parser;
}


/**
 * Free the memory associated with the USSenateContactWebPageParser.
 *
 * @param builder The USSenateContactWebPageParser instance.
 */
void USSenateContactWebPageParser_destroy(USSenateContactWebPageParser const parser) {
    guardNotNull(parser, "parser", "USSenateContactWebPageParser_destroy");

    regfree(parser->contactAddressRegex);
    free(parser);
}


/**
 * Parses senator contact web addresses from the given US Senate Contact web page HTML.
 *
 * @param parser The USSenateContactWebPageParser instance.
 * @param contactPageHtml The US Senate Contact web page HTML source.
 *
 * @returns A list of the addresses. The caller is responsible for freeing both the strings and the list.
 */
StringList USSenateContactWebPageParser_parseContactAddresses(
    ConstUSSenateContactWebPageParser const parser,
    char const * const contactPageHtml
) {
    StringList const addresses = StringList_create();

    char const * unparsedContactPageHtml = contactPageHtml;
    regmatch_t regexMatch[2];
    while (regexec(parser->contactAddressRegex, unparsedContactPageHtml, 2, regexMatch, 0) == 0) {
        regoff_t const matchEndOffset = regexMatch[0].rm_eo;

        regoff_t const addressStartOffset = regexMatch[1].rm_so;
        regoff_t const addressEndOffset = regexMatch[1].rm_eo;

        size_t const addressLength = (size_t)(addressEndOffset - addressStartOffset);
        char * const address = safeMalloc(
            sizeof *address * (addressLength + 1),
            "USSenateContactWebPageParser_parseContactAddresses"
        );
        strncpy(address, unparsedContactPageHtml + addressStartOffset, addressLength);
        address[addressLength] = '\0';

        StringList_add(addresses, address);

        unparsedContactPageHtml += matchEndOffset;
    }

    return addresses;
}
