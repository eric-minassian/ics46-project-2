#ifndef ___SKIP_LIST_HPP
#define ___SKIP_LIST_HPP

#include "runtimeexcept.hpp"
#include <cmath> // for log2
#include <cstddef>
#include <iostream>
#include <string>
#include <vector>

/**
 * flipCoin -- NOTE: Only read if you are interested in how the
 * coin flipping works.
 *
 * @brief Uses the bitwise representation of the key to simulate
 * the flipping of a deterministic coin.
 *
 * This function looks at the bitwise representation to determine
 * how many layers it occupies in the skip list. It takes the bitwise
 * XOR of each byte in a 32-bit unsigned number and uses the index
 * `previousFlips` to determine the truth value. It's best to look at an example
 * and to start with values that fit into one byte.
 *
 * Let's start with the function call `flipCoin(0, 0)`. This call is
 * asking whether or not we should insert the key `0` into layer `1`
 * (because `previousFlips` represents the number of previous flips).
 *
 * This function will get the bitwise representation of 0:
 *
 *        00000000 00000000 00000000 00000000
 *
 * All bytes are XOR'd together:
 *
 *
 *  c = 0 ^ 0 ^ 0 ^ 0
 *
 * Then the index `previousFlips` is used to obtain the bit in the
 * `previousFlips % 8` position.
 *
 * So the return value is `0 & 1 = 0`, since the value in the zeroth position
 * (obtained by bitmasking the value (1 << 0) at the bottom of the function
 * is 0.
 * Thus, this value `0` should never get added beyond the bottom-most layer.
 *
 * Before:
 *
 * S_1: -inf ----> inf
 * S_0: -inf ----> inf
 *
 * And after 0 is inserted
 *
 * S_1: -inf --------> inf
 * S_0: -inf --> 0 --> inf
 *
 * Let's look at something more interesting, like the call `flipCoin(5, 0)`.
 * Remember the binary representation for 5 is 00000101.
 *
 * c = 0 ^ 0 ^ 0 ^ 00000101 = 00000101
 *
 * Now we get the bit at 0th position (from `previousFlips`).
 *
 * 00000101 & 1 = 1. Thus, `5` DOES get inserted into the next layer,
 * layer 1.
 *
 * So the skip list before this insertion might look like:
 *
 * S_1: -inf ----> inf
 * S_0: -inf ----> inf
 *
 * And after it would look like:
 *
 * S_2: -inf --------> inf
 * S_1: -inf --> 5 --> inf
 * S_0: -inf --> 5 --> inf
 *
 * Observe that a new layer had to get created, because there should always
 * be an empty layer at the top.
 *
 * Since 5 got inserted into the next layer, we need to flip again to see if
 * it should get propagated once more.
 *
 * `flipCoin(5, 1)` this will produce a result of `0`, since 00000101 & (1 << 1)
 * = 0.
 *
 * Thus, `5` will not be pushed up to the next layer.
 *
 * @param key The integer key which will be added to the skip list
 * @param previousFlips The number of previous flips for this key
 * @return true simulates a "heads" from a coin flip
 * @return false simulates a "tails" from a coin flip
 */
inline bool flipCoin(unsigned key, unsigned previousFlips) {
  char c;
  unsigned first8Bits = (key & 0xFF000000) / 0x01000000;
  unsigned next8Bits = (key & 0x00FF0000) / 0x00010000;
  unsigned andThen = (key & 0x0000FF00) / 0x00000100;
  unsigned lastBits = (key & 0x000000FF);
  c = first8Bits ^ next8Bits ^ andThen ^ lastBits;
  previousFlips = previousFlips % 8;
  return (c & (1 << previousFlips)) != 0;
}

/**
 * @brief Works the same as the integer version above, except
 * it XORs chars in a string instead of the first four
 * bytes of an integer.
 *
 * @param key key that will be inserted into the skip list
 * @param previousFlips number of previous flips for this key
 * @return true simulates a "heads" from a coin flip
 * @return false simulates a "tails" from a coin flip
 */
inline bool flipCoin(std::string key, unsigned previousFlips) {
  char c = key[0];
  for (unsigned j = 1; j < key.length(); j++) {
    c = c ^ key[j];
  }
  previousFlips = previousFlips % 8;
  return (c & (1 << previousFlips)) != 0;
}

template <typename Key, typename Value> struct SkipNode {
  Key key;
  Value value;
  bool p_inf = false;
  bool n_inf = false;
  SkipNode<Key, Value> *next = nullptr;
  SkipNode<Key, Value> *previous = nullptr;
  SkipNode<Key, Value> *top = nullptr;
  SkipNode<Key, Value> *bottom = nullptr;
  SkipNode<Key, Value>(Key k, Value v) : key(k), value(v) {}
  SkipNode<Key, Value>() : key(Key()), value(Value()) {}
};

