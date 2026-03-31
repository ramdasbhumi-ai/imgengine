

/* src/cold/error.c */
#include "error.h"
#include <stdio.h>

const char* img_error_to_string(img_result_t res) {
    switch(res) {
        case IMG_SUCCESS:    return "Success";
        case IMG_ERR_NOMEM:  return "Slab Exhausted (Backpressure)";
        case IMG_ERR_FORMAT: return "Invalid/Malicious Image Format";
        default:             return "Unknown Internal Error";
    }
}
