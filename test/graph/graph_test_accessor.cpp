#include "gdwg/graph.hpp"

#include <catch2/catch.hpp>
#include <list>
#include <set>
#include <sstream>
#include <string>
#include <vector>

TEST_CASE("is_node const and non-const") {
	auto grap = gdwg::graph<int, int>{1, 2, 3};

	// no lint this line cause I want to test the const version of this function but the lint says
	// the grap is never modified and has a lint to remove it
	auto const grap_const = grap; // NOLINT

	CHECK(grap.is_node(1));
	CHECK(grap.is_node(2));
	CHECK(grap.is_node(3));

	CHECK(grap_const.is_node(1));
	CHECK(grap_const.is_node(2));
	CHECK(grap_const.is_node(3));
}

TEST_CASE("empty const and non-const") {
	auto grap = gdwg::graph<int, int>{1, 2, 3};

	// no lint this line cause I want to test the const version of this function but the lint says
	// the grap is never modified and has a lint to remove it
	auto const grap_const = grap; // NOLINT

	auto empty = gdwg::graph<int, int>{};

	auto const empty_const = gdwg::graph<int, int>{};

	CHECK_FALSE(grap.empty());
	CHECK_FALSE(grap_const.empty());

	CHECK(empty.empty());
	CHECK(empty_const.empty());
}

TEST_CASE("is_connected const and non-const") {
	auto grap = gdwg::graph<int, int>{1, 2, 3};

	CHECK(grap.insert_edge(1, 2, 4));
	CHECK(grap.insert_edge(2, 3, 4));
	CHECK(grap.insert_edge(2, 2, 4));

	// no lint this line cause I want to test the const version of this function but the lint says
	// the grap is never modified and has a lint to remove it
	auto const grap_const = grap; // NOLINT

	CHECK(grap.is_connected(1, 2));
	CHECK(grap.is_connected(2, 2));
	CHECK(grap.is_connected(2, 3));

	CHECK_FALSE(grap.is_connected(1, 1));
	CHECK_FALSE(grap.is_connected(1, 3));
	CHECK_FALSE(grap.is_connected(3, 3));

	CHECK(grap_const.is_connected(1, 2));
	CHECK(grap_const.is_connected(2, 2));
	CHECK(grap_const.is_connected(2, 3));

	CHECK_FALSE(grap_const.is_connected(1, 1));
	CHECK_FALSE(grap_const.is_connected(1, 3));
	CHECK_FALSE(grap_const.is_connected(3, 3));

	SECTION("check throws") {
		// 4 doesnt exist
		CHECK_THROWS_MATCHES(grap.is_connected(1, 4),
		                     std::runtime_error,
		                     Catch::Matchers::Message("Cannot call gdwg::graph<N, "
		                                              "E>::is_connected if src or dst node don't "
		                                              "exist in the graph"));
		// 4 and 5 doesnt exist
		CHECK_THROWS_MATCHES(grap.is_connected(4, 5),
		                     std::runtime_error,
		                     Catch::Matchers::Message("Cannot call gdwg::graph<N, "
		                                              "E>::is_connected if src or dst node don't "
		                                              "exist in the graph"));
		// 4 doesnt exist
		CHECK_THROWS_MATCHES(grap.is_connected(4, 3),
		                     std::runtime_error,
		                     Catch::Matchers::Message("Cannot call gdwg::graph<N, "
		                                              "E>::is_connected if src or dst node don't "
		                                              "exist in the graph"));
	}

	SECTION("check throws const") {
		// 4 doesnt exist
		CHECK_THROWS_MATCHES(grap_const.is_connected(1, 4),
		                     std::runtime_error,
		                     Catch::Matchers::Message("Cannot call gdwg::graph<N, "
		                                              "E>::is_connected if src or dst node don't "
		                                              "exist in the graph"));
		// 4 and 5 doesnt exist
		CHECK_THROWS_MATCHES(grap_const.is_connected(4, 5),
		                     std::runtime_error,
		                     Catch::Matchers::Message("Cannot call gdwg::graph<N, "
		                                              "E>::is_connected if src or dst node don't "
		                                              "exist in the graph"));
		// 4 doesnt exist
		CHECK_THROWS_MATCHES(grap_const.is_connected(4, 3),
		                     std::runtime_error,
		                     Catch::Matchers::Message("Cannot call gdwg::graph<N, "
		                                              "E>::is_connected if src or dst node don't "
		                                              "exist in the graph"));
	}
}

TEST_CASE("nodes const and non-const") {
	auto grap = gdwg::graph<int, int>{3, 2, 1};

	// no lint this line cause I want to test the const version of this function but the lint says
	// the grap is never modified and has a lint to remove it
	auto const grap_const = grap; // NOLINT

	auto empty = gdwg::graph<int, int>{};

	auto const empty_const = gdwg::graph<int, int>{};
	auto empty_vec = std::vector<int>{};

	CHECK(empty.nodes() == empty_vec);
	CHECK(empty_const.nodes() == empty_vec);

	CHECK(grap.nodes() == std::vector<int>{1, 2, 3});
	CHECK(grap_const.nodes() == std::vector<int>{1, 2, 3});
}

