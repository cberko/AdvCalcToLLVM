// AdvCalc - A simple calculator that supports variables and expressions
// Made by: @ceylanberk-tola and @ismail-tarik-erkan

/*
    In the parse functions, the return type is char * so that we can handle errors.
    If there is an error, the function returns "Error!".
    If there is no error, the function returns the result of the expression as a string.
    When we use these functions, we first check if the return value is "Error!".
    If it is, we return "Error!" directly.
    If it is not, we convert the string to an integer and use it.
*/

#include "stdlib.h"
#include "string.h"
#include <stdio.h>

// Define int as long long to support 64 bit integers
#define int long long

// Define the token types
typedef enum
{
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_MUL,
    TOKEN_AND,
    TOKEN_OR,
    TOKEN_XOR,
    TOKEN_LS,
    TOKEN_RS,
    TOKEN_LR,
    TOKEN_RR,
    TOKEN_NOT,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_COMMA,
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_ERROR,
    TOKEN_EOF,
    TOKEN_UNKNOWN,
    TOKEN_EQUALS
} TokenType;

// Define the token struct
typedef struct
{
    char *value;
    TokenType type;
} Token;

// Define the list struct
typedef struct List
{
    struct ListEntry *first;
} List;

// Define the list entry struct (linked list)
// This is used to store variables
typedef struct ListEntry
{
    struct ListEntry *next;
    int value;
    char *name;
} ListEntry;

// Define the lexer struct
typedef struct
{
    char *input;
    size_t input_size;
    size_t pos;
    char ch;
} Lexer;

Lexer *init_lexer(char *input);
Token *lexer_advance(Lexer *lexer);
Token *lexer_peek(Lexer *lexer);
void lex_identifier(Lexer *lexer, Token *token);
void lex_number(Lexer *lexer, Token *token);
void check_type(Token *token);
char *parse(char *input, List *list);
char *parse_expression(Lexer *lexer, List *list);
char *parse_term(Lexer *lexer, List *list);
char *parse_factor(Lexer *lexer, List *list);
char *parse_primary(Lexer *lexer, List *list);
Token *init_token(char *value, TokenType type);
List *init_list();
ListEntry *init_list_entry(char *name, int value);
void list_add(List *list, char *name, int value);
int list_get(List *list, char *name);
int check_equals(char *input);
int checkValid(Token *token);
int checkUnknown(char *input);
int checkParanthese(char *input);
void operation_helper(int* lhs, char* operation, int rhs);
int main_helper(int *is_pointer, char* operation, char* pointer_final, char* string_result, int *result, List* list, int* global_counter, Lexer* lexer, char* pointer_name);

int pointer_helper(int *is_pointer, char* operation, char* pointer_final, char* string_result, int *result, List* list, int* global_counter, Lexer* lexer, char* pointer_name) {
    if (checkValid(lexer_peek(lexer)) == 0)
    {
        return -1;
    }
    if(strcmp(operation, "add") == 0 || strcmp(operation, "sub") == 0) {
        string_result = parse_factor(lexer, list);
    }
    else if(strcmp(operation, "mul") == 0) {
        string_result = parse_primary(lexer, list);
    }
    if (strcmp(string_result, "Error!") == 0)
    {
        return -1;
    }
    else if (string_result[0] == '%')
    {
        if (*is_pointer == 1)
        {
            printf("%%%lld = %s i32 %s, %s\n", *global_counter, operation, pointer_name, string_result);
            operation_helper(result, operation, list_get(list, string_result));
            //*result += atoll(string_result);
            sprintf(pointer_final, "%%%lld", *global_counter);
            list_add(list, pointer_final, *result);
            return 0;
        }
        else
        {
            printf("%%%lld = %s i32 %lld, %s\n", *global_counter, operation, *result, string_result);
            operation_helper(result, operation, list_get(list, string_result));
            //*result += list_get(list, string_result);
            sprintf(pointer_final, "%%%lld", *global_counter);
            list_add(list, pointer_final, *result);
            return 1;
        }
    }
    else
    {
        if (*is_pointer == 1)
        {
            printf("%%%lld = add i32 %s, %lld\n", *global_counter, pointer_name, atoll(string_result));
            operation_helper(result, operation, atoll(string_result));
            //*result += atoll(string_result);
            sprintf(pointer_final, "%%%lld", *global_counter);
            list_add(list, pointer_final, *result);
            return 3;
        }
        else
        {
            printf("%%%lld = add i32 %lld, %lld\n", *global_counter, *result, atoll(string_result));
            operation_helper(result, operation, atoll(string_result));
            //*result += list_get(list, string_result);
            sprintf(pointer_final, "%%%lld", *global_counter);
            list_add(list, pointer_final, *result);
            return 2;
        }
    }
}

