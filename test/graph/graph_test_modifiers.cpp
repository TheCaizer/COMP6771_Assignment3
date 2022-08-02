#include "gdwg/graph.hpp"

#include <catch2/catch.hpp>
#include <list>
#include <set>
#include <sstream>
#include <string>
#include <vector>

TEST_CASE("insert node") {
	SECTION("insert int node") {
		auto grap = gdwg::graph<int, int>{};
		CHECK(grap.insert_node(1));
		CHECK(grap.insert_node(2));
		CHECK(grap.insert_node(3));

		CHECK(grap.is_node(1));
		CHECK(grap.is_node(2));
		CHECK(grap.is_node(3));
	}

	SECTION("insert same int") {
		auto grap = gdwg::graph<int, int>{};
		CHECK(grap.insert_node(1));
		CHECK(grap.insert_node(2));
		CHECK(grap.insert_node(3));

		CHECK(grap.is_node(1));
		CHECK(grap.is_node(2));
		CHECK(grap.is_node(3));

		CHECK_FALSE(grap.insert_node(1));
		CHECK_FALSE(grap.insert_node(2));
		CHECK_FALSE(grap.insert_node(3));
	}
}

TEST_CASE("insert edge") {
	SECTION("check throw") {
		auto grap = gdwg::graph<int, int>{1, 2};
		CHECK(grap.insert_edge(1, 2, 3));
		// 2 exist but 10 doesnt
		CHECK_THROWS_MATCHES(grap.insert_edge(10, 2, 3),
		                     std::runtime_error,
		                     Catch::Matchers::Message("Cannot call gdwg::graph<N, E>::insert_edge "
		                                              "when either src or dst node does not exist"));
		// 1 exist but 20 doesnt
		CHECK_THROWS_MATCHES(grap.insert_edge(1, 20, 3),
		                     std::runtime_error,
		                     Catch::Matchers::Message("Cannot call gdwg::graph<N, E>::insert_edge "
		                                              "when either src or dst node does not exist"));
		// 10 and 11 doesnt exist
		CHECK_THROWS_MATCHES(grap.insert_edge(10, 11, 3),
		                     std::runtime_error,
		                     Catch::Matchers::Message("Cannot call gdwg::graph<N, E>::insert_edge "
		                                              "when either src or dst node does not exist"));
	}
	SECTION("insert int edge") {
		auto grap = gdwg::graph<int, int>{};
		CHECK(grap.insert_node(1));
		CHECK(grap.insert_node(2));
		CHECK(grap.insert_node(3));

		CHECK(grap.is_node(1));
		CHECK(grap.is_node(2));
		CHECK(grap.is_node(3));

		CHECK(grap.insert_edge(1, 1, 2));
		CHECK(grap.insert_edge(1, 2, 3));
		CHECK(grap.insert_edge(1, 2, 4));

		CHECK(grap.insert_edge(1, 3, 4));
		CHECK(grap.insert_edge(2, 3, 5));
	}

	SECTION("insert same int") {
		auto grap = gdwg::graph<int, int>{};
		CHECK(grap.insert_node(1));
		CHECK(grap.insert_node(2));
		CHECK(grap.insert_node(3));

		CHECK(grap.is_node(1));
		CHECK(grap.is_node(2));
		CHECK(grap.is_node(3));

		CHECK(grap.insert_edge(1, 1, 2));
		CHECK(grap.insert_edge(1, 2, 3));
		CHECK(grap.insert_edge(1, 2, 4));

		CHECK(grap.insert_edge(1, 3, 4));
		CHECK(grap.insert_edge(2, 3, 5));

		CHECK_FALSE(grap.insert_edge(1, 1, 2));
		CHECK_FALSE(grap.insert_edge(2, 3, 5));
	}
}

