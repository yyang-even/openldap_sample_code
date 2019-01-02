#include "defines.h"

int main(int argc, char **argv) {
    use_ldap_version3();
    LdapConnection ldap = {URI};
    ldap.SynchSearchAndPrint(DEFAULT_BASE, DEFAULT_SCOPE, "cn=Yang Yang");
}
