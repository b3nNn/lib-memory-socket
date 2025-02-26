#ifndef LIB_MEMORY_SOCKET_BASE64ENCODING_H
#define LIB_MEMORY_SOCKET_BASE64ENCODING_H
#include <string>
#include "sodium.h"

class Base64Encoding {
public:
    static char *Encode(void *data, size_t data_size) {
        const unsigned char *buffer = (unsigned char *)data;
        const size_t  base64_max_len = sodium_base64_encoded_len(data_size, sodium_base64_VARIANT_ORIGINAL);
        std::string base64_str(base64_max_len-1,0);
        return sodium_bin2base64(
                 base64_str.data(),
                 base64_max_len,
                 buffer,
                 data_size,
                 sodium_base64_VARIANT_ORIGINAL
         );
    }
};

#endif //LIB_MEMORY_SOCKET_BASE64ENCODING_H
