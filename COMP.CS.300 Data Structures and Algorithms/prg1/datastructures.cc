// Datastructures.cc
//
// Student name:
// Student email:
// Student number:

#include "datastructures.hh"

#include <random>

#include <cmath>

#include <iostream>

using namespace std;

std::minstd_rand rand_engine; // Reasonably quick pseudo-random generator

template <typename Type>
Type random_in_range(Type start, Type end)
{
    auto range = end-start;
    ++range;

    auto num = std::uniform_int_distribution<unsigned long int>(0, range-1)(rand_engine);

    return static_cast<Type>(start+num);
}


Datastructures::Datastructures()
{
    // Write any initialization you need here
}

Datastructures::~Datastructures()
{
    // Write any cleanup you need here
}

unsigned int Datastructures::get_affiliation_count()
{
    return  allAff_.size();
}

void Datastructures::clear_all()
{
    // cleaning data set dirty data flags to default "true"
    flag_Affiliation_with_coord_= true;
    flag_Alphabet_ = true;
    flag_Affiliation_distanse_incr_ = true;

    if (!allAff_.empty())
    {
        allAff_.clear();
        allAffCoord_.clear();

    }
    if (!allPubl_.empty())
    {
        allPubl_.clear();
    }
}

std::vector<AffiliationID> Datastructures::get_all_affiliations()
{
    std::size_t size = allAff_.size();
    vector<AffiliationID> allAffIDvec(size);

    std::size_t ind = 0;
    for (auto it = allAff_.begin(); it != allAff_.end(); ++it)
    {
        allAffIDvec.at(ind) = it->first;
        ++ind;
    }

    return allAffIDvec ;
}

bool Datastructures::add_affiliation(AffiliationID id,
                                     const Name & name, Coord xy)
{
    // adding new affiliation data breaks sorting and
    //sets dirty data flags to "true"
    flag_Affiliation_with_coord_= true;
    flag_Alphabet_ = true;
    flag_Affiliation_distanse_incr_ = true;

    auto it = allAff_.find(id);
    if (it !=  allAff_.end())
    {
        return false;
    }

    Affiliation newAffiliation;
    newAffiliation.id = id ;
    newAffiliation.coord = xy;
    newAffiliation.name = name;

    allAff_[id] = newAffiliation;
    return true;
}

Name Datastructures::get_affiliation_name(AffiliationID id)
{
    auto it = allAff_.find(id);
    if (it ==  allAff_.end())
    {
        return NO_NAME;
    }
    return allAff_.at(id).name;
}

Coord Datastructures::get_affiliation_coord(AffiliationID id)
{
    auto it = allAff_.find(id);
    if (it == allAff_.end())
    {
        return NO_COORD;
    }
    return allAff_.at(id).coord;
}

std::vector<AffiliationID> Datastructures::get_affiliations_alphabetically()
{   
    // if data is not sorted form new sorted vector allAffname_sort_
    if (flag_Alphabet_ )
    {
        allAffname_sort_.clear();

        size_t size = allAff_.size();
        allAffname_sort_.resize(size);

        std::size_t ind = 0;
        for ( auto it = allAff_.begin(); it!= allAff_.end(); ++it )
        {
            allAffname_sort_.at(ind) = it->first;
            ++ind;
        }

        sort(allAffname_sort_.begin(), allAffname_sort_.end(),
                  [this](AffiliationID id1, AffiliationID id2)
        { return ( allAff_[id1].name < allAff_[id2].name ) ; } );

    }
    // set dirty data flag to false meaning data is sorted
    flag_Alphabet_=false;

    // return sorted vector
    return allAffname_sort_;
}


std::vector<AffiliationID>
Datastructures::get_affiliations_distance_increasing()
{  
    // if data is not sorted form new sorted vector allAffcoord_sort_

    if (flag_Affiliation_distanse_incr_)
    {
        std::size_t size = allAff_.size();
        allAffcoord_sort_.resize(size);

        std::size_t ind = 0;
        for ( auto it = allAff_.begin(); it!= allAff_.end(); ++it )
        {
            allAffcoord_sort_.at(ind) =  it->first;

            ++ ind;
        }

        std::sort( allAffcoord_sort_.begin(), allAffcoord_sort_.end(),
        [this](AffiliationID id1, AffiliationID id2)
         {
            Coord c1 = allAff_.at(id1).coord;
            Coord c2 = allAff_.at(id2).coord;

            if (  c1.x*c1.x + c1.y*c1.y < c2.x*c2.x + c2.y*c2.y)
            {
                return true;
            }
            else if (( c1.x*c1.x + c1.y*c1.y == c2.x*c2.x + c2.y*c2.y ) and (c1.y < c2.y) )
            {
                return true;
            }
        return false;
        });
    }

    // set dirty data flag to false meaning data is sorted
    flag_Affiliation_distanse_incr_ = false;

    // return sorted vector
    return allAffcoord_sort_;
}


