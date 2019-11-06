#include <algorithm>

#include "node.hpp"

Node::Node(uint64_t gid)
    : m_gid(gid)
{}

uint64_t Node::get_gid() const {
    return m_gid;
}

void Node::add_element(double* element_value, uint32_t element_id) {
    m_elements.push_back(element_value);
    m_element_ids.push_back(element_id);
}

void Node::fill_data(std::vector<double>::iterator it) {
    std::transform(m_elements.begin(), m_elements.end(), it, [](auto elem){ return *elem; });
}

void Node::refresh_pointers(refresh_function_t refresh_function) {
    for (auto &elem: m_elements) {
        elem = refresh_function(elem);
    }
}

