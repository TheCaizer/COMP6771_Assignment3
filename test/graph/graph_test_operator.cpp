#include "gdwg/graph.hpp"

#include <catch2/catch.hpp>
#include <sstream>
#include <string>
#include <vector>

TEST_CASE("simple extractor") {
	auto grap = gdwg::graph<int, int>{1, 2, 3};
	auto out = std::ostringstream{};
	out << grap;
	auto const expected_output = std::string_view(R"(1 (
)
2 (
)
3 (
)
)");
	CHECK(out.str() == expected_output);

	// no lint since i want to test const but lint for grap not being modified
	auto const g_const = grap; // NOLINT
	out.str(std::string());
	out << g_const;
	CHECK(out.str() == expected_output);
}

// test case in assignment specs
TEST_CASE("Sample extractor") {
	using graph = gdwg::graph<int, int>;
	auto const v = std::vector<graph::value_type>{
	   {4, 1, -4},
	   {3, 2, 2},
	   {2, 4, 2},
	   {2, 1, 1},
	   {6, 2, 5},
	   {6, 3, 10},
	   {1, 5, -1},
	   {3, 6, -8},
	   {4, 5, 3},
	   {5, 2, 7},
	};

	auto g = graph{};
	for (const auto& [from, to, weight] : v) {
		g.insert_node(from);
		g.insert_node(to);
		g.insert_edge(from, to, weight);
	}

	g.insert_node(64);
	auto out = std::ostringstream{};
	out << g;
	auto const expected_output = std::string_view(R"(1 (
  5 | -1
)
2 (
  1 | 1
  4 | 2
)
3 (
  2 | 2
  6 | -8
)
4 (
  1 | -4
  5 | 3
)
5 (
  2 | 7
)
6 (
  2 | 5
  3 | 10
)
64 (
)
)");
	CHECK(out.str() == expected_output);

	auto const g_const = g;
	out.str(std::string());
	out << g_const;
	CHECK(out.str() == expected_output);
}

TEST_CASE("extractor empty") {
	auto g = gdwg::graph<int, int>();
	auto oss = std::ostringstream{};
	oss << g;
	CHECK(oss.str().empty());
}

TEST_CASE("== and !=") {
	SECTION("empty") {
		CHECK(gdwg::graph<int, int>{} == gdwg::graph<int, int>({}));
		CHECK_FALSE(gdwg::graph<int, int>{} != gdwg::graph<int, int>({}));
	}

	SECTION("check nodes") {
		auto grap = gdwg::graph<int, int>{1, 2, 3};
		auto const grap_const = grap;

		auto compare = gdwg::graph<int, int>{2, 3};
		auto const compare_const = compare;

		CHECK(compare != grap);
		CHECK(compare != grap_const);

		CHECK(compare_const != grap);
		CHECK(compare_const != grap_const);

		CHECK_FALSE(compare == grap);
		CHECK_FALSE(compare == grap_const);

		CHECK_FALSE(compare_const == grap);
		CHECK_FALSE(compare_const == grap_const);

		CHECK(grap_const == grap);
		CHECK(compare_const == compare);

		CHECK(compare.insert_node(1));

		CHECK(compare == grap);
		CHECK(compare == grap_const);

		CHECK(grap.insert_node(4));

		CHECK(grap != grap_const);
		CHECK_FALSE(grap == grap_const);
	}

	SECTION("check nodes and edges") {
		auto grap = gdwg::graph<int, int>{1, 2, 3};
		auto const grap_const = grap;

		CHECK(grap_const == grap);

		CHECK(grap.insert_edge(1, 2, 4));

		CHECK(grap_const != grap);
		CHECK_FALSE(grap_const == grap);

		CHECK(grap.erase_edge(1, 2, 4));
		CHECK(grap_const == grap);
		CHECK(grap.insert_edge(1, 2, 4));

		SECTION("comapre with non-const") {
			auto compare = gdwg::graph<int, int>{2, 3};

			CHECK(compare != grap);
			CHECK_FALSE(compare == grap);

			CHECK(compare.insert_node(1));

			CHECK(compare != grap);
			CHECK_FALSE(compare == grap);

			CHECK(compare.insert_edge(1, 2, 4));
			CHECK(compare == grap);

			CHECK(grap.insert_edge(1, 1, 1));

			CHECK(compare != grap);
			CHECK_FALSE(compare == grap);

			CHECK(grap.erase_edge(1, 1, 1));
			CHECK(compare == grap);
		}
	}
}