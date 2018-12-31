#pragma once

#include "ldap_connection.h"

const std::string URI = "ldap://";
const std::string DEFAULT_BASE = "dc=yyangtech,dc=wordpress,dc=com";
const auto DEFAULT_SCOPE = LDAP_SCOPE_SUB;
