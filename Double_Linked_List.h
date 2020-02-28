#pragma once
#include <iostream>
using std::cout;
using std::ostream;
#include <string>
using std::string;
#include <vector>
using std::vector;
#include <sstream>
using std::ostringstream;
// DO NOT MODIFY
//
// Node
//
struct Node {
	string key{};
	int value{};
	Node* next = nullptr;
	Node* prev = nullptr;
	Node() = default;
	Node(string, int, Node* = nullptr, Node* = nullptr);
	bool operator==(const Node&) const;
	friend ostream& operator<<(ostream& out, const Node& n) {
		out << n.key << ":" << n.value;
		return out;
	}
};

Node::Node(string key_, int value_, Node* prev_, Node* next_) {
	key = key_;
	value = value_;
	prev = prev_;
	next = next_;
}

bool Node::operator==(const Node& n) const {
	if ((key == n.key) && (value == n.value)) {
		return true;
	}
	else {
		return false;
	}
}

// DO NOT MODIFY
//
// DLL
//
class DLL {
private:
	Node* head_ = nullptr;
	Node* tail_ = nullptr;
	size_t sz_ = 0;

public:
	DLL() = default;
	DLL(vector<Node>);
	DLL(const DLL&);
	DLL& operator=(DLL list);
	~DLL();
	Node* get_head();
	Node* get_tail();
	size_t get_size();
	Node add_back(string, int);
	Node add_front(string, int);
	Node add_after(string, string, int);
	Node search(string);
	Node remove_front();
	Node remove_back();
	Node remove_key(string);
	int update_value(string, int);
	friend ostream& operator<<(ostream& out, const DLL& list) {
		auto itr = list.head_;
		ostringstream oss;
		for (; itr != nullptr; itr = itr->next) {
			oss << itr->key << ":" << itr->value << ", ";
		}
		string ans = oss.str();
		ans = ans.substr(0, ans.size() - 2);
		out << ans;
		return out;
	}
};

Node* DLL::get_head() { return head_; }
Node* DLL::get_tail() { return tail_; }
size_t DLL::get_size() { return sz_; }

DLL::~DLL() {
	while (head_)
		delete std::exchange(head_, head_->next);
}

DLL& DLL::operator =(DLL list) {
	using std::swap;
	swap(head_, list.head_);
	swap(tail_, list.tail_);
	swap(sz_, list.sz_);
	return *this;
}

DLL::DLL(const DLL& list) {
	for (Node* p = list.head_; p; p = p->next)
		add_back(p->key, p->value);
}

DLL::DLL(vector<Node> vec) {
	for (const auto& node : vec)
		add_back(node.key, node.value);
}

/* BEGIN MODIFICATION BELOW */
Node DLL::add_back(string str, int num) {
	Node* new_node = new Node(str, num, tail_, nullptr);
	if (head_ == nullptr) {
		tail_ = head_ = new_node;
	}
	else {
		tail_->next = new_node;
		tail_ = new_node;
	}
	sz_++;
	return *tail_;
}

Node DLL::add_front(string str, int num) {
	Node* new_node = new Node(str, num, nullptr, head_);
	if (head_ == nullptr) {
		tail_ = head_ = new_node;
	}
	else {
		head_->prev = new_node;
		head_ = new_node;
	}
	sz_++;
	return *head_;
}

Node DLL::add_after(string add_key, string str, int num) {
	if (head_->next == nullptr) {
		return Node("", 0, nullptr, nullptr);
	}
	Node temp = search(add_key);
	Node* found = &temp;

	if (found->next == nullptr) {
		if (found->prev != nullptr) {
			Node out_node = add_back(str, num);
			return out_node;
		}
		else {
			return Node("", 0, nullptr, nullptr);
		}
	}
	else {
		for (Node* current = head_; current->next; current = current->next) {
			if (current->key == add_key) {
				Node* end_node = current->next;
				Node* new_node = new Node(str, num, current, end_node);
				end_node->prev = new_node;
				current->next = new_node;
				current = current->next;
				current->next = end_node;
				sz_++;
				return *new_node;
			}
		}
	}
}

Node DLL::remove_back() {
	Node empty_node = Node("", 0, nullptr, nullptr);
	if (head_ == nullptr) {
		return empty_node;
	}
	else {
		Node* back = tail_;
		tail_ = tail_->prev;
		sz_--;

		if (sz_ == 0) {
			head_ = tail_ = nullptr;
		}
		return *back;
	}
}

Node DLL::remove_front() {
	Node empty_node = Node("", 0, nullptr, nullptr);
	if (head_ == nullptr) {
		return empty_node;
	}
	else {
		Node* front = head_;
		head_ = head_->next;
		sz_--;

		if (sz_ == 0) {
			head_ = tail_ = nullptr;
		}
		return *front;
	}
}

Node DLL::remove_key(string to_remove) {
	Node temp = search(to_remove);
	Node* found = &temp;
	Node* empty = new Node("", 0, nullptr, nullptr);
	if (sz_ == 1 && found->key == to_remove) {
		head_ = tail_ = nullptr;
		return Node();
	}
	if (head_ == nullptr) { //if dll is empty or key not found
		return *empty;
	}
	if (temp.key == "") {
		return *empty;
	}
	if (sz_ == 2) {
		if (found->next == nullptr) {
			Node* out_node = found->prev;
			head_->next = nullptr;
			head_ = tail_ = out_node;
			sz_--;
			return *found;
		}
		if (found->prev == nullptr) {
			Node* out_node = found->next;
			head_->next = nullptr;
			head_ = tail_ = out_node;
			sz_--;
			return *found;

		}
	}

	if (found->prev == nullptr) {
		Node out_node = remove_front();
		return out_node;
	}
	if (found->next == nullptr) {
		Node* out_node = tail_;
		tail_ = found->prev;
		tail_->next = nullptr;
		sz_--;
		return *out_node;
	}
	for (Node* current = head_; current->next; current = current->next) {
		if (current->next == nullptr) {
			Node out_node = remove_back();
			return out_node;
		}
		else {
			if (current->key == to_remove) {
				Node* previous_node = current->prev;
				Node* next_node = current->next;
				current = current->prev;
				current->next = next_node;
				current = current->next;
				current->prev = previous_node;
				sz_--;
				return *found;
			}
		}
	}
	return *found;
}

Node DLL::search(string find_key) {
	Node* current_node = head_;
	Node* empty_node = new Node("", 0, nullptr, nullptr);
	if (current_node == nullptr) {
		return *empty_node;
	}
	while (current_node->key != find_key) {
		Node* next_node = current_node->next;
		if (next_node == nullptr) {
			return *empty_node;
		}
		else {
			current_node = next_node;
		}
	}
	return *current_node;
}

int DLL::update_value(string find_key, int new_value) {
	Node found = search(find_key);

	if (head_ == nullptr && tail_ == nullptr) {
		return -1;
	}                   // if list is empty or not found
	if (found.key == "") {
		return -1;
	}

	if (found.next == nullptr) {
		tail_->value = new_value;  // if found is the final element
		return new_value;
	}
	else if (found.prev == nullptr) {
		head_->value = new_value; // if found is the first element
		return new_value;
	}
	else {    //  if found is somewhere in the middle of the list
		found.prev->next->value = new_value;
		return new_value;
	}
}

