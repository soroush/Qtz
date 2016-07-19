#include "authenticator.hpp"


Authenticator::Authenticator() {
}

Authenticator::~Authenticator() {
}

void Authenticator::installAuthenticationSource(const AuthenticationSource* source) {
}

bool Authenticator::authenticate(const Identity* id, const Token* token) {
    return false;
}
