typedef uint64_t word64;

void key_schedule(const word64* key_words, const word64* tweak_words, const int s, word64* output);
int word_permutation(int input);
void mix_function(const word64 input1, const word64 input2, int j, int d, word64* output1, word64* output2);

int r_function(int d, int j);
/* Implement the following API.
 * You can add your own functions above, but don't modify below this line.
 */

/* Under key at k and tweak at t, encrypt 32 bytes of plaintext at p and store it at c. */
void threefish(unsigned char *c, const unsigned char *p, const unsigned char *k, const unsigned char *t);