void operation_helper(int* lhs, char* operation, int rhs) {
    if(strcmp(operation, "add") == 0) {
        *lhs += rhs;
    }
    else if(strcmp(operation, "sub") == 0) {
        *lhs -= rhs;
    }
    else if(strcmp(operation, "mul") == 0) {
        *lhs *= rhs;
    }
}

int main_helper(int *is_pointer, char* operation, char* pointer_final, char* string_result, int *result, List* list, int* global_counter, Lexer* lexer, char* pointer_name)
{
    int res = pointer_helper(is_pointer, operation, pointer_final, string_result, result, list, global_counter, lexer, pointer_name);
    if (res == -1)
    {
        return -1;
    }
    else if (res == 0)
    {
        //printf("%%%lld = %s i32 %s, %s\n", *global_counter, operation, pointer_name, string_result);
    }
    else if (res == 1)
    {
        //printf("%%%lld = %s i32 %lld, %s\n", *global_counter, operation, *result, string_result);
    }
    else if (res == 2)
    {
        //printf("%%%lld = %s i32 %lld, %s\n", *global_counter, operation, list_get(list, string_result), pointer_name);
    }
    else if (res == 3)
    {
        //printf("%%%lld = %s i32 %lld, %lld\n", *global_counter, operation, list_get(list, string_result), *result);
    }
    else
    {
        return -1;
    }
}


int global_counter = 1;

int main()
{
    // Create a list to store variables
    List *list = init_list();
    FILE *file_ptr;
    char line[256];
    char lines_array[256][256];  // array of strings to store lines
    int line_count = 0;
    int i = 0;

    file_ptr = fopen("../input.txt", "r");  // open the file for reading
    if (file_ptr == NULL) {  // if the file cannot be opened, print an error message and exit
        printf("Error opening file!\n");
        return -1;
    }

    while (fgets(line, 100, file_ptr)) {  // read lines from the file until the end is reached
        strcpy(lines_array[line_count], line);  // copy the line into the array
        line_count++;  // increment the line count
    }

    fclose(file_ptr);  // close the file


    while (fgets(line, 100, file_ptr) != NULL) {  // read lines from the file until the end is reached
        strcpy(lines_array[line_count], line);  // copy the line into the array
        line_count++;  // increment the line count
    }

    fclose(file_ptr);
    while (i < line_count)
    {
        //setbuf(stdout, 0);
        //printf("> ");

        char* input = calloc(256, sizeof(char));
        sprintf(input, "%s", lines_array[i]);

        // Check if there are any unknown characters
        if (checkUnknown(input) == -1)
        {
            printf("Error!\n");
            continue;
        }

        // Check if the input is an assignment or an expression
        int equal_type = check_equals(input);

        if (equal_type == -2) // empty line or just comment
            continue;

        int check_paran = checkParanthese(input);

        if (check_paran == -1) // there is a mismatch in the number of left and right parentheses
        {
            printf("Error!\n");
            continue;
        }

        else if (equal_type == -1) // more than one equals sign or the equal sign is somewhere other than the second place
        {
            printf("Error!\n");
        }
        else if (equal_type == 1) // there is an equals sign, so it is an assignment
        {
            // Create a lexer and get the first token
            char *input_copy = strdup(input);
            Lexer *lexer = init_lexer(input_copy);
            Token *token = lexer_advance(lexer);                 // get the identifier
            Token *token2 = lexer_advance(lexer);                // get the equals sign
            char *string_result = parse_expression(lexer, list); // get the result of the expression
            int result = 0;
            int is_pointer = 0;
            char *pointer_name = calloc(1, 256);
            char *result_string2 = calloc(1, 256);
            if (strcmp(string_result, "Error!") == 0)
            {
                printf("Error!\n");
                continue;
            }
            else if (string_result[0] == '%') // If pointer
            {
                sprintf(pointer_name, "%s", string_result);
                result = list_get(list, pointer_name);
                is_pointer = 1;
            }
            else
            {
                result = atoll(string_result);
            }
            list_add(list, token->value, result); // add the variable to the list
            printf("%%%s = alloca i32\n", token->value);
            if (is_pointer == 1)
            {
                printf("store i32 %s, i32* %%%s\n", pointer_name, token->value);
            }
            else
            {
                printf("store i32 %lld, i32* %%%s\n", result, token->value);
            }
        }
        else // it is an expression
        {
            setbuf(stdout, 0);
            char *result = calloc(1, 256);
            result = parse(input, list);

            if (result[0] == '%')
            {
                printf("%lld\n", list_get(list, result));
            }
            else
            {
                printf("%s\n", parse(input, list));
            }
        }
        i++;
    }

    return 0;
}

