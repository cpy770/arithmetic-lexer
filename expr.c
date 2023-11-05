
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum {
    OPERATOR,
    NUMBER,
    LEFT_PAREN,
    RIGHT_PAREN
} token_type;

typedef struct Token {
    token_type type;
    char* value;
    struct Token* next;
} Token;

void bind_token(Token** head, Token** tail, token_type type, char* value) {
    Token* new_token = (Token*) malloc(sizeof(Token));
    new_token->type = type;
    new_token->value = value;
    new_token->next = NULL;

    if (*head == NULL) {
        *head = new_token;
        *tail = new_token;
    } else {
        (*tail)->next = new_token;
        *tail = new_token;
    }
}

void free_tokens(Token* head) {
    Token* current = head;
    Token* next;

    while (current != NULL) {
        next = current->next;
        free(current->value);
        free(current);
        current = next;
    }
}

void show_tokens(Token* head) {
    Token* current = head;

    while (current != NULL) {
        switch (current->type) {
            case OPERATOR:
                printf("OPERATOR: %s\n", current->value);
                break;
            case NUMBER:
                printf("NUMBER: %s\n", current->value);
                break;
            case LEFT_PAREN:
                printf("LEFT_PAREN\n");
                break;
            case RIGHT_PAREN:
                printf("RIGHT_PAREN\n");
                break;
        }
        current = current->next;
    }
}

void lex_expression(char* expression, Token** head, Token** tail) {
    char buffer[256];
    int buffer_index = 0;
    int i = 0;

    while (expression[i] != '\0') {
        if (isspace(expression[i])) {
            i++;
            continue;
        }

        if (expression[i] == '+' || expression[i] == '-' || expression[i] == '*' || expression[i] == '/') {
            buffer[buffer_index] = '\0';
            bind_token(head, tail, NUMBER, strdup(buffer));
            buffer_index = 0;
            bind_token(head, tail, OPERATOR, (char[]) {expression[i], '\0'});
        } else if (isdigit(expression[i])) {
            buffer[buffer_index++] = expression[i];
        } else if (expression[i] == '(') {
            bind_token(head, tail, LEFT_PAREN, NULL);
        } else if (expression[i] == ')') {
            buffer[buffer_index] = '\0';
            bind_token(head, tail, NUMBER, strdup(buffer));
            buffer_index = 0;
            bind_token(head, tail, RIGHT_PAREN, NULL);
        }

        i++;
    }

    if (buffer_index > 0) {
        buffer[buffer_index] = '\0';
        bind_token(head, tail, NUMBER, strdup(buffer));
    }
}

int main() {
    char expression[256];
    Token* head = NULL;
    Token* tail = NULL;

    printf("Enter a mathematical expression: ");
    scanf("%[^\n]", expression);

    lex_expression(expression, &head, &tail);
    show_tokens(head);
    free_tokens(head);

    return 0;
}
