#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "threefish.h"

const word64 C240 = 0x1bd11bdaa9fc1a22;
const int ROUNDS = 72; // count from 0 to 71 we have 72 rounds;
#if 0 /* TODO change to 0 after you complete this task */
/*

TODO FREE TEXT ASSIGNMENT

If there was a FAQ for this assignment, what question and answer would've helped you the most?

Q: <FILL ME IN>

A: <FILL ME IN>

*/
#warning "FAIL you have not completed the free text assignment"
#endif

void key_schedule(const word64* key_words, const word64* tweak_words, const int s, word64* output) {
    // Since the value is uint64_t, we can use overflow to work as if we are doing modulo
    // addition over 2^64
    output[0] = key_words[s % 5];
    output[1] = key_words[(s + 1) % 5] + tweak_words[s % 3];
    output[2] = key_words[(s + 2) % 5] + tweak_words[(s+1) % 3];
    output[3] = key_words[(s + 3) % 5] + s;
}

int word_permutation(int input) {
    switch (input) {
        case 0:
            return 0;
        case 1:
            return 3;
        case 2:
            return 2;
        case 3:
            return 1;
        default:
            return -1;
    }
}

void mix_function(const word64 input1, const word64 input2, int j, int d, word64* output1, word64* output2) {
    *output1 = input1 + input2;
    int R = r_function(d%8, j);
    *output2 = ((input2 << (R)) | (input2 >> ((64-R)))) ^ *output1;
}

int r_function(int d, int j) {
    if (j == 0) {
        switch (d) {
            case 0:
                return 14;
            case 1:
                return 52;
            case 2:
                return 23;
            case 3:
                return 5;
            case 4:
                return 25;
            case 5:
                return 46;
            case 6:
                return 58;
            case 7:
                return 32;
            default:
                return -1;
        }
    }
    if (j == 1) {
        switch (d) {
            case 0:
                return 16;
            case 1:
                return 57;
            case 2:
                return 40;
            case 3:
                return 37;
            case 4:
                return 33;
            case 5:
                return 12;
            case 6:
                return 22;
            case 7:
                return 32;
            default:
                return -1;
        }
    }
}

/* Under key at k and tweak at t, encrypt 32 bytes of plaintext at p and store it at c. */
void threefish(unsigned char *c, const unsigned char *p, const unsigned char *k, const unsigned char *t) {
    word64 key_words[5];
    word64 tweak_words[3];
    word64 plain_words[4];
    word64 e_words[4];
    word64 c_words[4];
    word64 v_words[4];
    word64 current_subkey_words[4];
    word64 f_words[4];
    memcpy(key_words, k, 32);
    memcpy(tweak_words, t, 16);
    memcpy(plain_words, p, 32);
    memcpy(v_words, p, 32);

    /* Calculate the word k_Nw */
    word64 k_Nw = key_words[0] ^ key_words[1] ^ key_words[2] ^ key_words[3] ^ C240;
    key_words[4] = k_Nw;
    /* Calculate the word t2 */
    word64 t2 = tweak_words[0] ^ tweak_words[1];
    tweak_words[2] = t2;

    for (int d = 0; d < ROUNDS; d++) {
        if (d % 4 == 0) {
            key_schedule(key_words, tweak_words, d/4, current_subkey_words);
            e_words[0] = v_words[0] + current_subkey_words[0];
            e_words[1] = v_words[1] + current_subkey_words[1];
            e_words[2] = v_words[2] + current_subkey_words[2];
            e_words[3] = v_words[3] + current_subkey_words[3];
        } else {
            memcpy(e_words, v_words, 32);
        }
        mix_function(e_words[0], e_words[1], 0, d, &f_words[0], &f_words[1]);
        mix_function(e_words[2], e_words[3], 1, d, &f_words[2], &f_words[3]);
        v_words[0] = f_words[word_permutation(0)];
        v_words[1] = f_words[word_permutation(1)];
        v_words[2] = f_words[word_permutation(2)];
        v_words[3] = f_words[word_permutation(3)];
    }
    key_schedule(key_words, tweak_words, ROUNDS/4, current_subkey_words);
    c_words[0] = v_words[0] + current_subkey_words[0];
    c_words[1] = v_words[1] + current_subkey_words[1];
    c_words[2] = v_words[2] + current_subkey_words[2];
    c_words[3] = v_words[3] + current_subkey_words[3];
    memcpy(c, c_words, 32);
}

