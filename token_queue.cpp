#include "token_queue.h"
std::deque<Token> token_queue;

void print_token_queue() {
    if (token_queue.empty()) {
        std::cout << "token_queue is empty.\n";
        return;
    }

    std::cout << "Current token_queue contents:\n";
    for (size_t i = 0; i < token_queue.size(); ++i) {
        const Token& t = token_queue[i];
        std::cout << "  [" << i << "] type: " << t.type 
                  << ", text: '" << t.text << "'\n";
    }
}