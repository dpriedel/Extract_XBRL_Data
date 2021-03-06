
-- automate our connection

-- dbext:profile=sec_extracts

select 1;

-- how many entries in our table

select * from live_unified_extracts.sec_filing_id limit 5 ;
select count(*) from live_extracts.edgar_filing_data ;

select count(*) from xbrl_extracts.edgar_filing_id ;
select count(*) from xbrl_extracts.edgar_filing_data ;


select count(*) from live_extracts.edgar_filing_data where user_label = 'Missing Value' ;

select count(*) from xbrl_extracts.edgar_filing_data where user_label = 'Missing Value' ;

select count(distinct(filing_id)) from live_extracts.edgar_filing_data where user_label = 'Missing Value' ;


select * from live_extracts.edgar_filing_data where user_label = 'Missing Value' limit 20;

select t2.filing_id, t2.cik, t2.company_name, t2.file_name from  (select distinct(filing_id) from live_extracts.edgar_filing_data where user_label = 'Missing Value') as t1 inner join live_extracts.edgar_filing_id as t2 on t1.filing_id = t2.filing_id limit 5;

select filing_id, count(*) as ctr from live_extracts.edgar_filing_data where user_label = 'Missing Value' group by filing_id order by ctr desc limit 20;
select filing_id, count(*) as ctr from live_extracts.edgar_filing_data where user_label = 'Missing Value' group by filing_id order by ctr desc;


-- find filings with most missing data.

select t2.filing_id, t2.cik, t2.company_name, t2.file_name, t1.ctr from (select filing_id, count(*) as ctr from live_extracts.edgar_filing_data where user_label = 'Missing Value' group by filing_id order by ctr desc limit 20) as t1 inner join live_extracts.edgar_filing_id as t2 on t1.filing_id = t2.filing_id;

select t2.filing_id, t2.cik, t2.company_name, t2.file_name, t1.ctr from (select filing_id, count(*) as ctr from live_extracts.edgar_filing_data where user_label = 'Missing Value' group by filing_id order by ctr desc) as t1 inner join live_extracts.edgar_filing_id as t2 on t1.filing_id = t2.filing_id;

-- from test DB

select t2.filing_id, t2.cik, t2.company_name, t2.file_name, t1.ctr from (select filing_id, count(*) as ctr from xbrl_extracts.edgar_filing_data where user_label = 'Missing Value' group by filing_id order by ctr desc) as t1 inner join xbrl_extracts.edgar_filing_id as t2 on t1.filing_id = t2.filing_id;

-- =============================================

-- create extension tablefunc;

select * from live_extracts.edgar_filing_data limit 5; 

select filing_id, user_label, xbrl_value from live_extracts.edgar_filing_data limit 5;

-- total revenue - XBRL

select t1.filing_id, t1.label, t1.value, t1.period_end, t2.company_name, t2.period_ending from live_unified_extracts.sec_xbrl_data as t1 inner join live_unified_extracts.sec_filing_id as t2 on t1.filing_ID = t2.filing_ID and t1.period_end = t2.period_ending where t1.tsv @@ phraseto_tsquery('total & revenue') and t1.context_ID like '%YTD' and t1.period_end between '2015-01-01' and '2019-12-31'order by t2.cik, t2.period_ending asc  limit 150;

-- total revenue - HTML

select t1.filing_id, t1.label, t1.value, t2.company_name, t2.period_ending from live_unified_extracts.sec_stmt_of_ops_data as t1 inner join live_unified_extracts.sec_filing_id as t2 on t1.filing_ID = t2.filing_ID where t1.tsv @@ phraseto_tsquery('total & revenue') and t2.period_ending between '2015-01-01' and '2019-12-31'order by t2.cik, t2.period_ending asc  limit 150;



select count(*) from live_extracts.edgar_filing_data where tsv @@ phraseto_tsquery('revenue') and period_begin = period_end  and period_end between '2010-01-01' and '2017-12-31' ;

select * from live_extracts.edgar_filing_data where tsv @@ phraseto_tsquery('operating & income') and period_begin = period_end  and period_end between '2010-01-01' and '2017-12-31' limit 20;

select count(*) from live_extracts.edgar_filing_data where tsv @@ phraseto_tsquery('total & revenue') ;

select * from crosstab
(
    'select filing_id, label, value from unified_extracts.sec_xbrl_data where tsv @@ phraseto_tsquery(''total & revenue'') and period_end between ''2012-01-01'' and ''2015-12-31'' order by filing_id, period_end asc  limit 5',
    'select distinct filing_id from unified_extracts.sec_xbrl_data order by 1'
)
as
(
    filing_id text,
    user_label text,
    xbrl_value text,
    xbrl_value2 text,
    xbrl_value3 text,
    xbrl_value4 text,
    xbrl_value5 text
);
