#ifndef _MD5_
#define _MD5_
#include <string>

typedef unsigned int uint32;
typedef std::string bitStream;
typedef std::string byteStream;

class MD5 {
    public:
        /**
         *  encrypt a string using MD5 Algorithm
         *  @param string, input string
         *  @return string, string encrypted by MD5 Algorithm
         * */
        static std::string encrypt(const std::string&);
    private:
        static const unsigned int r[64];
        static unsigned int k[64];
        static unsigned int h0, h1, h2, h3;
        /**
         * initialize k, h0~h3
         */
        static void init();
};

#endif
