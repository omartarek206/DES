#ifdef __GNUC__
#define __rdtsc__builtin_ia32_rdtsc
#else
#include <intrin.h>
#endif

#include <iostream>
#include <vector>
#include <map>
#include <bitset>
#include <algorithm>


using namespace std;

typedef unsigned long long u64;

// Implementation of the DES algorithm
// The key is a 64-bit bitmap
// The message is a 64-bit bitmap
// The output is a 64-bit bitmap

//////////////////////// DES TABLES ////////////////////////

// Data tables

// Initial Permutation Table
const char initial[64] =      { 58, 50, 42, 34, 26, 18, 10, 2,
                            60, 52, 44, 36, 28, 20, 12, 4,
                            62, 54, 46, 38, 30, 22, 14, 6,
                            64, 56, 48, 40, 32, 24, 16, 8,
                            57, 49, 41, 33, 25, 17,  9, 1,
                            59, 51, 43, 35, 27, 19, 11, 3,
                            61, 53, 45, 37, 29, 21, 13, 5,
                            63, 55, 47, 39, 31, 23, 15, 7 };

// Expansion Table
const char expansion[48] = { 32,  1,  2,  3,  4,  5,
                            4,  5,  6,  7,  8,  9,
                            8,  9, 10, 11, 12, 13,
                            12, 13, 14, 15, 16, 17,
                            16, 17, 18, 19, 20, 21,
                            20, 21, 22, 23, 24, 25,
                            24, 25, 26, 27, 28, 29,
                            28, 29, 30, 31, 32,  1 };

// S-Box Tables
const u64 s_box_table [8][64] =  { { 14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7,
                          0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8,
                          4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0,
                          15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13 },
                        { 15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10,
                          3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5,
                          0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15,
                          13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9 },

                        { 10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8,
                          13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1,
                          13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7,
                          1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12 },
                        { 7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15,
                          13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9,
                          10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4,
                          3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14 },
                        { 2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9,
                          14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6,
                          4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14,
                          11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3 },
                        { 12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11,
                          10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8,
                          9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6,
                          4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13 },
                        { 4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1,
                          13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6,
                          1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2,
                          6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12 },
                        { 13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7,
                          1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2,
                          7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8,
                          2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11 } };


// pbox Table
const char pbox[32] =     { 16,  7, 20, 21,
                           29, 12, 28, 17,
                            1, 15, 23, 26,
                            5, 18, 31, 10,
                            2,  8, 24, 14,
                            32, 27,  3,  9,
                            19, 13, 30,  6,
                            22, 11,  4, 25 };

// Final Permutation Table
const char final[64] =     { 40, 8, 48, 16, 56, 24, 64, 32,
                            39, 7, 47, 15, 55, 23, 63, 31,
                            38, 6, 46, 14, 54, 22, 62, 30,
                            37, 5, 45, 13, 53, 21, 61, 29,
                            36, 4, 44, 12, 52, 20, 60, 28,
                            35, 3, 43, 11, 51, 19, 59, 27,
                            34, 2, 42, 10, 50, 18, 58, 26,
                            33, 1, 41,  9, 49, 17, 57, 25 };


// Key tables

// Permuted Choice 1 Table
const char choice1[56] =   { 57, 49, 41, 33, 25, 17, 9,
                            1, 58, 50, 42, 34, 26, 18,
                            10, 2, 59, 51, 43, 35, 27,
                            19, 11, 3, 60, 52, 44, 36,
                            63, 55, 47, 39, 31, 23, 15,
                            7, 62, 54, 46, 38, 30, 22,
                            14, 6, 61, 53, 45, 37, 29,
                            21, 13, 5, 28, 20, 12, 4 };

// Permuted Choice 2 Table
const char choice2[48] =   { 14, 17, 11, 24,  1,  5,
                            3, 28, 15,  6, 21, 10,
                            23, 19, 12,  4, 26,  8,
                            16,  7, 27, 20, 13,  2,
                            41, 52, 31, 37, 47, 55,
                            30, 40, 51, 45, 33, 48,
                            44, 49, 39, 56, 34, 53,
                            46, 42, 50, 36, 29, 32 };

// Shift Table
const char shift[16] =     { 1, 1, 2, 2, 2, 2, 2, 2,
                            1, 2, 2, 2, 2, 2, 2, 1 };

// Global keys
u64 global_keys[16];
u64 global_keys_inv[16];

//////////////////////// END OF DES TABLES ////////////////////////

//////////////////////// DES FUNCTIONS ///////////////////////////

/// Do permutation on the message
u64 permute(u64 data,const char*permutation,int in,int out)
{
    u64 result = 0;
    for (int i = 0; i < out; ++i)
        result|=((data>>(in-permutation[out-1-i]))&1)<<i;
    return result;
}


