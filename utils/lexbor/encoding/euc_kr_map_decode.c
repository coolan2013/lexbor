/*
 * Copyright (C) 2019 Alexander Borisov
 *
 * Author: Alexander Borisov <borisov@lexbor.com>
 */

#include "lexbor/encoding/encoding.h"
#include "lexbor/encoding/multi.h"


#define append_to_file(fc, data, len, codepoint)                               \
    do {                                                                       \
        for (size_t j = 0; j < len; j++) {                                     \
            fprintf(fc, "\\x%02X", data[j]);                                   \
        }                                                                      \
                                                                               \
        fprintf(fc, " 0x%04X\n", codepoint);                                   \
    }                                                                          \
    while (0)

#define encode_to_file(fc, _cp)                                                \
    do {                                                                       \
        cp = &_cp;                                                             \
                                                                               \
        lxb_encoding_encode_init(&ctx, enc_data, data, sizeof(data));          \
                                                                               \
        status = enc_data->encode(&ctx, &cp, (cp + 1));                        \
        if (status != LXB_STATUS_OK) {                                         \
            printf("Failed to encoding code point: %04X\n", _cp);              \
            return EXIT_FAILURE;                                               \
        }                                                                      \
                                                                               \
        append_to_file(fc, ctx.buffer_out, ctx.buffer_used, _cp);              \
    }                                                                          \
    while (0)


int main(int argc, const char * argv[])
{
    size_t size;
    lxb_char_t data[8];
    lxb_status_t status;
    lxb_encoding_encode_t ctx;
    const lxb_codepoint_t *cp;
    const lxb_encoding_data_t *enc_data;
    const lxb_encoding_multi_index_t *entry;

    const char *filepath = "./euc_kr_map_decode.txt";

    enc_data = lxb_encoding_data(LXB_ENCODING_EUC_KR);

    FILE *fc = fopen(filepath, "w");
    if (fc == NULL) {
        printf("Failed to opening file: %s\n", filepath);
        exit(EXIT_FAILURE);
    }

    fprintf(fc, "#\n"
            "# Copyright (C) 2019 Alexander Borisov\n"
            "#\n"
            "# Author: Alexander Borisov <borisov@lexbor.com>\n"
            "#\n\n");

    fprintf(fc, "#\n"
            "# This file generated by the program\n"
            "# \"utils/lexbor/encoding/euc_kr_map_decode.c\"\n"
            "#\n\n");

    size = sizeof(lxb_encoding_multi_index_euc_kr)
           / sizeof(lxb_encoding_multi_index_t);

    for (size_t i = 0; i < size; i++) {
        entry = &lxb_encoding_multi_index_euc_kr[i];

        if (entry->codepoint == LXB_ENCODING_ERROR_CODEPOINT) {
            continue;
        }

        encode_to_file(fc, entry->codepoint);
    }

    for (lxb_codepoint_t i = 0x00; i < 0x80; i++) {
        encode_to_file(fc, i);
    }

    fprintf(fc, "\n# END\n");

    fclose(fc);

    return EXIT_SUCCESS;
}
