#include "gdwg/graph.hpp"

#include <catch2/catch.hpp>

// Rationale: The test is to ensure that any edges cases, mainly from empty graph works for the
// functions as well as ensuring that internal representation for the graph even out of scope. Then
// test each function to ensure that the effects, postcondition, throws and returns are correct to
// the test. We can use is_node to ensure the node is inside the graph and the extractor to print
// the graph to ensure it is the right graph.

// basic test provided by the assignment
TEST_CASE("basic test") {
	// This will not compile straight away
	auto g = gdwg::graph<int, std::string>{};
	auto n = 5;
	g.insert_node(n);
	CHECK(g.is_node(n));
}

// Internal representation to ensure that out of scope graph keeps their resources
TEST_CASE("Compulsory internal representation") {
	gdwg::graph<std::string, int> g;
	{
		std::string s1{"Hello"};
		g.insert_node(s1);
	}

	// Even though s1 has gone out of scope, g has its own
	//  copied resource that it has stored, so the node
	//  will still be in here.
	CHECK(g.is_node("Hello") == true);
}

// Internal representation to ensure that out of scope graph keeps their resources
TEST_CASE("Compulsory internal representation with edges") {
	gdwg::graph<std::string, int> g;
	{
		std::string s1{"Hello"};
		std::string s2{"Bob"};
		g.insert_node(s1);
		g.insert_node(s2);
		g.insert_edge(s1, s2, 5);
	}

	// Even though s1 has gone out of scope, g has its own
	//  copied resource that it has stored, so the node
	//  will still be in here.
	CHECK(g.is_node("Hello") == true);
	CHECK(g.is_node("Bob") == true);
	CHECK(g.is_connected("Hello", "Bob") == true);
	CHECK(g.weights("Hello", "Bob") == std::vector<int>{5});
}