TEST_CASE("replace node") {
	SECTION("check throw") {
		auto grap = gdwg::graph<int, int>{1, 2};
		CHECK(grap.insert_edge(1, 2, 3));
		// 4 (old data does not exist)
		CHECK_THROWS_MATCHES(grap.replace_node(4, 2),
		                     std::runtime_error,
		                     Catch::Matchers::Message("Cannot call gdwg::graph<N, E>::replace_node "
		                                              "on a node that doesn't exist"));
		// 5 and 6 doesnt exist
		CHECK_THROWS_MATCHES(grap.replace_node(5, 6),
		                     std::runtime_error,
		                     Catch::Matchers::Message("Cannot call gdwg::graph<N, E>::replace_node "
		                                              "on a node that doesn't exist"));
		// 2 already exist
		CHECK_FALSE(grap.replace_node(1, 2));
		CHECK(grap.is_node(1));
		CHECK(grap.is_node(2));
	}

	SECTION("check replace works") {
		auto grap = gdwg::graph<int, int>{1, 2, 3};
		CHECK(grap.replace_node(1, 10));
		CHECK(grap.is_node(10));
		CHECK_FALSE(grap.is_node(1));
	}

	SECTION("check edges are the same") {
		auto grap = gdwg::graph<int, int>{1, 2, 3};

		CHECK(grap.insert_edge(1, 1, 2));
		CHECK(grap.insert_edge(1, 2, 3));
		CHECK(grap.insert_edge(1, 2, 4));
		CHECK(grap.insert_edge(1, 3, 4));
		CHECK(grap.insert_edge(2, 3, 5));

		CHECK(grap.weights(1, 1) == std::vector<int>{2});
		CHECK(grap.weights(1, 2) == std::vector<int>{3, 4});
		CHECK(grap.weights(1, 3) == std::vector<int>{4});

		CHECK(grap.replace_node(1, 10));

		CHECK_FALSE(grap.is_node(1));
		CHECK(grap.is_node(10));

		CHECK(grap.weights(10, 10) == std::vector<int>{2});
		CHECK(grap.weights(10, 2) == std::vector<int>{3, 4});
		CHECK(grap.weights(10, 3) == std::vector<int>{4});
	}
}

TEST_CASE("merge and replace node") {
	SECTION("check throw") {
		auto grap = gdwg::graph<int, int>{1, 2};
		CHECK(grap.insert_edge(1, 2, 3));
		// 4 (old data does not exist)
		CHECK_THROWS_MATCHES(grap.merge_replace_node(4, 2),
		                     std::runtime_error,
		                     Catch::Matchers::Message("Cannot call gdwg::graph<N, "
		                                              "E>::merge_replace_node on old or new data if "
		                                              "they don't exist in the graph"));
		// 5 and 6 doesnt exist
		CHECK_THROWS_MATCHES(grap.merge_replace_node(5, 6),
		                     std::runtime_error,
		                     Catch::Matchers::Message("Cannot call gdwg::graph<N, "
		                                              "E>::merge_replace_node on old or new data if "
		                                              "they don't exist in the graph"));
		// 6 doesnt exist
		CHECK_THROWS_MATCHES(grap.merge_replace_node(1, 6),
		                     std::runtime_error,
		                     Catch::Matchers::Message("Cannot call gdwg::graph<N, "
		                                              "E>::merge_replace_node on old or new data if "
		                                              "they don't exist in the graph"));
	}

	// example in assignment spec where 1 is a, b is 2 and c is 3 and d is 4
	SECTION("check when there are no duplicate edges") {
		auto grap = gdwg::graph<int, int>{1, 2, 3, 4};

		CHECK(grap.insert_edge(1, 2, 1));
		CHECK(grap.insert_edge(1, 3, 2));
		CHECK(grap.insert_edge(1, 4, 3));

		CHECK(grap.weights(1, 2) == std::vector<int>{1});
		CHECK(grap.weights(1, 3) == std::vector<int>{2});
		CHECK(grap.weights(1, 4) == std::vector<int>{3});

		grap.merge_replace_node(1, 2);

		CHECK_FALSE(grap.is_node(1));

		CHECK(grap.weights(2, 2) == std::vector<int>{1});
		CHECK(grap.weights(2, 3) == std::vector<int>{2});
		CHECK(grap.weights(2, 4) == std::vector<int>{3});
	}

	// example in assignment spec where 1 is a, b is 2 and c is 3 and d is 4
	SECTION("check when there are duplicate edges") {
		auto grap = gdwg::graph<int, int>{1, 2, 3, 4};

		CHECK(grap.insert_edge(1, 2, 1));
		CHECK(grap.insert_edge(1, 3, 2));
		CHECK(grap.insert_edge(1, 4, 3));

		CHECK(grap.insert_edge(2, 2, 1));

		CHECK(grap.weights(1, 2) == std::vector<int>{1});
		CHECK(grap.weights(1, 3) == std::vector<int>{2});
		CHECK(grap.weights(1, 4) == std::vector<int>{3});
		CHECK(grap.weights(2, 2) == std::vector<int>{1});

		grap.merge_replace_node(1, 2);

		CHECK_FALSE(grap.is_node(1));

		CHECK(grap.weights(2, 2) == std::vector<int>{1});
		CHECK(grap.weights(2, 3) == std::vector<int>{2});
		CHECK(grap.weights(2, 4) == std::vector<int>{3});

		auto output = std::ostringstream();
		output << grap;
		auto const expected_os = std::string_view(R"(2 (
  2 | 1
  3 | 2
  4 | 3
)
3 (
)
4 (
)
)");
		CHECK(output.str() == expected_os);
	}
}

