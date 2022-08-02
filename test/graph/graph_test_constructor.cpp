#include "gdwg/graph.hpp"

#include <catch2/catch.hpp>
#include <list>
#include <set>
#include <sstream>
#include <string>
#include <vector>

// default constructor is empty
TEST_CASE("default Constructor") {
	SECTION("string, int") {
		auto grap1 = gdwg::graph<std::string, int>{};
		CHECK(grap1.empty());
	}

	SECTION("int, string") {
		auto grap2 = gdwg::graph<int, std::string>{};
		CHECK(grap2.empty());
	}
	SECTION("int, int") {
		auto grap3 = gdwg::graph<int, int>{};
		CHECK(grap3.empty());

		auto output = std::stringstream();
		output << grap3;
		CHECK(output.str().empty());
	}
}

// initializer_list
TEST_CASE("initializer list Constructor") {
	SECTION("int, int") {
		auto grap1 = gdwg::graph<int, int>{1, 2, 3, 4, 5, 6};
		CHECK(grap1.is_node(1));
		CHECK(grap1.is_node(2));
		CHECK(grap1.is_node(3));
		CHECK(grap1.is_node(4));
		CHECK(grap1.is_node(5));
		CHECK(grap1.is_node(6));

		auto output = std::stringstream();
		output << grap1;
		std::string s_grap = "1 (\n)\n2 (\n)\n3 (\n)\n4 (\n)\n5 (\n)\n6 (\n)\n";
		CHECK(output.str() == s_grap);
	}

	SECTION("string, string") {
		auto grap2 = gdwg::graph<std::string, std::string>{"a", "b", "c", "d"};
		CHECK(grap2.is_node("a"));
		CHECK(grap2.is_node("b"));
		CHECK(grap2.is_node("c"));
		CHECK(grap2.is_node("d"));

		auto output = std::stringstream();
		output << grap2;
		std::string s_grap = "a (\n)\nb (\n)\nc (\n)\nd (\n)\n";
		CHECK(output.str() == s_grap);
	}
	SECTION("vector, vector") {
		auto vec1 = std::vector<int>{1, 2, 3, 4};
		auto vec2 = std::vector<int>{5, 6, 7, 8};
		auto vec3 = std::vector<int>{9, 10, 11, 12};
		auto grap3 = gdwg::graph<std::vector<int>, std::vector<int>>{vec1, vec2, vec3};
		CHECK(grap3.is_node(vec1));
		CHECK(grap3.is_node(vec2));
		CHECK(grap3.is_node(vec3));
	}
	SECTION("list, list") {
		auto lis1 = std::list<std::string>{"1", "2", "3", "4"};
		auto lis2 = std::list<std::string>{"5", "6", "7", "8"};
		auto lis3 = std::list<std::string>{"9", "10", "11", "12"};
		auto grap3 = gdwg::graph<std::list<std::string>, std::list<std::string>>{lis1, lis2, lis3};
		CHECK(grap3.is_node(lis1));
		CHECK(grap3.is_node(lis2));
		CHECK(grap3.is_node(lis3));
	}
	// should be equivilent to il.begin, li.end
	SECTION("list of stirng") {
		auto lis1 = std::initializer_list<int>{1, 2, 3};
		auto grap3 = gdwg::graph<int, int>{lis1};
		CHECK(grap3.is_node(1));
		CHECK(grap3.is_node(2));
		CHECK(grap3.is_node(3));
		auto grap4 = gdwg::graph<int, int>{lis1.begin(), lis1.end()};
		CHECK(grap3 == grap4);
	}
}

