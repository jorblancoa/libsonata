/*************************************************************************
 * Copyright (C) 2018-2019 Blue Brain Project
 *
 * This file is part of 'libsonata', distributed under the terms
 * of the GNU Lesser General Public License.
 *
 * See top-level LICENSE.txt file for details.
 *************************************************************************/

#pragma once

#include "common.h"

#include <cstdint>
#include <memory>
#include <set>
#include <string>
#include <vector>


namespace bbp {
namespace sonata {

//--------------------------------------------------------------------------------------------------

class SONATA_API Selection
{
  public:
    using Value = uint64_t;
    using Values = std::vector<Value>;
    using Range = std::pair<Value, Value>;
    using Ranges = std::vector<Range>;

    explicit Selection(Ranges&& ranges);
    explicit Selection(const Ranges& ranges);

    template <typename Iterator>
    static Selection fromValues(Iterator first, Iterator last);
    static Selection fromValues(const Values& values);

    const Ranges& ranges() const;

    Values flatten() const;

    size_t flatSize() const;

    bool empty() const;

  private:
    const Ranges ranges_;
};

template <typename Iterator>
Selection Selection::fromValues(Iterator first, Iterator last) {
    Selection::Ranges ranges;

    Selection::Range range{0, 0};
    while (first != last) {
        const auto v = *first;
        if (v == range.second) {
            ++range.second;
        } else {
            if (range.first < range.second) {
                ranges.push_back(range);
            }
            range.first = v;
            range.second = v + 1;
        }
        ++first;
    }

    if (range.first < range.second) {
        ranges.push_back(range);
    }

    return Selection(std::move(ranges));
}

//--------------------------------------------------------------------------------------------------

class SONATA_API Population
{
  public:
    /**
     * Name of the population used for identifying it in circuit composition
     */
    std::string name() const;

    /**
     * Total number of elements
     */
    uint64_t size() const;

    /**
     * All attribute names (CSV columns + required attributes + union of attributes in groups)
     */
    const std::set<std::string>& attributeNames() const;

    /**
     * All attribute names that are explicit enumerations
     *
     * See also:
     * https://github.com/AllenInstitute/sonata/blob/master/docs/SONATA_DEVELOPER_GUIDE.md#nodes---enum-datatypes
     */
    const std::set<std::string>& enumerationNames() const;

    /**
     * Get attribute values for given Selection
     *
     * If string values are requested and the attribute is a explicit
     * enumeration, values will be resolved to strings.
     *
     * See also:
     * https://github.com/AllenInstitute/sonata/blob/master/docs/SONATA_DEVELOPER_GUIDE.md#nodes---enum-datatypes
     *
     * @param name is a string to allow attributes not defined in spec
     * @throw if there is no such attribute for the population
     * @throw if the attribute is not defined for _any_ element from the selection
     */
    template <typename T>
    std::vector<T> getAttribute(const std::string& name, const Selection& selection) const;

    /**
     * Get attribute values for given Selection
     *
     * If string values are requested and the attribute is a explicit
     * enumeration, values will be resolved to strings.
     *
     * See also:
     * https://github.com/AllenInstitute/sonata/blob/master/docs/SONATA_DEVELOPER_GUIDE.md#nodes---enum-datatypes
     *
     * @param name is a string to allow attributes not defined in spec
     * @param default is a value to use for groups w/o given attribute
     * @throw if there is no such attribute for the population
     */
    template <typename T>
    std::vector<T> getAttribute(const std::string& name,
                                const Selection& selection,
                                const T& defaultValue) const;

    /**
     * Get enumeration values for given attribute and Selection
     *
     * See also:
     * https://github.com/AllenInstitute/sonata/blob/master/docs/SONATA_DEVELOPER_GUIDE.md#nodes---enum-datatypes
     *
     * @param name is a string to allow enumeration attributes not defined in spec
     * @throw if there is no such attribute for the population
     * @throw if the attribute is not defined for _any_ element from the selection
     */
    template <typename T>
    std::vector<T> getEnumeration(const std::string& name, const Selection& selection) const;

    /**
     * Get all allowed attribute enumeration values
     *
     * @param name is a string to allow attributes not defined in spec
     * @throw if there is no such attribute for the population
     */
    std::vector<std::string> enumerationValues(const std::string& name) const;
    /**
     * Get attribute data type, optionally translating enumeration types

     * @internal
     * It is a helper method for dynamic languages bindings;
     * and is not intended for use in the ordinary client C++ code.
     */
    std::string _attributeDataType(const std::string& name,
                                   bool translate_enumeration = false) const;

    /**
     * All dynamics attribute names (JSON keys + union of attributes in groups)
     */
    const std::set<std::string>& dynamicsAttributeNames() const;

    /**
     * Get dynamics attribute values for given Selection
     *
     * @param name is a string to allow attributes not defined in spec
     * @throw if there is no such attribute for the population
     * @throw if the attribute is not defined for _any_ edge from the edge selection
     */
    template <typename T>
    std::vector<T> getDynamicsAttribute(const std::string& name, const Selection& selection) const;

    /**
     * Get dynamics attribute values for given Selection
     *
     * @param name is a string to allow attributes not defined in spec
     * @param default is a value to use for edge groups w/o given attribute
     * @throw if there is no such attribute for the population
     */
    template <typename T>
    std::vector<T> getDynamicsAttribute(const std::string& name,
                                        const Selection& selection,
                                        const T& defaultValue) const;

    /**
     * Get dynamics attribute data type

     * @internal
     * It is a helper method for dynamic languages bindings;
     * and is not intended for use in the ordinary client C++ code.
     */
    std::string _dynamicsAttributeDataType(const std::string& name) const;

  protected:
    Population(const std::string& h5FilePath,
               const std::string& csvFilePath,
               const std::string& name,
               const std::string& prefix);

    Population(const Population&) = delete;

    Population(Population&&) noexcept;

    virtual ~Population() noexcept;

    struct Impl;
    std::unique_ptr<Impl> impl_;
};

template <>
std::vector<std::string> Population::getAttribute<std::string>(const std::string& name,
                                                               const Selection& selection) const;

//--------------------------------------------------------------------------------------------------

template <typename Population>
class SONATA_API PopulationStorage
{
  public:
    PopulationStorage(const std::string& h5FilePath, const std::string& csvFilePath = "");

    PopulationStorage(const PopulationStorage&) = delete;

    PopulationStorage(PopulationStorage&&) noexcept;

    ~PopulationStorage() noexcept;

    /**
     * List all populations.
     *
     */
    std::set<std::string> populationNames() const;

    /**
     * Open specific population.
     * @throw if no population with such a name exists
     */
    std::shared_ptr<Population> openPopulation(const std::string& name) const;

  protected:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

//--------------------------------------------------------------------------------------------------

}  // namespace sonata
}  // namespace bbp
