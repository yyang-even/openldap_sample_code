#include "defines.h"

int main(int argc, char **argv) {
    LdapConnection ldap = {URI};

    AttributesArray<2> attributes = {"dn", "sn"};

    ldap.SynchSearchAndPrint(DEFAULT_BASE, DEFAULT_SCOPE, "", attributes.C_StrArray());
}