// input iterator
TEST_CASE("input iterator Constructor") {
	SECTION("vector it") {
		auto vec = std::vector<int>{1, 2, 3, 4, 5};
		auto grap1 = gdwg::graph<int, int>{vec.begin(), vec.end()};
		CHECK(grap1.is_node(1));
		CHECK(grap1.is_node(2));
		CHECK(grap1.is_node(3));
		CHECK(grap1.is_node(4));
		CHECK(grap1.is_node(5));

		auto output = std::stringstream();
		output << grap1;
		std::string s_grap = "1 (\n)\n2 (\n)\n3 (\n)\n4 (\n)\n5 (\n)\n";
		CHECK(output.str() == s_grap);
	}

	SECTION("empty vec") {
		auto vec = std::vector<int>{};
		auto grap1 = gdwg::graph<int, int>{vec.begin(), vec.end()};
		CHECK(grap1.empty());

		auto output = std::stringstream();
		output << grap1;
		std::string s_grap = "1 (\n)\n2 (\n)\n3 (\n)\n4 (\n)\n5 (\n)\n6 (\n)\n";
		CHECK(output.str().empty());
	}
	SECTION("list it") {
		auto lis = std::list<int>{1, 2, 3, 4, 5};
		auto grap1 = gdwg::graph<int, int>{lis.begin(), lis.end()};
		CHECK(grap1.is_node(1));
		CHECK(grap1.is_node(2));
		CHECK(grap1.is_node(3));
		CHECK(grap1.is_node(4));
		CHECK(grap1.is_node(5));

		auto output = std::stringstream();
		output << grap1;
		std::string s_grap = "1 (\n)\n2 (\n)\n3 (\n)\n4 (\n)\n5 (\n)\n";
		CHECK(output.str() == s_grap);
	}
	SECTION("set it") {
		auto se = std::set<int>{1, 2, 3, 4, 5};
		auto grap1 = gdwg::graph<int, int>{se.begin(), se.end()};
		CHECK(grap1.is_node(1));
		CHECK(grap1.is_node(2));
		CHECK(grap1.is_node(3));
		CHECK(grap1.is_node(4));
		CHECK(grap1.is_node(5));

		auto output = std::stringstream();
		output << grap1;
		std::string s_grap = "1 (\n)\n2 (\n)\n3 (\n)\n4 (\n)\n5 (\n)\n";
		CHECK(output.str() == s_grap);
	}
}

// move constructor
TEST_CASE("move Constructor") {
	SECTION("int nodes") {
		auto grap1 = gdwg::graph<int, int>{1, 2, 3, 4};
		CHECK(grap1.is_node(1));
		CHECK(grap1.is_node(2));
		CHECK(grap1.is_node(3));
		CHECK(grap1.is_node(4));
		auto moved = gdwg::graph<int, int>(std::move(grap1));
		CHECK(moved.is_node(1));
		CHECK(moved.is_node(2));
		CHECK(moved.is_node(3));
		CHECK(moved.is_node(4));

		// Had to no lint this since it warned it is using a variable after move
		CHECK(grap1.empty()); // NOLINT
	}
	SECTION("include edges") {
		auto grap1 = gdwg::graph<int, int>{1, 2, 3};
		CHECK(grap1.is_node(1));
		CHECK(grap1.is_node(2));
		CHECK(grap1.is_node(3));
		grap1.insert_edge(1, 2, 6);

		auto moved = gdwg::graph<int, int>(std::move(grap1));
		CHECK(moved.is_node(1));
		CHECK(moved.is_node(2));
		CHECK(moved.is_node(3));
		CHECK(moved.is_connected(1, 2));
		CHECK(moved.weights(1, 2) == std::vector<int>{6});

		// Had to no lint this since it warned it is using a variable after move
		CHECK(grap1.empty()); // NOLINT
	}
}

// move assignment
TEST_CASE("move assignment") {
	SECTION("int nodes") {
		auto grap1 = gdwg::graph<int, int>{1, 2, 3, 4};
		CHECK(grap1.is_node(1));
		CHECK(grap1.is_node(2));
		CHECK(grap1.is_node(3));
		CHECK(grap1.is_node(4));

		auto moved = gdwg::graph<int, int>{};
		moved = std::move(grap1);
		CHECK(moved.is_node(1));
		CHECK(moved.is_node(2));
		CHECK(moved.is_node(3));
		CHECK(moved.is_node(4));

		// Had to no lint this since it warned it is using a variable after move
		CHECK(grap1.empty()); // NOLINT
	}
	SECTION("include edges") {
		auto grap1 = gdwg::graph<int, int>{1, 2, 3};
		CHECK(grap1.is_node(1));
		CHECK(grap1.is_node(2));
		CHECK(grap1.is_node(3));
		grap1.insert_edge(1, 2, 6);

		auto moved = gdwg::graph<int, int>{};
		moved = std::move(grap1);
		CHECK(moved.is_node(1));
		CHECK(moved.is_node(2));
		CHECK(moved.is_node(3));
		CHECK(moved.is_connected(1, 2));
		CHECK(moved.weights(1, 2) == std::vector<int>{6});

		// Had to no lint this since it warned it is using a variable after move
		CHECK(grap1.empty()); // NOLINT
	}
}

