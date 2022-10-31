# Red Black Tree in C++ From the Ground Up
## Motivation
In my algorithms class at Rice University (COMP 382) we learned about a lot of trees. Red Black trees seemed like black magic and are also very cool so I wanted to get a better understanding for how they work.
As for using C++, I have used C a lot and have a lot of exposure to C++, but I have never really done a proper project. I ended up really liking the language, it gives a lot of OOP niceness like in C# and Java while also giving you the raw power (like pointers) you get from being so close to the machine like in C.

-Rant warning- <br>
I almost feel like writing in different languages is like driving different types of cars. C is like an F1 car: pure raw power and speed but easy to mess up. C++ is more like a sports car: you still feel close to the road but have a few more features. I would equate C# to a Volkwagen Golf GTI, still decently fast and connected but a little higer level features. Python is more like a self driving car, you don't really have to do much. In assembly you have to build the car yourself. I seem to like the connection to the computer, though I do like some of the higher level features here and there, especially for prototyping.

## Cool features
All in all this is mostly a standard bottom up red black tree implementation.

Type parameters and parameters for values of nodes are optional so the tree can be used like a set or a map.

Overloads of the subscripting operator (operator\[\]) allow one to index the map a lot like in python.
- tree[key]: returns the value if it exists, undefined otherwise
- tree[keyInMap] = value: sets the value in the map
- tree[keyNotInMap] = value: inserts new key value pair into map
the overload returns a proxy object which handles the get or set depending on what the user attempts do do with the value.     
     
<img src="https://user-images.githubusercontent.com/52176894/199084028-8442a6a9-e66f-44bd-af6d-30f56ec9a4b9.png" width=50% height=50%>
<img src="https://user-images.githubusercontent.com/52176894/199084913-837565c8-d01c-43ab-ad9c-922b4ac8ad03.png" width=50% height=50%>     

## API

```cpp
/**
* @brief pretty print a representation of the rbtree
*/
void printTree()
```
```cpp
/**
* @brief recursively deletes every node to clear the tree, this tree object still lives
*/
void clear()
```
```cpp
/**
* @brief makes a new tree with the same contents as this one
*
* @return rbtree<K, V>*, a pointer to a new tree with the same contents
*/
rbtree<K, V> *copy()
```
```cpp
/**
* @brief checks if the key is contained within the set/map
*
* @param[in] key the key to look for
* @return true if the key is in the set/map
* @return false otherwise
*/
bool contains(K key)
```
```cpp
/**
* @brief get the value of a key in the map.*
* NOTE: this method has undefined behavior if the key is not in the map
*
* @param[in] key the key to look for
* @return V the value of the key if it exists in the map, undefined value otherwise
*/
V getValue(K key)
```
```cpp
/**
* @brief gets the key's value if it is in the map
*
* @param[in] key the key to look for
* @param[out] out the location to put the value if found
* @return true if the key is in the map
* @return false otherwise
*/
bool tryGetValue(K key, V *out)
```
```cpp
/**
* @brief Set the value of the key if it is in the map
*
* @param[in] key the key to find
* @param[in] value the value to set
* @return true if the key is in the map
* @return false otherwise
*/
bool setValue(K key, V value)
```
```cpp
/**
* @brief insert a key/key-value pair in the map if it does not exist in the map
*
* @param[in] key the key to insert
* @param[in] value [OPTIONAL] the value to insert (optional if only representing a set)
* @return true if the key is not in the map/set and the value was inserted
* @return false otherwise
*/
bool insert(K key, V value = (V)NULL)
```
```cpp
/**
* @brief get/set/insert operator with [], works like in python
*
* | tree[key]: returns the value if it exists, undefined otherwise
* | tree[keyInMap] = value: sets the value in the map
* | tree[keyNotInMap] = value: inserts new key value pair into map
*
* @param[in] key the key to get/set/insert
* @return rbtree<K, V>::getSetProxy a proxy object that overloads
* the right functions to make get/set/insert work smoothly
*/
rbtree<K, V>::getSetProxy operator[](K key)
```
```cpp
/**
* @brief removes the key from the set/map if it exists
*
* @param[in] key the key to remove
* @return true if the key was in the map and removed
* @return false otherwise
*/
bool remove(K key)
```
```cpp
/**
* @brief checks if the tree is valid in terms of black height property and red red property
*
* @return true of the tree is valid
* @return false otherwise
*/
bool checkValid()
```
```cpp
/**
* @brief clear all nodes and destroy the rbtree object
*/
~rbtree()
```
