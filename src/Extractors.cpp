// =====================================================================================
//
//       Filename:  Extractors.cpp
//
//    Description:  module which scans the set of collected EDGAR files and extracts
//                  relevant data from the file.
//
//      Inputs:
//
//        Version:  1.0
//        Created:  03/20/2018
//       Revision:  none
//       Compiler:  g++
//
//         Author:  David P. Riedel (dpr), driedel@cox.net
//        License:  GNU General Public License v3
//        Company:
//
// =====================================================================================
//


	/* This file is part of EEData. */

	/* EEData is free software: you can redistribute it and/or modify */
	/* it under the terms of the GNU General Public License as published by */
	/* the Free Software Foundation, either version 3 of the License, or */
	/* (at your option) any later version. */

	/* EEData is distributed in the hope that it will be useful, */
	/* but WITHOUT ANY WARRANTY; without even the implied warranty of */
	/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the */
	/* GNU General Public License for more details. */

	/* You should have received a copy of the GNU General Public License */
	/* along with EEData.  If not, see <http://www.gnu.org/licenses/>. */
#include "ExtractEDGAR_XBRL.h"

#include <iostream>

#include <boost/algorithm/string/predicate.hpp>
#include <boost/regex.hpp>

#include "Extractors.h"

const auto XBLR_TAG_LEN{7};

const boost::regex regex_fname{R"***(^<FILENAME>(.*?)$)***"};
const boost::regex regex_ftype{R"***(^<TYPE>(.*?)$)***"};


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  SelectExtractors
 *  Description:  
 * =====================================================================================
 */
FilterList SelectExtractors (int argc, const char* argv[])
{
    FilterList filters;

//    filters.emplace_back(XBRL_data{});
//    filters.emplace_back(XBRL_Label_data{});
    filters.emplace_back(SS_data{});
//    filters.emplace_back(DocumentCounter{});

    filters.emplace_back(HTM_data{});
    filters.emplace_back(Count_SS{});
    return filters;
}		/* -----  end of function SelectExtractors  ----- */

void XBRL_data::UseExtractor(sview document, const fs::path& output_directory, const EE::SEC_Header_fields& fields)
{
    if (auto xbrl_loc = document.find(R"***(<XBRL>)***"); xbrl_loc != sview::npos)
    {
        auto output_file_name = FindFileName(output_directory, document, regex_fname);
        auto file_type = FindFileType(document, regex_ftype);

        // now, we need to drop the extraneous XML surrounding the data we need.

        document.remove_prefix(xbrl_loc + XBLR_TAG_LEN);

        auto xbrl_end_loc = document.rfind(R"***(</XBRL>)***");
        if (xbrl_end_loc != sview::npos)
        {
            document.remove_suffix(document.length() - xbrl_end_loc);
        }
        else
        {
            throw std::runtime_error("Can't find end of XBRL in document.\n");
        }

        if (boost::algorithm::ends_with(file_type, ".INS") && output_file_name.extension() == ".xml")
        {

            std::cout << "got one" << '\n';

            // ParseTheXMl(document, fields);
        }
        WriteDataToFile(output_file_name, document);
    }
}

void XBRL_Label_data::UseExtractor(sview document, const fs::path& output_directory, const EE::SEC_Header_fields& fields)
{
    if (auto xbrl_loc = document.find(R"***(<XBRL>)***"); xbrl_loc != sview::npos)
    {
        auto output_file_name = FindFileName(output_directory, document, regex_fname);
        auto file_type = FindFileType(document, regex_ftype);

        // now, we need to drop the extraneous XML surrounding the data we need.

        document.remove_prefix(xbrl_loc + XBLR_TAG_LEN);

        auto xbrl_end_loc = document.rfind(R"***(</XBRL>)***");
        if (xbrl_end_loc != sview::npos)
        {
            document.remove_suffix(document.length() - xbrl_end_loc);
        }
        else
        {
            throw std::runtime_error("Can't find end of XBRL in document.\n");
        }

        if (boost::algorithm::ends_with(file_type, ".LAB") && output_file_name.extension() == ".xml")
        {

            std::cout << "got one" << '\n';

            ParseTheXMl_Labels(document, fields);
        }
        WriteDataToFile(output_file_name, document);
    }
}

void SS_data::UseExtractor(sview document, const fs::path& output_directory, const EE::SEC_Header_fields& fields)
{
    if (auto ss_loc = document.find(R"***(.xls)***"); ss_loc != sview::npos)
    {
        std::cout << "spread sheet\n";

        auto output_file_name = FindFileName(output_directory, document, regex_fname);

        // now, we just need to drop the extraneous XML surrounding the data we need.

        auto x = document.find(R"***(<TEXT>)***", ss_loc + 1);
        // skip 1 more lines.

        x = document.find('\n', x + 1);
        // x = document.find('\n', x + 1);

        document.remove_prefix(x);

        auto xbrl_end_loc = document.rfind(R"***(</TEXT>)***");
        if (xbrl_end_loc != sview::npos)
        {
            document.remove_suffix(document.length() - xbrl_end_loc);
        }
        else
        {
            throw std::runtime_error("Can't find end of spread sheet in document.\n");
        }

        WriteDataToFile(output_file_name, document);
    }
}

void Count_SS::UseExtractor (sview document,  const fs::path&, const EE::SEC_Header_fields& fields)
{
    if (auto ss_loc = document.find(R"***(.xlsx)***"); ss_loc != sview::npos)
    {
        ++SS_counter;
    }
}		/* -----  end of method Count_SS::UseExtractor  ----- */


void DocumentCounter::UseExtractor(sview, const fs::path&, const EE::SEC_Header_fields& fields)
{
    ++document_counter;
}


void HTM_data::UseExtractor(sview document, const fs::path& output_directory, const EE::SEC_Header_fields& fields)
{
    auto output_file_name = FindFileName(output_directory, document, regex_fname);
    if (output_file_name.extension() == ".htm")
    {
        std::cout << "got htm" << '\n';

        // now, we just need to drop the extraneous XMLS surrounding the data we need.

        auto x = document.find(R"***(<TEXT>)***");

        // skip 1 more line.

        x = document.find('\n', x + 1);

        document.remove_prefix(x);

        auto xbrl_end_loc = document.rfind(R"***(</TEXT>)***");
        if (xbrl_end_loc != sview::npos)
        {
            document.remove_suffix(document.length() - xbrl_end_loc);
        }
        else
        {
            throw std::runtime_error("Can't find end of HTML in document.\n");
        }

        WriteDataToFile(output_file_name, document);
    }
}

void ALL_data::UseExtractor(sview document, const fs::path& output_directory, const EE::SEC_Header_fields& fields)
{
    auto output_file_name = FindFileName(output_directory, document, regex_fname);
    std::cout << "got another" << '\n';

    // now, we just need to drop the extraneous XMLS surrounding the data we need.

    auto x = document.find(R"***(<TEXT>)***");

    // skip 1 more line.

    x = document.find('\n', x + 1);

    document.remove_prefix(x);

    auto xbrl_end_loc = document.rfind(R"***(</TEXT>)***");
    if (xbrl_end_loc != sview::npos)
    {
        document.remove_suffix(document.length() - xbrl_end_loc);
    }
    else
    {
        throw std::runtime_error("Can't find end of XBRL in document.\n");
    }

    WriteDataToFile(output_file_name, document);
}
