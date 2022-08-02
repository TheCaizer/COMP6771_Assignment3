#include "gdwg/graph.hpp"

#include <catch2/catch.hpp>
#include <list>
#include <set>
#include <sstream>
#include <string>
#include <vector>

TEST_CASE("Iterator access") {
	SECTION("empty") {
		auto const empty = gdwg::graph<int, int>{};
		CHECK(empty.begin() == empty.end());
	}
	// Pursuant to the requirements of std::forward_iterator, two value-initialised iterators shall
	// compare equal.
	CHECK(gdwg::graph<int, int>::iterator() == gdwg::graph<int, int>::iterator());
	SECTION("begin and explicit iterator") {
		auto grap = gdwg::graph<int, int>{1, 2, 3};
		grap.insert_edge(1, 2, 3);
		grap.insert_edge(2, 2, 3);
		CHECK(grap.begin() == gdwg::graph<int, int>::iterator(grap.find(1, 2, 3)));
		CHECK(++grap.begin() == gdwg::graph<int, int>::iterator(grap.find(2, 2, 3)));
	}
}

TEST_CASE("dereferencing") {
	auto grap = gdwg::graph<int, int>{1, 2, 3};
	grap.insert_edge(1, 2, 3);
	grap.insert_edge(2, 2, 3);

	auto const value_type = grap.begin();
	CHECK((*value_type).from == 1);
	CHECK((*value_type).to == 2);
	CHECK((*value_type).weight == 3);

	auto const grap_const = grap;

	auto const value_type2 = ++grap_const.begin();
	CHECK((*value_type2).from == 2);
	CHECK((*value_type2).to == 2);
	CHECK((*value_type2).weight == 3);
}

TEST_CASE("traversal pre -- and ++") {
	auto grap = gdwg::graph<int, int>{1, 2, 3};
	grap.insert_edge(1, 2, 3);
	grap.insert_edge(2, 2, 3);
	grap.insert_edge(1, 1, 2);
	grap.insert_edge(1, 3, 2);
	grap.insert_edge(3, 2, 10);

	auto iter = grap.begin();
	++iter;
	CHECK(grap.begin() == --iter);

	SECTION("Loop through check ++") {
		iter = grap.begin();
		auto index = static_cast<unsigned long>(0);
		auto expected = std::vector<gdwg::graph<int, int>::value_type>{{1, 1, 2},
		                                                               {1, 2, 3},
		                                                               {1, 3, 2},
		                                                               {2, 2, 3},
		                                                               {3, 2, 10}};
		for (; iter != grap.end(); ++iter, ++index) {
			CHECK((*iter).from == expected[index].from);
			CHECK((*iter).to == expected[index].to);
			CHECK((*iter).weight == expected[index].weight);
		}
	}

	SECTION("Loop through check --") {
		iter = grap.begin();
		++iter;
		++iter;
		++iter;
		++iter;

		auto expected = std::vector<gdwg::graph<int, int>::value_type>{{1, 1, 2},
		                                                               {1, 2, 3},
		                                                               {1, 3, 2},
		                                                               {2, 2, 3},
		                                                               {3, 2, 10}};
		auto index = static_cast<unsigned long>(expected.size() - 1);
		for (; iter != grap.begin(); --iter, --index) {
			CHECK((*iter).from == expected[index].from);
			CHECK((*iter).to == expected[index].to);
			CHECK((*iter).weight == expected[index].weight);
		}

		CHECK((*iter).from == expected[index].from);
		CHECK((*iter).to == expected[index].to);
		CHECK((*iter).weight == expected[index].weight);
	}
}
TEST_CASE("traversal post -- and ++") {
	auto grap = gdwg::graph<int, int>{1, 2, 3};
	grap.insert_edge(1, 2, 3);
	grap.insert_edge(2, 2, 3);
	grap.insert_edge(1, 1, 2);
	grap.insert_edge(1, 3, 2);
	grap.insert_edge(3, 2, 10);
	SECTION("post ++") {
		auto iter = grap.begin();
		auto next = iter++;

		CHECK((*iter).from == 1);
		CHECK((*iter).to == 2);
		CHECK((*iter).weight == 3);

		CHECK((*next).from == 1);
		CHECK((*next).to == 1);
		CHECK((*next).weight == 2);
	}

	SECTION("post --") {
		auto iter = grap.begin();
		++iter;
		auto next = iter--;
		CHECK((*iter).from == 1);
		CHECK((*iter).to == 1);
		CHECK((*iter).weight == 2);

		CHECK((*next).from == 1);
		CHECK((*next).to == 2);
		CHECK((*next).weight == 3);
	}
}

TEST_CASE("iterator comparision ==") {
	auto grap = gdwg::graph<int, int>{1, 2, 3};
	grap.insert_edge(1, 2, 3);
	grap.insert_edge(2, 2, 3);
	grap.insert_edge(1, 1, 2);
	grap.insert_edge(1, 3, 2);
	grap.insert_edge(3, 2, 10);
	SECTION("true comps") {
		auto iter = grap.begin();
		CHECK(iter == grap.find(1, 1, 2));
		++iter;
		CHECK(iter == grap.find(1, 2, 3));
		++iter;
		CHECK(iter == grap.find(1, 3, 2));
		++iter;
		CHECK(iter == grap.find(2, 2, 3));
		++iter;
		CHECK(iter == grap.find(3, 2, 10));
		++iter;
		CHECK(iter == grap.end());
	}

	SECTION("false comps") {
		auto iter = grap.begin();
		CHECK_FALSE(iter == grap.end());
		CHECK(iter != grap.end());

		CHECK_FALSE(++iter == grap.end());
		CHECK(++iter != grap.end());

		CHECK_FALSE(--iter == grap.end());
		CHECK(--iter != grap.end());
	}
}