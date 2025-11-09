#pragma once
#include <deque>
#include <string>
#include <iostream>

struct Token {
    int type;
    std::string text;
};

extern std::deque<Token> token_queue;
void print_token_queue();