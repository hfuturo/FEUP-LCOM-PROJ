#include <stdint.h>
/**
 * @brief Scancodes and corresponding caracters
 */
char *scancodes[256] = {
  [0x00] = "",
  [0x01] = "", // esc
  [0x02] = "1",
  [0x03] = "2",
  [0x04] = "3",
  [0x05] = "4",
  [0x06] = "5",
  [0x07] = "6",
  [0x08] = "7",
  [0x09] = "8",
  [0x0a] = "9",
  [0x0b] = "0",
  [0x0c] = "'",
  [0x0d] = "«",
  [0x0e] = "", // backspace
  [0x0f] = "", // tab

  [0x10] = "q",
  [0x11] = "w",
  [0x12] = "e",
  [0x13] = "r",
  [0x14] = "t",
  [0x15] = "y",
  [0x16] = "u",
  [0x17] = "i",
  [0x18] = "o",
  [0x19] = "p",
  [0x1a] = "+",
  [0x1b] = "´",
  [0x1c] = "", // enter
  [0x1d] = "", // control
  [0x1e] = "a",
  [0x1f] = "s",

  [0x20] = "d",
  [0x21] = "f",
  [0x22] = "g",
  [0x23] = "h",
  [0x24] = "j",
  [0x25] = "k",
  [0x26] = "l",
  [0x27] = "ç",
  [0x28] = "º",
  [0x29] = "\\",
  [0x2a] = "", // shift
  [0x2b] = "~",
  [0x2c] = "z",
  [0x2d] = "x",
  [0x2e] = "c",
  [0x2f] = "v",

  [0x30] = "b",
  [0x31] = "n",
  [0x32] = "m",
  [0x33] = ",",
  [0x34] = ".",
  [0x35] = "-",
  [0x36] = "", // shift
  [0x37] = "*",
  [0x38] = "", // alt
  [0x39] = " ",
  [0x3a] = "", // caps
  [0x3b] = "", // f1
  [0x3c] = "", // f2
  [0x3d] = "", // f3
  [0x3e] = "", // f4
  [0x3f] = "", // f5

  [0x40] = "", // f6
  [0x41] = "", // f7
  [0x42] = "", // f8
  [0x43] = "", // f9
  [0x44] = "", // f10
  [0x45] = "", // non lock
  [0x46] = "",
  [0x47] = "", // num7
  [0x48] = "", // num8
  [0x49] = "", // num9
  [0x4a] = "-",
  [0x4b] = "", // num4
  [0x4c] = "", // num5
  [0x4d] = "", // num6
  [0x4e] = "+",
  [0x4f] = "", // num1

  [0x50] = "", // num2
  [0x51] = "", // num3
  [0x52] = "", // num0
  [0x53] = "", // delete
  [0x54] = "",
  [0x55] = "",
  [0x56] = "<",
  [0x57] = "", // f11
  [0x58] = "", // f12
  [0x59] = "",
  [0x5a] = "",
  [0x5b] = "",
  [0x5c] = "",
  [0x5d] = "",
  [0x5e] = "",
  [0x5f] = "",

  [0x60] = "",
  [0x61] = "",
  [0x62] = "",
  [0x63] = "",
  [0x64] = "",
  [0x65] = "",
  [0x66] = "",
  [0x67] = "",
  [0x68] = "",
  [0x69] = "",
  [0x6a] = "",
  [0x6b] = "",
  [0x6c] = "",
  [0x6d] = "",
  [0x6e] = "",
  [0x6f] = "",

  [0x70] = "",
  [0x71] = "",
  [0x72] = "",
  [0x73] = "",
  [0x74] = "",
  [0x75] = "",
  [0x76] = "",
  [0x77] = "",
  [0x78] = "",
  [0x79] = "",
  [0x7a] = "",
  [0x7b] = "",
  [0x7c] = "",
  [0x7d] = "",
  [0x7e] = "",
  [0x7f] = "",

  [0x80] = "",
  [0x81] = "",
  [0x82] = "",
  [0x83] = "",
  [0x84] = "",
  [0x85] = "",
  [0x86] = "",
  [0x87] = "",
  [0x88] = "",
  [0x89] = "",
  [0x8a] = "",
  [0x8b] = "",
  [0x8c] = "",
  [0x8d] = "",
  [0x8e] = "",
  [0x8f] = "",

  [0x90] = "",
  [0x91] = "",
  [0x92] = "",
  [0x93] = "",
  [0x94] = "",
  [0x95] = "",
  [0x96] = "",
  [0x97] = "",
  [0x98] = "",
  [0x99] = "",
  [0x9a] = "",
  [0x9b] = "",
  [0x9c] = "",
  [0x9d] = "",
  [0x9e] = "",
  [0x9f] = "",

  [0xa0] = "",
  [0xa1] = "",
  [0xa2] = "",
  [0xa3] = "",
  [0xa4] = "",
  [0xa5] = "",
  [0xa6] = "",
  [0xa7] = "",
  [0xa8] = "",
  [0xa9] = "",
  [0xaa] = "",
  [0xab] = "",
  [0xac] = "",
  [0xad] = "",
  [0xae] = "",
  [0xaf] = "",

  [0xb0] = "",
  [0xb1] = "",
  [0xb2] = "",
  [0xb3] = "",
  [0xb4] = "",
  [0xb5] = "",
  [0xb6] = "",
  [0xb7] = "",
  [0xb8] = "",
  [0xb9] = "",
  [0xba] = "",
  [0xbb] = "",
  [0xbc] = "",
  [0xbd] = "",
  [0xbe] = "",
  [0xbf] = "",

  [0xc0] = "",
  [0xc1] = "",
  [0xc2] = "",
  [0xc3] = "",
  [0xc4] = "",
  [0xc5] = "",
  [0xc6] = "",
  [0xc7] = "",
  [0xc8] = "",
  [0xc9] = "",
  [0xca] = "",
  [0xcb] = "",
  [0xcc] = "",
  [0xcd] = "",
  [0xce] = "",
  [0xcf] = "",

  [0xd0] = "",
  [0xd1] = "",
  [0xd2] = "",
  [0xd3] = "",
  [0xd4] = "",
  [0xd5] = "",
  [0xd6] = "",
  [0xd7] = "",
  [0xd8] = "",
  [0xd9] = "",
  [0xda] = "",
  [0xdb] = "",
  [0xdc] = "",
  [0xdd] = "",
  [0xde] = "",
  [0xdf] = "",

  [0xe0] = "",
  [0xe1] = "",
  [0xe2] = "",
  [0xe3] = "",
  [0xe4] = "",
  [0xe5] = "",
  [0xe6] = "",
  [0xe7] = "",
  [0xe8] = "",
  [0xe9] = "",
  [0xea] = "",
  [0xeb] = "",
  [0xec] = "",
  [0xed] = "",
  [0xee] = "",
  [0xef] = "",

  [0xf0] = "",
  [0xf1] = "",
  [0xf2] = "",
  [0xf3] = "",
  [0xf4] = "",
  [0xf5] = "",
  [0xf6] = "",
  [0xf7] = "",
  [0xf8] = "",
  [0xf9] = "",
  [0xfa] = "",
  [0xfb] = "",
  [0xfc] = "",
  [0xfd] = "",
  [0xfe] = "",
  [0xff] = "",
};

