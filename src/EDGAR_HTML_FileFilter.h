// =====================================================================================
//
//       Filename:  EDGAR_HTML_FileFilter.h
//
//    Description:  class which identifies EDGAR files which contain proper XML for extracting.
//
//        Version:  1.0
//        Created:  11/14/2018 09:37:16 AM
//       Revision:  none
//       Compiler:  g++
//
//         Author:  David P. Riedel (dpr), driedel@cox.net
//        License:  GNU General Public License v3
//        Company:
//
// =====================================================================================


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

// =====================================================================================
//        Class:  EDGAR_HTML_FileFilter
//  Description:  class which EDGAR files to extract data from.
// =====================================================================================

#ifndef  _EDGAR_HTML_FILEFILTER_
#define  _EDGAR_HTML_FILEFILTER_

#include <map>
#include <string>
#include <string_view>
#include <tuple>
#include <vector>

#include "ExtractEDGAR.h"
#include "ExtractEDGAR_Utils.h"
#include "AnchorsFromHTML.h"

#include "gq/Node.h"

using sview = std::string_view;

// HTML content related functions

struct FileHasHTML
{
    bool operator()(const EE::SEC_Header_fields&, sview file_content);
};

// Extracting the desired content from each financial statement section
// will likely differ for each so let's encapsulate the code.

struct BalanceSheet
{
    std::string the_data_;
    CNode parsed_data_;

    inline bool empty() const { return the_data_.empty(); }
};

struct StatementOfOperations
{
    std::string the_data_;
    CNode parsed_data_;

    inline bool empty() const { return the_data_.empty(); }
};

struct CashFlows
{
    std::string the_data_;
    CNode parsed_data_;

    inline bool empty() const { return the_data_.empty(); }
};

struct StockholdersEquity
{
    std::string the_data_;
    CNode parsed_data_;

    inline bool empty() const { return the_data_.empty(); }
};

struct FinancialStatements
{
    BalanceSheet balance_sheet_;
    StatementOfOperations statement_of_operations_;
    CashFlows cash_flows_;
    StockholdersEquity stockholders_equity_;

    bool is_complete() const
    {
//        return AllNotEmpty(balance_sheet_, statement_of_operations_, cash_flows_, stockholders_equity_);
        return AllNotEmpty(balance_sheet_, statement_of_operations_, cash_flows_);
    }
};

////using AnchorData = std::tuple<std::string, std::string, sview>;
//struct AnchorData
//{
//    std::string href;
//    std::string name;
//    std::string text;
//    sview anchor_content;
//    sview html_document;
//
//    // get rid of unwanted content in our collected fields.
//
//    void CleanData();
//};
//
//using AnchorList = std::vector<AnchorData>;
//using MultiplierData = std::pair<std::string, const char*>;
struct MultiplierData
{
    sview multiplier;
    sview html_document;
};

inline bool operator==(const MultiplierData& lhs, const MultiplierData& rhs)
{
    return lhs.multiplier == rhs.multiplier && lhs.html_document == rhs.html_document;
}

inline bool operator<(const MultiplierData& lhs, const MultiplierData& rhs)
{
    return lhs.multiplier < rhs.multiplier || lhs.html_document < rhs.html_document;
}

using MultDataList = std::vector<MultiplierData>;

//AnchorList CollectAllAnchors(sview html);
//
//const char* FindAnchorEnd(const char* start, const char* end, int level);
//
//AnchorData ExtractDataFromAnchor(sview whole_anchor, sview html);
//
//AnchorList FilterFinancialAnchors(const AnchorList& all_anchors);

bool FinancialStatementFilter (const AnchorData& an_anchor);

bool FinancialAnchorFilter(const AnchorData& an_anchor);

sview FindFinancialContent (sview file_content);

//AnchorList FindAnchorDestinations(const AnchorList& financial_anchors, const AnchorList& all_anchors);
AnchorData FindDestinationAnchor (const AnchorData& financial_anchor, const AnchorList& all_anchors);
//
//AnchorList FindAllDocumentAnchors(const std::vector<sview>& documents);

MultDataList FindDollarMultipliers(const AnchorList& financial_anchors);

std::vector<sview> LocateFinancialTables(const MultDataList& multiplier_data);

BalanceSheet ExtractBalanceSheet(const std::vector<sview>& tables);

StatementOfOperations ExtractStatementOfOperations(const std::vector<sview>& tables);

CashFlows ExtractCashFlowStatement(const std::vector<sview>& tables);

StockholdersEquity ExtractStatementOfStockholdersEquity(const std::vector<sview>& tables);

//FinancialStatements ExtractFinancialStatements(const std::string& file_content);

MultDataList CreateMultiplierListWhenNoAnchors (sview file_content);

#endif