AffiliationID Datastructures::find_affiliation_with_coord(Coord xy)
{
    // We use unstructured map allAffCoord_ with coordinate as key
    // because search is very fast.

    // If new data has been insrted form new allAffCoord_
    if (flag_Affiliation_with_coord_)
    {
        allAffCoord_.clear();

        for ( auto it = allAff_.begin(); it!= allAff_.end(); ++it )
        {
            allAffCoord_[it->second.coord] = it->first;           
        }
    }

    // set dirty data flag to false meaning allAffCoord_ is up to date
    flag_Affiliation_with_coord_ = false;

    auto it =  allAffCoord_.find(xy);
    if (it == allAffCoord_.end() )
    {
        return NO_AFFILIATION ;
    }

    return (it->second);

}


bool Datastructures::change_affiliation_coord(AffiliationID id, Coord newcoord)
{    
    // chane affiliation information sets dirty data flags to default true
    flag_Affiliation_with_coord_= true;
    flag_Alphabet_ = true;
    flag_Affiliation_distanse_incr_ = true;

    auto it = allAff_.find(id);       
    if (it == allAff_.end())
    {
        return false ;
    }
    allAff_[id].coord = newcoord;
    return true;
}

bool Datastructures::add_publication(PublicationID id,
                                     const Name &name,
                                     Year year,
                                     const std::vector<AffiliationID>&
                                     affiliations)
{
    auto it = allPubl_.find(id);
    if (it != allPubl_.end())
    {
        return false;
    }
    Publication pub;
    pub.referenced =  nullptr;
    pub.referencing = {};
    pub.id = id;
    pub.year = year;
    pub.title = name;
    pub.affPub = affiliations;
    allPubl_[id] = pub;

    return true;
}

std::vector<PublicationID> Datastructures::all_publications()
{    
    std::size_t size = this->allPubl_.size();
    vector<PublicationID> allPubIDvec(size);  

    std::size_t ind = 0;
    for (auto it = allPubl_.begin(); it != allPubl_.end(); ++it)
    {
        allPubIDvec.at(ind) = it->first;
        ++ind;
    }

    return allPubIDvec;
}

Name Datastructures::get_publication_name(PublicationID id)
{    
    auto it = allPubl_.find(id);
    if (it ==  allPubl_.end())
    {
        return NO_NAME;
    }

    return allPubl_.at(id).title;
}

Year Datastructures::get_publication_year(PublicationID id)
{    
    auto it = allPubl_.find(id);
    if (it ==  allPubl_.end())
    {
        return NO_YEAR;
    }

    return allPubl_.at(id).year;
}

std::vector<AffiliationID> Datastructures::get_affiliations(PublicationID id)
{    

    auto it = allPubl_.find(id);
    if (it ==  allPubl_.end())
    {
        auto v ={ NO_AFFILIATION};
        return v;
    }

    return allPubl_.at(id).affPub;
}

bool Datastructures::add_reference(PublicationID id, PublicationID parentid)
{    

    if( allPubl_.find(id) == allPubl_.end() or allPubl_.find(parentid) == allPubl_.end() )
    {
        return false;
    }

    auto ref = (allPubl_[parentid].referencing);
    if( find(ref.begin(), ref.end(), &allPubl_[parentid]) == ref.end())
    {
        (allPubl_[parentid].referencing).push_back(&allPubl_[id]) ;
    }
    (allPubl_[id].referenced) = &allPubl_[parentid] ;

    return true;
}


std::vector<PublicationID> Datastructures::get_direct_references(PublicationID id)
{
    std::vector<PublicationID> ref;

    if (allPubl_.find(id) == allPubl_.end())
    {
        ref.push_back(NO_PUBLICATION);
        return ref;
    }

    auto referencing = (allPubl_[id].referencing);
    for (auto it = referencing.begin(); it != referencing.end(); ++ it)
    {
        ref.push_back((*(*it)).id);
    }
    return ref;
}


