#include "defines.h"

const auto DEBUG_LEVEL = -1;

int main(int argc, char **argv) {
    use_ldap_version3();
    // DEBUG_LEVEL only works with Global.
    Call_LDAP_Func(ber_set_option, nullptr, LBER_OPT_DEBUG_LEVEL, &DEBUG_LEVEL);
    Call_LDAP_Func(ldap_set_option, nullptr, LDAP_OPT_DEBUG_LEVEL, &DEBUG_LEVEL);

    LdapConnection ldap = {URI};
    ldap.SynchSearch(DEFAULT_BASE, DEFAULT_SCOPE);
}
