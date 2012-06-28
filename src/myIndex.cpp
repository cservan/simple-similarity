#include "myIndex.h"

// The following class defines a hash function for strings


using namespace std;
using namespace Tools;
using namespace boost::xpressive;

myIndex::myIndex()
{

}
void myIndex::addIndex(string s, long unsigned int docId, bool direct, bool reverse)
{
    size_t hashvalue=hashValueBoost(s);
    if (hashvalue!=0)
    {
	if (direct)
	{
	
	      multimap< size_t, multimap< unsigned long, unsigned long >  >:: iterator it = m_mapIndex.find(hashvalue);
	      if (it==m_mapIndex.end())
	      {
		  multimap< unsigned long, unsigned long > element;
		  element.insert(pair<unsigned long, unsigned long >(docId,1));
		  m_mapIndex.insert(pair < size_t, multimap< unsigned long, unsigned long >  > (hashvalue,element));
		  m_mapInfo.insert(pair < size_t, string >(hashvalue,s));

	      }
	      else
	      {
		multimap< unsigned long, unsigned long > & element=(*it).second;
		multimap< unsigned long, unsigned long > :: iterator it_element=element.find(docId);
		if (it_element==element.end())
		{
		    element.insert(pair<unsigned long, unsigned long >(docId,1));
		}
		else
		{
		    (*it_element).second++;
		}
	    }
	}
// 	else
// 	{
	
    //     if (reverse)
    //     {
	    multimap<unsigned long , multimap <size_t , unsigned long  > >:: iterator r_it = m_mapReverseIndex.find(docId);
	    if (r_it==m_mapReverseIndex.end())
	    {
		multimap <size_t , unsigned long  > element;
		element.insert(pair<size_t, unsigned long >(hashvalue,1));
		m_mapReverseIndex.insert(pair < unsigned long , multimap <size_t , unsigned long> > (docId,element));
	    }
	    else
	    {
		multimap <size_t , unsigned long  > & element=(*r_it).second;
		multimap <size_t , unsigned long  > :: iterator r_it_element=element.find(hashvalue);
		if (r_it_element==element.end())
		{
		    element.insert(pair<size_t, unsigned long >(hashvalue,1));
		}
		else
		{
		    (*r_it_element).second++;
		}
	    }
// 	}
    }
}

myIndex::myIndex(string s, long unsigned int docId)
{
    addIndex(s,docId,1,1);
}


multimap< size_t, multimap< unsigned long, unsigned long >  > & myIndex::getMyIndex()
{
    return m_mapIndex;
}
multimap< size_t , string > & myIndex::getMyIndexMapInfo()
{
    return m_mapInfo;
}
multimap<unsigned long , multimap <size_t , unsigned long  > > & myIndex::getMyReverseIndex()
{
    return m_mapReverseIndex;
}
unsigned long myIndex::getReversInfos(long unsigned int docId, size_t hashValue)
{
    vector< std::pair< unsigned long, unsigned long> > to_return;
    multimap<unsigned long , multimap <size_t , unsigned long  > >:: iterator r_it = m_mapReverseIndex.find(docId);
    if (r_it==m_mapReverseIndex.end())
    {
	return 0;
    }
    else
    {
	multimap <size_t , unsigned long  > & element=(*r_it).second;
	multimap <size_t , unsigned long  > :: iterator r_it_element=element.find(hashValue);
	if (r_it_element==element.end())
	{
	    return 0;
	}
	else
	{
	    return (*r_it_element).second;
	}
    }
}
string myIndex::toString()
{
    stringstream to_return;
    multimap< size_t, multimap< unsigned long, unsigned long >  >:: iterator it = m_mapIndex.begin();
    for (it= m_mapIndex.begin();it!=m_mapIndex.end(); it++)
    {
	to_return << "\t"<< (*it).first <<endl;
	multimap< unsigned long, unsigned long > & element=(*it).second;
	multimap< unsigned long, unsigned long > ::iterator it2;
	for (it2= element.begin();it2!=element.end(); it2++)
	{
	    to_return << "\t\t"<< (*it2).first <<" | "<<(*it2).second<<endl;
	}	
    }
    multimap<unsigned long , multimap <size_t , unsigned long  > >:: iterator r_it = m_mapReverseIndex.begin();
    for (r_it= m_mapReverseIndex.begin();r_it!=m_mapReverseIndex.end(); r_it++)
    {
	to_return << "\t"<< (*r_it).first <<endl;
	multimap <size_t , unsigned long  > & element=(*r_it).second;
	multimap <size_t , unsigned long  > :: iterator r_it_element;
	for (r_it_element= element.begin();r_it_element!=element.end(); r_it_element++)
	{
	    to_return << "\t\t"<< (*r_it_element).first <<" | "<<(*r_it_element).second<<endl;
	}	
    }
    return to_return.str();
}

void myIndex::compileComplementDataForOkapi()
{
    multimap<unsigned long , multimap <size_t , unsigned long  > >:: iterator r_it;
    for (r_it=m_mapReverseIndex.begin(); r_it!=m_mapReverseIndex.end(); r_it++)
    {
        multimap <size_t , unsigned long  > & element=(*r_it).second;
	multimap <size_t , unsigned long  > :: iterator r_it_element;
	unsigned long l_docSize=0;
	for (r_it_element=element.begin(); r_it_element!=element.end(); r_it_element++)
	{
	    l_docSize=+(*r_it_element).second;
	}
	pair <unsigned long, unsigned long> l_tmp((*r_it).first,l_docSize);
	m_mapDocSize.insert(l_tmp);
    }
}
std::multimap<unsigned long , unsigned long > myIndex::getMapDocSize()
{
    return m_mapDocSize;
}