TEST_CASE("weights const and non-const") {
	auto grap = gdwg::graph<int, int>{1, 2, 3};

	CHECK(grap.insert_edge(1, 2, 4));
	CHECK(grap.insert_edge(2, 3, 4));
	CHECK(grap.insert_edge(2, 2, 4));
	CHECK(grap.insert_edge(2, 2, 3));

	CHECK(grap.insert_edge(1, 2, 3));
	CHECK(grap.insert_edge(1, 2, 2));
	CHECK(grap.insert_edge(1, 2, 5));
	CHECK(grap.insert_edge(1, 2, 1));

	// no lint this line cause I want to test the const version of this function but the lint says
	// the grap is never modified and has a lint to remove it
	auto const grap_const = grap; // NOLINT
	auto empty_vec = std::vector<int>{};

	SECTION("check empty") {
		CHECK(grap.weights(1, 1) == empty_vec);
		CHECK(grap_const.weights(1, 1) == empty_vec);

		CHECK(grap.weights(1, 3) == empty_vec);
		CHECK(grap_const.weights(1, 3) == empty_vec);
	}

	SECTION("Check ascending") {
		CHECK(grap.weights(1, 2) == std::vector{1, 2, 3, 4, 5});
		CHECK(grap_const.weights(1, 2) == std::vector{1, 2, 3, 4, 5});
		CHECK(grap.weights(2, 2) == std::vector{3, 4});
		CHECK(grap_const.weights(2, 2) == std::vector{3, 4});
	}

	SECTION("check throws") {
		// 4 doesnt exist
		CHECK_THROWS_MATCHES(grap.weights(1, 4),
		                     std::runtime_error,
		                     Catch::Matchers::Message("Cannot call gdwg::graph<N, E>::weights if src "
		                                              "or dst node don't exist in the graph"));
		// 4 and 5 doesnt exist
		CHECK_THROWS_MATCHES(grap.weights(4, 5),
		                     std::runtime_error,
		                     Catch::Matchers::Message("Cannot call gdwg::graph<N, E>::weights if src "
		                                              "or dst node don't exist in the graph"));
		// 4 doesnt exist
		CHECK_THROWS_MATCHES(grap.weights(4, 3),
		                     std::runtime_error,
		                     Catch::Matchers::Message("Cannot call gdwg::graph<N, E>::weights if src "
		                                              "or dst node don't exist in the graph"));
	}

	SECTION("check throws const") {
		// 4 doesnt exist
		CHECK_THROWS_MATCHES(grap_const.weights(1, 4),
		                     std::runtime_error,
		                     Catch::Matchers::Message("Cannot call gdwg::graph<N, E>::weights if src "
		                                              "or dst node don't exist in the graph"));
		// 4 and 5 doesnt exist
		CHECK_THROWS_MATCHES(grap_const.weights(4, 5),
		                     std::runtime_error,
		                     Catch::Matchers::Message("Cannot call gdwg::graph<N, E>::weights if src "
		                                              "or dst node don't exist in the graph"));
		// 4 doesnt exist
		CHECK_THROWS_MATCHES(grap_const.weights(4, 3),
		                     std::runtime_error,
		                     Catch::Matchers::Message("Cannot call gdwg::graph<N, E>::weights if src "
		                                              "or dst node don't exist in the graph"));
	}
}

