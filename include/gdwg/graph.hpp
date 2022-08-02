#ifndef GDWG_GRAPH_HPP
#define GDWG_GRAPH_HPP

#include <algorithm>
#include <cassert>
#include <experimental/iterator>
#include <iostream>
#include <iterator>
#include <memory>
#include <set>
#include <sstream>
#include <stdexcept>
#include <tuple>
#include <utility>
#include <vector>

// This will not compile straight away
namespace gdwg {
	template<typename N, typename E>
	class graph {
	public:
		class iterator;
		// struct to represent a edge from source to the destination(dest) Node with a weight
		struct edge {
			N* source;
			N* dest;
			E weight;
		};
		// Value type struct from assignment specs
		struct value_type {
			N from;
			N to;
			E weight;
		};

		// default constructor
		graph() noexcept = default;
		// using iterator to initialize the list constructor since its the same as using iterator
		graph(std::initializer_list<N> il)
		: graph(il.begin(), il.end()) {}

		// iterator constructor
		template<typename InputIt>
		graph(InputIt first, InputIt last)
		: graph() {
			std::for_each(first, last, [this](N const& n) { insert_node(n); });
		}

		// Move Constructor
		graph(graph&& other) noexcept
		: graph() {
			nodes_ = std::move(other.nodes_);
			edges_ = std::move(other.edges_);
		}

		// Move Assignment
		auto operator=(graph&& other) noexcept -> graph& {
			// no self assignment
			if (this == std::addressof(other)) {
				return *this;
			}

			auto copy = graph(std::move(other));
			copy.swap(*this);

			// ensure the other is empty
			other.nodes_.clear();
			other.edges_.clear();

			return *this;
		}

		// Copy Constructor, copy the nodes as well as the edges of the struct
		graph(graph const& other)
		: graph() {
			std::transform(other.nodes_.begin(),
			               other.nodes_.end(),
			               std::inserter(nodes_, nodes_.end()),
			               [&](auto const& node_ptr) { return std::make_shared<N>(*node_ptr); });

			std::transform(other.edges_.begin(),
			               other.edges_.end(),
			               std::inserter(edges_, edges_.end()),
			               [&](auto const& edge_ptr) {
				               struct edge n_edge = edge{(*(nodes_.find(*(edge_ptr->source)))).get(),
				                                         (*(nodes_.find(*(edge_ptr->dest)))).get(),
				                                         edge_ptr->weight};
				               return std::make_shared<edge>(n_edge);
			               });
		}

		// Copy Assignment
		auto operator=(graph const& other) -> graph& {
			auto copy = graph(other);
			copy.swap(*this);
			return *this;
		}

		// Modifiers

		// insert node, checks if there is a value already in the graph
		auto insert_node(N const& value) -> bool {
			if (!is_node(value)) {
				return nodes_.emplace(std::make_shared<N>(value)).second;
			}
			return false;
		}

		// insert edge,checks if a edge of this already exist
		auto insert_edge(N const& src, N const& dst, E const& weight) -> bool {
			if (!is_node(src) || !is_node(dst)) {
				throw std::runtime_error("Cannot call gdwg::graph<N, E>::insert_edge when either src "
				                         "or dst node does not exist");
			}
			auto source_node = (*(nodes_.find(src))).get();
			auto dest_node = (*(nodes_.find(dst))).get();
			struct edge n_edge = {source_node, dest_node, weight};
			return edges_.emplace(std::make_shared<edge>(n_edge)).second;
		}

		// replace a node, checks if new_data exist and is_node(old_data) is false
		auto replace_node(N const& old_data, N const& new_data) -> bool {
			if (!is_node(old_data)) {
				throw std::runtime_error("Cannot call gdwg::graph<N, E>::replace_node on a node that "
				                         "doesn't exist");
			}
			if (is_node(new_data)) {
				return false;
			}
			// Insert the node and then just replace old node with the new node
			insert_node(new_data);
			merge_replace_node(old_data, new_data);
			return true;
		}

