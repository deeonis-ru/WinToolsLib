// Copyright 2013 Daniel Parker
// Distributed under the Boost license, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// See https://sourceforge.net/projects/jsoncons/files/ for latest version
// See https://sourceforge.net/p/jsoncons/wiki/Home/ for documentation.

#ifndef JSONCONS_JSON_STRUCTURES_HPP
#define JSONCONS_JSON_STRUCTURES_HPP

#include <string>
#include <vector>
#include <exception>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <utility>
#include <new>
#include "json1.hpp"

namespace jsoncons {

template <class Char>
using json_member = std::tuple<std::basic_string<Char>, basic_json<Char>, size_t>;

template <class Char>
class key_compare
{
public:
    bool operator()(const json_member<Char>& a,
                    const std::basic_string<Char>& b) const
    {
        return std::get<0>(a) < b;
    }
};

template <class Char>
class member_compare
{
public:
    bool operator()(const json_member<Char>& a,
                    const json_member<Char>& b) const
    {
        return std::get<0>(a) < std::get<0>(b);
    }
};

template <class Char>
class json_array 
{
public:
    typedef typename std::vector<basic_json<Char>>::iterator iterator;
    typedef typename std::vector<basic_json<Char>>::const_iterator const_iterator;

    json_array()
    {
    }

    json_array(size_t n)
        : elements_(n)
    {
    }

    json_array(size_t n, const basic_json<Char>& val)
        : elements_(n,val)
    {
    }

    json_array(std::vector<basic_json<Char>> elements)
        : elements_(elements)
    {
    }

    template <class InputIterator>
    json_array(InputIterator begin, InputIterator end)
        : elements_(begin,end)
    {
    }

    json_array<Char>* clone() 
    {
        return new json_array(elements_);
    }

    size_t size() const {return elements_.size();}

    size_t capacity() const {return elements_.capacity();}

    void clear() {elements_.clear();}

    void reserve(size_t n) {elements_.reserve(n);}

    void remove_range(size_t from_index, size_t to_index) 
    {
        JSONCONS_ASSERT(from <= to);
        JSONCONS_ASSERT(to <= elements_.size());
        elements_.erase(elements_.begin()+from,elements_.begin()+to);
    }

    basic_json<Char>& at(size_t i) {return elements_[i];}

    const basic_json<Char>& at(size_t i) const {return elements_[i];}

    void push_back(const basic_json<Char>& value);

    void add(size_t index, const basic_json<Char>& value);

#ifndef JSONCONS_NO_CXX11_RVALUE_REFERENCES

    void push_back(basic_json<Char>&& value)
    {
        elements_.push_back(value);
    }

    void add(size_t index, basic_json<Char>&& value)
    {
        json_array<Char>::iterator position = index < elements_.size() ? elements_.begin() + index : elements_.end();
        elements_.insert(position, value);
    }

#endif

    iterator begin() {return elements_.begin();}

    iterator end() {return elements_.end();}

    const_iterator begin() const {return elements_.begin();}

    const_iterator end() const {return elements_.end();}

    std::vector<basic_json<Char>> elements_;

    bool operator==(const json_array<Char>& rhs) const
    {
        if (size() != rhs.size())
        {
            return false;
        }
        for (size_t i = 0; i < size(); ++i)
        {
            if (elements_[i] != rhs.elements_[i])
            {
                return false;
            }
        }
        return true;
    }
};

template <class Char>
class json_object
{
public:
    typedef json_member<Char> json_member;
    typedef std::vector<json_member> json_members;
    typedef typename json_members::iterator iterator;
    typedef typename json_members::const_iterator const_iterator;

    class const_order_iterator : public std::iterator<std::forward_iterator_tag, json_member, size_t>
    {
        size_t index_;
        json_object<Char>* const container_;
    public:
        explicit const_order_iterator(json_object<Char>* const container, size_t index = 0) :
            container_(container),
            index_(index)
        {
        }

        const_order_iterator operator++()
        {
            index_++;
            return const_order_iterator(*this);
        }

        bool operator==(const const_order_iterator& other) const
        {
            return index_ == other.index_;
        }

        bool operator!=(const const_order_iterator& other) const
        {
            return index_ != other.index_;
        }

        const reference operator*() const
        {
            const auto it = std::find_if(std::begin(container_->members_), std::end(container_->members_), [this](const reference value)
            {
                return std::get<2>(value) == this->index_;
            });
            if (std::end(container_->members_) != it)
                return *it;
            else
                throw std::out_of_range("Out of range");
        }
    };
    friend class const_order_iterator;

    json_object()
    {
    }

    json_object(size_t n)
        : members_(n)
    {
    }

    json_object(json_members members)
        : members_(members)
    {
    }

    json_object<Char>* clone() 
    {
        return new json_object(members_);
    }

    size_t size() const {return members_.size();}

    size_t capacity() const {return members_.capacity();}

    void clear() {members_.clear();}

    void reserve(size_t n) {members_.reserve(n);}

    void remove_range(size_t from_index, size_t to_index) 
    {
        JSONCONS_ASSERT(from <= to);
        JSONCONS_ASSERT(to <= members_.size());
        members_.erase(members_.begin()+from,members_.begin()+to);
    }