bool Datastructures::add_affiliation_to_publication(AffiliationID affiliationid, PublicationID publicationid)
{
    if (allPubl_.find(publicationid) == allPubl_.end() or
            allAff_.find(affiliationid) == allAff_.end())
    {
        return false;
    }
    allPubl_[publicationid].affPub.push_back(affiliationid);  
    return true;
}


std::vector<PublicationID> Datastructures::get_publications(AffiliationID id)
{
   vector<PublicationID> pub;

   auto it = allAff_.find(id);
   if (it == allAff_.end())
   {
       pub.push_back(NO_PUBLICATION);
   }
   for (auto it = allPubl_.begin(); it != allPubl_.end(); ++it)
   {
       auto it1= find((it->second).affPub.begin(), (it->second).affPub.end(), id);
       if (it1 != (it->second).affPub.end())
       {
           pub.push_back(it->first);
       }
   }
   return pub;    
}


PublicationID Datastructures::get_parent(PublicationID id)
{    
    if (allPubl_.find(id) == allPubl_.end())
    {
        return NO_PUBLICATION;
    }
    if (allPubl_[id].referenced == nullptr)
    {
        return NO_PUBLICATION;
    }
    return allPubl_[id].referenced->id;
}

std::vector<std::pair<Year, PublicationID> >
Datastructures::get_publications_after(AffiliationID affiliationid, Year year)
{
    std::vector<std::pair<Year, PublicationID>> y_pub;
    auto it = allAff_.find(affiliationid);

    if (it == allAff_.end())
    {
        y_pub.push_back({NO_YEAR, NO_PUBLICATION});
        return y_pub;
    }   
    auto pub_aff = this-> get_publications(affiliationid);

    vector<PublicationID> pub_aff_filt;

    for(auto it = pub_aff.begin(); it != pub_aff.end(); ++it)
    {
        if(allPubl_[*it].year >= year)
        {
            pub_aff_filt.push_back( allPubl_[*it].id);
        }
    }

    for (auto it = pub_aff_filt.begin(); it != pub_aff_filt.end(); ++it )
    {
        y_pub.push_back({allPubl_[*it].year, allPubl_[*it].id});
    }

    std::sort(y_pub.begin(), y_pub.end(),
              [](std::pair<Year, PublicationID> pair1,
              std::pair<Year, PublicationID> pair2)
    {
        if (pair1.first != pair2.first)
        {
            return (pair1.first < pair2.first);
        }
        else
        {
            return (pair1.second < pair2.second);
        }
    });
    return y_pub;
}


void traverseUp(vector<PublicationID>& ref, Publication* slave)
// This is auxilary function for traversing up the publication tree
// "slave" node means the child or referencing publcation.
// *slave.referenced is parent or "master" node -referenced publicaiton
{
    if(! ((*slave).referenced == nullptr))
    {
        slave = (*slave).referenced;
        ref.push_back((*slave).id);

        traverseUp(ref, slave) ;
    }
}


std::vector<PublicationID> Datastructures::get_referenced_by_chain(PublicationID id)
{
    std::vector<PublicationID> pub;
    auto it = allPubl_.find(id);
    if (it == allPubl_.end())
    {
        pub.push_back(NO_PUBLICATION);
        return pub;
    }
    // we go up by publication tree untill meet the root
    // or publication which references no other
    traverseUp(pub, &allPubl_[id]);
    return pub;
}

void traverseDown(vector<PublicationID>& ref, Publication* master)
// This is auxilary function for traversing down the publication tree
// "master"  means parent node publicaiton which is referenced by
// child node publications
{
    if( ! ((*master).referencing.size() == 0 ))
    {
        for (auto it = (*master).referencing.begin(); it != (*master).referencing.end(); ++it)
        {        
            ref.push_back((*(*it)).id);
            traverseDown(ref, (*it) );
        }
    }
}

std::vector<PublicationID> Datastructures::get_all_references(PublicationID id)
{
    std::vector<PublicationID> pub;
    auto it = allPubl_.find(id);
    if (it == allPubl_.end())
    {
        pub.push_back(NO_PUBLICATION);
        return pub;
    }
    // we use auxilary function to traverse down the publication
    // tree untill leaves are reached or publications referencing no
    // others
    traverseDown(pub, &allPubl_[id]);
    return pub;
}