/// Sbox Function
u64 sbox_func(u64 data){
    const char index_permutation[6] = {1,6,2,3,4,5};
    u64 index;
    u64 out = 0;
    for (int i = 0; i < 8; i++)
    {
        index = (data>>(i*6)) & 0x3F;
        index = permute(index,index_permutation,6,6);
        out |= s_box_table[7-i][index] << i*4;
    }
    return out; 
}

/// convert hexadecimal to u64
u64 hex_to_u64(string hex) {
    u64 ans = 0000000000000000000000000000000000000000000000000000000000000000;
    for (int i = 0; i < hex.size(); i++) {
        char c = hex[i];
        if (c >= '0' && c <= '9') {
            ans |= (u64) (c - '0') << ((15-i) << 2);
        } else if (c >= 'a' && c <= 'f') {
            ans |= (u64) (c - 'a' + 10) << ((15-i) << 2);
        } else if (c >= 'A' && c <= 'F') {
            ans |= (u64) (c - 'A' + 10) << ((15-i) << 2);
        }
    }
    return ans;
}

/// Feistel Cipher function for one round
u64 feistel(u64 right, u64 key){
    u64 expanded = permute(right, expansion, 32, 48);
    u64 xored    = expanded ^ key;
    u64 sboxed   = sbox_func(xored);
    u64 permuted = permute(sboxed, pbox, 32, 32);
    return permuted;
}

/// Key shift left function
u64 key_shift(u64 key, int round){
    u64 result = 0;
    for (int i = 0; i < 28; i++)
    {
        result |= (key>>(28-shift[round]))&1;
        key <<= 1;
    }
    return result;
}

/// Generate keys
void key_gen(u64 key){
    u64 permuted_key = permute(key, choice1, 64, 56);
    u64 result[2];
    result[0] = (permuted_key>>28) & 0xFFFFFFFF;
    result[1] =  permuted_key      & 0xFFFFFFFF;
    u64 active_keys[2] = {result[0], result[1]};
    for (int i = 0; i < 16; i++) {
        active_keys[0] = key_shift(active_keys[0], i);
        active_keys[1] = key_shift(active_keys[1], i);
        global_keys[i] = permute(active_keys[0] + active_keys[1], choice2, 56, 48);
    }
    for(int i = 0; i < 16; i++) 
        global_keys_inv[i] = global_keys[15-i];
    
}

/// Encrypt a 64-bit message with a 64-bit key
u64 crypt(u64 key, u64 plain, int mode){
    u64 initial_perm = permute(plain, initial, 64, 64);
/*     cout<< "After initial permutation: " << bin_to_hex(initial_perm) << endl; */

    u64 left  = (initial_perm>>32) & 0xFFFFFFFF;
    u64 right =  initial_perm      & 0xFFFFFFFF;
/*     cout<< "After splitting: " << "L0= " << bin_to_hex(left) << ", R0= " << bin_to_hex(right) << endl; */
    u64 temp, temp_key;
    u64 keys[16];


    if(mode ==0){
        for(int i=0; i<16; i++)
            keys[i] = global_keys[i];
    } 
    else if(mode == 1){
        for(int i=0; i<16; i++)
            keys[i] = global_keys_inv[i];
    }

    for (int i = 0; i < 16; i++) {
        temp = right;
        right = left ^ feistel(right, keys[i]);
        left = temp;
/*         cout << "Round " << i + 1 << " " << bin_to_hex(left) << " "
             << bin_to_hex(right) << " " << bin_to_hex(temp_key) << endl; */
    }
    
    u64 final_perm = permute(right+left, final, 64, 64);
    return final_perm;
}

//////////////////////// END OF DES FUNCTIONS ////////////////////////

/////////////////////// Main  ///////////////////////////////

int main(int argc, char **argv){
    
    char* operation = argv[1];
    char* data_c = argv[2];
    char* key_c = argv[3];

    u64 data = hex_to_u64(data_c);
    u64 key = hex_to_u64(key_c);
    u64 output = 0;

    key_gen(key);

    long long t1 =__rdtsc();
    if (operation == "encrypt"){
        output = crypt(key, data, 0);
        printf("Cipher: %016llx\n", output);
    }    
    else if (operation == "decrypt"){
        output = crypt(key, data, 1);
        printf("Plain: %016llx\n", output);
    }    
    else    cout << "Recheck the desired operation";
    long long t2 =__rdtsc();
    printf ("Cycles: %llx\n", t2-t1);
    return 0;
}

/////////////////////// End of Main ///////////////////////////////
