#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <iterator>
#include <map>
#include "TTrie.h"

template <typename T>
//default constructor
TTrie<T>::TTrie(): isEnd(false) {
}

template <typename T>
//copy constructor
TTrie<T>::TTrie(const TTrie& rhs) {
    *this = rhs; //note this is the overloaded assignment operator
}

template <typename T>
//destructor
TTrie<T>::~TTrie() {
    //iterate through child nodes and delete them recursively
    for (typename std::map<T, TTrie *>::iterator i = this->child.begin(); i != child.end(); i++) {
        delete i->second;
    }
    this->child.clear();
}

template <typename T>
//assignment operator
TTrie<T>& TTrie<T>::operator=(const TTrie<T> &rhs) {
    if(this != &rhs) {
        //use destructor to empty left-hand side
        this->~TTrie(); 
        //iterate through rhs' chidld nodes and copy
        for (typename std::map<T, TTrie *>::const_iterator i = rhs.child.begin(); i != rhs.child.end(); i++) {
            this->child[i->first] = new TTrie(*i->second);
        }
        //set endpoint
        this->isEnd = rhs.isEnd;
    }
    return *this;
}

template <typename T>
//addition/assignment operator (overload)
TTrie<T>& TTrie<T>::operator+=(const std::vector<T>& sequence) {
    //temp TTrie object
    TTrie<T>* temp = this;
    //iterate through child nodes, and if there is no child, create new TTrie
    for (typename std::vector<T>::const_iterator i = sequence.begin(); i != sequence.end(); i++) {
        if (!temp->hasChild(*i)) {
            temp->child.emplace(*i, new TTrie<T>);
        }
        temp = temp->child[*i];
    }
    //end the Trie
    temp->isEnd = true;
    return *this;
 }

template <typename T>
//carat operator (overload)
bool TTrie<T>::operator^(const std::vector<T> &sequence) const {
    //check if sequence is empty
    if (sequence.empty()) {
        return this->isEndpoint();
    }
    //iterate through child nodes and recursively call operator
    for (typename std::map<T, TTrie *>::const_iterator i = this->child.cbegin(); i != this->child.cend(); i++) {
        if (sequence[0] == i->first) {
            std::vector<T> temp = sequence;
            temp.erase(temp.begin());
            return (*(i->second)^temp); //recursive call
        }
    }
    return false;
}

template <typename T>
//equality operator (overload)
bool TTrie<T>::operator==(const TTrie<T>& rhs) const {
    //check for numChildren and endpoint
    if (this->numChildren() != rhs.numChildren() || this->isEndpoint() != rhs.isEndpoint()) return false;
    
    //iterate through child nodes (this)
    for (typename std::map<T, TTrie *>::const_iterator i = this->child.begin(); i != this->child.end(); i++) {
        //iterator for child nodes (rhs)
        typename std::map<T, TTrie *>::const_iterator j;
        j = rhs.child.find(i->first);
        //check for j having a child with character
        if (j == rhs.child.end()) return false;    
        //recursive step
        if (!(*i->second == *j->second)) return false;
    }
    //else (no false conditions are reached)
    return true;
}

template <typename T>
//output stream operator (overload)
//helper function to take in stringstream as a parameter
void TTrie<T>::output_trie(std::ostream &os, std::stringstream &ss) const {
    //iterate through child nodes
    for (typename std::map<T, TTrie *>::const_iterator i = this->child.begin(); i != this->child.end(); i++) {
        //temp string
        std::string temp = ss.str();
        //put key of temp string into ss
        ss << i->first;
        //add line to output stream
        if (i->second->isEndpoint()) os << ss.str() << std::endl;
        ss << "-"; //separate lines
        //recursive step
        i->second->output_trie(os, ss);
        ss.str(std::string());
        ss << temp;
    }
}

template <typename U>
std::ostream& operator<<(std::ostream& os, const TTrie<U>& tt) {
    //simple call, from hint
    std::stringstream ss;
    tt.output_trie(os, ss);
    return os;
}

//return the number of children
template <typename T>
unsigned TTrie<T>::numChildren() const {
    return this->child.size();
}

//return true if there are any children, false otherwise
template <typename T>
bool TTrie<T>::hasChild() const {
    if (this->child.size() != 0) return true;
    //else
    return false;
}

//check whether a child linked by specific character exists
template <typename T>
bool TTrie<T>::hasChild(const T &value) const {
    typename std::map<T, TTrie *>:: const_iterator i = this->child.find(value);
    if (i != child.end()) return true;
    //else
    return false;
}

//get pointer to child node reachable via link labeled with the specified character
template <typename T>
const TTrie<T>* TTrie<T>::getChild(const T &value) const {
    typename std::map<T,TTrie *>::const_iterator i = this->child.find(value);
    if (i != child.end()) return i->second;
    //else
    return nullptr;
}

//return true if this node is an endpoint, false otherwise
template <typename T>
bool TTrie<T>::isEndpoint() const {
    return this->isEnd;
}
