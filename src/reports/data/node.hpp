#pragma once

#include <cstdint>
#include <vector>

typedef double* (*refresh_function_t)(double*);

class Node {
public:
    Node(uint64_t gid);
    virtual ~Node() = default;

    uint64_t get_gid() const;

    void fill_data(std::vector<double>::iterator it);
    void refresh_pointers(refresh_function_t refresh_function);

    virtual void add_element(double* element_value, uint32_t element_id);
    virtual size_t get_num_elements() const { return m_elements.size(); }
    
    const std::vector<uint32_t>& get_element_ids() const { return m_element_ids; }

  protected:
    std::vector<uint32_t > m_element_ids;
    std::vector<double*> m_elements;

    uint64_t m_gid;
};

