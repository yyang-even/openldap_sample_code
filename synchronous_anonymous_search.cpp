#include "defines.h"

int main(int argc, char **argv) {
    LdapConnection ldap = {URI};
    ldap.SynchSearchAndPrint(DEFAULT_BASE, DEFAULT_SCOPE);
}
