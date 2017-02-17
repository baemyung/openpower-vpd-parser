## This file is a template.  The comment below is emitted
## into the rendered file; feel free to edit this file.
// WARNING: Generated header. Do not edit!


#pragma once

#include <map>
#include <iostream>
#include "defines.hpp"
#include "store.hpp"
#include "types.hpp"
#include "utils.hpp"

namespace openpower
{
namespace vpd
{
namespace inventory
{

/** @brief API to write parsed VPD to inventory,
 *      for a specifc FRU
 *
 *  @param [in] vpdStore - Store object containing
 *      parsed VPD
 *  @param [in] path - FRU object path
 */
template<Fru F>
void writeFru(const Store& vpdStore, const std::string& path);

% for key in fruDict.iterkeys():
<%
    fru = fruDict[key]
%>\
// Specialization of ${key}
template<>
void writeFru<Fru::${key}>(const Store& vpdStore,
                           const std::string& path)
{
    ObjectMap objects;
    InterfaceMap interfaces;

    // Inventory manager needs object path, list of interface names to be
    // implemented, and property:value pairs contained in said interfaces

    % for interface, properties in fru.iteritems():
<%
        names = interface.split(".")
        intfName = names[0] + names[-1]
%>\
    PropertyMap ${intfName}Props;
        % for name, value in properties.iteritems():
            % if fru and interface and name and value:
<%
                record, keyword = value.split(",")
%>\
    ${intfName}Props["${name}"] =
        vpdStore.get<Record::${record}, record::Keyword::${keyword}>();
            % endif
        % endfor
    interfaces.emplace("${interface}",
                       std::move(${intfName}Props));
    % endfor

    sdbusplus::message::object_path object(path);
    objects.emplace(std::move(object), std::move(interfaces));

    callPIM(std::move(objects));
}

% endfor
} // namespace inventory
} // namespace vpd
} // namespace openpower