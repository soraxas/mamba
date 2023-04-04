﻿// Copyright (c) 2023, QuantStack and Mamba Contributors
//
// Distributed under the terms of the BSD 3-Clause License.
//
// The full license is in the file LICENSE, distributed with this software.

#include <string>

#include <gtest/gtest.h>

#include "mamba/core/environment.hpp"
#include "mamba/core/util_random.hpp"


namespace
{
    void test_set_get_unset_env_variables(const std::string& key, const std::string& value)
    {
        const bool did_set = mamba::env::set(key, value);
        ASSERT_TRUE(did_set);

        const auto result = mamba::env::get(key);
        ASSERT_TRUE(result.has_value());
        EXPECT_EQ(*result, value);

        mamba::env::unset(key);
        const auto result_empty = mamba::env::get(key);
        ASSERT_FALSE(result_empty.has_value());
    }
}

TEST(system_env, set_get_unset_env_variables)
{
    const auto key = mamba::generate_random_alphanumeric_string(128);
    const auto value = mamba::generate_random_alphanumeric_string(128);

    test_set_get_unset_env_variables(key, value);
}

TEST(system_env, set_get_unset_variables_unicode)
{
    const std::string key = u8"Joël私のにほん";
    const std::string value = u8"Hello, I am Joël. 私のにほんごわへたです";

    test_set_get_unset_env_variables(key, value);
}

#ifdef _WIN32

TEST(system_env, get_predefined_env_variable)
{
    // We check that normal pre-defined variables are accessible and do not fail access even if some
    // of these values have unicode.
    const std::vector<std::string> predefined_keys{ "PATH",        "OS",           "PATHEXT",
                                                    "ProgramData", "SystemRoot",   "windir",
                                                    "APPDATA",     "COMPUTERNAME", "TEMP",
                                                    "UserName",    "USERPROFILE" };

    for (const auto& key : predefined_keys)
    {
        const auto maybe_value = mamba::env::get(key);
        ASSERT_TRUE(maybe_value.has_value()) << "key = " + key;
        const auto value = *maybe_value;
        EXPECT_FALSE(value.empty()) << "key = " + key;
    }
}

#endif