		// merge replace node
		auto merge_replace_node(N const& old_data, N const& new_data) -> void {
			if (!is_node(old_data) || !is_node(new_data)) {
				throw std::runtime_error("Cannot call gdwg::graph<N, E>::merge_replace_node on old or "
				                         "new data if they don't exist in the graph");
			}
			// get all the nodes that are old_data and merge them together
			auto e_ptrs = std::vector<std::shared_ptr<edge>>();
			std::copy_if(edges_.begin(),
			             edges_.end(),
			             std::back_inserter(e_ptrs),
			             [&](auto const& edge_ptr) {
				             return *(edge_ptr->source) == old_data || *(edge_ptr->dest) == old_data;
			             });
			auto new_node = nodes_.find(new_data);
			for (auto const& ptr : e_ptrs) {
				auto new_source = (*new_node).get();
				auto new_dest = (*new_node).get();
				if (*(ptr->source) != old_data) {
					new_source = ptr->source;
				}
				if (*(ptr->dest) != old_data) {
					new_dest = ptr->dest;
				}
				// make a new edge and erase the old one
				struct edge n_edge = edge{new_source, new_dest, ptr->weight};
				edges_.erase(ptr);
				// check if edge exist
				if (edges_.find(n_edge) == edges_.end()) {
					edges_.emplace(std::make_shared<edge>(n_edge));
				}
			}
			// erase the old node
			auto remove_old = nodes_.find(old_data);
			nodes_.erase(remove_old);
		}

		// erase node and edges of that node
		auto erase_node(N const& value) -> bool {
			// No lint this line becuase of the redudant literal since we need to check if the node
			// exist if not we return false if it does then we continue with program
			if (!is_node(value)) {
				return false; // NOLINT
			}
			// in the edges if the source or dest of the edges is the value we erase it
			std::erase_if(edges_, [&](auto const& erase_edge) {
				return *(erase_edge->source) == value || *(erase_edge->dest) == value;
			});
			// after erasing edges erase the node
			nodes_.erase(nodes_.find(value));
			return true;
		}

		// erase a edge, given src, dest and weight
		auto erase_edge(N const& src, N const& dst, E const& weight) -> bool {
			// since is_node is O(log(n)) this would be 2log(n)
			if (!is_node(src) || !is_node(dst)) {
				throw std::runtime_error("Cannot call gdwg::graph<N, E>::erase_edge on src or dst if "
				                         "they don't exist in the graph");
			}
			// since it goes through all edges it is e time
			// count the number of edges erased if edge equal the src, dest and weight
			auto num_erased = std::erase_if(edges_, [&](auto const& erase_edge) {
				return *(erase_edge->source) == src && *(erase_edge->dest) == dst
				       && erase_edge->weight == weight;
			});
			// Check if it has erased any edges
			return !(num_erased == 0);
			// time complexity = O(2log(n) + e) = O(log(n) + e)
		}
		// erase edge using a iterator implemened after iterator implemented
		auto erase_edge(iterator i) -> iterator;
		auto erase_edge(iterator i, iterator s) -> iterator;

		// clear all nodes and edges
		auto clear() noexcept -> void {
			nodes_.clear();
			edges_.clear();
		}

		// Accessors
		// check if a node exist with value
		// since nodes_ is a set, find will be a O(log(n)) time complexity
		// const it so that a const graph can also use this function
		[[nodiscard]] auto is_node(N const& value) const noexcept -> bool {
			return nodes_.find(value) != nodes_.end();
		}

		// check if nodes set are empty
		// const it so that a const graph can also use this function
		[[nodiscard]] auto empty() const noexcept -> bool {
			return nodes_.empty();
		}

		// check if two nodes are connected
		[[nodiscard]] auto is_connected(N const& src, N const& dst) const -> bool {
			if (!is_node(src) || !is_node(dst)) {
				throw std::runtime_error("Cannot call gdwg::graph<N, E>::is_connected if src or dst "
				                         "node don't exist in the graph");
			}
			// using the any_of function we can check if any of of edge struct equal to src and dst
			return std::any_of(edges_.begin(), edges_.end(), [&](auto const& edge_ptr) {
				return *(edge_ptr->source) == src && *(edge_ptr->dest) == dst;
			});
		}

		// gets in ascending order of all stored nodes
		[[nodiscard]] auto nodes() const -> std::vector<N> {
			auto ret = std::vector<N>();
			// for each node push back into vector n complexity since it goes through the nodes_
			std::for_each(nodes_.begin(), nodes_.end(), [&ret](auto const& node_ptr) {
				ret.push_back(*node_ptr);
			});
			return ret;
		}

