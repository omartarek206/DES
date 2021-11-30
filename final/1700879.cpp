#include <bits/stdc++.h>
 
using namespace std;

// Implementation of the DES algorithm
// The key is a 64-bit bitmap
// The message is a 64-bit bitmap
// The output is a 64-bit bitmap

//////////////////////// DES TABLES ////////////////////////

// Data tables

// Initial Permutation Table
vector<int> initial =      { 58, 50, 42, 34, 26, 18, 10, 2,
                            60, 52, 44, 36, 28, 20, 12, 4,
                            62, 54, 46, 38, 30, 22, 14, 6,
                            64, 56, 48, 40, 32, 24, 16, 8,
                            57, 49, 41, 33, 25, 17,  9, 1,
                            59, 51, 43, 35, 27, 19, 11, 3,
                            61, 53, 45, 37, 29, 21, 13, 5,
                            63, 55, 47, 39, 31, 23, 15, 7 };

// Expansion Table
vector<int> expansion = { 32,  1,  2,  3,  4,  5,
                            4,  5,  6,  7,  8,  9,
                            8,  9, 10, 11, 12, 13,
                            12, 13, 14, 15, 16, 17,
                            16, 17, 18, 19, 20, 21,
                            20, 21, 22, 23, 24, 25,
                            24, 25, 26, 27, 28, 29,
                            28, 29, 30, 31, 32,  1 };

// S-Box Tables
vector< vector <vector <int> > > sbox  = 
                    {   
                        { 
                            { 14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7 },
                            { 0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8 },
                            { 4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0 },
                            { 15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13 }
                        },{
                            { 15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10 },
                            { 3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5 },
                            { 0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15 },
                            { 13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9 }
                        },{
                            { 10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8 },
                            { 13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1 },
                            { 13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7 },
                            { 1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12 }
                        },{ 
                            { 7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15 },
                            { 13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9 },
                            { 10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4 },
                            { 3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14 }
                        },{
                            { 2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9 },
                            { 14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6 },
                            { 4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14 },
                            { 11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3 }
                        },{
                            { 12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11 },
                            { 10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8 },
                            { 9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6 },
                            { 4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13 }
                        },{
                            { 4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1 },
                            { 13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6 },
                            { 1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2 },
                            { 6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12 }
                        },{
                            { 13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7 },
                            { 1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2 },
                            { 7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8 },
                            { 2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11 }
                        }
                    };

// pbox Table
vector<int> pbox =     { 16,  7, 20, 21,
                           29, 12, 28, 17,
                            1, 15, 23, 26,
                            5, 18, 31, 10,
                            2,  8, 24, 14,
                            32, 27,  3,  9,
                            19, 13, 30,  6,
                            22, 11,  4, 25 };

// Final Permutation Table
vector<int> final =     { 40, 8, 48, 16, 56, 24, 64, 32,
                            39, 7, 47, 15, 55, 23, 63, 31,
                            38, 6, 46, 14, 54, 22, 62, 30,
                            37, 5, 45, 13, 53, 21, 61, 29,
                            36, 4, 44, 12, 52, 20, 60, 28,
                            35, 3, 43, 11, 51, 19, 59, 27,
                            34, 2, 42, 10, 50, 18, 58, 26,
                            33, 1, 41,  9, 49, 17, 57, 25 };


// Key tables

// Permuted Choice 1 Table
vector<int> choice1 =   { 57, 49, 41, 33, 25, 17, 9,
                            1, 58, 50, 42, 34, 26, 18,
                            10, 2, 59, 51, 43, 35, 27,
                            19, 11, 3, 60, 52, 44, 36,
                            63, 55, 47, 39, 31, 23, 15,
                            7, 62, 54, 46, 38, 30, 22,
                            14, 6, 61, 53, 45, 37, 29,
                            21, 13, 5, 28, 20, 12, 4 };

