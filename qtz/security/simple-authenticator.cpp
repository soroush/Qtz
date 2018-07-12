#include "simple-authenticator.hpp"

SimpleAuthenticator::SimpleAuthenticator() {
}

void SimpleAuthenticator::installAuthenticationSource() {
}

bool SimpleAuthenticator::authenticate(const Identity* id, const Token* token) {
    // TODO: Implement
    Q_UNUSED(id)
    Q_UNUSED(token)
    return true;
}
