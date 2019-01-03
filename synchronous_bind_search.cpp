#include "defines.h"

const std::string USER_DN = "uid=yyang,ou=People,dc=yyangtech,dc=wordpress,dc=com";

int main(int argc, char **argv) {
    use_ldap_version3();
    LdapConnection ldap = {URI};
    AttributesArray<1> attributes = {"userPassword"};

    // Anonymously search first
    ldap.SynchSearchAndPrint(USER_DN, LDAP_SCOPE_BASE, "", attributes.C_StrArray());

    ldap.SynchSimpleBind(USER_DN, "123456");
    ldap.SynchSearchAndPrint(USER_DN, LDAP_SCOPE_BASE, "", attributes.C_StrArray());
}
