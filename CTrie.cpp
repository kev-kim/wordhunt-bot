#include <cassert>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <iterator>

#include "CTrie.h"

//default constructor
CTrie::CTrie(): isEnd(false) {
}

//copy constructor
CTrie::CTrie(const CTrie& rhs) {
    *this = rhs; //note this is the overloaded assignment operator
}

//destructor
CTrie::~CTrie() {
    //Iterate through all children and delete them recursively
    for (std::map<char, CTrie *>::iterator i = this->child.begin(); i != child.end(); i++) {
        delete i->second;
    }
    this->child.clear();
}

//assignment operator
CTrie& CTrie::operator=(const CTrie &rhs) {
    if (this != &rhs) {
        //use destructor to empty left-hand side
        this->~CTrie();
        //iterate through rhs' child nodes and copy
        for (std::map<char, CTrie *>::const_iterator i = rhs.child.begin(); i != rhs.child.end(); i++) {
            this->child[i->first] = new CTrie(*i->second);
        }
        //set endpoint
        this->isEnd = rhs.isEnd;
    }
    return *this;
}

//addition/assignment operator (overload)
CTrie& CTrie::operator+=(const std::string& word) {
    //CTrie object to store data
    CTrie *temp = this;
    //iterate through word
    for (unsigned i = 0; i < word.length(); i++) {
        //add element to map if it doesn't exist
        if (!temp->hasChild(word[i])) {
            temp->child.emplace(word[i], new CTrie);
        }
        temp = temp->child[word[i]];
    }
    //set endpoint
    temp->isEnd = true;
    return *this;
}

//carat operator (overload)
bool CTrie::operator^(const std::string& word) const {
    //base case: check if word is 0
    if (word.length() == 0) {
        return this->isEndpoint();
    }
    //iterate through child nodes and recursively call operator
    for (std::map<char, CTrie *>::const_iterator i = this->child.begin(); i != this->child.end(); i++) {
        if (word[0] == i->first) {
            return *(i->second)^word.substr(1); //recursive call
        }
    }
    return false;
}

//equality operator (overload)
bool CTrie::operator==(const CTrie& rhs) const {
    //check for numChildren and endpoint
    if (this->numChildren() != rhs.numChildren() || this->isEndpoint() != rhs.isEndpoint()) return false;
    //Iterate through child nodes (this)
    for (std::map<char, CTrie *>::const_iterator i = this->child.begin(); i != this->child.end(); i++) {
        //iterator for child nodes (rhs)
        std::map<char, CTrie *>::const_iterator j;
        j = rhs.child.find(i->first);
        //check for j having a child with character
        if (j == rhs.child.end()) return false;
        //recursive step for each node
        if (!(*i->second == *j->second)) return false;
    }
    //else (no false conditions are reached)
    return true;
}

//output stream operator (overload)
//helper function to take string in as a parameter
void CTrie::output_trie(std::ostream &os, std::string updated_str) const {
    //iterate through child nodes
    std::string fullstr = updated_str;
    for (std::map<char, CTrie *>:: const_iterator i = this->child.begin(); i != this->child.end(); i++) {
        //add characters into string until endpoint
        updated_str = fullstr;
        updated_str.push_back(i->first);
        //add line to output stream
        if (i->second->isEndpoint()) os << updated_str << std::endl;
        //recursive step
        i->second->output_trie(os, updated_str);
    }
}

std::ostream& operator<<(std::ostream& os, const CTrie& ct) {
    //simple call, from hint in google docs
    ct.output_trie(os, "");
    return os;
}

//return the number of children
unsigned CTrie::numChildren() const {
    return this->child.size();
}

//return true if there are any children, false otherwise
bool CTrie::hasChild() const {
    if (this->child.size() != 0) return true;
    //else
    return false;
}

//check whether a child linked by specific character exists
bool CTrie::hasChild(char character) const {
    //iterator to point to character
    std::map<char, CTrie *>::const_iterator i = this->child.find(character);
    if (i != child.end()) return true;
    //else
    return false;
}

//get pointer to child node reachable via link labeled with the specified character
const CTrie* CTrie::getChild(char character) const {
    //same logic as hasChild, except different return
    std::map<char, CTrie *>::const_iterator i = this->child.find(character);
    if (i != child.end()) return i->second;
    //else
    return nullptr;
}

//return true if this node is an endpoint, false otherwise
bool CTrie::isEndpoint() const {
    return this->isEnd;
}