int checkParanthese(char *input)
{
    // Check the number of left and right parentheses
    int left = 0;
    int right = 0;
    for (int i = 0; i < strlen(input); i++)
    {
        if (input[i] == '(')
            left++;
        else if (input[i] == ')')
            right++;
    }
    if (left == right)
        return 1;
    else
        return -1;
}

int checkValid(Token *token)
{
    // This is used to eliminate cases where 2 operators are next to each other
    if (token->type == TOKEN_AND ||
        token->type == TOKEN_OR ||
        token->type == TOKEN_PLUS ||
        token->type == TOKEN_MINUS ||
        token->type == TOKEN_MUL ||
        token->type == TOKEN_EOF ||
        token->type == TOKEN_COMMA ||
        token->type == TOKEN_RPAREN)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

int checkUnknown(char *input)
{
    // Check if there are any unknown characters
    char *input_copy = strdup(input);
    Lexer *lexer = init_lexer(input_copy);
    Token *token = lexer_advance(lexer);
    while (token->type != TOKEN_EOF)
    {
        if (token->type == TOKEN_UNKNOWN)
        {
            return -1;
        }
        token = lexer_advance(lexer);
    }
    return 1;
}

int check_equals(char *input)
{
    // Create a lexer and check if the first token is an identifier and the second is an equals sign
    // if there is more than one equals sign or the equal sign is somewhere other than the second place, return -1
    // if there is no equals sign, return 0
    // if there is an equals sign, return 1
    // if empty line or just comment return -2
    int result = 0;
    char *input_copy = strdup(input);
    Lexer *lexer = init_lexer(input_copy);
    Token *token = lexer_advance(lexer);

    if (token->type == TOKEN_EOF)
    {
        return -2;
    }

    if (token->type == TOKEN_IDENTIFIER)
    {
        Token *token2 = lexer_advance(lexer);
        if (token2->type == TOKEN_EQUALS)
        {
            result = 1;
        }
    }

    while (token->type != TOKEN_EOF)
    {
        if (token->type == TOKEN_EQUALS)
        {
            result = -1;
        }
        token = lexer_advance(lexer);
    }

    return result;
}

char *parse(char *input, List *list)
{
    // We dont actually need this function, but it is here for consistency
    Lexer *lexer = init_lexer(input);
    char *result = parse_expression(lexer, list);

    return result;
}

char *parse_expression(Lexer *lexer, List *list)
{
    // This function parses the expression for and and or
    int result = 0;
    char *string_result = parse_term(lexer, list);
    int is_pointer = 0;
    char *pointer_name = calloc(1, 256);

    // Check if there is an error
    if (strcmp(string_result, "Error!") == 0)
    {
        return string_result;
    }
    else if (string_result[0] == '%') // If pointer
    {
        Token *tmptoken = lexer_peek(lexer);
        if (tmptoken->type == TOKEN_AND || tmptoken->type == TOKEN_OR)
        {
            sprintf(pointer_name, "%s", string_result);
            result = list_get(list, pointer_name);
            is_pointer = 1;
        }
        else
        {
            sprintf(pointer_name, "%s", string_result);
            return pointer_name;
        }
    }
    else
    {
        result = atoll(string_result);
    }

    // Check if the next token is an and or an or
    // We use peek here because we dont want to advance the lexer if the next token is not an and or an or
    Token *token = lexer_peek(lexer);
    while (token->type == TOKEN_AND || token->type == TOKEN_OR)
    {
        // Read the operator token
        token = lexer_advance(lexer);
        if (token->type == TOKEN_AND)
        {
            // Check if the next token is valid
            if (checkValid(lexer_peek(lexer)) == 0)
            {
                return "Error!";
            }
            // Parse the next term
            string_result = parse_term(lexer, list);
            // Check if there is an error
            if (strcmp(string_result, "Error!") == 0)
            {
                return string_result;
            }
            else
            {
                // If there is no error, bitwise and the result with the previous result
                result &= atoll(string_result);
            }
        }
        else if (token->type == TOKEN_OR) // Same as AND operator
        {
            if (checkValid(lexer_peek(lexer)) == 0)
            {
                return "Error!";
            }
            string_result = parse_term(lexer, list);
            if (strcmp(string_result, "Error!") == 0)
            {
                return string_result;
            }
            else
            {
                result |= atoll(string_result);
            }
        }
        else
        {
            return "Error!";
        }
        token = lexer_peek(lexer);
    }

    // Convert the result to a string and return it
    char *result_string = calloc(1, 256);
    sprintf(result_string, "%lld", result);
    return result_string;
}

// Same as parse_expression
char *parse_term(Lexer *lexer, List *list)
{
    char *string_result = parse_factor(lexer, list);
    int result = 0;
    int is_pointer = 0;
    char *pointer_name = calloc(1, 256);
    char *pointer_final = calloc(1, 256);

    if (strcmp(string_result, "Error!") == 0)
    {
        return string_result;
    }
    else if (string_result[0] == '%') // If pointer
    {
        Token *tmptoken = lexer_peek(lexer);
        if (tmptoken->type == TOKEN_PLUS || tmptoken->type == TOKEN_MINUS)
        {
            sprintf(pointer_name, "%s", string_result);
            result = list_get(list, pointer_name);
            is_pointer = 1;
        }
        else
        {
            sprintf(pointer_name, "%s", string_result);
            return pointer_name;
        }
    }
    else
    {
        result = atoll(string_result);
    }

    Token *token = lexer_peek(lexer);

    while (token->type == TOKEN_PLUS || token->type == TOKEN_MINUS)
    {
        token = lexer_advance(lexer);

        if (token->type == TOKEN_PLUS)
        {
            int res = main_helper(&is_pointer, "add", pointer_final, string_result, &result, list, &global_counter, lexer, pointer_name);
            if (res == -1)
            {
                return "Error!";
            }
            global_counter++;
            is_pointer = 1;
        }
        else if (token->type == TOKEN_MINUS)
        {
            int res = main_helper(&is_pointer, "sub", pointer_final, string_result, &result, list, &global_counter, lexer, pointer_name);
            if (res == -1)
            {
                return "Error!";
            }
            global_counter++;
            is_pointer = 1;
        }
        else
        {
            return "Error!";
        }

        token = lexer_peek(lexer);
    }

    if (is_pointer == 1)
    {
        return pointer_final;
    }
    char *result_string = calloc(1, 256);
    sprintf(result_string, "%lld", result);
    return result_string;
}

// Same as parse_expression
char *parse_factor(Lexer *lexer, List *list)
{
    char *string_result = parse_primary(lexer, list);
    int result = 0;
    int is_pointer = 0;
    char *pointer_name = calloc(1, 256);
    char *pointer_final = calloc(1, 256);

    if (strcmp(string_result, "Error!") == 0)
    {
        return string_result;
    }
    else if (string_result[0] == '%') // If pointer
    {
        Token *tmptoken = lexer_peek(lexer);
        if (tmptoken->type == TOKEN_MUL)
        {
            sprintf(pointer_name, "%s", string_result);
            result = list_get(list, pointer_name);
            is_pointer = 1;
        }
        else
        {
            sprintf(pointer_name, "%s", string_result);
            return pointer_name;
        }
    }
    else
    {
        result = atoll(string_result);
    }

    Token *token = lexer_peek(lexer);

    while (token->type == TOKEN_MUL)
    {
        token = lexer_advance(lexer);

        if (token->type == TOKEN_MUL)
        {
            int res = main_helper(&is_pointer, "mul", pointer_final, string_result, &result, list, &global_counter, lexer, pointer_name);
            if (res == -1)
            {
                return "Error!";
            }
            global_counter++;
            is_pointer = 1;
        }
        else
        {
            return "Error!";
        }

        token = lexer_peek(lexer);
    }

    if (is_pointer == 1)
    {
        return pointer_final;
    }
    char *result_string = calloc(1, 256);
    sprintf(result_string, "%lld", result);
    return result_string;
}

char *parse_primary(Lexer *lexer, List *list)
{
    int result = 0;
    char *string_result = calloc(1, 256);
    Token *token = lexer_peek(lexer);
    token = lexer_advance(lexer);
    if (token->type == TOKEN_IDENTIFIER) // If the token is an identifier, get the value from the list
    {
        // result = list_get(list, token->value);
        printf("%%%lld = load i32, i32* %%%s\n", global_counter, token->value);
        char *result_string2 = calloc(1, 256);
        sprintf(result_string2, "%%%lld", global_counter);
        list_add(list, result_string2, list_get(list, token->value));
        global_counter++;
        return result_string2;
    }
    else if (token->type == TOKEN_NUMBER) // If the token is a number, get the value from the token
    {
        result = atoll(token->value);
    }
    else if (token->type == TOKEN_LPAREN) // If the token is a left parenthesis, parse the expression
    {
        string_result = parse_expression(lexer, list);
        token = lexer_advance(lexer);
        if (strcmp(string_result, "Error!") == 0)
        {
            return string_result;
        }
        else if (string_result[0] == '%')
        {
            return string_result;
        }
        else
        {
            result = atoll(string_result);
        }
        if (token->type != TOKEN_RPAREN) // If the token is not a right parenthesis, return an error
        {
            return "Error!";
        }
    }
    else if (token->type == TOKEN_NOT) // If the token is a NOT operator, there will be a left parenthesis, parse the expression, and then a right parenthesis
    {
        token = lexer_advance(lexer);
        if (token->type != TOKEN_LPAREN)
        {
            return "Error!";
        }
        string_result = parse_expression(lexer, list);
        if (strcmp(string_result, "Error!") == 0)
        {
            return string_result;
        }
        else
        {
            result = ~atoll(string_result);
        }
        token = lexer_advance(lexer);
        if (token->type != TOKEN_RPAREN)
        {
            return "Error!";
        }
    }
    else if (token->type == TOKEN_XOR || token->type == TOKEN_RR || token->type == TOKEN_RS || token->type == TOKEN_LR || token->type == TOKEN_LS)
    {
        // If the token is a function other than and operator, there will be a left parenthesis, parse the expression, a comma, parse the expression, and then a right parenthesis
        int ttype = token->type;
        token = lexer_advance(lexer);
        if (token->type != TOKEN_LPAREN)
        {
            return "Error!";
        }
        int result2 = 0;
        string_result = parse_expression(lexer, list);
        if (strcmp(string_result, "Error!") == 0)
        {
            return string_result;
        }
        else
        {
            result2 = atoll(string_result);
        }
        token = lexer_advance(lexer);
        if (token->type != TOKEN_COMMA)
        {
            return "Error!";
        }
        int result3 = 0;
        string_result = parse_expression(lexer, list);
        if (strcmp(string_result, "Error!") == 0)
        {
            return string_result;
        }
        else
        {
            result3 = atoll(string_result);
        }
        token = lexer_advance(lexer);
        if (token->type != TOKEN_RPAREN)
        {
            return "Error!";
        }
        // Do the operation based on the function
        if (ttype == TOKEN_XOR)
        {
            result = result2 ^ result3;
        }
        else if (ttype == TOKEN_RR)
        {
            result = result2 >> result3;
        }
        else if (ttype == TOKEN_RS)
        {
            result = result2 >> result3;
        }
        else if (ttype == TOKEN_LR)
        {
            result = result2 << result3;
        }
        else if (ttype == TOKEN_LS)
        {
            result = result2 << result3;
        }
    }

    // if the next token is not an operator, return an error
    if (!(lexer_peek(lexer)->type == TOKEN_AND ||
          lexer_peek(lexer)->type == TOKEN_OR ||
          lexer_peek(lexer)->type == TOKEN_PLUS ||
          lexer_peek(lexer)->type == TOKEN_MINUS ||
          lexer_peek(lexer)->type == TOKEN_MUL ||
          lexer_peek(lexer)->type == TOKEN_EOF ||
          lexer_peek(lexer)->type == TOKEN_COMMA ||
          lexer_peek(lexer)->type == TOKEN_RPAREN))
    {
        return "Error!";
    }

    // change result to string and return it
    char *result_string = calloc(1, 256);
    sprintf(result_string, "%lld", result);
    return result_string;
}

List *init_list()
{
    List *list = calloc(1, sizeof(List));
    list->first = NULL;

    return list;
}

ListEntry *init_list_entry(char *name, int value)
{
    ListEntry *entry = calloc(1, sizeof(ListEntry));
    entry->name = name;
    entry->value = value;
    entry->next = NULL;

    return entry;
}

// Add a new entry to the list or update the value of an existing entry
void list_add(List *list, char *name, int value)
{
    ListEntry *entry = init_list_entry(name, value);

    if (list->first == NULL)
    {
        list->first = entry;
    }
    else
    {
        ListEntry *current = list->first;
        if (strcmp(current->name, name) == 0)
        {
            current->value = value;
            return;
        }
        while (current->next != NULL)
        {
            if (strcmp(current->name, name) == 0)
            {
                current->value = value;
                return;
            }
            current = current->next;
        }
        if (strcmp(current->name, name) == 0)
        {
            current->value = value;
            return;
        }

        current->next = entry;
    }
}

// Get the value of an entry in the list by name or return 0 if the entry does not exist
int list_get(List *list, char *name)
{
    ListEntry *current = list->first;
    while (current != NULL)
    {
        if (strcmp(current->name, name) == 0)
        {
            return current->value;
        }

        current = current->next;
    }

    return 0;
}

Lexer *init_lexer(char *input)
{
    Lexer *lexer = calloc(1, sizeof(Lexer));
    lexer->input = input;
    lexer->input_size = strlen(input);
    lexer->pos = 0;
    lexer->ch = lexer->input[lexer->pos];

    return lexer;
}

// This function is used to get the next token from the lexer without advancing the lexer
Token *lexer_peek(Lexer *lexer)
{
    // save lexer state
    size_t pos = lexer->pos;
    char ch = lexer->ch;
    // get next token
    Token *token = lexer_advance(lexer);
    // restore lexer state
    lexer->pos = pos;
    lexer->ch = ch;

    return token;
}

// This function is used to advance the lexer to the next token
Token *lexer_advance(Lexer *lexer)
{
    // init token
    Token *token = init_token('\0', TOKEN_UNKNOWN);

    // skip whitespace
    while (lexer->ch && lexer->ch == ' ')
    {
        lexer->pos++;
        lexer->ch = lexer->input[lexer->pos];
    }

    // check for EOF
    // if there is a comment character('%'), set the token type to TOKEN_EOF so that the parser will stop
    if (lexer->ch == '\0' || lexer->ch == '\n' || lexer->ch == '%')
    {
        token->type = TOKEN_EOF;
    }
    else if (lexer->ch >= 48 && lexer->ch <= 57) // check for number
    {
        lex_number(lexer, token);
    }
    else if ((lexer->ch >= 65 && lexer->ch <= 90) || (lexer->ch >= 97 && lexer->ch <= 122)) // check for identifier and functions
    {
        lex_identifier(lexer, token);
    }
    else // check for operators
    {
        token->value = calloc(2, sizeof(char));
        token->value[0] = lexer->ch;
        token->value[1] = '\0';

        switch (lexer->ch)
        {
            case '+':
                token->type = TOKEN_PLUS;
                break;
            case '-':
                token->type = TOKEN_MINUS;
                break;
            case '*':
                token->type = TOKEN_MUL;
                break;
            case '&':
                token->type = TOKEN_AND;
                break;
            case '|':
                token->type = TOKEN_OR;
                break;
            case '(':
                token->type = TOKEN_LPAREN;
                break;
            case ')':
                token->type = TOKEN_RPAREN;
                break;
            case ',':
                token->type = TOKEN_COMMA;
                break;
            case '=':
                token->type = TOKEN_EQUALS;
                break;
            default:
                token->type = TOKEN_UNKNOWN;
                break;
        }

        lexer->pos++;
        lexer->ch = lexer->input[lexer->pos];
    }

    return token;
}

void lex_identifier(Lexer *lexer, Token *token)
{
    // while current char is a letter, add it to token value
    int sz = 0;
    while ((lexer->ch >= 65 && lexer->ch <= 90) || (lexer->ch >= 97 && lexer->ch <= 122))
    {
        sz++;
        lexer->pos++;
        lexer->ch = lexer->input[lexer->pos];
    }

    // allocate memory for token value
    token->value = calloc(sz + 1, sizeof(char));
    // copy token value from input
    strncpy(token->value, lexer->input + lexer->pos - sz, sz);
    // set last char to null
    token->value[sz] = '\0';
    // set token type
    check_type(token);
}

void check_type(Token *token)
{
    // check if tokens value is equals to: "xor", "ls", "rs", "lr", "rr", "not"
    // if it is, then set token type to function
    // else set token type to identifier
    if (strcmp(token->value, "xor") == 0)
    {
        token->type = TOKEN_XOR;
    }
    else if (strcmp(token->value, "ls") == 0)
    {
        token->type = TOKEN_LS;
    }
    else if (strcmp(token->value, "rs") == 0)
    {
        token->type = TOKEN_RS;
    }
    else if (strcmp(token->value, "lr") == 0)
    {
        token->type = TOKEN_LR;
    }
    else if (strcmp(token->value, "rr") == 0)
    {
        token->type = TOKEN_RR;
    }
    else if (strcmp(token->value, "not") == 0)
    {
        token->type = TOKEN_NOT;
    }
    else
    {
        token->type = TOKEN_IDENTIFIER;
    }
}

void lex_number(Lexer *lexer, Token *token)
{
    // Continue until we find a non-digit character
    int sz = 0;
    while (lexer->ch >= 48 && lexer->ch <= 57)
    {
        sz++;
        lexer->pos++;
        lexer->ch = lexer->input[lexer->pos];
    }

    // Allocate a buffer large enough to hold the number
    token->value = calloc(sz + 1, sizeof(char));
    // Copy the number into the buffer
    strncpy(token->value, lexer->input + lexer->pos - sz, sz);
    // Add a null terminator
    token->value[sz] = '\0';
    token->type = TOKEN_NUMBER;
}

Token *init_token(char *value, TokenType type)
{
    Token *token = calloc(1, sizeof(Token));
    token->value = value;
    token->type = type;

    return token;
}