/**
 * @brief Scancodes and corresponding caracters when shift is pressed
 */
char *scancodesShift[256] = {
  [0x00] = "",
  [0x01] = "", // esc
  [0x02] = "!",
  [0x03] = "",
  [0x04] = "#",
  [0x05] = "$",
  [0x06] = "%",
  [0x07] = "&",
  [0x08] = "/",
  [0x09] = "(",
  [0x0a] = ")",
  [0x0b] = "=",
  [0x0c] = "?",
  [0x0d] = "»",
  [0x0e] = "", // backspace
  [0x0f] = "", // tab

  [0x10] = "Q",
  [0x11] = "W",
  [0x12] = "E",
  [0x13] = "R",
  [0x14] = "T",
  [0x15] = "Y",
  [0x16] = "U",
  [0x17] = "I",
  [0x18] = "O",
  [0x19] = "P",
  [0x1a] = "*",
  [0x1b] = "`",
  [0x1c] = "", // enter
  [0x1d] = "", // control
  [0x1e] = "A",
  [0x1f] = "S",

  [0x20] = "D",
  [0x21] = "F",
  [0x22] = "G",
  [0x23] = "H",
  [0x24] = "J",
  [0x25] = "K",
  [0x26] = "L",
  [0x27] = "Ç",
  [0x28] = "ª",
  [0x29] = "|",
  [0x2a] = "", // shift
  [0x2b] = "^",
  [0x2c] = "Z",
  [0x2d] = "X",
  [0x2e] = "C",
  [0x2f] = "V",

  [0x30] = "B",
  [0x31] = "N",
  [0x32] = "M",
  [0x33] = ";",
  [0x34] = ":",
  [0x35] = "_",
  [0x36] = "", // shift
  [0x37] = "*",
  [0x38] = "", // alt
  [0x39] = " ",
  [0x3a] = "", // caps
  [0x3b] = "", // f1
  [0x3c] = "", // f2
  [0x3d] = "", // f3
  [0x3e] = "", // f4
  [0x3f] = "", // f5

  [0x40] = "", // f6
  [0x41] = "", // f7
  [0x42] = "", // f8
  [0x43] = "", // f9
  [0x44] = "", // f10
  [0x45] = "", // non lock
  [0x46] = "",
  [0x47] = "", // num7
  [0x48] = "", // num8
  [0x49] = "", // num9
  [0x4a] = "-",
  [0x4b] = "", // num4
  [0x4c] = "", // num5
  [0x4d] = "", // num6
  [0x4e] = "+",
  [0x4f] = "", // num1

  [0x50] = "", // num2
  [0x51] = "", // num3
  [0x52] = "", // num0
  [0x53] = "", // delete
  [0x54] = "",
  [0x55] = "",
  [0x56] = ">",
  [0x57] = "", // f11
  [0x58] = "", // f12
  [0x59] = "",
  [0x5a] = "",
  [0x5b] = "",
  [0x5c] = "",
  [0x5d] = "",
  [0x5e] = "",
  [0x5f] = "",

  [0x60] = "",
  [0x61] = "",
  [0x62] = "",
  [0x63] = "",
  [0x64] = "",
  [0x65] = "",
  [0x66] = "",
  [0x67] = "",
  [0x68] = "",
  [0x69] = "",
  [0x6a] = "",
  [0x6b] = "",
  [0x6c] = "",
  [0x6d] = "",
  [0x6e] = "",
  [0x6f] = "",

  [0x70] = "",
  [0x71] = "",
  [0x72] = "",
  [0x73] = "",
  [0x74] = "",
  [0x75] = "",
  [0x76] = "",
  [0x77] = "",
  [0x78] = "",
  [0x79] = "",
  [0x7a] = "",
  [0x7b] = "",
  [0x7c] = "",
  [0x7d] = "",
  [0x7e] = "",
  [0x7f] = "",

  [0x80] = "",
  [0x81] = "",
  [0x82] = "",
  [0x83] = "",
  [0x84] = "",
  [0x85] = "",
  [0x86] = "",
  [0x87] = "",
  [0x88] = "",
  [0x89] = "",
  [0x8a] = "",
  [0x8b] = "",
  [0x8c] = "",
  [0x8d] = "",
  [0x8e] = "",
  [0x8f] = "",

  [0x90] = "",
  [0x91] = "",
  [0x92] = "",
  [0x93] = "",
  [0x94] = "",
  [0x95] = "",
  [0x96] = "",
  [0x97] = "",
  [0x98] = "",
  [0x99] = "",
  [0x9a] = "",
  [0x9b] = "",
  [0x9c] = "",
  [0x9d] = "",
  [0x9e] = "",
  [0x9f] = "",

  [0xa0] = "",
  [0xa1] = "",
  [0xa2] = "",
  [0xa3] = "",
  [0xa4] = "",
  [0xa5] = "",
  [0xa6] = "",
  [0xa7] = "",
  [0xa8] = "",
  [0xa9] = "",
  [0xaa] = "",
  [0xab] = "",
  [0xac] = "",
  [0xad] = "",
  [0xae] = "",
  [0xaf] = "",

  [0xb0] = "",
  [0xb1] = "",
  [0xb2] = "",
  [0xb3] = "",
  [0xb4] = "",
  [0xb5] = "",
  [0xb6] = "",
  [0xb7] = "",
  [0xb8] = "",
  [0xb9] = "",
  [0xba] = "",
  [0xbb] = "",
  [0xbc] = "",
  [0xbd] = "",
  [0xbe] = "",
  [0xbf] = "",

  [0xc0] = "",
  [0xc1] = "",
  [0xc2] = "",
  [0xc3] = "",
  [0xc4] = "",
  [0xc5] = "",
  [0xc6] = "",
  [0xc7] = "",
  [0xc8] = "",
  [0xc9] = "",
  [0xca] = "",
  [0xcb] = "",
  [0xcc] = "",
  [0xcd] = "",
  [0xce] = "",
  [0xcf] = "",

  [0xd0] = "",
  [0xd1] = "",
  [0xd2] = "",
  [0xd3] = "",
  [0xd4] = "",
  [0xd5] = "",
  [0xd6] = "",
  [0xd7] = "",
  [0xd8] = "",
  [0xd9] = "",
  [0xda] = "",
  [0xdb] = "",
  [0xdc] = "",
  [0xdd] = "",
  [0xde] = "",
  [0xdf] = "",

  [0xe0] = "",
  [0xe1] = "",
  [0xe2] = "",
  [0xe3] = "",
  [0xe4] = "",
  [0xe5] = "",
  [0xe6] = "",
  [0xe7] = "",
  [0xe8] = "",
  [0xe9] = "",
  [0xea] = "",
  [0xeb] = "",
  [0xec] = "",
  [0xed] = "",
  [0xee] = "",
  [0xef] = "",

  [0xf0] = "",
  [0xf1] = "",
  [0xf2] = "",
  [0xf3] = "",
  [0xf4] = "",
  [0xf5] = "",
  [0xf6] = "",
  [0xf7] = "",
  [0xf8] = "",
  [0xf9] = "",
  [0xfa] = "",
  [0xfb] = "",
  [0xfc] = "",
  [0xfd] = "",
  [0xfe] = "",
  [0xff] = "",
};

