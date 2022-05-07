# include <stdint.h>
# include <string.h>
# include <stdlib.h>
# include <stdio.h>

// required in this code

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t ui8;
typedef uint16_t ui16;
typedef uint32_t ui32;
typedef uint64_t ui64;

typedef ui8 bool;
typedef char* string;

# define true                           ((bool) 1)
# define false                          ((bool) 0)

# define ERR_ERROR                      "Error!"
# define ABORT_IF_NULL_PTR(ptr)         if (ptr == NULL) abort()

# define OVERLFOW_BUFF_LEN              ( 16 )

// required for generating code

# define TAB                            "  "
# define NWL                            ";\n"
# define PRINT_TABS(j, tabs, c_code)    for (j = 0; j < tabs; j++) strcat (c_code, TAB)

# define REPLACE_INC                    "*p += "               // [+]{0:x} - 7+
# define REPLACE_DEC                    "*p -= "               // [-]{0:x} - 7+
# define REPLACE_RSHIFT                 "p += "                // [>]{0:y} - 6+
# define REPLACE_LSHIFT                 "p -= "                // [<]{0:y} - 6+
# define REPLACE_LOOP_STRT              "if (*p) do {\n"       // [        - 13
# define REPLACE_LOOP_STOP              "} while (*p);\n"      // ]        - 14
# define REPLACE_I                      "*p = getchar();\n"    // ,        - 16
# define REPLACE_O                      "putchar(*p);\n"       // .        - 13

/**
 * @brief Create a new string of size len using calloc
 * @param len The string length
 * @return char* Pointer to heap allocated string. Remember to free it.
 */
string new_string (ui64 len)
{
    string str =  (string) calloc (len + OVERLFOW_BUFF_LEN, sizeof (char));
    ABORT_IF_NULL_PTR (str);
    return str;
}

/**
 * @brief Remove characters that are not valid bf instructions
 * @param src The char array containing the raw source code
 * @return char* Pointer to heap allocated stripped source. Remember to free it.
 */
string bf_strip (const string src)
{
    ui64 i, len;
    string stripped_src, stripped_src_ptr;

    len = strlen (src);

    stripped_src = new_string (len);
    stripped_src_ptr = stripped_src;

    for (i = 0; i < len; i++)
        if (strchr ("+-><.,[]", src[i]))
             *(stripped_src_ptr++) = src[i];
    *stripped_src_ptr = 0;

    return stripped_src;
}

/**
 * @brief Validates the brainfuck code.
 *
 * This function only checks for proper closing of loops.
 *
 * @param src The char array containing the stripped source code.
 * @return bool 1 if valid, 0 otherwise (macroed as true and false respectively).
 */
bool bf_validate (const string src)
{
    ui64 i, len;
    i64 count;

    len = strlen (src);
    count = 0;

    for (i = 0; i < len; i++) {
        if (src[i] == '[')
            count++;
        else if (src[i] == ']')
            count--;
    }
    return (bool) !count;
}

/**
 * @brief Removes annihilating occurance of instructions
 *
 * Eg: if input is "--+++", post optimization will be "+"
 *
 * @param src The char array containing the validated source code.
 * @return char* Pointer to heap allocated optimized source. Remember to free it.
 */
string bf_optimize (const string src)
{
    ui64 len;
    string opt_stack, opt_stack_top, src_ptr;
    char c;

    len = strlen (src);
    src_ptr = src;

    opt_stack = new_string (len);
    opt_stack_top = opt_stack;

    # define stk_top (opt_stack_top - 1)

    c = *(src_ptr++);
    while (c != 0) {
        if (opt_stack_top == opt_stack)
            *(opt_stack_top++) = c;
        else if (*stk_top == '+' && c == '-')
            opt_stack_top--;
        else if (*stk_top == '-' && c == '+')
            opt_stack_top--;
        else if (*stk_top == '>' && c == '<')
            opt_stack_top--;
        else if (*stk_top == '<' && c == '>')
            opt_stack_top--;
        else if (*stk_top == '[' && c == ']')
            opt_stack_top--;
        else
            *(opt_stack_top++) = c;
        c = *(src_ptr++);
    }

    *(opt_stack_top) = 0;
    return opt_stack;
}

/**
 * @brief Count occurances of a character in a string
 */
ui64 count (const string str, const char c)
{
    ui64 i, count, len;

    count = 0;
    len = strlen (str);

    for (i = 0; i < len; i++)
        if (str[i] == c)
            count++;
    return count;
}

/**
 * @brief Count consecutive occurances of a character in a string
 */
