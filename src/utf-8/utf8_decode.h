/* https://www.json.org/JSON_checker/ */

/* utf8_decode.h */

#define UTF8_END   -1
#define UTF8_ERROR -2

extern int  utf8_decode_at_byte();
extern int  utf8_decode_at_character();
extern void utf8_decode_init(char p[], int length);
extern int  utf8_decode_next();

/* ADITIONAL MADE BY GROUP */

/**
 * @brief Convert utf32 string to utf8
 * 
 * @param data String to convert
 * @param size Size of string that will be converted
*/
extern char *utf32_to_utf8_malloced_string(int *data, int size);
