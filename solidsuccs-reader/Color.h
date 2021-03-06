#ifndef READER_COLOR_H
#define READER_COLOR_H

#ifndef READER_NO_COLOR_GRAPHICS
#define READER_COLOR_RESET               "\e[0m"
#define READER_COLOR_B_ON                "\e[1m"
#define READER_COLOR_I_ON                "\e[3m"
#define READER_COLOR_U_ON                "\e[4m"
#define READER_COLOR_INVERSE_ON          "\e[7m"
#define READER_COLOR_STRIKETHROUGH_ON    "\e[9m"
#define READER_COLOR_B_OFF               "\e[21m"
#define READER_COLOR_I_OFF               "\e[23m"
#define READER_COLOR_U_OFF               "\e[24m"
#define READER_COLOR_INVERSE_OFF         "\e[27m"
#define READER_COLOR_STRIKETHROUGH_OFF   "\e[29m"
#define READER_COLOR_FG_BLACK            "\e[30m"
#define READER_COLOR_FG_RED              "\e[31m"
#define READER_COLOR_FG_GREEN            "\e[32m"
#define READER_COLOR_FG_YELLOW           "\e[33m"
#define READER_COLOR_FG_BLUE             "\e[34m"
#define READER_COLOR_FG_MAGENTA          "\e[35m"
#define READER_COLOR_FG_CYAN             "\e[36m"
#define READER_COLOR_FG_WHITE            "\e[37m"
#define READER_COLOR_FG_DEFAULT          "\e[39m"
#define READER_COLOR_BG_BLACK            "\e[40m"
#define READER_COLOR_BG_RED              "\e[41m"
#define READER_COLOR_BG_GREEN            "\e[42m"
#define READER_COLOR_BG_YELLOW           "\e[43m"
#define READER_COLOR_BG_BLUE             "\e[44m"
#define READER_COLOR_BG_MAGENTA          "\e[45m"
#define READER_COLOR_BG_CYAN             "\e[46m"
#define READER_COLOR_BG_WHITE            "\e[47m"
#define READER_COLOR_BG_DEFAULT          "\e[49m"
#else
#define READER_COLOR_RESET               ""
#define READER_COLOR_B_ON                ""
#define READER_COLOR_I_ON                ""
#define READER_COLOR_U_ON                ""
#define READER_COLOR_INVERSE_ON          ""
#define READER_COLOR_STRIKETHROUGH_ON    ""
#define READER_COLOR_B_OFF               ""
#define READER_COLOR_I_OFF               ""
#define READER_COLOR_U_OFF               ""
#define READER_COLOR_INVERSE_OFF         ""
#define READER_COLOR_STRIKETHROUGH_OFF   ""
#define READER_COLOR_FG_BLACK            ""
#define READER_COLOR_FG_RED              ""
#define READER_COLOR_FG_GREEN            ""
#define READER_COLOR_FG_YELLOW           ""
#define READER_COLOR_FG_BLUE             ""
#define READER_COLOR_FG_MAGENTA          ""
#define READER_COLOR_FG_CYAN             ""
#define READER_COLOR_FG_WHITE            ""
#define READER_COLOR_FG_DEFAULT          ""
#define READER_COLOR_BG_BLACK            ""
#define READER_COLOR_BG_RED              ""
#define READER_COLOR_BG_GREEN            ""
#define READER_COLOR_BG_YELLOW           ""
#define READER_COLOR_BG_BLUE             ""
#define READER_COLOR_BG_MAGENTA          ""
#define READER_COLOR_BG_CYAN             ""
#define READER_COLOR_BG_WHITE            ""
#define READER_COLOR_BG_DEFAULT          ""
#endif

#endif //READER_COLOR_H
