#pragma once
#include <vector>
#include <iostream>
#include "Vec3.h"

using std::vector;

template<typename Data>
class Node {
    public:
        Node(vec3 bound_1, vec3 bound_2, Node* parent = nullptr, Data* data = nullptr, size_t datacount = 0) : bound_1(bound_1), bound_2(bound_2), datacount(datacount), data(data), parent(parent) {};
        vec3 bound_1;
        vec3 bound_2;
        size_t datacount = 0;
        Data* data = nullptr;
        Node* parent = nullptr;
        Node* children[8] = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};

        friend inline std::ostream& operator<<(std::ostream& os, const Node& self){
            os << "Node { bound_high: " << self.bound_1 << ", bound_low: " << self.bound_2 << ", ";
            if (self.data != nullptr)
                os << "data: " << *self.data << ", datacount: " << self.datacount << ", children: [ ";
            else
                os << "data: nullptr, datacount: " << self.datacount << ", children: [ ";
            for (Node* child : self.children)
                if (child != nullptr)
                    os << *child << ", ";
            os << "] }";
            return os;
        }

        ~Node() {
            for (Node* child : children) {
                delete child;
            }
        }

        inline bool in_bounds(vec3 candidate) {
            return bound_1 >= candidate && bound_2 <= candidate;
        }

        inline void subdivide() {
            vec3 b_rel = bound_1 - bound_2;
            this->children[0] = new Node(
                bound_1,
                b_rel/2 + bound_2,
                this
            );
            this->children[1] = new Node(
                vec3(b_rel.get_x()/2, b_rel.get_y(), b_rel.get_z()) + bound_2,
                vec3(0, b_rel.get_y()/2, b_rel.get_z()/2) + bound_2,
                this
            );
            this->children[2] = new Node(
                vec3(b_rel.get_x(), b_rel.get_y()/2, b_rel.get_z()) + bound_2,
                vec3(b_rel.get_x()/2, 0, b_rel.get_z()/2) + bound_2,
                this
            );
            this->children[3] = new Node(
                vec3(b_rel.get_x()/2, b_rel.get_y()/2, b_rel.get_z()) + bound_2,
                vec3(0, 0, b_rel.get_z()/2) + bound_2,
                this
            );
            this->children[4] = new Node(
                vec3(b_rel.get_x()/2, b_rel.get_y()/2, b_rel.get_z()/2) + bound_2,
                bound_2,
                this
            );
            this->children[5] = new Node(
                vec3(b_rel.get_x(), b_rel.get_y()/2, b_rel.get_z()/2) + bound_2,
                vec3(b_rel.get_x()/2, 0, 0) + bound_2,
                this
            );
            this->children[6] = new Node(
                vec3(b_rel.get_x(), b_rel.get_y(), b_rel.get_z()/2) + bound_2,
                vec3(b_rel.get_x()/2, b_rel.get_y()/2, 0) + bound_2,
                this
            );
            this->children[7] = new Node(
                vec3(b_rel.get_x()/2, b_rel.get_y(), b_rel.get_z()/2) + bound_2,
                vec3(0, b_rel.get_y()/2, 0) + bound_2,
                this
            );
        }

        inline bool insert(vec3 in_key, Data* in_data) {
            if (this->in_bounds(in_key)) {
                datacount++;
                if (this->datacount == 1) {
                    this->data = in_data;
                    return true;
                } else if (this->datacount == 2) {
                    this->subdivide();
                }
                auto ret = false;
                for (Node* child : this->children) {
                    if (ret = child->insert(in_key, in_data))
                        return ret;
                }
            }
            return false;
        }

        inline Data* get(vec3 in_key) {
            if (this->in_bounds(in_key)) {
                if (this->datacount > 1) {
                    Data* ret = nullptr;
                    for (Node* child : this->children)
                        if (ret = child->get(in_key); ret != nullptr)
                            return ret;
                }
                else if (this->datacount == 1)
                    return this->data;
            }
            return nullptr;
        }
    };

template<typename Data>
class octree {
public:
    octree(vec3 bound_greater, vec3 bound_less) :
        root(new Node<Data>(bound_greater, bound_less))
    {};
    inline void insert(vec3 in_key, Data* in_data) {
        this->root->insert(in_key, in_data);
    }

    inline Data* get(vec3 in_key) {
        return this->root->get(in_key);
    }

    friend inline std::ostream& operator<<(std::ostream& os, const octree& self){
        os << "octree { " << *self.root << " }";
        return os;
    }

    ~octree() { delete root; }
private:
    Node<Data>* root;
};