// copy constructor
TEST_CASE("copy constructor") {
	SECTION("int nodes") {
		auto grap1 = gdwg::graph<int, int>{1, 2, 3, 4};
		CHECK(grap1.is_node(1));
		CHECK(grap1.is_node(2));
		CHECK(grap1.is_node(3));
		CHECK(grap1.is_node(4));

		auto graph1_copy = grap1;
		CHECK(graph1_copy.is_node(1));
		CHECK(graph1_copy.is_node(2));
		CHECK(graph1_copy.is_node(3));
		CHECK(graph1_copy.is_node(4));
		// Postconditions: *this == other is true.
		CHECK(graph1_copy == grap1);

		// Check changing 1 would not change another
		CHECK(graph1_copy.replace_node(1, 10));
		CHECK_FALSE(graph1_copy.is_node(1));
		CHECK(graph1_copy.is_node(10));
		CHECK(graph1_copy.is_node(2));
		CHECK(graph1_copy.is_node(3));
		CHECK(graph1_copy.is_node(4));

		CHECK_FALSE(grap1.is_node(10));
		CHECK(grap1.is_node(1));
		CHECK(grap1.is_node(2));
		CHECK(grap1.is_node(3));
		CHECK(grap1.is_node(4));
	}
	SECTION("include edges") {
		auto grap1 = gdwg::graph<int, int>{1, 2, 3};
		CHECK(grap1.is_node(1));
		CHECK(grap1.is_node(2));
		CHECK(grap1.is_node(3));
		grap1.insert_edge(1, 2, 6);
		grap1.insert_edge(2, 3, 10);

		auto graph1_copy = grap1;

		CHECK(graph1_copy.is_node(1));
		CHECK(graph1_copy.is_node(2));
		CHECK(graph1_copy.is_node(3));
		CHECK(graph1_copy.is_connected(1, 2));
		CHECK(graph1_copy.weights(1, 2) == std::vector<int>{6});
		CHECK(graph1_copy.is_connected(2, 3));
		CHECK(graph1_copy.weights(2, 3) == std::vector<int>{10});
	}
}

// copy assignment
TEST_CASE("copy assignment") {
	SECTION("int nodes") {
		auto grap1 = gdwg::graph<int, int>{1, 2, 3, 4};
		CHECK(grap1.is_node(1));
		CHECK(grap1.is_node(2));
		CHECK(grap1.is_node(3));
		CHECK(grap1.is_node(4));

		auto graph1_copy = gdwg::graph<int, int>{5, 6, 7, 8};
		CHECK(graph1_copy.is_node(5));
		CHECK(graph1_copy.is_node(6));
		CHECK(graph1_copy.is_node(7));
		CHECK(graph1_copy.is_node(8));

		graph1_copy = grap1;
		CHECK(graph1_copy.is_node(1));
		CHECK(graph1_copy.is_node(2));
		CHECK(graph1_copy.is_node(3));
		CHECK(graph1_copy.is_node(4));
		CHECK(grap1.is_node(1));
		CHECK(grap1.is_node(2));
		CHECK(grap1.is_node(3));
		CHECK(grap1.is_node(4));
		// Postconditions: *this == other is true.
		CHECK(graph1_copy == grap1);

		// Check changing 1 would not change another
		CHECK(graph1_copy.replace_node(1, 10));
		CHECK_FALSE(graph1_copy.is_node(1));
		CHECK(graph1_copy.is_node(10));
		CHECK(graph1_copy.is_node(2));
		CHECK(graph1_copy.is_node(3));
		CHECK(graph1_copy.is_node(4));

		CHECK_FALSE(grap1.is_node(10));
		CHECK(grap1.is_node(1));
		CHECK(grap1.is_node(2));
		CHECK(grap1.is_node(3));
		CHECK(grap1.is_node(4));
	}
	SECTION("include edges") {
		auto grap1 = gdwg::graph<int, int>{1, 2, 3};
		CHECK(grap1.is_node(1));
		CHECK(grap1.is_node(2));
		CHECK(grap1.is_node(3));
		grap1.insert_edge(1, 2, 6);
		grap1.insert_edge(2, 3, 10);

		auto graph1_copy = gdwg::graph<int, int>{5, 6, 7, 8};
		CHECK(graph1_copy.is_node(5));
		CHECK(graph1_copy.is_node(6));
		CHECK(graph1_copy.is_node(7));
		CHECK(graph1_copy.is_node(8));

		graph1_copy = grap1;
		CHECK_FALSE(graph1_copy.is_node(5));
		CHECK(graph1_copy.is_node(1));
		CHECK(graph1_copy.is_node(2));
		CHECK(graph1_copy.is_node(3));
		CHECK(grap1.is_node(1));
		CHECK(grap1.is_node(2));
		CHECK(grap1.is_node(3));
		// Postconditions: *this == other is true.
		CHECK(graph1_copy == grap1);

		CHECK(graph1_copy.is_connected(1, 2));
		CHECK(graph1_copy.weights(1, 2) == std::vector<int>{6});
		CHECK(graph1_copy.is_connected(2, 3));
		CHECK(graph1_copy.weights(2, 3) == std::vector<int>{10});
	}
}