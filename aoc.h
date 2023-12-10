#ifndef _LEETCODE_H_
#define _LEETCODE_H_

#include <iostream>
#include <list>
#include <vector>
#include <queue>
#include <stack>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <limits>
#include <iterator>
#include <cctype>

#include <string>
#include <iomanip>
#include <iterator>
#include <stdexcept>
#include <fstream>

using namespace std;

// Definition for unidirected graph.
struct UndirectedGraphNode {
  int label;
  vector<UndirectedGraphNode *> neighbors;
  UndirectedGraphNode(int x) : label(x) {};
};


//Definition for a binary tree node.
struct TreeNode {
  int val;
  TreeNode *left;
  TreeNode *right;
  TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};

//Definition for singly-linked list.
struct ListNode {
  int val;
  ListNode *next;
  ListNode(int x) : val(x), next(NULL) {}
};

// make list from vector<int>
ListNode* ml(const std::vector<int>& v) {
  if (v.empty()) return nullptr;

  auto head = new ListNode(v[0]);
  auto p = head;
  for (auto i = 1; i < v.size(); ++i) {
    p->next = new ListNode(v[i]);
    p = p->next;
  }

  return head;
}

// make vector<int> from list
std::vector<int> mv(const ListNode* head) {
  std::vector<int> v;
  auto p = head;

  while (p) {
    v.push_back(p->val);
    p = p->next;
  }

  return v;
}

vector<string> split(const char* str, char c=' ') {
  vector<string> result;
  do {
    while (*str && *str == c) str++;
    const char* beg = str;
    while (*str && *str != c) str++;

    if (beg != str) {
      result.push_back(string(beg, str));
    }
  } while (0 != *str++);
  
  return result;
}

// make Tree
TreeNode* mt(string s) {
  auto v = split(s.c_str());
  if (v.empty()) return nullptr;

  queue<TreeNode*> q;
  TreeNode* root = new TreeNode(std::atoi(v[0].c_str()));
  q.push(root);

  for (int i = 1; i < v.size(); i += 2) {
    auto node = q.front();
    q.pop();
    if (v[i] != "null") {
      node->left = new TreeNode(std::atoi(v[i].c_str()));
      q.push(node->left);
    }

    if (i + 1 < v.size() && v[i + 1] != "null") {
      node->right = new TreeNode(std::atoi(v[i + 1].c_str()));
      q.push(node->right);
    }
  }

  return root;
}

string ts(TreeNode* root) {
  if (!root) return string();
  queue<TreeNode*> q;
  q.push(root);
  stringstream ss;
  int cnt = 1;
  bool start = true;

  while (!q.empty()) {
    auto node = q.front();
    q.pop();
    if (node) {
      cnt--;
      if (!start) ss << " ";
      else start = false;
      ss << node->val;
      if (node->left) cnt++;
      if (node->right) cnt++;
      q.push(node->left);
      q.push(node->right);
    } else {
      if (cnt) ss << " null";
    }
  }

  return ss.str();
}

// print list
void pl(const ListNode* head) {
  auto p = head;
  while (p) {
    std::cout << p->val;
    p = p->next;
    if (p) {
      std::cout << ", ";
    }
  }

  std::cout << std::endl;
}

template <class T1, class T2>
void phm(const unordered_map<T1, T2>& hm, string prefix="") {
  std::cout << (prefix.empty() ? "" : prefix + ":");
  for (const auto& item : hm) {
    std::cout << " {" << item.first << ": " << item.second << "}";
  }
  std::cout << std::endl;
}

// aliases
using vi = vector<int>;
using vs = vector<string>;

namespace test {

namespace impl {

static inline void _assert(const std::string& msg, bool val) {
  if (!val) {
    throw std::runtime_error(msg);
  }
}

} // namespace impl


class _assert_pred {
  public:
    enum predicate {
      eq,
      ne,
      lt,
      le,
      gt,
      ge
    };

    _assert_pred(predicate pred, const char* file, int line)
      : _pred(pred), _file(file), _line(line) {}
   