		// gets the sequence of weights between two nodes
		[[nodiscard]] auto weights(N const& src, N const& dst) const -> std::vector<E> {
			// log(n) time
			if (!is_node(src) || !is_node(dst)) {
				throw std::runtime_error("Cannot call gdwg::graph<N, E>::weights if src or dst node "
				                         "don't exist in the graph");
			}
			auto ret = std::vector<E>();
			// e time
			std::for_each(edges_.begin(), edges_.end(), [&dst, &src, &ret](auto const& edge_ptr) {
				if (*(edge_ptr->source) == src && *(edge_ptr->dest) == dst) {
					ret.push_back(edge_ptr->weight);
				}
			});
			// time complexity log(n) + e
			return ret;
		}

		// finds edge with src, dst and weight
		[[nodiscard]] auto find(N const& src, N const& dst, E const& weight) const -> iterator {
			// use the set find function should be log(n) + log(e)
			return iterator{edges_.find(value_type{src, dst, weight})};
		}

		// find the sequence of nodes connected to src
		[[nodiscard]] auto connections(N const& src) const -> std::vector<N> {
			// log(n) time complexity
			if (!is_node(src)) {
				throw std::runtime_error("Cannot call gdwg::graph<N, E>::connections if src doesn't "
				                         "exist in the graph");
			}
			auto ret = std::vector<N>();
			// time to go through edge = e
			std::for_each(edges_.begin(), edges_.end(), [&src, &ret](auto const& edge_ptr) {
				if (*(edge_ptr->source) == src) {
					ret.push_back(*(edge_ptr->dest));
				}
			});
			return ret;
			// time complexity = log(n) + e
		}
		// Iterator Access
		[[nodiscard]] auto begin() const -> iterator {
			return iterator{edges_.begin()};
		}
		[[nodiscard]] auto end() const -> iterator {
			return iterator{edges_.end()};
		}
		// Comparision
		// check if two graphs are the same
		// should be const qualified: https://edstem.org/au/courses/8629/discussion/944057
		[[nodiscard]] auto operator==(graph const& other) const noexcept -> bool {
			// use the std::equals function to check if the nodes set and edge sets are equal
			// by checking each node and all the source, dest and weight for the edges. In O(n + e)
			// since it traverse through both graph once
			auto node_equal = std::equal(nodes_.begin(),
			                             nodes_.end(),
			                             other.nodes_.begin(),
			                             other.nodes_.end(),
			                             [](auto const& lhs, auto const& rhs) { return *lhs == *rhs; });
			auto edge_equal = std::equal(edges_.begin(),
			                             edges_.end(),
			                             other.edges_.begin(),
			                             other.edges_.end(),
			                             [](auto const& lhs, auto const& rhs) {
				                             return *(lhs->source) == *(rhs->source)
				                                    && *(lhs->dest) == *(rhs->dest)
				                                    && lhs->weight == rhs->weight;
			                             });
			;
			return node_equal && edge_equal;
		}

		// Extractor
		friend auto operator<<(std::ostream& os, graph const& g) -> std::ostream& {
			std::for_each(g.nodes_.begin(), g.nodes_.end(), [&](auto const& node_ptr) {
				os << *node_ptr << " (\n";
				std::for_each(g.edges_.begin(), g.edges_.end(), [&, node_ptr](auto const& edge_ptr) {
					if (*(edge_ptr->source) == *node_ptr) {
						os << "  " << *(edge_ptr->dest) << " | " << edge_ptr->weight << "\n";
					}
				});
				os << ")\n";
			});
			return os;
		}

	private:
		struct node_comp {
			// adding the comparisions for the node pointers between a N and a shared pointer of N and
			// the different combination to comapre them
			using is_transparent = void;

			auto operator()(std::shared_ptr<N> const& lhs, std::shared_ptr<N> const& rhs) const noexcept
			   -> bool {
				return *lhs < *rhs;
			}

			auto operator()(N const& lhs, std::shared_ptr<N> const& rhs) const noexcept -> bool {
				return lhs < *rhs;
			}

