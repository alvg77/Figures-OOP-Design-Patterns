#include <catch2/catch_test_macros.hpp>

#include "../../src/figure/triangle/Triangle.hpp"

TEST_CASE( "Triangle construction fails with negative parameters" ) {
    REQUIRE_THROWS(Triangle(-1, -1, -1));
}

TEST_CASE( "Triangle with very large parameters fails" ) {
    REQUIRE_THROWS(Triangle(1.7976931348623157E+309, 1.7976931348623157E+309, 1.7976931348623157E+309));
}

TEST_CASE( "Triangle fails with NaN parameters" ) {
    REQUIRE_THROWS(Triangle(std::numeric_limits<double>::quiet_NaN(), 1, 1));
}

TEST_CASE( "Triangle fails when sides are large enough to make parameter infinity" ) {
    REQUIRE_THROWS(Triangle(1.7976931348623157E+308, 1.7976931348623157E+308, 1.7976931348623157E+308));
}

