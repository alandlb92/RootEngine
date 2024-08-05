#pragma once
#include "pch.h"
#include "Faia/HashUtils.h"

TEST(FaiaUtils, CreateHash) {
    uint32_t hash1 = Faia::HashUtils::CharToHashFnv1a("hashtest");
    uint32_t hash2 = Faia::HashUtils::CharToHashFnv1a("hashtestgreatestringofalotof123123456789characters");
    EXPECT_EQ(hash1, 3165874921);
    EXPECT_EQ(hash2, 1925710553);
}
