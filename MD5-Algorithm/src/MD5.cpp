#include "MD5.hpp"
#include <cmath>
#include <iostream>
using std::string;
using std::cout;
using std::endl;

const unsigned int MD5::r[64] = {
    7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
    5, 9,  14, 20, 5, 9,  14, 20, 5, 9,  14, 20, 5, 9,  14, 20,
    4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
    6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21};

unsigned int MD5::k[64];
unsigned int MD5::h0, MD5::h1, MD5::h2, MD5::h3;

/**
 * initialize k, h0~h3
 */
void MD5::init() {
    for (int i = 0; i < 64; i++) k[i] = floor(fabs(sin(i + 1) * pow(2, 32)));
    h0 = 0x67452301;
    h1 = 0xEFCDAB89;
    h2 = 0x98BADCFE;
    h3 = 0x10325476;
}

/*
 * turn byteStream to bitStream
 * @param text input byteSteam
 * @return bitStream output bitStream
 */
bitStream byteStreamTobitStream(const byteStream& text) {
    bitStream message = "";
    bitStream temp = "";
    for (int i = 0; i < text.length(); i++) {
        temp = "";
        int t = 1;
        int c = (int)text[i];
        for (int j = 1; j <= 8; j++) {
            if (c & t)
                temp.push_back('1');
            else
                temp.push_back('0');
            t = t << 1;
        }
        message += temp;
    }
    return message;
}

/**
 *@param bitStream input bitStream
 *@return unsigned int little-endian interger
 */
unsigned int getLittleEndianWords(const bitStream& bs) {
    unsigned int ans = 0;
    unsigned int t = 0;
    for (int i = 0; i < bs.length(); i++) {
        t = bs[i] == '0' ? 0 : 1;
        ans += (t << i);
    }
    return ans;
}

/**
 * binary leftrotate
 * @param unsigned int input unsigned int
 * @return unsigned int after leftrotate
 */
unsigned int leftrotate(unsigned int x, unsigned int y) {
    return (x << y) | (x >> (32 - y));
}

/**
 * turn unsigned int to bitStream
 * @param unsigned int, input unsigned int
 * @return bitStream. output bitStream
 */
bitStream uInt32TobitStream(const unsigned int& x) {
    bitStream ans = "";
    unsigned int t = 1;
    for (int i = 1; i <= 32; i++) {
        if (x & t) {
            ans.push_back('1');
        } else {
            ans.push_back('0');
        }
        t = t << 1;
    }
    return ans;
}

/**
 * turn int in decimal to hex
 * @param int, input integer in decimal
 * @return char, integer in hex
 */
char intToHex(const int& x) {
    if (0 <= x && x <= 9) {
        return '0' + x;
    } else {
        return 'a' + x - 10;
    }
}

/**
 * turn bitStream to String in Hex
 * @param bitStream, input bitStream
 * @return string, output String
 */
string bitStreamToHexString(const bitStream& bs) {
    string ans = "";
    int i = 0;
    int t = 0, t2 = 0;
    int temp;
    while (i < bs.length()) {
        t = 0;
        for (int j = 0; j < 4; j++) {
            temp = bs[i + j] == '0' ? 0 : 1;
            t = t + temp * (1 << j);
        }
        t2 = 0;
        for (int j = 4; j < 8; j++) {
            temp = bs[i + j] == '0' ? 0 : 1;
            t2 = t2 + temp * (1 << (j - 4));
        }
        i += 8;
        ans.push_back(intToHex(t2));
        ans.push_back(intToHex(t));
    }
    return ans;
}

/**
 *  encrypt a string using MD5 Algorithm
 *  @param string, input string
 *  @return string, string encrypted by MD5 Algorithm
 */
string MD5::encrypt(const string& text) {
    init();
    bitStream message = byteStreamTobitStream(text);
    message += "00000001";
    int len = message.length();
    while (len % 512 != 448) {
        message.push_back('0');
        len = message.length();
    }
    int t = 1;
    len = text.length() * 8;
    for (int i = 1; i <= 64; i++) {
        if (len & t) {
            message.push_back('1');
        } else {
            message.push_back('0');
        }
        t = t << 1;
    }
    int index = 0;
    unsigned int w[16];
    unsigned int a, b, c, d, f, g, temp;
    while (index < message.length()) {
        for (int i = 0; i < 16; i++) {
            w[i] = getLittleEndianWords(message.substr(index + i * 32, 32));
        }
        a = h0;
        b = h1;
        c = h2;
        d = h3;
        for (int i = 0; i < 64; i++) {
            if (0 <= i && i <= 15) {
                f = (b & c) | ((~b) & d);
                g = i;
            } else if (16 <= i && i <= 31) {
                f = (d & b) | ((~d) & c);
                g = (5 * i + 1) % 16;
            } else if (32 <= i && i <= 47) {
                f = b ^ c ^ d;
                g = (3 * i + 5) % 16;
            } else if (48 <= i && i <= 63) {
                f = c ^ (b | (~d));
                g = (7 * i) % 16;
            }
            temp = d;
            d = c;
            c = b;
            b = leftrotate((a + f + k[i] + w[g]), r[i]) + b;
            a = temp;
        }
        h0 += a;
        h1 += b;
        h2 += c;
        h3 += d;
        index = index + 512;
    }
    bitStream ans = "";
    ans += uInt32TobitStream(h0);
    ans += uInt32TobitStream(h1);
    ans += uInt32TobitStream(h2);
    ans += uInt32TobitStream(h3);
    return bitStreamToHexString(ans);
}
