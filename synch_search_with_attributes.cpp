#include "defines.h"

int main(int argc, char **argv) {
    LdapConnection ldap = {URI};

    AttributesArray attributes{2};
    attributes.PushBack("dn");
    attributes.PushBack("sn");

    ldap.SynchSearchAndPrint(DEFAULT_BASE, DEFAULT_SCOPE, "", attributes.C_StrArray());
}
