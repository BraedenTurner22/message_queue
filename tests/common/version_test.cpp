#include "common/version.h"

#include <gtest/gtest.h>

TEST(Version, IsSet) {
    EXPECT_FALSE(mq::kVersion.empty());
    EXPECT_EQ(mq::kVersion, "0.1.0");
}
