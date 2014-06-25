#ifndef UTILS_EXCEPTION_H
#define UTILS_EXCEPTION_H
#include <stdexcept>
#include <sstream>
#include <string>
#include <iostream>

#ifndef READER_NO_COLOR_GRAPHICS
#define READER_C_RESET               "\033[0m"
#define READER_C_B_ON                "\033[1m"
#define READER_C_FG_RED              "\033[31m"
#define READER_C_FG_GREEN            "\033[32m"
#define READER_C_FG_YELLOW           "\033[33m"
#define READER_C_FG_BLUE             "\033[34m"
#define READER_C_FG_DEFAULT          "\033[39m"
#else
#define READER_C_RESET               ""
#define READER_C_B_ON                ""
#define READER_C_FG_RED              ""
#define READER_C_FG_GREEN            ""
#define READER_C_FG_YELLOW           ""
#define READER_C_FG_BLUE             ""
#define READER_C_FG_DEFAULT          ""
#endif

#define READER_EXCEPTION(MESSAGE)					\
  {									\
    std::cout << READER_C_B_ON;						\
    std::cout << "Exception " << READER_C_FG_GREEN << "caught";		\
    std::cout << READER_C_FG_DEFAULT << ": " << MESSAGE << endl ;	\
    std::cout << "\t" << READER_C_B_ON << READER_C_FG_BLUE << __FILE__;	\
    std::cout << READER_C_FG_DEFAULT << ":";				\
    std::cout << READER_C_FG_BLUE << __LINE__ <<READER_C_FG_DEFAULT << endl; \
    std::cout << "\t" << READER_C_FG_YELLOW << __func__;		\
    std::cout << READER_C_RESET << endl;				\
    throw;								\
  }
#define READER_NEW_EXCEPTION(MESSAGE)					\
  {									\
    std::cout << READER_C_B_ON << "Exception " <<READER_C_FG_RED<< "thrown"; \
    std::cout << READER_C_FG_DEFAULT << ": " << MESSAGE << endl ;	\
    std::cout << "\t" << READER_C_B_ON << READER_C_FG_BLUE<<__FILE__;	\
    std::cout << READER_C_FG_DEFAULT << ":";				\
    std::cout << READER_C_FG_BLUE <<__LINE__ <<READER_C_FG_DEFAULT << endl; \
    std::cout << "\t" << READER_C_FG_YELLOW << __func__;		\
    std::cout << READER_C_RESET<<endl;					\
    stringstream ss; ss << MESSAGE; throw std::runtime_error(ss.str());	\
  }

#define READER_TRY try {
#define READER_CATCH } catch (exception &e) {READER_EXCEPTION("");} catch (int i) {READER_EXCEPTION("");} 
#define READER_CATCH_MSG(MESSAGE) } catch (exception &e) {READER_EXCEPTION(MESSAGE);} catch (int i) {READER_EXCEPTION(MESSAGE);}

#endif // UTILS_EXCEPTION_H
