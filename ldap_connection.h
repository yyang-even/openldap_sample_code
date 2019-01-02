#pragma once

#include <cassert>
#include <iostream>
#include <memory>

#include <ldap.h>


using MessagePtr = std::unique_ptr<LDAPMessage, decltype(&ldap_msgfree)>;


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

template <std::size_t NUMBER_ATTRIBUTES>
class AttributesArray {
    char *mHead[NUMBER_ATTRIBUTES + 1];
public:
    AttributesArray(const AttributesArray &) = delete;
    AttributesArray &operator = (const AttributesArray &) = delete;
    AttributesArray(AttributesArray &&other) = default;
    AttributesArray &operator = (AttributesArray &&other) = default;
    AttributesArray(std::initializer_list<const char *> attributes) {
        assert(attributes.size() <= NUMBER_ATTRIBUTES);
        for (std::size_t i = 0; i < attributes.size(); ++i) {
            const auto *str = *(attributes.begin() + i);
            const std::size_t length = strlen(str) + 1;
            mHead[i] = new char[length] {};
            strncpy(mHead[i], str, length);
        }
        mHead[NUMBER_ATTRIBUTES] = nullptr;
    }

    ~AttributesArray() {
        for (auto **p = mHead; *p != nullptr; ++p) {
            delete[](*p);
            *p = nullptr;
        }
    }

    auto C_StrArray() {
        return mHead;
    }
};

class LdapConnection {
    std::unique_ptr<LDAP, decltype(&ldap_unbind_with_log)> mLdap;

    auto synchSearch(const std::string &base, const ber_int_t &scope,
                     const std::string &filter = "", char **attributes = nullptr) const {
        LDAPMessage *pMessage = nullptr;

        Call_LDAP_Func(ldap_search_ext_s, mLdap.get(), base.c_str(), scope,
                       //NULL may be specified to indicate the library should send the filter (objectClass=*)
                       (filter.empty() ? nullptr : filter.c_str()),
                       attributes, /*attrsonly=*/0, nullptr, nullptr, nullptr, 0, &pMessage);

        return MessagePtr{pMessage, &ldap_msgfree};
    }

public:
    LdapConnection(const std::string &uri) : mLdap(ldap_init_with_log(uri), &ldap_unbind_with_log) {
    }

    template <typename ...Args>
    void SynchSearch(Args &&...args) const {
        synchSearch(std::forward<Args>(args)...);
    }

    template <typename ...Args>
    void SynchSearchAndPrint(Args &&...args) const {
        const auto pMessage = synchSearch(std::forward<Args>(args)...);
        BerElement *pBerEl = nullptr;
        struct berval **pValues = nullptr;

        for (auto *pEntry = ldap_first_entry(mLdap.get(), pMessage.get());
             pEntry != nullptr; pEntry = ldap_next_entry(mLdap.get(), pEntry)) {

            auto *pDN = ldap_get_dn(mLdap.get(), pEntry);
            if (pDN != nullptr) {
                std::cout << "dn: " << pDN << "\n";
                ldap_memfree(pDN);
            }

            for (auto *pAttribute = ldap_first_attribute(mLdap.get(), pEntry, &pBerEl);
                 pAttribute != nullptr; pAttribute = ldap_next_attribute(mLdap.get(), pEntry, pBerEl)) {
                if ((pValues = ldap_get_values_len(mLdap.get(), pEntry, pAttribute)) != nullptr) {
                    for (auto iVal = 0; pValues[iVal] != nullptr; ++iVal) {
                        std::cout << pAttribute << ": " << pValues[iVal]->bv_val << "\n";
                    }

                    ldap_value_free_len(pValues);
                }

                ldap_memfree(pAttribute);
            }

            if (pBerEl != nullptr) {
                ber_free(pBerEl, 0);
            }
            std::cout << std::endl;
        }
    }

    void LdapSetOption(const int option, const void *invalue) const {
        Call_LDAP_Func(ldap_set_option, mLdap.get(), option, invalue);
    }
    auto BerSetOption(const int option, const void *invalue) const {
        return ber_set_option(mLdap.get(), option, invalue);
    }
};
