#pragma once

#include "./util/lists.h"

struct USSenateContactWebPageParser;
typedef struct USSenateContactWebPageParser * USSenateContactWebPageParser;
typedef struct USSenateContactWebPageParser const * ConstUSSenateContactWebPageParser;

USSenateContactWebPageParser USSenateContactWebPageParser_create(void);
void USSenateContactWebPageParser_destroy(USSenateContactWebPageParser parser);

StringList USSenateContactWebPageParser_parseContactAddresses(
    ConstUSSenateContactWebPageParser parser,
    char const *contactPageHtml
);
