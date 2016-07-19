#include "simple-authenticator.hpp"

SimpleAuthenticator::SimpleAuthenticator() {
}

void SimpleAuthenticator::installAuthenticationSource() {
}

bool SimpleAuthenticator::authenticate(const Identity* id, const Token* token) {
    return true;
}