			auto operator()(std::shared_ptr<N> const& lhs, N const& rhs) const noexcept -> bool {
				return *lhs < rhs;
			}
		};
		struct edge_comp {
			// adding the comparision for the edges between the edge struct, the edge pointers and the
			// value type
			using is_transparent = void;
			// since the struct comprise of N, N* and E we can convert the structs to tuples and then
			// compare them using std::tie
			auto operator()(struct value_type const& lhs,
			                std::shared_ptr<edge> const& rhs) const noexcept -> bool {
				return std::tie(lhs.from, lhs.to, lhs.weight)
				       < std::tie(*(rhs->source), *(rhs->dest), rhs->weight);
			}
			auto operator()(std::shared_ptr<edge> const& lhs,
			                struct value_type const& rhs) const noexcept -> bool {
				return std::tie(*(lhs->source), *(lhs->dest), lhs->weight)
				       < std::tie(rhs.from, rhs.to, rhs.weight);
			}
			auto operator()(std::shared_ptr<edge> const& lhs,
			                std::shared_ptr<edge> const& rhs) const noexcept -> bool {
				return std::tie(*(lhs->source), *(lhs->dest), lhs->weight)
				       < std::tie(*(rhs->source), *(rhs->dest), rhs->weight);
			}
			auto operator()(std::shared_ptr<edge> const& lhs, struct edge const& rhs) const noexcept
			   -> bool {
				return std::tie(*(lhs->source), *(lhs->dest), lhs->weight)
				       < std::tie(*(rhs.source), *(rhs.dest), rhs.weight);
			}
			auto operator()(struct edge const& lhs, std::shared_ptr<edge> const& rhs) const noexcept
			   -> bool {
				return std::tie(*(lhs.source), *(lhs.dest), lhs.weight)
				       < std::tie(*(rhs->source), *(rhs->dest), rhs->weight);
			}
			auto operator()(struct edge const& lhs, struct edge const& rhs) const noexcept -> bool {
				return std::tie(*(lhs.source), *(lhs.dest), lhs.weight)
				       < std::tie(*(rhs.source), *(rhs.dest), rhs.weight);
			}
			auto operator()(struct value_type const& lhs, struct value_type const& rhs) const noexcept
			   -> bool {
				return std::tie(lhs.from, lhs.to, lhs.weight) < std::tie(rhs.from, rhs.to, rhs.weight);
			}
		};
		// nodes are just set of N
		std::set<std::shared_ptr<N>, node_comp> nodes_;
		// edges are just a set of the struct edge
		std::set<std::shared_ptr<edge>, edge_comp> edges_;

		// function to swap two graphs
		auto swap(graph<N, E>& other) noexcept -> void {
			std::swap(nodes_, other.nodes_);
			std::swap(edges_, other.edges_);
		}
	};
	template<typename N, typename E>
	class graph<N, E>::iterator {
	private:
		using edge_iterator = typename std::set<std::shared_ptr<edge>, edge_comp>::iterator;
		edge_iterator iter_;

	public:
		// iterator class from the assignment specs
		using value_type = graph<N, E>::value_type;
		using reference = value_type;
		using pointer = void;
		using difference_type = std::ptrdiff_t;
		using iterator_category = std::bidirectional_iterator_tag;

		// Iterator constructor
		iterator() = default;

		// Iterator source
		auto operator*() const -> reference {
			auto ret = value_type{};
			ret.from = *((*iter_)->source);
			ret.to = *((*iter_)->dest);
			ret.weight = (*iter_)->weight;
			return ret;
		}
		// auto operator->() -> pointer not required

		// Iterator traversal
		auto operator++() -> iterator& {
			++iter_;
			return *this;
		}
		auto operator++(int) -> iterator {
			auto copy = *this;
			++(*this);
			return copy;
		}
		auto operator--() -> iterator& {
			--iter_;
			return *this;
		}
		auto operator--(int) -> iterator {
			auto copy = *this;
			--(*this);
			return copy;
		}

		// Iterator comparison
		// Should be const qualified: https://edstem.org/au/courses/8629/discussion/944057
		auto operator==(iterator const& other) const -> bool {
			return iter_ == other.iter_;
		}

	private:
		explicit iterator(edge_iterator it)
		: iter_{it} {}
		friend class graph<N, E>;
	};
	template<typename N, typename E>
	// erase edge given the iterator and return next element
	auto graph<N, E>::erase_edge(iterator i) -> iterator {
		// for when it is the end of the iterator or when it is the begin of the iterator
		// since it would seg fault if not here
		if (i == end() || i == iterator{}) {
			return end();
		}
		// use the sets erase
		return iterator{edges_.erase(i.iter_)};
	}
	// erase edge in O(d) time where d is the number of edges between i and s
	template<typename N, typename E>
	auto graph<N, E>::erase_edge(iterator i, iterator s) -> iterator {
		return iterator{edges_.erase(i.iter_, s.iter_)};
	}

} // namespace gdwg

#endif // GDWG_GRAPH_HPP