ui64 block_count (const string str, const char c)
{
    ui64 i, count, block_cnt, len;

    count = 0;
    block_cnt = 0;
    len = strlen (str);

    for (i = 0; i < len + 1; i++) {
        if (str[i] == c)
            count++;
        else if (count > 0) {
            block_cnt++;
            count = 0;
        }
    }
    return count;
}

/**
 * @brief Converts optimized bf code to C source code
 */
string bf_replace_to_c (const string opt_src)
{
    ui64 i, j, tabs, count_operations, len;
    string c_code;
    char c;

    tabs = 0;
    count_operations = 0;
    len = strlen (opt_src);

    // allocating memory for c code
    c_code = new_string (
          block_count (opt_src, '+') * strlen (REPLACE_INC "1000" NWL)
        + block_count (opt_src, '-') * strlen (REPLACE_DEC "1000" NWL)
        + block_count (opt_src, '>') * strlen (REPLACE_RSHIFT "1000" NWL)
        + block_count (opt_src, '<') * strlen (REPLACE_LSHIFT "1000" NWL)
        + count (opt_src, '[') * strlen (REPLACE_LOOP_STRT)
        + count (opt_src, ']') * strlen (REPLACE_LOOP_STOP)
        + count (opt_src, '.') * strlen (REPLACE_I)
        + count (opt_src, ',') * strlen (REPLACE_O)
    );

    for (i = 0; i < len; i++) {
        c = opt_src[i];
        count_operations = 0;
        if (c == '+' || c == '-' || c == '>' || c == '<') {
            PRINT_TABS (j, tabs, c_code);
            while (opt_src[i] == c) {
                count_operations++;
                i++;
            }
            i--;
            char count_buffer[65] = { 0 };
            if (c == '+') {
                strcat (c_code, REPLACE_INC);
                sprintf (count_buffer, "%ld", count_operations);
                strcat (c_code, count_buffer);
                strcat (c_code, NWL);
            } else if (c == '-') {
                strcat (c_code, REPLACE_DEC);
                sprintf (count_buffer, "%ld", count_operations);
                strcat (c_code, count_buffer);
                strcat (c_code, NWL);
            } else if (c == '>') {
                strcat (c_code, REPLACE_RSHIFT);
                sprintf (count_buffer, "%ld", count_operations);
                strcat (c_code, count_buffer);
                strcat (c_code, NWL);
            } else if (c == '<') {
                strcat (c_code, REPLACE_LSHIFT);
                sprintf (count_buffer, "%ld", count_operations);
                strcat (c_code, count_buffer);
                strcat (c_code, NWL);
            }
        } else if (c == ',') {
            PRINT_TABS (j, tabs, c_code);
            strcat (c_code, REPLACE_I);
        } else if (c == '.') {
            PRINT_TABS (j, tabs, c_code);
            strcat (c_code, REPLACE_O);
        } else if (c == '[') {
            PRINT_TABS (j, tabs, c_code);
            strcat (c_code, REPLACE_LOOP_STRT);
            tabs++;
        } else if (c == ']') {
            tabs--;
            PRINT_TABS (j, tabs, c_code);
            strcat (c_code, REPLACE_LOOP_STOP);
        }
    }

    return c_code;
}

/**
 * @brief Wrapper to do all the stuff of translation
 */
string brainfuck_to_c (const string src)
{
    string stripped_src, opt_src, c_code;

    // remove non-code characters
    stripped_src = bf_strip (src);

    # ifdef DEBUG
        printf ("striped:   %s\n", stripped_src);
    # endif

    // validate
    if (!bf_validate (stripped_src))
        return ERR_ERROR;

    // optimize
    # ifndef UNOPTIMISE
        opt_src = bf_optimize (stripped_src);
        free (stripped_src);
    # else
        opt_src = stripped_src;
    # endif

    # ifdef DEBUG
        printf ("optimised: %s\n", opt_src);
    # endif

    // validate
    if (!bf_validate (opt_src))
        return ERR_ERROR;

    // replace codes
    c_code = bf_replace_to_c (opt_src);

    free (opt_src);

    # ifdef DEBUG
        printf ("\nc_code:\n%s\n", c_code);
    # endif

    return c_code;
}

int main (int argsc, char **argsv) {
    string c_code;
    if (argsc > 1) {
        c_code = brainfuck_to_c (argsv[1]);
        # ifndef DEBUG
            printf ("%s\n", c_code);
        # endif
        exit (0);
    } else {
        fprintf (stderr, "error: no args provided\n");
        printf ("USAGE: bfc \"[brainfuck_code]\"\n");
        exit (1);
    }
    return 0;
}