// Permuted Choice 2 Table
vector<int> choice2 =   { 14, 17, 11, 24,  1,  5,
                            3, 28, 15,  6, 21, 10,
                            23, 19, 12,  4, 26,  8,
                            16,  7, 27, 20, 13,  2,
                            41, 52, 31, 37, 47, 55,
                            30, 40, 51, 45, 33, 48,
                            44, 49, 39, 56, 34, 53,
                            46, 42, 50, 36, 29, 32 };

// Shift Table
vector<int> shift =     { 1, 1, 2, 2, 2, 2, 2, 2,
                            1, 2, 2, 2, 2, 2, 2, 1 };

// Global keys
vector<string> global_keys;

// map table names to tables
map<string, vector<int>*> tables = {
    {"initial"  , &initial  },
    {"expansion", &expansion},
    {"pbox"     , &pbox     },
    {"final"    , &final    },
    {"choice1"  , &choice1  },
    {"choice2"  , &choice2  },
};

//////////////////////// END OF DES TABLES ////////////////////////

//////////////////////// DES FUNCTIONS ///////////////////////////

/// XOR Function
string xor_func(string a, string b)
{
    int n = a.length();
    if ( n == 32 ){
        bitset<32> a_bits(a);
        bitset<32> b_bits(b);
        bitset<32> result = a_bits ^ b_bits;
        return result.to_string();
    }
    else if ( n == 48 ){
        bitset<48> a_bits(a);
        bitset<48> b_bits(b);
        bitset<48> result = a_bits ^ b_bits;
        return result.to_string();
    }
    else if ( n == 64 ){
        bitset<64> a_bits(a);
        bitset<64> b_bits(b);
        bitset<64> result = a_bits ^ b_bits;
        return result.to_string();
    }
    else{
        /* cout << "Invalid XOR Function" << endl; */
        return "";
    }
}

/// Sbox Function
string sbox_func(string x){
    string result = "";
		for (int i = 0; i < 8; i++) {
			int row = 2 * int(x[i * 6] - '0') + int(x[i * 6 + 5] - '0');
			int col = 8 * int(x[i * 6 + 1] - '0') + 4 * int(x[i * 6 + 2] - '0') + 2 * int(x[i * 6 + 3] - '0') + int(x[i * 6 + 4] - '0');
			int val = sbox[i][row][col];
			result += char(val / 8 + '0');
			val = val % 8;
			result += char(val / 4 + '0');
			val = val % 4;
			result += char(val / 2 + '0');
			val = val % 2;
			result += char(val + '0');
		}
    return result;
}

/// Convert hexadecimal string to binary string
string hex_to_bin(string hex) {
    string bin;
    for (int i = 0; i < hex.length(); i++) {
        switch (hex[i]) {
            case '0':
                bin += "0000";
                break;
            case '1':
                bin += "0001";
                break;
            case '2':
                bin += "0010";
                break;
            case '3':
                bin += "0011";
                break;
            case '4':
                bin += "0100";
                break;
            case '5':
                bin += "0101";
                break;
            case '6':
                bin += "0110";
                break;
            case '7':
                bin += "0111";
                break;
            case '8':
                bin += "1000";
                break;
            case '9':
                bin += "1001";
                break;
            case 'A':
                bin += "1010";
                break;
            case 'B':
                bin += "1011";
                break;
            case 'C':
                bin += "1100";
                break;
            case 'D':
                bin += "1101";
                break;
            case 'E':
                bin += "1110";
                break;
            case 'F':
                bin += "1111";
                break;
        }
    }
    return bin;
}

