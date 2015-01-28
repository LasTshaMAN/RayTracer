// Copyright 2013 Daniel Parker
// Distributed under the Boost license, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// See https://sourceforge.net/projects/jsoncons/files/ for latest version
// See https://sourceforge.net/p/jsoncons/wiki/Home/ for documentation.

#ifndef JSONCONS_EXT_TYPE_EXTENSIONS_HPP
#define JSONCONS_EXT_TYPE_EXTENSIONS_HPP

#include "jsoncons/json.hpp"
#include "boost/date_time/gregorian/gregorian.hpp"

#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wswitch"
#endif

namespace jsoncons {

    template <typename Storage>
    class value_adapter<char,Storage,boost::gregorian::date>
    {
    public:
        bool is(const basic_json<char,Storage>& val) const
        {
            if (!val.is_string())
            {
                return false;
            }
            std::string s = val.template as<std::string>();
            try
            {
                boost::gregorian::date_from_iso_string(s);
                return true;
            }
            catch (...)
            {
                return false;
            }
        }

        boost::gregorian::date as(const basic_json<char,Storage>& val) const
        {
            std::string s = val.template as<std::string>();
            return boost::gregorian::from_simple_string(s);
        }

        void assign(basic_json<char,Storage>& self, boost::gregorian::date val)
        {
            std::string s(to_iso_extended_string(val));
            self = s;
        }
    };
}

#if defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

#endif