template <typename Key, typename Value> class SkipList {

private:
  // private variables go here.
  unsigned num_layers;
  std::size_t num_keys;
  SkipNode<Key, Value> *head;
  SkipNode<Key, Value> *tail;

public:
  SkipList();

  // You DO NOT need to implement a copy constructor or an assignment
  // operator.

  ~SkipList();

  // How many distinct keys are in the skip list?
  size_t size() const noexcept;

  // Does the Skip List contain zero keys?
  bool isEmpty() const noexcept;

  // How many layers are in the skip list?
  // Note that an empty Skip List has two layers by default,
  // the "base" layer S_0 and the top layer S_1.
  //
  // [S_1: Top layer]    -inf ------> inf
  // [S_0: Bottom layer] -inf ------> inf
  //
  // This "empty" Skip List has two layers and a height of one.
  unsigned numLayers() const noexcept;

  // What is the height of this key, assuming the "base" layer S_0
  // contains keys with a height of 1?
  // For example, "0" has a height of 1 in the following skip list.
  //
  // [S_1]  -inf --------> inf
  // [S_0]  -inf --> 0 --> inf
  //
  // Throw an exception if this key is not in the Skip List.
  unsigned height(const Key &k) const;

  // If this key is in the SkipList and there is a next largest key
  // return the next largest key.
  // This function should throw a RuntimeException if either the key doesn't
  // exist or there is no subsequent key. A consequence of this is that this
  // function will throw a RuntimeException if *k* is the *largest* key in the
  // Skip List.
  Key nextKey(const Key &k) const;

  // If this key is in the SkipList and a next smallest key exists,
  // return the next smallest key.
  // This function should throw a RuntimeException if either the key doesn't
  // exist or there is no previous key. A consequence of this is that this
  // function will throw a RuntimeException if *k* is the *smallest* key in
  // the Skip List.
  Key previousKey(const Key &k) const;

  // These return the value associated with the given key.
  // Throw a RuntimeException if the key does not exist.
  Value &find(const Key &k);
  const Value &find(Key k) const;

  // Return true if this key/value pair is successfully inserted, false
  // otherwise. See the project write-up for conditions under which the key
  // should be "bubbled up" to the next layer. If the key already exists, do
  // not insert one -- return false.
  bool insert(const Key &k, const Value &v);

  // Return a vector containing all inserted keys in increasing order.
  std::vector<Key> allKeysInOrder() const;

  // Is this the smallest key in the SkipList? Throw a RuntimeException
  // if the key *k* does not exist in the Skip List.
  bool isSmallestKey(const Key &k) const;

  // Is this the largest key in the SkipList? Throw a RuntimeException
  // if the key *k* does not exist in the Skip List.
  bool isLargestKey(const Key &k) const;

  // I am not requiring you to implement remove.
  void print() const;
};

template <typename Key, typename Value>
void SkipList<Key, Value>::print() const {
  SkipNode<Key, Value> *temp = head;
  SkipNode<Key, Value> *temp_head = head;
  while (temp_head != nullptr) {
    temp = temp_head;
    while (temp != nullptr) {
      std::cout << temp->key << " ";
      temp = temp->next;
    }
    std::cout << std::endl;
    temp_head = temp_head->bottom;
  }
}

template <typename Key, typename Value> SkipList<Key, Value>::SkipList() {
  head = new SkipNode<Key, Value>();
  tail = new SkipNode<Key, Value>();
  SkipNode<Key, Value> *temp_head = new SkipNode<Key, Value>();
  SkipNode<Key, Value> *temp_tail = new SkipNode<Key, Value>();

  head->n_inf = true;
  tail->p_inf = true;
  temp_head->n_inf = true;
  temp_tail->p_inf = true;

  head->next = tail;
  tail->previous = head;
  temp_head->next = temp_tail;
  temp_tail->previous = temp_head;
  head->bottom = temp_head;
  tail->bottom = temp_tail;
  temp_head->top = head;
  temp_tail->top = tail;
  num_layers = 2;
  num_keys = 0;
}

template <typename Key, typename Value> SkipList<Key, Value>::~SkipList() {
  SkipNode<Key, Value> *temp;

  while (head) {
    temp = head;
    head = head->bottom;
    while (temp) {
      SkipNode<Key, Value> *temp2 = temp;
      temp = temp->next;
      delete temp2;
    }
  }
}

template <typename Key, typename Value>
size_t SkipList<Key, Value>::size() const noexcept {
  return num_keys;
}

template <typename Key, typename Value>
bool SkipList<Key, Value>::isEmpty() const noexcept {
  return num_keys == 0;
}

template <typename Key, typename Value>
unsigned SkipList<Key, Value>::numLayers() const noexcept {
  return num_layers;
}

template <typename Key, typename Value>
unsigned SkipList<Key, Value>::height(const Key &k) const {
  SkipNode<Key, Value> *temp = head;
  while (temp->bottom) {
    temp = temp->bottom;
    while (!temp->next->p_inf && k >= temp->next->key) {
      temp = temp->next;
    }
  }

  if (temp->key != k || temp->n_inf) {
    throw RuntimeException("Key not found");
  }

  unsigned height = 1;
  while (temp->top) {
    temp = temp->top;
    height++;
  }

  return height;
}

