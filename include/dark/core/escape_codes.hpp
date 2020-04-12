#if !defined(AMT_TERMINAL_ENGINE_ESCAPE_CODES_HPP)
#define AMT_TERMINAL_ENGINE_ESCAPE_CODES_HPP

#include <array>
#include <string>

namespace amt::core{
    
    constexpr std::array<char const*, 256> number_to_str = {
        "0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
        "10", "11", "12", "13", "14", "15", "16", "17", "18", "19",
        "20", "21", "22", "23", "24", "25", "26", "27", "28", "29",
        "30", "31", "32", "33", "34", "35", "36", "37", "38", "39",
        "40", "41", "42", "43", "44", "45", "46", "47", "48", "49",
        "50", "51", "52", "53", "54", "55", "56", "57", "58", "59",
        "60", "61", "62", "63", "64", "65", "66", "67", "68", "69",
        "70", "71", "72", "73", "74", "75", "76", "77", "78", "79",
        "80", "81", "82", "83", "84", "85", "86", "87", "88", "89",
        "90", "91", "92", "93", "94", "95", "96", "97", "98", "99",
        "100", "101", "102", "103", "104", "105", "106", "107", "108", "109",
        "110", "111", "112", "113", "114", "115", "116", "117", "118", "119",
        "120", "121", "122", "123", "124", "125", "126", "127", "128", "129",
        "130", "131", "132", "133", "134", "135", "136", "137", "138", "139",
        "140", "141", "142", "143", "144", "145", "146", "147", "148", "149",
        "150", "151", "152", "153", "154", "155", "156", "157", "158", "159",
        "160", "161", "162", "163", "164", "165", "166", "167", "168", "169",
        "170", "171", "172", "173", "174", "175", "176", "177", "178", "179",
        "180", "181", "182", "183", "184", "185", "186", "187", "188", "189",
        "190", "191", "192", "193", "194", "195", "196", "197", "198", "199",
        "200", "201", "202", "203", "204", "205", "206", "207", "208", "209",
        "210", "211", "212", "213", "214", "215", "216", "217", "218", "219",
        "220", "221", "222", "223", "224", "225", "226", "227", "228", "229",
        "230", "231", "232", "233", "234", "235", "236", "237", "238", "239",
        "240", "241", "242", "243", "244", "245", "246", "247", "248", "249",
        "250", "251", "252", "253", "254", "255"
    };

    enum class escape_codes : unsigned char{
        RESET                   = 0,
        BOLD                    = 1,
        FAINT                   = 2,
        ITALIC                  = 3,
        UNDERLINE               = 4,
        BLINK                   = 5,
        REVERSE                 = 7,
        HIDE                    = 8,
        BOLD_OFF                = 22,
        ITALIC_OFF              = 23,
        UNDERLINE_OFF           = 24,
        BLINK_OFF               = 25,
        REVERSE_OFF             = 26,
        SHOW                    = 28,
        DARK_FG_BLACK           = 30,
        DARK_FG_RED             = 31,
        DARK_FG_GREEN           = 32,
        DARK_FG_YELLOW          = 33,
        DARK_FG_BLUE            = 34,
        DARK_FG_MAGENTA         = 35,
        DARK_FG_CYAN            = 36,
        DARK_FG_WHITE           = 37,
        DARK_FONT_COLOR         = 38,
        DARK_BG_BLACK           = 40,
        DARK_BG_RED             = 41,
        DARK_BG_GREEN           = 42,
        DARK_BG_YELLOW          = 43,
        DARK_BG_BLUE            = 44,
        DARK_BG_MAGENTA         = 45,
        DARK_BG_CYAN            = 46,
        DARK_BG_WHITE           = 47,
        DEFAULT_FONT_COLOR      = 49,
        DARK_BACKGROUND_COLOR   = 48,
        DEFAULT_BACKGROUND_COLOR= 49,
        BRIGHT_FG_BLACK         = 90,
        BRIGHT_FG_RED           = 91,
        BRIGHT_FG_GREEN         = 92,
        BRIGHT_FG_YELLOW        = 93,
        BRIGHT_FG_BLUE          = 94,
        BRIGHT_FG_MAGENTA       = 95,
        BRIGHT_FG_CYAN          = 96,
        BRIGHT_FG_WHITE         = 97,
        BRIGHT_BG_BLACK         = 100,
        BRIGHT_BG_RED           = 101,
        BRIGHT_BG_GREEN         = 102,
        BRIGHT_BG_YELLOW        = 103,
        BRIGHT_BG_BLUE          = 104,
        BRIGHT_BG_MAGENTA       = 105,
        BRIGHT_BG_CYAN          = 106,
        BRIGHT_BG_WHITE         = 107,
    };

    inline static constexpr char escape_seq[3]  = "\x1b[";
    inline static constexpr char reset[4]       = "\x1b[m";
    inline static constexpr char clear_screen[6]= "\x1b[2J";
    inline static constexpr char clear_line[4]  = "\x1b[K";
    inline static constexpr char save_pos[4]    = "\x1b[s";
    inline static constexpr char restore_pos[4] = "\x1b[u";


} // namespace amt::core

namespace amt{
    
    using amt::core::escape_codes;
    using amt::core::escape_seq;
    using amt::core::reset;
    using amt::core::clear_screen;
    using amt::core::clear_line;
    using amt::core::save_pos;
    using amt::core::restore_pos;

} // namespace amt


#endif // AMT_TERMINAL_ENGINE_ESCAPE_CODES_HPP