    const json_member& get(size_t i) const 
    {
        return members_[i];
    }

    basic_json<Char>& at(size_t i) {return members_[i].value_;}

    const basic_json<Char>& at(size_t i) const {return members_[i].value_;}

    void set(const std::basic_string<Char>& name, const basic_json<Char>& value);

#ifndef JSONCONS_NO_CXX11_RVALUE_REFERENCES

    void set(std::basic_string<Char>&& name, basic_json<Char>&& value)
    {
        iterator it = std::lower_bound(begin(),end(),name ,key_compare<Char>());
        if (it != end() && (*it).first == name)
        {
            remove(it);
        }
        //auto pos = members_.size();
        insert(it,std::make_tuple(name,value,0));
    }
    
    void push_back(std::pair<std::basic_string<Char>, basic_json<Char>>&& pair)
    {
        push_back(std::make_tuple(pair.first, pair.second, 0));
    }

    void push_back(json_member&& member)
    {
        std::get<2>(member) = size();
        members_.push_back(member);
    }
#endif
    void remove(iterator at); 

    template <class Iter>
    static const std::basic_string<Char>& get_name(Iter& it)
    {
        return std::get<0>(*it);
    }

    template <class Iter>
    static const basic_json<Char>& get_basic_json(Iter& it)
    {
        return std::get<1>(*it);
    }

    static const std::basic_string<Char>& get_name(const_order_iterator& it)
    {
        return std::get<0>(*it);
    }

    static const basic_json<Char>& get_basic_json(const_order_iterator& it)
    {
        return std::get<1>(*it);
    }

    basic_json<Char>& get(const std::basic_string<Char>& name);

    const basic_json<Char>& get(const std::basic_string<Char>& name) const;

    iterator find(const std::basic_string<Char>& name);

    const_iterator find(const std::basic_string<Char>& name) const;

    void insert(const_iterator it, json_member member);

    void push_back(const json_member& member)
    {
        std::get<2>(member) = size();
        members_.push_back(member);
    }

    void sort_members();

    const_order_iterator begin_order() { return const_order_iterator(this); }

    const_order_iterator end_order() { return const_order_iterator(this, members_.size()); }

    iterator begin() {return members_.begin();}

    iterator end() {return members_.end();}

    const_iterator begin() const {return members_.begin();}

    const_iterator end() const {return members_.end();}

    bool operator==(const json_object<Char>& rhs) const
    {
        if (size() != rhs.size())
        {
            return false;
        }
        for (const_iterator it = members_.begin(); it != members_.end(); ++it)
        {
            bool exists = std::binary_search(rhs.members_.begin(),rhs.members_.end(),*it,member_compare<Char>());
            if (!exists)
            {
                return false;
            }
        }
        return true;
    }

    json_members members_;
};

template <class Char>
void json_array<Char>::push_back(const basic_json<Char>& value)
{
    elements_.push_back(value);
}

template <class Char>
void json_array<Char>::add(size_t index, const basic_json<Char>& value)
{
    json_array<Char>::iterator position = index < elements_.size() ? elements_.begin() + index : elements_.end();
    elements_.insert(position, value);
}

template <class Char>
void json_object<Char>::sort_members()
{
    std::sort(members_.begin(),members_.end(),member_compare<Char>());
}

template <class Char>
void json_object<Char>::insert(const_iterator it, json_member member)
{
    std::get<2>(member) = size();
    members_.insert(it,member);
}

template <class Char>
void json_object<Char>::remove(iterator at)
{
    members_.erase(at);
}

template <class Char>
void json_object<Char>::set(const std::basic_string<Char>& name, const basic_json<Char>& value)
{
    iterator it = std::lower_bound(begin(),end(),name ,key_compare<Char>());
    if (it != end() && get_name(it) == name)
    {
        remove(it);
    }
    //auto pos = size();
    insert(it,std::make_tuple(name,value,0));
}

template <class Char>
basic_json<Char>& json_object<Char>::get(const std::basic_string<Char>& name) 
{
    iterator it = find(name);
    if (it == end())
    {
        JSONCONS_THROW_EXCEPTION_1("Member %s not found.",name);
    }
    return std::get<1>(*it);
}

template <class Char>
const basic_json<Char>& json_object<Char>::get(const std::basic_string<Char>& name) const
{
    const_iterator it = find(name);
    if (it == end())
    {
        JSONCONS_THROW_EXCEPTION_1("Member %s not found.",name);
    }
    //return (*it).value_;
    return std::get<1>(*it);
}

template <class Char>
typename json_object<Char>::iterator json_object<Char>::find(const std::basic_string<Char>& name)
{
    key_compare<Char> comp;
    iterator it = std::lower_bound(begin(),end(), name, comp);
    return (it != end() && std::get<0>(*it) == name) ? it : end();
}

template <class Char>
typename json_object<Char>::const_iterator json_object<Char>::find(const std::basic_string<Char>& name) const
{
    key_compare<Char> comp;
    const_iterator it = std::lower_bound(begin(),end(),name, comp);
    return (it != end() && std::get<0>(*it) == name) ? it : end();
}

}

#endif