template <typename Key, typename Value>
Key SkipList<Key, Value>::nextKey(const Key &k) const {
  SkipNode<Key, Value> *temp = head;
  while (temp->bottom) {
    temp = temp->bottom;
    while (!temp->next->p_inf && k >= temp->next->key) {
      temp = temp->next;
    }
  }

  if (temp->key != k || temp->n_inf || temp->next->p_inf) {
    throw RuntimeException("Key not found");
  }

  return temp->next->key;
}

template <typename Key, typename Value>
Key SkipList<Key, Value>::previousKey(const Key &k) const {
  SkipNode<Key, Value> *temp = head;
  while (temp->bottom) {
    temp = temp->bottom;
    while (!temp->next->p_inf && k >= temp->next->key) {
      temp = temp->next;
    }
  }

  if (temp->key != k || temp->n_inf || temp->previous->n_inf) {
    throw RuntimeException("Key not found");
  }

  return temp->previous->key;
}

template <typename Key, typename Value>
const Value &SkipList<Key, Value>::find(Key k) const {
  SkipNode<Key, Value> *temp = head;
  while (temp->bottom) {
    temp = temp->bottom;
    while (!temp->next->p_inf && k >= temp->next->key) {
      temp = temp->next;
    }
  }

  if (temp->key == k && !temp->n_inf) {
    return temp->value;
  } else {
    throw RuntimeException("Key not found");
  }
}

template <typename Key, typename Value>
Value &SkipList<Key, Value>::find(const Key &k) {
  SkipNode<Key, Value> *temp = head;
  while (temp->bottom) {
    temp = temp->bottom;
    while (!temp->next->p_inf && k >= temp->next->key) {
      temp = temp->next;
    }
  }

  if (temp->key == k && !temp->n_inf) {
    return temp->value;
  } else {
    throw RuntimeException("Key not found");
  }
}

template <typename Key, typename Value>
bool SkipList<Key, Value>::insert(const Key &k, const Value &v) {

  SkipNode<Key, Value> *temp = head;
  while (temp->bottom != nullptr) {
    temp = temp->bottom;
    while (!temp->next->p_inf && k >= temp->next->key) {
      temp = temp->next;
    }
  }

  if (!temp->n_inf && temp->key == k) {
    return false;
  }

  SkipNode<Key, Value> *new_node = new SkipNode<Key, Value>(k, v);

  new_node->next = temp->next;
  new_node->previous = temp;
  temp->next->previous = new_node;
  temp->next = new_node;
  num_keys++;

  unsigned max_flips = 0;
  if (num_keys <= 16) {
    max_flips = 12;
  } else {
    max_flips = 3 * ceil(log2(num_keys));
  }

  unsigned height = 0;
  while (flipCoin(k, height) && height + 1 < max_flips) {
    height++;
    if (height + 1 >= num_layers) {
      num_layers++;

      SkipNode<Key, Value> *temp_head = new SkipNode<Key, Value>();
      SkipNode<Key, Value> *temp_tail = new SkipNode<Key, Value>();
      temp_head->n_inf = true;
      temp_tail->p_inf = true;

      temp_head->next = temp_tail;
      temp_tail->previous = temp_head;
      temp_head->bottom = head;
      temp_tail->bottom = tail;
      head->top = temp_head;
      tail->top = temp_tail;
      head = temp_head;
      tail = temp_tail;
    }
    while (temp->previous && temp->top == nullptr) {
      temp = temp->previous;
    }
    temp = temp->top;

    SkipNode<Key, Value> *new_node_top = new SkipNode<Key, Value>(k, v);

    new_node_top->next = temp->next;
    new_node_top->previous = temp;
    temp->next->previous = new_node_top;
    temp->next = new_node_top;
    new_node_top->bottom = new_node;
    new_node->top = new_node_top;
    new_node = new_node_top;
  }

  return true;
}

template <typename Key, typename Value>
std::vector<Key> SkipList<Key, Value>::allKeysInOrder() const {
  std::vector<Key> keys;
  SkipNode<Key, Value> *temp = head;
  while (temp->bottom) {
    temp = temp->bottom;
  }
  while (temp->next->p_inf == false) {
    keys.push_back(temp->next->key);
    temp = temp->next;
  }
  return keys;
}

template <typename Key, typename Value>
bool SkipList<Key, Value>::isSmallestKey(const Key &k) const {
  SkipNode<Key, Value> *temp = head;
  while (temp->bottom) {
    temp = temp->bottom;
  }
  if (temp->next->key == k) {
    return true;
  }
  return false;
}

template <typename Key, typename Value>
bool SkipList<Key, Value>::isLargestKey(const Key &k) const {
  SkipNode<Key, Value> *temp = head;
  while (temp->bottom) {
    temp = temp->bottom;
  }
  while (temp->next->p_inf == false) {
    temp = temp->next;
  }
  if (temp->key == k) {
    return true;
  }
  return false;
}

#endif