TEST_CASE("erase node") {
	SECTION("check no node exist and simple") {
		auto grap = gdwg::graph<int, int>{1, 2, 3};
		CHECK_FALSE(grap.erase_node(4));
		CHECK(grap.erase_node(3));
		CHECK_FALSE(grap.is_node(3));
	}

	SECTION("check edges are the same") {
		auto grap = gdwg::graph<int, int>{1, 2, 3};

		CHECK(grap.insert_edge(1, 1, 2));
		CHECK(grap.insert_edge(1, 2, 3));
		CHECK(grap.insert_edge(1, 2, 4));
		CHECK(grap.insert_edge(1, 3, 4));
		CHECK(grap.insert_edge(2, 3, 5));

		CHECK_FALSE(grap.find(1, 1, 2) == grap.end());
		CHECK_FALSE(grap.find(1, 2, 3) == grap.end());
		CHECK_FALSE(grap.find(1, 2, 4) == grap.end());
		CHECK_FALSE(grap.find(1, 3, 4) == grap.end());

		CHECK(grap.erase_node(1));

		CHECK_FALSE(grap.find(2, 3, 5) == grap.end());
		CHECK(grap.find(1, 1, 2) == grap.end());
		CHECK(grap.find(1, 2, 3) == grap.end());
		CHECK(grap.find(1, 2, 4) == grap.end());
		CHECK(grap.find(1, 3, 4) == grap.end());
	}
}

TEST_CASE("erase edge(N const& src, N const& dst, E const& weight) -> bool") {
	SECTION("check throws and errors") {
		auto grap = gdwg::graph<int, int>{1, 2, 3};

		CHECK(grap.insert_edge(1, 2, 10));

		// 10 and 11 doesnt exist
		CHECK_THROWS_MATCHES(grap.erase_edge(10, 11, 3),
		                     std::runtime_error,
		                     Catch::Matchers::Message("Cannot call gdwg::graph<N, E>::erase_edge on "
		                                              "src or dst if they don't exist in the graph"));
		// 11 doesnt exist
		CHECK_THROWS_MATCHES(grap.erase_edge(1, 11, 3),
		                     std::runtime_error,
		                     Catch::Matchers::Message("Cannot call gdwg::graph<N, E>::erase_edge on "
		                                              "src or dst if they don't exist in the graph"));
		// 10 doesnt exist
		CHECK_THROWS_MATCHES(grap.erase_edge(10, 2, 3),
		                     std::runtime_error,
		                     Catch::Matchers::Message("Cannot call gdwg::graph<N, E>::erase_edge on "
		                                              "src or dst if they don't exist in the graph"));
		CHECK_FALSE(grap.erase_edge(1, 2, 5));
	}

	SECTION("removed edge") {
		auto grap = gdwg::graph<int, int>{1, 2, 3};

		CHECK(grap.insert_edge(1, 1, 2));
		CHECK(grap.insert_edge(1, 2, 3));
		CHECK(grap.insert_edge(1, 2, 4));
		CHECK(grap.insert_edge(1, 3, 4));
		CHECK(grap.insert_edge(2, 3, 5));

		CHECK_FALSE(grap.find(1, 1, 2) == grap.end());
		CHECK_FALSE(grap.find(1, 2, 3) == grap.end());
		CHECK_FALSE(grap.find(1, 2, 4) == grap.end());
		CHECK_FALSE(grap.find(1, 3, 4) == grap.end());

		CHECK(grap.erase_edge(1, 2, 3));

		CHECK(grap.find(1, 2, 3) == grap.end());

		CHECK_FALSE(grap.find(2, 3, 5) == grap.end());
		CHECK_FALSE(grap.find(1, 1, 2) == grap.end());
		CHECK_FALSE(grap.find(1, 2, 4) == grap.end());
		CHECK_FALSE(grap.find(1, 3, 4) == grap.end());
	}
}