/**
 * @brief Scancodes and corresponding caracters when the scancode is two bytes
 */
char *scancode2[] = {
  [0x00] = "",
  [0x01] = "",
  [0x02] = "",
  [0x03] = "",
  [0x04] = "",
  [0x05] = "",
  [0x06] = "",
  [0x07] = "",
  [0x08] = "",
  [0x09] = "",
  [0x0a] = "",
  [0x0b] = "",
  [0x0c] = "",
  [0x0d] = "",
  [0x0e] = "",
  [0x0f] = "",

  [0x10] = "",
  [0x11] = "",
  [0x12] = "",
  [0x13] = "",
  [0x14] = "",
  [0x15] = "",
  [0x16] = "",
  [0x17] = "",
  [0x18] = "",
  [0x19] = "",
  [0x1a] = "",
  [0x1b] = "",
  [0x1c] = "", // enter
  [0x1d] = "", // control
  [0x1e] = "",
  [0x1f] = "",

  [0x20] = "",
  [0x21] = "",
  [0x22] = "",
  [0x23] = "",
  [0x24] = "",
  [0x25] = "",
  [0x26] = "",
  [0x27] = "",
  [0x28] = "",
  [0x29] = "",
  [0x2a] = "",
  [0x2b] = "",
  [0x2c] = "",
  [0x2d] = "",
  [0x2e] = "",
  [0x2f] = "",

  [0x30] = "",
  [0x31] = "",
  [0x32] = "",
  [0x33] = "",
  [0x34] = "",
  [0x35] = "/",
  [0x36] = "",
  [0x37] = "",
  [0x38] = "", // alt_gr
  [0x39] = "",
  [0x3a] = "",
  [0x3b] = "",
  [0x3c] = "",
  [0x3d] = "",
  [0x3e] = "",
  [0x3f] = "",

  [0x40] = "",
  [0x41] = "",
  [0x42] = "",
  [0x43] = "",
  [0x44] = "",
  [0x45] = "",
  [0x46] = "",
  [0x47] = "", // home
  [0x48] = "", // up
  [0x49] = "",
  [0x4a] = "",
  [0x4b] = "", // left
  [0x4c] = "",
  [0x4d] = "", // right
  [0x4e] = "",
  [0x4f] = "",

  [0x50] = "", // down
  [0x51] = "",
  [0x52] = "",
  [0x53] = "", // delete
  [0x54] = "",
  [0x55] = "",
  [0x56] = "",
  [0x57] = "",
  [0x58] = "",
  [0x59] = "",
  [0x5a] = "",
  [0x5b] = "", // super
  [0x5c] = "",
  [0x5d] = "",
  [0x5e] = "",
  [0x5f] = "",

  [0x60] = "",
  [0x61] = "",
  [0x62] = "",
  [0x63] = "",
  [0x64] = "",
  [0x65] = "",
  [0x66] = "",
  [0x67] = "",
  [0x68] = "",
  [0x69] = "",
  [0x6a] = "",
  [0x6b] = "",
  [0x6c] = "",
  [0x6d] = "",
  [0x6e] = "",
  [0x6f] = "",

  [0x70] = "",
  [0x71] = "",
  [0x72] = "",
  [0x73] = "",
  [0x74] = "",
  [0x75] = "",
  [0x76] = "",
  [0x77] = "",
  [0x78] = "",
  [0x79] = "",
  [0x7a] = "",
  [0x7b] = "",
  [0x7c] = "",
  [0x7d] = "",
  [0x7e] = "",
  [0x7f] = "",

  [0x80] = "",
  [0x81] = "",
  [0x82] = "",
  [0x83] = "",
  [0x84] = "",
  [0x85] = "",
  [0x86] = "",
  [0x87] = "",
  [0x88] = "",
  [0x89] = "",
  [0x8a] = "",
  [0x8b] = "",
  [0x8c] = "",
  [0x8d] = "",
  [0x8e] = "",
  [0x8f] = "",

  [0x90] = "",
  [0x91] = "",
  [0x92] = "",
  [0x93] = "",
  [0x94] = "",
  [0x95] = "",
  [0x96] = "",
  [0x97] = "",
  [0x98] = "",
  [0x99] = "",
  [0x9a] = "",
  [0x9b] = "",
  [0x9c] = "",
  [0x9d] = "",
  [0x9e] = "",
  [0x9f] = "",

  [0xa0] = "",
  [0xa1] = "",
  [0xa2] = "",
  [0xa3] = "",
  [0xa4] = "",
  [0xa5] = "",
  [0xa6] = "",
  [0xa7] = "",
  [0xa8] = "",
  [0xa9] = "",
  [0xaa] = "",
  [0xab] = "",
  [0xac] = "",
  [0xad] = "",
  [0xae] = "",
  [0xaf] = "",

  [0xb0] = "",
  [0xb1] = "",
  [0xb2] = "",
  [0xb3] = "",
  [0xb4] = "",
  [0xb5] = "",
  [0xb6] = "",
  [0xb7] = "",
  [0xb8] = "",
  [0xb9] = "",
  [0xba] = "",
  [0xbb] = "",
  [0xbc] = "",
  [0xbd] = "",
  [0xbe] = "",
  [0xbf] = "",

  [0xc0] = "",
  [0xc1] = "",
  [0xc2] = "",
  [0xc3] = "",
  [0xc4] = "",
  [0xc5] = "",
  [0xc6] = "",
  [0xc7] = "",
  [0xc8] = "",
  [0xc9] = "",
  [0xca] = "",
  [0xcb] = "",
  [0xcc] = "",
  [0xcd] = "",
  [0xce] = "",
  [0xcf] = "",

  [0xd0] = "",
  [0xd1] = "",
  [0xd2] = "",
  [0xd3] = "",
  [0xd4] = "",
  [0xd5] = "",
  [0xd6] = "",
  [0xd7] = "",
  [0xd8] = "",
  [0xd9] = "",
  [0xda] = "",
  [0xdb] = "",
  [0xdc] = "",
  [0xdd] = "",
  [0xde] = "",
  [0xdf] = "",

  [0xe0] = "",
  [0xe1] = "",
  [0xe2] = "",
  [0xe3] = "",
  [0xe4] = "",
  [0xe5] = "",
  [0xe6] = "",
  [0xe7] = "",
  [0xe8] = "",
  [0xe9] = "",
  [0xea] = "",
  [0xeb] = "",
  [0xec] = "",
  [0xed] = "",
  [0xee] = "",
  [0xef] = "",

  [0xf0] = "",
  [0xf1] = "",
  [0xf2] = "",
  [0xf3] = "",
  [0xf4] = "",
  [0xf5] = "",
  [0xf6] = "",
  [0xf7] = "",
  [0xf8] = "",
  [0xf9] = "",
  [0xfa] = "",
  [0xfb] = "",
  [0xfc] = "",
  [0xfd] = "",
  [0xfe] = "",
  [0xff] = "",
};
