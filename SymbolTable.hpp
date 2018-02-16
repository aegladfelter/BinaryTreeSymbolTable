#ifndef SYMBOL_TABLE_INTERFACE_H
#define SYMBOL_TABLE_INTERFACE_H

template <typename Key, typename Value>
class SymbolTable
{
  public:
    virtual ~SymbolTable() {}

    // Puts key-value pair into the table
    virtual void put(const Key& key, const Value& val = Value{}) = 0;

    // acquire the value paired with key
    virtual bool get(const Key& key, Value& val = Value{}) const = 0;

    // remove key (and its value) from table
    virtual void remove(const Key& key) = 0;

    // Is there a value paired with key?
    virtual bool contains(const Key& key) const = 0;

    // Is the table empty?
    virtual bool empty() const = 0;

    // Number of key-value pairs.
    virtual int size() const = 0;

    // Smallest key
    virtual bool min(Key& key = Key{}) const = 0;

    // Largest key
    virtual bool max(Key& key = Key{}) const = 0;

    // Largest key less than or equal to key
    virtual bool floor(const Key& key, Key& floorKey) const = 0;

    // Smallest key greater than or equal to key
    virtual bool ceiling(const Key& key, Key& ceilingKey) const = 0;

    // Number of keys less than key
    virtual int rank(const Key& key) const = 0;

    // key of rank k
    virtual bool select(int k = 0, Key& key = Key{}) const = 0;

    // Delete the smallest key
    virtual bool deleteMin() = 0;

    // Delete the largest key
    virtual bool deleteMax() = 0;

    // number of keys in [low, high] (including low, high)
    virtual int size(const Key& low, const Key& high) const = 0;

    // keys in [low, high] (including low, high), in sorted order
    virtual std::vector<Key> keys(const Key& low, const Key& high) const = 0;

    // all keys in the table, in sorted order
    virtual std::vector<Key> keys() const = 0;

    // Removes all elements from the table
    virtual void clear() = 0;

    // Internal integrity check
    virtual bool check() const = 0;
};

#endif