/// Convert binary string to hexadecimal string
string bin_to_hex(string bin){
    map<string, string> hex_conv = {
        {"0000", "0"}, {"0001", "1"}, {"0010", "2"}, {"0011", "3"}, 
        {"0100", "4"}, {"0101", "5"}, {"0110", "6"}, {"0111", "7"}, 
        {"1000", "8"}, {"1001", "9"}, {"1010", "A"}, {"1011", "B"}, 
        {"1100", "C"}, {"1101", "D"}, {"1110", "E"}, {"1111", "F"}
    };

    string hex = "";
    if      (bin.length() % 4 == 1) bin = "000" + bin;
    else if (bin.length() % 4 == 2) bin = "00" + bin;
    else if (bin.length() % 4 == 3) bin = "0" + bin;
    
    for (int i = 0; i < bin.length(); i += 4) {
        hex += hex_conv[bin.substr(i, 4)];
    }
    return hex;
}

/// Do permutation on the message
string permute(string data, string table, int in, int out){
    string result = "";
    vector<int> op_table = *tables[table];
    for(int i = 0; i < out; i++)
        result += data[op_table[i]-1];
    return result;
}

/// Split the message into two halves
vector<string> split(string data){
    vector<string> result = {
        data.substr(0, data.length()/2), 
        data.substr(data.length()/2, data.length())
    };
    return result;
}

/// Feistel Cipher function for one round
string feistel(string right, string key){
    string expanded = permute(right, "expansion", 32, 48);
    string xored    = xor_func(expanded, key);
    string sboxed   = sbox_func(xored);
    string permuted = permute(sboxed, "pbox", 32, 32);
    return permuted;
}

/// Key shift left function
string key_shift(string key, int round){
    string result = "";
    for (int i = 0; i < key.length(); i++) {
        result += key[(i + shift[round]) % key.length()];
    }
    return result;
}

/// Generate keys
void key_gen(string key){
    string permuted_key = permute(key, "choice1", 64, 56);
    vector<string> result = split(permuted_key);
    vector<string> active_key = result;
    for (int i = 0; i < 16; i++) {
        active_key[0] = key_shift(active_key[0], i);
        active_key[1] = key_shift(active_key[1], i);
        global_keys.push_back(permute(active_key[0] + active_key[1], "choice2", 56, 48));
    }
}

/// Encrypt a 64-bit message with a 64-bit key
string crypt(string key, string plain){
    string initial_perm = permute(plain, "initial", 64, 64);
/*     cout<< "After initial permutation: " << bin_to_hex(initial_perm) << endl; */


    vector<string> half = split(initial_perm);
    string left = half[0];
    string right = half[1];
/*     cout<< "After splitting: " << "L0= " << bin_to_hex(left) << ", R0= " << bin_to_hex(right) << endl; */
    string temp, temp_key;
    
    for (int i = 0; i < 16; i++) {
        temp = right;
        right = xor_func(left, feistel(right, global_keys[i]));
        left = temp;
/*         cout << "Round " << i + 1 << " " << bin_to_hex(left) << " "
             << bin_to_hex(right) << " " << bin_to_hex(temp_key) << endl; */
    }
    
    string final_perm = permute(right  + left, "final", 64, 64);
    return bin_to_hex(final_perm);
}

string encrypt(string key, string plain){
    key_gen(key);
    cout << "Cipher: ";
    return crypt(key, plain);
}

string decrypt(string key, string plain){
    key_gen(key);
    reverse(global_keys.begin(), global_keys.end());
    cout << "Plain: ";
    return crypt(key, plain);
}

//////////////////////// END OF DES FUNCTIONS ////////////////////////

/////////////////////// Main  ///////////////////////////////

int main(int argc, char **argv){
    
    if (argc != 4) {
        cout << "Usage: ./des <key> <plaintext> <ciphertext>" << endl;
        return 0;
    }
    string o(argv[0]);
    string a(argv[1]);
    string b(argv[2]);
    string c(argv[3]);

    string data = hex_to_bin(b);
    string key = hex_to_bin(c);
    string cipher = "";
    
    if      (a == "encrypt")    cipher = encrypt(key, data);
    else if (a == "decrypt")    cipher = decrypt(key, data);
    else    cout << "Recheck the desired operation";
    
    cout << cipher << endl;
    return 0;
}

/////////////////////// End of Main ///////////////////////////////