    template <class T1, class T2>
    void operator()(const T1& a, const T2& b) const {
      try {
        std::stringstream ss;
        ss <<  "expected: ";

        switch (_pred) {
          case eq:
            ss << a << " == " << b;
            impl::_assert(ss.str(), a == b);
            break;
          case ne:
            ss << a << " != " << b;
            impl::_assert(ss.str(), a != b);
            break;
          case lt:
            ss << a << " < " << b;
            impl::_assert(ss.str(), a < b);
            break;
          case le:
            ss << a << " <= " << b;
            impl::_assert(ss.str(), a <= b);
            break;
          case gt:
            ss << a << " > " << b;
            impl::_assert(ss.str(), a > b);
            break;
          case ge:
            ss << a << " >= " << b;
            impl::_assert(ss.str(), a >= b);
            break;
        }
      } catch (std::exception& e) {
        std::cout << _file << ":" << _line << " " << e.what() << std::endl;
      }
    }

  private:
    predicate _pred;
    const char* _file;
    const int _line;
};

class _assert_container_eq {
  public:
    _assert_container_eq(const char* file, int line)
      : _file(file), _line(line) {}
    
    template <class T>
    std::ostream& print(std::ostream& out, const T& c) const {
      out << '{' << c.size() << '}' << '['; 
      for (auto it = c.begin(); it != c.end(); ++it) {
        out << *it << ",";
      }
      
      if (!c.empty()) {
        out << "\b";
      }
      out << "]";
      
      return out;
    }

    template <class T1, class T2>
    void operator()(const T1& c1, const T2& c2) const {
      auto it1 = c1.begin();
      auto it2 = c2.begin();

      while (it1 != c1.end() && it2 != c2.end()) {
        if (*it1 != *it2) {
          break;
        }

        ++it1;
        ++it2;
      }

      if (it1 != c1.end() || it2 != c2.end()) {
        std::stringstream ss;
        ss << _file << ":" << _line << std::endl << std::setw(4);
        print(ss, c1);
        ss << std::endl << "!=" << std::endl << std::setw(4);
        print(ss, c2);
        std::cout << ss.str() << std::endl;
      }
    }

  private:
    const char* _file;
    const int _line;
};

class _assert_list_eq {
  public:
    _assert_list_eq(const char* file, int line)
      : _file(file), _line(line) {}
    
    std::ostream& print(std::ostream& out, const ListNode* l) const {
      auto c = mv(l);

      out << '{' << c.size() << '}' << '['; 
      for (auto it = c.begin(); it != c.end(); ++it) {
        out << *it << ",";
      }
      
      if (!c.empty()) {
        out << "\b";
      }
      out << "]";
      
      return out;
    }

    void operator()(const ListNode* c1, const ListNode* c2) const {
      auto it1 = c1;
      auto it2 = c2;

      while (it1 != nullptr && it2 != nullptr) {
        if (it1->val != it2->val) {
          break;
        }

        it1 = it1->next;
        it2 = it2->next;
      }

      if (it1 != nullptr || it2 != nullptr) {
        std::stringstream ss;
        ss << _file << ":" << _line << std::endl << std::setw(4);
        print(ss, c1);
        ss << std::endl << "!=" << std::endl << std::setw(4);
        print(ss, c2);
        std::cout << ss.str() << std::endl;
      }
    }

  private:
    const char* _file;
    const int _line;
};



} // namepsace test

#define assert_true(x) (::test::_assert_pred(test::_assert_pred::predicate::eq, __FILE__, __LINE__))((x), true)
#define assert_false(x) (::test::_assert_pred(test::_assert_pred::predicate::eq, __FILE__, __LINE__))((x), false)
#define assert_eq (::test::_assert_pred(test::_assert_pred::predicate::eq, __FILE__, __LINE__))
#define assert_ne (::test::_assert_pred(test::_assert_pred::predicate::ne, __FILE__, __LINE__))
#define assert_lt (::test::_assert_pred(test::_assert_pred::predicate::lt, __FILE__, __LINE__))
#define assert_le (::test::_assert_pred(test::_assert_pred::predicate::le, __FILE__, __LINE__))
#define assert_gt (::test::_assert_pred(test::_assert_pred::predicate::gt, __FILE__, __LINE__))
#define assert_ge (::test::_assert_pred(test::_assert_pred::predicate::ge, __FILE__, __LINE__))

#define assert_container_eq (::test::_assert_container_eq(__FILE__, __LINE__))
#define assert_list_eq (::test::_assert_list_eq(__FILE__, __LINE__))

#


#endif // _LEETCODE_H_
