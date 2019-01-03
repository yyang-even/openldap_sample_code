#include "defines.h"

int main(int argc, char **argv) {
    use_ldap_version3();
    LdapConnection ldap = {URI};

    AttributesArray<2> attributes = {"sn", "memberof"};

    ldap.SynchSearchAndPrint(DEFAULT_BASE, DEFAULT_SCOPE, "objectClass=inetOrgPerson",
                             attributes.C_StrArray());
}
