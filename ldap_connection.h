#pragma once

#include <memory>
#include <iostream>

#include <ldap.h>


#define Call_LDAP_Func(ldap_func, ...) {                                            \
    const auto iStatus = ldap_func(__VA_ARGS__);                                    \
    if (iStatus != LDAP_SUCCESS) {                                                  \
        std::cerr << "Error: LDAP function " << #ldap_func << "() call failed: "    \
            << ldap_err2string(iStatus) << std::endl;                               \
    }                                                                               \
}

inline void ldap_unbind_with_log(LDAP *ld) {
    Call_LDAP_Func(ldap_unbind_ext_s, ld, nullptr, nullptr);
}

inline auto ldap_init_with_log(const std::string &uri) {
    LDAP *ld = nullptr;
    Call_LDAP_Func(ldap_initialize, &ld, uri.c_str());
    return ld;
}

class LdapConnection {
    std::unique_ptr<LDAP, decltype(&ldap_unbind_with_log)> mLdap;

public:
    LdapConnection(const std::string &uri) : mLdap(ldap_init_with_log(uri), &ldap_unbind_with_log) {
    }

    void SynchSearch(const std::string &base, const ber_int_t &scope,
                     const std::string &filter = "", char **attributes = nullptr) const {
        LDAPMessage *pMessage;

        Call_LDAP_Func(ldap_search_ext_s, mLdap.get(), base.c_str(), scope,
                       //NULL may be specified to indicate the library should send the filter (objectClass=*)
                       (filter.empty() ? nullptr : filter.c_str()),
                       attributes, /*attrsonly=*/0, nullptr, nullptr, nullptr, 0, &pMessage);

        ldap_msgfree(pMessage);
    }

    void LdapSetOption(const int option, const void *invalue) const {
        Call_LDAP_Func(ldap_set_option, mLdap.get(), option, invalue);
    }
    int BerSetOption(const int option, const void *invalue) const {
        return ber_set_option(mLdap.get(), option, invalue);
    }
};
