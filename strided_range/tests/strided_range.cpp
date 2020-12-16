/* Test file for strided_range task.
 * @file
 * @date 2020
 * @author Серафима Рысина
 */

#include <boost/range/iterator_range.hpp>
#include <vector>
#include <catch2/catch.hpp>
#include <strided_range.hpp>

// Two checks with different strides.
TEST_CASE("strided_range::stride") {
    const size_t N = 1000;
    std::vector<int> rng;
    rng.assign(N, 0);

    const size_t width = 5;

    SECTION("stride > width") {
        const size_t stride = 6;
        const auto range = make_strided_range(rng, width, stride);
        for (const auto i : range) {
            CHECK(i == 0);
        }
    }

    SECTION("stride < width") {
        const size_t stride = 1;
        const auto range = make_strided_range(rng, width, stride);
        for (const auto i : range) {
            CHECK(i == 0);
        }
    }
}

// Two checks with different widths.
TEST_CASE("strided_range::width") {
    const size_t N = 1000;
    std::vector<int> rng;
    rng.assign(N, 0);

    const size_t stride = 5;

    SECTION("stride > width") {
        const size_t width = 6;
        const auto range = make_strided_range(rng, width, stride);
        for (const auto i : range) {
            CHECK(i == 0);
        }
    }

    SECTION("stride < width") {
        const size_t width = 1;
        const auto range = make_strided_range(rng, width, stride);
        for (const auto i : range) {
            CHECK(i == 0);
        }
    }
}

TEST_CASE("strided_range::stride_equal_width") {
    const size_t N = 1000;
    std::vector<int> rng;
    rng.assign(N, 0);

    const size_t width = 10;
    const size_t stride = 10;

    const auto range = make_strided_range(rng, width, stride);

    for (const auto el : range) {
        CHECK(el == 0);
    }
}

// Use boost::size.
TEST_CASE("strided_range::distance") {
    const size_t N = 1000;
    std::vector<int> rng;
    rng.assign(N, 0);

    const size_t width = 5;
    const size_t stride = 6;

    const auto range = make_strided_range(rng, width, stride);

    CHECK(boost::size(range) != boost::size(rng) );
}

// Make strided_range from another strided_range.
TEST_CASE("strided_range::from_strided") {
    const size_t N = 1000;
    std::vector<int> rng;
    rng.assign(N, 0);

    const size_t width = 5;
    const size_t stride = 6;

    const auto range1 = make_strided_range(rng, width, stride);
    const auto range2 = make_strided_range(range1, width, stride);
    
    for (const auto el : range2) {
        CHECK(el == 0);
    }
}

// Provide example with shifted data. It's like a window in the image. Hint: boost::make_iterator_range.
TEST_CASE("strided_range::shift") {
    const size_t N = 1000;
    std::vector<int> rng;
    rng.assign(N, 0);

    const size_t width = 5;
    const size_t stride = 6;

    const auto range = make_strided_range(rng, width, stride);
    
    auto begin = range.begin();
    auto end = range.begin();
    boost::advance(begin, 100);
    boost::advance(end, 200);

    const auto new_range = boost::make_iterator_range(begin, end);

    for (const auto el : new_range) {
        CHECK(el == 0);
    }
}

// if call make_strided_range with zero on 3 parametr, will be devision by zero and undefined behavior

TEST_CASE("strided_range::corner_case1") {
    std::vector<int> a;
    const auto str_range = make_strided_range(a, 1, 1);
    CHECK(boost::size(str_range) == 0);
}

TEST_CASE("strided_range::corner_case2") {
    std::vector a{0, 1, 2, 3, 4};
    const auto str_range = make_strided_range(a, 0, 1);
    CHECK(boost::size(str_range) == 0);
}