std::vector<AffiliationID> Datastructures::get_affiliations_closest_to(Coord xy)
{
    vector<AffiliationID> allAffIDvec;
    for (auto it = allAff_.begin(); it != allAff_.end(); ++it)
    {
        allAffIDvec.push_back(it->first);
    }
    std::sort(allAffIDvec.begin(), allAffIDvec.end(),
    [this, &xy](AffiliationID id1, AffiliationID id2)
     {
        Coord c1 = allAff_.at(id1).coord;
        Coord c2 = allAff_.at(id2).coord;

        c1.x = c1.x - xy.x;
        c1.y = c1.y - xy.y;

        c2.x = c2.x - xy.x;
        c2.y = c2.y - xy.y;

        if (c1.x*c1.x + c1.y*c1.y < c2.x*c2.x + c2.y*c2.y)
        {
            return true;
        }
        else if (( c1.x*c1.x + c1.y*c1.y == c2.x*c2.x + c2.y*c2.y ) and (c1.y < c2.y) )
        {
            return true;
        }
    return false;
    });

    vector<AffiliationID> affIDfiltered;

    if (allAffIDvec.size()>3)
    {
        for (auto it = allAffIDvec.begin(); it != allAffIDvec.begin()+3; ++it)
        {
            affIDfiltered.push_back(*it) ;
        }
    }
    else
    {
        affIDfiltered = allAffIDvec;
    }

    return affIDfiltered;
}

bool Datastructures::remove_affiliation(AffiliationID id)
{      
    // changing affiliation data sets dirty data flags to default true
    flag_Affiliation_with_coord_= true;
    flag_Alphabet_ = true;
    flag_Affiliation_distanse_incr_ = true;
    auto it = allAff_.find(id);

    if (it == allAff_.end())
    {
        return false;
    }
    allAff_.erase(it);

    auto pub_id = this-> get_publications(id);    

    for(auto it = pub_id.begin(); it!=pub_id.end(); ++it)
    {
        auto removeIt = remove_if(allPubl_[*it].affPub.begin(), allPubl_[*it].affPub.end(),
                                  [&id](AffiliationID aff) { return (aff == id); });

        allPubl_[*it].affPub.erase(removeIt, allPubl_[*it].affPub.end());
    }
    return true;    
}


PublicationID Datastructures::get_closest_common_parent(PublicationID id1, PublicationID id2)
{
    auto allPubIDvec = this->all_publications();
    auto it1 = find(allPubIDvec.begin(), allPubIDvec.end(), id1);
    auto it2 = find(allPubIDvec.begin(), allPubIDvec.end(), id2);

   if (it1 == allPubIDvec.end() or it2 == allPubIDvec.end() )
    {
        return NO_PUBLICATION;
    }

    auto pub_v1 = this->get_referenced_by_chain(id1);
    auto pub_v2 = this->get_referenced_by_chain(id2);

    if (*(prev(pub_v1.end())) != *(prev(pub_v2.end())) )
    {
        return NO_PUBLICATION;
    }

    auto last_parent = *(prev(pub_v1.end()));

    while( pub_v1.size()>0 and pub_v2.size()>0 and *(prev(pub_v1.end())) == *(prev(pub_v2.end()))  )
    {
        last_parent = *(prev(pub_v1.end()));

        pub_v1.pop_back();
        pub_v2.pop_back();
    }
    return (last_parent);
}


bool Datastructures::remove_publication(PublicationID publicationid)
{    
    if (allPubl_.find(publicationid) == allPubl_.end())
    {
        return false;
    }

    auto parent = this->get_parent(publicationid) ;
    if (parent != NO_PUBLICATION)
    {
        auto removeIt = remove_if((allPubl_[parent].referencing).begin(), (allPubl_[parent].referencing).end(),
                                  [&publicationid](Publication* pub) { return (pub->id == publicationid); });

        (allPubl_[parent].referencing).erase(removeIt, (allPubl_[parent].referencing).end());
    }

    if(allPubl_[publicationid].referencing.size() != 0)
    {       
        for (auto it1 = (allPubl_[publicationid].referencing).begin();
             it1 != (allPubl_[publicationid].referencing).end(); ++it1)
        {
            (*it1)->referenced = nullptr;
        }        
    }
    auto it2 = allPubl_.find(publicationid);

    if (it2 != allPubl_.end())
    {
            allPubl_.erase(it2);
    }
    return true;
}