TEST_CASE("erase edge(iterator i) -> iterator") {
	SECTION("check large case") {
		auto grap = gdwg::graph<int, int>{1, 2, 3};
		CHECK(grap.insert_edge(1, 2, 5));
		CHECK(grap.insert_edge(1, 2, 3));
		CHECK(grap.insert_edge(1, 3, 2));
		CHECK(grap.insert_edge(1, 1, 2));

		// should be the next iterator
		CHECK(grap.erase_edge(grap.find(1, 2, 3)) == grap.find(1, 2, 5));

		CHECK(grap.find(1, 2, 3) == grap.end());
		CHECK(grap.find(1, 2, 5) != grap.end());
		CHECK(grap.find(1, 3, 2) != grap.end());
		CHECK(grap.find(1, 1, 2) != grap.end());

		// last edge iterator return end after removing
		CHECK(grap.erase_edge(grap.find(1, 3, 2)) == grap.end());

		CHECK(grap.find(1, 2, 3) == grap.end());
		CHECK(grap.find(1, 2, 5) != grap.end());
		CHECK(grap.find(1, 3, 2) == grap.end());
		CHECK(grap.find(1, 1, 2) != grap.end());
	}

	SECTION("check no edge exist and simple case") {
		auto grap = gdwg::graph<int, int>{1, 2, 3};
		// return end()
		CHECK(grap.erase_edge(grap.begin()) == grap.end());
		CHECK(grap.erase_edge(grap.end()) == grap.end());
		CHECK(grap.erase_edge(gdwg::graph<int, int>::iterator()) == grap.end());

		CHECK(grap.insert_edge(1, 2, 5));
		CHECK(grap.erase_edge(grap.begin()) == grap.end());
		CHECK(grap.find(1, 2, 5) == grap.end());
	}
}

TEST_CASE("erase edge(iterator i, iterator s) -> iterator") {
	SECTION("remove from begin to end") {
		auto grap = gdwg::graph<int, int>{1, 2, 3};
		CHECK(grap.insert_edge(1, 2, 5));
		CHECK(grap.insert_edge(1, 2, 3));
		CHECK(grap.insert_edge(1, 3, 2));
		CHECK(grap.insert_edge(1, 1, 2));

		// remove all
		CHECK(grap.erase_edge(grap.begin(), grap.end()) == grap.end());
		auto output = std::ostringstream();
		output << grap;
		auto const expected_os = std::string_view(R"(1 (
)
2 (
)
3 (
)
)");
		CHECK(output.str() == expected_os);
	}
	SECTION("remove large case") {
		auto grap = gdwg::graph<int, int>{1, 2, 3};
		CHECK(grap.insert_edge(1, 2, 5));
		CHECK(grap.insert_edge(1, 2, 3));
		CHECK(grap.insert_edge(1, 3, 2));
		CHECK(grap.insert_edge(1, 1, 2));

		CHECK(grap.erase_edge(grap.find(1, 2, 3), grap.find(1, 2, 5)) == grap.find(1, 2, 5));
		CHECK(grap.find(1, 2, 3) == grap.end());

		CHECK(grap.erase_edge(grap.find(1, 1, 2), grap.find(1, 3, 2)) == grap.find(1, 3, 2));
		CHECK(grap.find(1, 1, 2) == grap.end());
		CHECK(grap.find(1, 2, 5) == grap.end());
		CHECK(grap.find(1, 3, 2) != grap.end());

		CHECK(grap.insert_edge(1, 3, 3));
		// s does not exist so return end and remove everything between 1,3,2 and end
		CHECK(grap.erase_edge(grap.find(1, 3, 2), grap.find(10, 30, 20)) == grap.end());
		CHECK(grap.find(1, 3, 3) == grap.end());
		CHECK(grap.find(1, 3, 2) == grap.end());
	}
	SECTION("check no edge exist and simple case") {
		auto grap = gdwg::graph<int, int>{1, 2, 3};
		// return end()
		CHECK(grap.erase_edge(grap.begin(), grap.end()) == grap.end());

		CHECK(grap.insert_edge(1, 2, 5));
		CHECK(grap.erase_edge(grap.begin(), grap.end()) == grap.end());
		CHECK(grap.find(1, 2, 5) == grap.end());
	}
}

TEST_CASE("clear") {
	SECTION("empty graph") {
		auto grap = gdwg::graph<int, int>{};
		CHECK(grap.empty());
		grap.clear();
		CHECK(grap.empty());
	}

	SECTION("nodes") {
		auto grap = gdwg::graph<int, int>{1, 2, 3};
		CHECK_FALSE(grap.empty());
		grap.clear();
		CHECK(grap.empty());
	}
	SECTION("empty graph") {
		auto grap = gdwg::graph<int, int>{1, 2, 3};
		CHECK(grap.insert_edge(1, 2, 5));
		CHECK_FALSE(grap.empty());
		grap.clear();
		CHECK_THROWS_MATCHES(grap.is_connected(1, 2),
		                     std::runtime_error,
		                     Catch::Matchers::Message("Cannot call gdwg::graph<N, E>::is_connected "
		                                              "if src or dst node don't exist in the graph"));
		CHECK(grap.empty());
	}
}