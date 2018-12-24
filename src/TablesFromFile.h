/*
 * =====================================================================================
 *
 *       Filename:  TablesFromFile.h
 *
 *    Description:  Extract HTML tables from a block of text.
 *
 *        Version:  1.0
 *        Created:  12/21/2018 09:22:12 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  David P. Riedel (), driedel@cox.net
 *        License:  GNU General Public License v3
 *   Organization:  
 *
 * =====================================================================================
 */

	/* This file is part of ExtractEDGARData. */

	/* ExtractEDGARData is free software: you can redistribute it and/or modify */
	/* it under the terms of the GNU General Public License as published by */
	/* the Free Software Foundation, either version 3 of the License, or */
	/* (at your option) any later version. */

	/* ExtractEDGARData is distributed in the hope that it will be useful, */
	/* but WITHOUT ANY WARRANTY; without even the implied warranty of */
	/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the */
	/* GNU General Public License for more details. */

	/* You should have received a copy of the GNU General Public License */
	/* along with ExtractEDGARData.  If not, see <http://www.gnu.org/licenses/>. */


#ifndef  _TABLESFROMFILE_INC_
#define  _TABLESFROMFILE_INC_

#include <iterator>
#include <string>
#include <string_view>

using sview = std::string_view;

#include <boost/regex.hpp>

/*
 * =====================================================================================
 *        Class:  TablesFromHTML
 *  Description:  iterate over tables (if any) in block of text.
 * =====================================================================================
 */
class TablesFromHTML
{
public:

    class iterator: public std::iterator<
                    std::forward_iterator_tag,      // iterator_category
                    sview                           // value_type
                    >
    {
        const boost::regex regex_table_{R"***(<table.*?>.*?</table>)***",
            boost::regex_constants::normal | boost::regex_constants::icase};
        boost::cregex_token_iterator doc_;
        boost::cregex_token_iterator end_;

        sview html_;
        mutable sview current_table_;

    public:

        iterator() = default;
        explicit iterator(sview html);

        iterator& operator++();
        iterator operator++(int) { iterator retval = *this; ++(*this); return retval; }

        bool operator==(iterator other) const { return doc_ == other.doc_; }
        bool operator!=(iterator other) const { return !(*this == other); }

        reference operator*() const { return current_table_; };
    };

    public:
        /* ====================  LIFECYCLE     ======================================= */
        TablesFromHTML (sview html);                             /* constructor */

        /* ====================  ACCESSORS     ======================================= */

    iterator begin() const;
    iterator end() const;

        /* ====================  MUTATORS      ======================================= */

        /* ====================  OPERATORS     ======================================= */

    protected:
        /* ====================  METHODS       ======================================= */

        /* ====================  DATA MEMBERS  ======================================= */

    private:
        /* ====================  METHODS       ======================================= */

        /* ====================  DATA MEMBERS  ======================================= */

        sview html_;

}; /* -----  end of class TablesFromHTML  ----- */

#endif   /* ----- #ifndef TABLESFROMFILE_INC  ----- */