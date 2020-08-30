/*
 * Aidan Matheney
 * aidan.matheney@und.edu
 *
 * CSCI 451 HW1
 */

#include "../include/hw1.h"

#include <stdio.h>

int main(void) {
    HW1Result const hw1Result = hw1("http://www.senate.gov/general/contact_information/senators_cfm.cfm");
    if (!HW1Result_isSuccess(hw1Result)) {
        fprintf(
            stderr,
            "ERROR: Failed to download US Senate Contact web page HTML (wget exit code: %d)\n",
            HW1Result_getError(hw1Result)
        );
        HW1Result_destroy(hw1Result);
        return EXIT_FAILURE;
    }
    HW1Result_destroy(hw1Result);

    return EXIT_SUCCESS;
}