TEST_CASE("find const and non-const") {
	auto grap = gdwg::graph<int, int>{1, 2, 3};

	CHECK(grap.insert_edge(1, 2, 4));
	CHECK(grap.insert_edge(2, 3, 4));
	CHECK(grap.insert_edge(2, 2, 4));
	CHECK(grap.insert_edge(2, 2, 3));

	CHECK(grap.insert_edge(1, 2, 3));
	CHECK(grap.insert_edge(1, 2, 2));
	CHECK(grap.insert_edge(1, 2, 5));
	CHECK(grap.insert_edge(1, 2, 1));

	// no lint this line cause I want to test the const version of this function but the lint says
	// the grap is never modified and has a lint to remove it
	auto const grap_const = grap; // NOLINT

	SECTION("cant find") {
		CHECK(grap.find(1, 1, 2) == grap.end());
		CHECK(grap.find(1, 3, 2) == grap.end());
		CHECK(grap.find(1, 2, 6) == grap.end());
		// node exist but edge is other way around
		CHECK(grap.find(2, 1, 4) == grap.end());
		CHECK(grap.find(3, 2, 4) == grap.end());

		CHECK(grap_const.find(1, 1, 2) == grap_const.end());
		CHECK(grap_const.find(1, 3, 2) == grap_const.end());
		CHECK(grap_const.find(1, 2, 6) == grap_const.end());
		// node exist but edge is other way around
		CHECK(grap_const.find(2, 1, 4) == grap_const.end());
		CHECK(grap_const.find(3, 2, 4) == grap_const.end());
	}

	SECTION("can find") {
		auto value_type1 = grap.find(1, 2, 4);
		auto value_type2 = grap.find(2, 2, 4);
		auto value_type3 = grap.find(2, 2, 3);
		auto value_type4 = grap.find(1, 2, 2);

		CHECK((*value_type1).from == 1);
		CHECK((*value_type1).to == 2);
		CHECK((*value_type1).weight == 4);

		CHECK((*value_type2).from == 2);
		CHECK((*value_type2).to == 2);
		CHECK((*value_type2).weight == 4);

		CHECK((*value_type3).from == 2);
		CHECK((*value_type3).to == 2);
		CHECK((*value_type3).weight == 3);

		CHECK((*value_type4).from == 1);
		CHECK((*value_type4).to == 2);
		CHECK((*value_type4).weight == 2);

		CHECK((*value_type1).from == 1);
		CHECK((*value_type1).to == 2);
		CHECK((*value_type1).weight == 4);
	}

	SECTION("can find const") {
		auto value_type1 = grap_const.find(1, 2, 4);
		auto value_type2 = grap_const.find(2, 2, 4);
		auto value_type3 = grap_const.find(2, 2, 3);
		auto value_type4 = grap_const.find(1, 2, 2);

		CHECK((*value_type1).from == 1);
		CHECK((*value_type1).to == 2);
		CHECK((*value_type1).weight == 4);

		CHECK((*value_type2).from == 2);
		CHECK((*value_type2).to == 2);
		CHECK((*value_type2).weight == 4);

		CHECK((*value_type3).from == 2);
		CHECK((*value_type3).to == 2);
		CHECK((*value_type3).weight == 3);

		CHECK((*value_type4).from == 1);
		CHECK((*value_type4).to == 2);
		CHECK((*value_type4).weight == 2);

		CHECK((*value_type1).from == 1);
		CHECK((*value_type1).to == 2);
		CHECK((*value_type1).weight == 4);
	}
}

TEST_CASE("connections const and non-const") {
	auto grap = gdwg::graph<int, int>{1, 2, 3, 4};

	CHECK(grap.insert_edge(1, 2, 4));
	CHECK(grap.insert_edge(2, 3, 4));
	CHECK(grap.insert_edge(2, 2, 4));
	CHECK(grap.insert_edge(2, 2, 3));

	CHECK(grap.insert_edge(1, 2, 3));
	CHECK(grap.insert_edge(1, 2, 2));
	CHECK(grap.insert_edge(1, 2, 5));
	CHECK(grap.insert_edge(1, 2, 1));

	CHECK(grap.insert_edge(1, 1, 4));
	// no lint this line cause I want to test the const version of this function but the lint says
	// the grap is never modified and has a lint to remove it
	auto const grap_const = grap; // NOLINT
	auto empty_vec = std::vector<int>{};

	SECTION("check empty") {
		CHECK(grap.connections(4) == empty_vec);
		CHECK(grap_const.connections(4) == empty_vec);

		CHECK(grap.connections(3) == empty_vec);
		CHECK(grap_const.connections(3) == empty_vec);
	}

	SECTION("Check ascending") {
		CHECK(grap.connections(1) == std::vector{1, 2, 2, 2, 2, 2});
		CHECK(grap_const.connections(1) == std::vector{1, 2, 2, 2, 2, 2});

		CHECK(grap.connections(2) == std::vector{2, 2, 3});
		CHECK(grap_const.connections(2) == std::vector{2, 2, 3});
	}

	SECTION("check throws") {
		// 6 doesnt exist
		CHECK_THROWS_MATCHES(grap.connections(6),
		                     std::runtime_error,
		                     Catch::Matchers::Message("Cannot call gdwg::graph<N, E>::connections if "
		                                              "src doesn't exist in the graph"));
		// 5 doesnt exist
		CHECK_THROWS_MATCHES(grap.connections(5),
		                     std::runtime_error,
		                     Catch::Matchers::Message("Cannot call gdwg::graph<N, E>::connections if "
		                                              "src doesn't exist in the graph"));
	}

	SECTION("check throws const") {
		// 6 doesnt exist
		CHECK_THROWS_MATCHES(grap_const.connections(6),
		                     std::runtime_error,
		                     Catch::Matchers::Message("Cannot call gdwg::graph<N, E>::connections if "
		                                              "src doesn't exist in the graph"));
		// 5 doesnt exist
		CHECK_THROWS_MATCHES(grap_const.connections(5),
		                     std::runtime_error,
		                     Catch::Matchers::Message("Cannot call gdwg::graph<N, E>::connections if "
		                                              "src doesn't exist in the graph"));
	}
}