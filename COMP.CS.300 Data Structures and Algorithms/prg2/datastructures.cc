// Datastructures.cc
//
// Student name:
// Student email:
// Student number:

#include <iostream>

#include "datastructures.hh"
#include <random>
#include <cmath>
#include <queue>
#include <stack>
#include<unordered_map>
#include<unordered_set>
#include<set>
#include<random>
#include<map>
#include<algorithm>

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

// Modify the code below to implement the functionality of the class.
// Also remove comments from the parameter names when you implement
// an operation (Commenting out parameter name prevents compiler from
// warning about unused parameters on operations you haven't yet implemented.)

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
    flag_Affiliation_with_coord_= true;
    flag_Alphabet_ = true;
    flag_Affiliation_distanse_incr_ = true;
    flag_nodes = true;
    flag_edges = true;
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
    flag_Affiliation_with_coord_= true;
    flag_Alphabet_ = true;
    flag_Affiliation_distanse_incr_ = true;
    flag_nodes= true;
    flag_edges = true;

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

Coord Datastructures::
get_affiliation_coord(AffiliationID id)
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
   // cout << "flag " << flag_Alphabet_ <<  endl;

    if (flag_Alphabet_ )
    {
        allAffname_sort_.clear();

        size_t size = allAff_.size();
        allAffname_sort_.resize(size);

//        cout << "all Aff size " << size << "test vect " << allAffIDvec_.size() <<  endl;

        std::size_t ind = 0;
        for ( auto it = allAff_.begin(); it!= allAff_.end(); ++it )
        {
          //  allAffCoord_[it->second.coord] = it->first;

            allAffname_sort_.at(ind) = it->first;
            ++ind;

  //          cout << "test " << it->second.coord.x << " "<< it->second.coord.y << endl;

        }

        sort(allAffname_sort_.begin(), allAffname_sort_.end(),
                  [this](AffiliationID id1, AffiliationID id2)
        { return ( allAff_[id1].name < allAff_[id2].name ) ; } );

    }
    flag_Alphabet_=false;

    return allAffname_sort_;
}

//
std::vector<AffiliationID>
Datastructures::get_affiliations_distance_increasing()
{
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

    flag_Affiliation_distanse_incr_ = false;
    return allAffcoord_sort_;
}

//
AffiliationID Datastructures::find_affiliation_with_coord(Coord xy)
{
    if (flag_Affiliation_with_coord_)
    {
        allAffCoord_.clear();

        for ( auto it = allAff_.begin(); it!= allAff_.end(); ++it )
        {
            allAffCoord_[it->second.coord] = it->first;
        }
    }
    flag_Affiliation_with_coord_ = false;

    auto it =  allAffCoord_.find(xy);
    if (it == allAffCoord_.end() )
    {
        return NO_AFFILIATION ;
    }
    return (it->second);
}

//
bool Datastructures::change_affiliation_coord(AffiliationID id, Coord newcoord)
{
    //cout << "flag " << flagAffiliation_ << endl;

    flag_Affiliation_with_coord_= true;
    flag_Alphabet_ = true;
    flag_Affiliation_distanse_incr_ = true;

    //cout << "flag " << flagAffiliation_ << endl;

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
    flag_edges = true;
    flag_nodes = true;

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
    flag_edges = true;
    flag_nodes = true;
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

//
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

//
bool Datastructures::add_affiliation_to_publication(AffiliationID affiliationid, PublicationID publicationid)
{
    flag_edges=true;
    flag_nodes = true;

    if (allPubl_.find(publicationid) == allPubl_.end() or
            allAff_.find(affiliationid) == allAff_.end())
    {
        return false;
    }
    allPubl_[publicationid].affPub.push_back(affiliationid);
    return true;
}

//
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

//
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
    traverseUp(pub, &allPubl_[id]);
    return pub;
}

void traverseDown(vector<PublicationID>& ref, Publication* master)
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

        if (c1.x*c1.x + c1.y*c1.y < c2.x*c2.x + c2.y*c2.y)// Function to perform Breadth-First Search

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
    flag_Affiliation_with_coord_= true;
    flag_Alphabet_ = true;
    flag_Affiliation_distanse_incr_ = true;
    flag_nodes = true;
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


std::vector<Connection> Datastructures::get_connected_affiliations(AffiliationID id)
{
//    std::size_t size = allAff_.size();
//    vector<Connection> AffConnected(size);

    unordered_map <AffiliationID, Connection> conn_map;

    vector<Connection> conn_vec;
    Connection new_conn;

    auto it = allAff_.find(id);
    if (it == allAff_.end())
    {
        return conn_vec ;
    }
    auto pub = get_publications(id);
    for (auto it_pub = pub.begin(); it_pub != pub.end(); it_pub ++ )
    {
        auto aff = get_affiliations( *it_pub ) ;
        for (auto it_aff = aff.begin(); it_aff != aff.end(); it_aff ++)
        {
            if (id != *it_aff )
            {
                auto it_map = conn_map.find(*it_aff);

                if (it_map == conn_map.end())
                {
                    new_conn.aff1 = id;
                    new_conn.aff2 = *it_aff;
                    new_conn.weight =1;
                    conn_map[*it_aff] = new_conn;
                }
                else
                {
                    conn_map[*it_aff].weight +=1;
                }
            }
        }
    }
    conn_vec.resize(conn_map.size());
    auto it_aff_vec = conn_vec.begin();
    for ( auto it_aff = conn_map.begin(); it_aff != conn_map.end(); it_aff++)
    {
        *it_aff_vec = it_aff ->second;

        it_aff_vec ++;
    }
    return conn_vec ;
}

/*
std::vector<Connection> Datastructures::get_all_connections()
{
   std::vector<Connection> conn_vec;

   set<pair<AffiliationID, AffiliationID>> seenConnections;

   std::vector<AffiliationID> affID_vec = get_all_affiliations();

    for (auto affID : affID_vec)
    {
        auto conn_ID = get_connected_affiliations(affID);

        for (auto target_Edge : conn_ID)
        {
           auto edgePair
            = std::make_pair(target_Edge.aff1, target_Edge.aff2);

           auto edgePair1
            = std::make_pair(target_Edge.aff2, target_Edge.aff1);

            if (seenConnections.find(edgePair) == seenConnections.end() and
                    seenConnections.find(edgePair1) == seenConnections.end())
            {
                seenConnections.insert(edgePair);
                conn_vec.push_back(target_Edge);
            }
        }
    }
    for (auto& connection : conn_vec) {
        if (connection.aff2 < connection.aff1) {
            std::swap(connection.aff1, connection.aff2);
        }
    }
    return conn_vec;
}
*/

std::vector<Connection> Datastructures::get_all_connections()
{

    //cout << "***********************" << endl;
    std::vector<Connection> conn_vec;


    if (!flag_nodes )
    {
        conn_vec.resize( all_edges_.size());
        int ind = 0;

        for (auto iter = all_edges_.begin(); iter != all_edges_.end(); ++iter )
        {
            conn_vec.at(ind) = iter->second;
            ind ++;
        }
        return conn_vec;
    }

   // if ( flag_nodes )
   all_edges_.clear();


   //set<pair<AffiliationID, AffiliationID>> seenConnections;
   //map < pair<AffiliationID, AffiliationID>, Connection> edges;

   std::vector<AffiliationID> affID_vec = get_all_affiliations();
   std::set<AffiliationID> affID_set(affID_vec.begin(), affID_vec.end());

   unordered_map<AffiliationID, bool> visited;

   queue<AffiliationID> queue_vert;

   while (!affID_set.empty())
   {
       AffiliationID root_vert = *affID_set.begin() ;
       affID_set.erase(root_vert);
       queue_vert.push(root_vert);
       visited[root_vert] =true;

       while(!queue_vert.empty())
       {

           auto current_vert = queue_vert.front();
           queue_vert.pop();

           affID_set.erase(current_vert);

            vector<Connection> graph_current = get_connected_affiliations(current_vert);

            visited[current_vert] =true;

            for (auto attached_edge : graph_current)
            {
                if (!visited[attached_edge.aff2])
                {
                     queue_vert.push(attached_edge.aff2);
                     visited[attached_edge.aff2] = true;
                     affID_set.erase(attached_edge.aff2);
                }
                //cout << attached_edge.aff1 << " "<< attached_edge.aff2 << endl;

                    if (attached_edge.aff1 < attached_edge.aff2)
                    {
                        auto edgePair
                         = std::make_pair(attached_edge.aff1, attached_edge.aff2);

                        all_edges_[edgePair] = attached_edge;
                    }
                    else
                    {
                        auto edgePair
                         = std::make_pair(attached_edge.aff2, attached_edge.aff1);

                        Connection new_conn = attached_edge;
                        new_conn.aff1 = attached_edge.aff2;
                        new_conn.aff2 = attached_edge.aff1;

                        all_edges_[edgePair] = new_conn;
                    }

                    /*

                    auto edgePair
                     = std::make_pair(attached_edge.aff1, attached_edge.aff2);

                    seenConnections.insert(edgePair);

                    auto edgePair1
                     = std::make_pair(attached_edge.aff2, attached_edge.aff1);

                if (seenConnections.find(edgePair) == seenConnections.end() and
                         seenConnections.find(edgePair1) == seenConnections.end())
                 {
                     seenConnections.insert(edgePair);
                     conn_vec.push_back(attached_edge);
                 }
                 */
             }       
       }
   }

   flag_nodes = false;

   conn_vec.resize(all_edges_.size());
   int ind=0;
   for (auto& connection : all_edges_)
   {
       conn_vec.at(ind) = connection.second;
       ind ++;
   }
   return conn_vec;
}


Path Datastructures::get_any_path(AffiliationID source, AffiliationID target)
{
    Path path;
    Connection current_edge;

    auto it = allAff_.find(source);
    auto it1 = allAff_.find(target);
    if (it == allAff_.end() or it1 == allAff_.end())
    {
        return path ;
    }

    unordered_map <AffiliationID, bool> visited;
    unordered_map <AffiliationID, Node> nodes_visited;

    //queue<AffiliationID> queue_vert;

    stack<AffiliationID> stack_vert;

    Node source_node ;
    source_node.id = source;
    source_node.parent = nullptr;

    nodes_visited[source] = source_node;

    //queue_vert.push(source);
    stack_vert.push(source);

    visited[source] = true;

    //while (!queue_vert.empty())
    while (!stack_vert.empty())
    {
      AffiliationID current = stack_vert.top();
      stack_vert.pop();

      if (!visited[current])
      {
          stack_vert.push(current);
          visited[current] = true;
      }

      if (current == target)
      {
          while ( nodes_visited[current].parent != nullptr )
          {
              current_edge.aff1= nodes_visited[current].id;
              current_edge.aff2= nodes_visited[current].parent->id;
              current_edge.weight = nodes_visited[current].weight;
              path.push_back(current_edge);
              current = current_edge.aff2= nodes_visited[current].parent->id;
          }

          std::reverse(path.begin(), path.end());
           for (auto iter=path.begin();  iter!= path.end(); ++iter )
           { auto aff = iter->aff2; iter->aff2 = iter->aff1; iter->aff1 = aff;  }

       return path;
      }

      if (flag_edges)
      {
        auto graph_current = get_connected_affiliations(current);
        all_node_connections_[current] = graph_current;
      }

      for (auto attached_edges :  all_node_connections_[current]) //  all_node_connections_[current])
      {
        if (!visited[attached_edges.aff2])
        {
          //queue_vert.push(attached_edges.aff2);
          stack_vert.push(attached_edges.aff2);

          visited[attached_edges.aff2] = true;

          Node current_node;
          current_node.id = attached_edges.aff2;
          current_node.parent = & nodes_visited [attached_edges.aff1];
          current_node.weight = attached_edges.weight;

          nodes_visited[attached_edges.aff2] = current_node;
         }
       }
    }
    flag_edges = false;

   return path;
}


Path Datastructures::get_path_with_least_affiliations(AffiliationID source, AffiliationID target)
{
    Path path;

    Connection current_edge;

    auto it = allAff_.find(source);
    auto it1 = allAff_.find(target);
    if (it == allAff_.end() or it1 == allAff_.end())
    {
        return path ;
    }

    unordered_map<AffiliationID, bool> visited;
    unordered_map<AffiliationID, Node> nodes_visited;

    queue<AffiliationID> queue_vert; // Use queue for BFS

    Node source_node;
    source_node.id = source;
    source_node.parent = nullptr;

    nodes_visited[source] = source_node;

    queue_vert.push(source);
    visited[source] = true;

    while (!queue_vert.empty())
    {
        AffiliationID current = queue_vert.front();
        queue_vert.pop();

        if (current == target)
        {
            while (nodes_visited[current].parent != nullptr)
            {
                current_edge.aff1 = nodes_visited[current].id;
                current_edge.aff2 = nodes_visited[current].parent->id;
                current_edge.weight = nodes_visited[current].weight;
                path.push_back(current_edge);
                current = nodes_visited[current].parent->id;
            }

            std::reverse(path.begin(), path.end());
            for (auto &edge : path)
            {
                std::swap(edge.aff1, edge.aff2);
            }

            return path;
        }

        if (flag_edges)
        {
            auto graph_current = get_connected_affiliations(current);
            all_node_connections_[current] = graph_current;
        }

        for (auto &attached_edges : all_node_connections_[current])
        {
            if (!visited[attached_edges.aff2])
            {
                queue_vert.push(attached_edges.aff2);
                visited[attached_edges.aff2] = true;

                Node current_node;
                current_node.id = attached_edges.aff2;
                current_node.parent = &nodes_visited[attached_edges.aff1];
                current_node.weight = attached_edges.weight;

                nodes_visited[attached_edges.aff2] = current_node;
            }
        }
    }
    flag_edges = false;

    return path;
}

Distance get_distance (AllAffiliations & allAff_, Connection current_edge )
{
return  pow(
        pow((allAff_[current_edge.aff1].coord.x - allAff_[current_edge.aff2].coord.x), 2)
        +
        pow((allAff_[current_edge.aff1].coord.y - allAff_[current_edge.aff2].coord.y), 2),
        0.5);
}


Distance get_distance_int ()
{
return 1.0;
}

 double max(const double a, const double b)
{
    return (a < b) ? b : a;
}

 double min(const double a, const double b)
{
    return (a > b) ? b : a;
}


 struct ComparePaths {
     bool operator()(
             std::pair< std::pair<double, double>,  AffiliationID> a,
             std::pair< std::pair<double, double>,  AffiliationID> b
             )
     {
        if ( a.first.first != b.first.first )
        {
         return a.first.first < b.first.first ;
        }

        else
        {
         return a.first.second < b.first.second ;
        }
     }
 };

 // ////////////////////////////////////////////////
 Path Datastructures::get_path_of_least_friction(AffiliationID source, AffiliationID target)
 {
     std::random_device rd;
     std::mt19937 g(rd());

    // cout << "***************************************" << endl;
     Connection current_edge;
     Path path;

     std::vector< pair< int, Path>> allShortestPaths;
     //std::map<int, Path> allShortestPaths;

     auto it = allAff_.find(source);
     auto it1 = allAff_.find(target);
     if (it == allAff_.end() or it1 == allAff_.end())
     {
         return path ;
     }
     unordered_map<AffiliationID, bool> visited;
     unordered_map<AffiliationID, bool> black;
     unordered_map<AffiliationID, Node> nodes_visited;

     queue<AffiliationID> queue_vert;

     Node source_node;
     source_node.id = source;
     source_node.parent = nullptr;

     // Initialize distances with infinite values
     std::unordered_map < AffiliationID, double> distances;
     std::unordered_map<AffiliationID, double> old_distances;
     std::unordered_map < AffiliationID, int> manhattand_dist;

     std::unordered_map < AffiliationID, int> old_manhattand_dist;

     for (const auto& aff : allAff_)
     {
         old_manhattand_dist[aff.first] = 0;
         manhattand_dist[aff.first] = 0;
         distances[aff.first] = 0;
         visited [aff.first] = false;
         black [aff.first] = false;
     }
     distances[source] = std::numeric_limits<double>::infinity();


     for (const auto& aff : allAff_)
     {
         old_distances[aff.first] = 0;
     }
     old_distances[source] = 0;

     std::priority_queue<std::pair<double, AffiliationID>,
             std::vector<std::pair<double, AffiliationID>>, std::less <>> pq;

     pq.push({std::numeric_limits<double>::infinity(), source});
     nodes_visited[source] = source_node;
     visited[source] = true;

     int COUNT =0;

     while (!pq.empty() )
     {
        auto x = pq.top().second;
        pq.pop();

         if (flag_edges)
         {
             auto graph_current = get_connected_affiliations(x);
             all_node_connections_[x] = graph_current;            
         }

         if (x == target)
         {
             Path path;
             int length =0;
             while (nodes_visited[x].parent != nullptr)
             {
                 //cout << "path constr" << nodes_visited[x].id << " " << nodes_visited[x].parent->id << endl;

                 current_edge.aff1 = nodes_visited[x].id;
                 current_edge.aff2 = nodes_visited[x].parent->id;
                 current_edge.weight = nodes_visited[x].weight;

                 path.push_back(current_edge);

               x = nodes_visited[x].parent->id;
             }
             std::reverse( path.begin(), path.end() );

             // //////////////////////////
             for (auto &edge : path)
             {
                 length++;
                 std::swap(edge.aff1, edge.aff2);               
             }

             for (auto &edge : path)
             {
                 //cout << edge.aff1 << " " << edge.aff2 << " ";
             }
             //cout << endl << "length " << length << endl;
             // //////////////////////

             allShortestPaths.push_back( {length, path} );

             COUNT ++;
            /*
             nodes_visited.clear();
                          nodes_visited[source] = source_node;
                          pq.push({std::numeric_limits<double>::infinity(), source});
                          old_distances[source] = 0;
                                       distances[source] = std::numeric_limits<double>::infinity();
                                       */
         }

         else {

         for (auto &attached_edges : all_node_connections_[x])
         {
           //  cout << "attached weight "<< attached_edges.weight  << " "<< attached_edges.aff2 <<  endl;
             auto y = attached_edges.aff2;
             old_distances[y] = distances[y];
             old_manhattand_dist[y] = manhattand_dist[y];

             if ( distances[y] < min( distances[x] , attached_edges.weight ))
             {
                 distances[y] = min(distances[x] , attached_edges.weight);
                  manhattand_dist[y] = manhattand_dist[x] +1;
             }
             else if (distances[y] == min( distances[x] , attached_edges.weight ))
             {
                 if ( manhattand_dist[y] > manhattand_dist[x] +1)
                 {
                     manhattand_dist[y] = manhattand_dist[x] +1;
                 }
             }

            if (! visited[y] and ! black[y])
             {
                 visited[y] = true;

                 Node current_node;
                 current_node.id = y;
                         //attached_edges.aff2;
                 current_node.parent = &nodes_visited[x];
                        // &nodes_visited[attached_edges.aff1];
                 current_node.weight = attached_edges.weight;
                 nodes_visited[attached_edges.aff2] = current_node;

                 pq.push({distances[y], y});

             }

              if ( visited[y] and ! black[y])
             {
                 if (old_distances[y] < distances[y] )
                 {
                     nodes_visited[y].weight = attached_edges.weight;

                     pq.push({distances[y], y});

                     nodes_visited[y].parent = &nodes_visited[x];
                 }

                 else if  (old_distances[y] == distances[y] )
                         {
                     if(old_manhattand_dist[y] > manhattand_dist[y]) {
                             nodes_visited[y].weight = attached_edges.weight;

                             pq.push({distances[y], y});

                             nodes_visited[y].parent = &nodes_visited[x];
                     }
                         }
             }
          }

         black[x] = true;
         }
     }

     flag_edges = false;
    // cout << "num of path " << allShortestPaths.size() << endl;
     if (allShortestPaths.size() >0)
     {
     return allShortestPaths.at(0).second;
     }
     else {
         return path;
     }
 }




/*

Path Datastructures::get_path_of_least_friction(AffiliationID source, AffiliationID target)
{
    cout << "***************************************" << endl;
    Path path;
    Path path_new;

    Connection current_edge;

    auto it = allAff_.find(source);
    auto it1 = allAff_.find(target);
    if (it == allAff_.end() or it1 == allAff_.end())
    {
        return path ;
    }
    unordered_map<AffiliationID, bool> visited;
    unordered_map<AffiliationID, bool> black;
    unordered_map<AffiliationID, Node> nodes_visited;

    queue<AffiliationID> queue_vert;

    Node source_node;
    source_node.id = source;
    source_node.parent = nullptr;

    //unsigned long current_path_length = allAff_.size();

    //cout << "size " << path_new.size() << " " << current_path_length<< endl;
    // Initialize distances with infinite values

    std::unordered_map < AffiliationID, double> distances;

    std::unordered_map<AffiliationID, double> distances_manhattan;

    for (const auto& aff : allAff_)
    {
        distances[aff.first] = 0;
        distances_manhattan[aff.first] = std::numeric_limits<double>::infinity();

        visited [aff.first] = false;
        black [aff.first] = false;
    }
    distances[source] = std::numeric_limits<double>::infinity();
    distances_manhattan[source] = 0.0;

    std::unordered_map<AffiliationID, double> old_distances;
    std::unordered_map<AffiliationID, double> old_distances_manhattan;

    for (const auto& aff : allAff_)
    {
        old_distances[aff.first] = 0;
        old_distances_manhattan[aff.first] = std::numeric_limits<double>::infinity();

    }
    old_distances[source] = 0;
    old_distances_manhattan[source] = 0.0;

    // Priority queue to store nodes based on their distances
    std::priority_queue< std::pair< pair<double, double>, AffiliationID >,
            std::vector< std::pair< pair<double, double> , AffiliationID > >, ComparePaths > pq;
           // std::less <>> pq;

    pq.push({ {0.0, 0.0 }, source});
    nodes_visited[source] = source_node;
    visited[source] = true;

    while (!pq.empty())
    {
       auto x = pq.top().second;
       auto x_d = pq.top().first.first;

         cout << "extract "<<x << " " << x_d << endl;

        pq.pop();

        if (flag_edges)
        {
            auto graph_current = get_connected_affiliations(x);
            all_node_connections_[x] = graph_current;
        }

        for (auto &attached_edges : all_node_connections_[x])
        {
          //  cout << "attached weight "<< attached_edges.weight  << " "<< attached_edges.aff2 <<  endl;
            auto y = attached_edges.aff2;

            old_distances[y] = distances[y];
            if ( distances[y] < min( distances[x] , attached_edges.weight ))
                 //distances[x] + get_distance(allAff_, attached_edges))
            {
                distances[y] = min(distances[x] , attached_edges.weight);
            }

            old_distances_manhattan[y] = distances_manhattan[y];
            if ( distances_manhattan[y] > distances_manhattan[x] + get_distance_int())
                        {
                            distances_manhattan[y] = distances_manhattan[x] + get_distance_int() ;
                        }


           // cout<< "weight update  " << y << " " << old_distances[y] << " " << distances[y] << endl;

            if (! visited[y] and ! black[y])
            {
                visited[y] = true;

                Node current_node;
                current_node.id = y;
                        //attached_edges.aff2;
                current_node.parent = &nodes_visited[x];
                       // &nodes_visited[attached_edges.aff1];
                current_node.weight = attached_edges.weight;
                nodes_visited[attached_edges.aff2] = current_node;

                pq.push({ {distances[y], distances_manhattan[y] }, y});


             //   cout<< "weight init " << y << " " << nodes_visited[y].parent->id << " "<< distances[y] << endl ;
            }
            else
            {
                if (old_distances[y] < distances[y] and !black[y])
                {
                    nodes_visited[y].weight = attached_edges.weight;

                    pq.push({ {distances[y], distances_manhattan[y] }, y});

                    nodes_visited[y].parent = &nodes_visited[x];
                //    cout<< "weight update parent " << y << " " << nodes_visited[y].parent->id << " "<< distances[y] << endl;

                }

                else if ( (old_distances[y] == distances[y] and !black[y]) and
                          old_distances_manhattan[y] > distances_manhattan[y] )
                {
                    pq.push({ {distances[y], distances_manhattan[y] }, y});

                    nodes_visited[y].parent = &nodes_visited[x];
                }
            }
         }

        black[x] = true;

        // calculate path
        if (x == target)
        {
             path_new.clear();
             while (nodes_visited[x].parent != nullptr)
             {
                //cout << "path constr" << nodes_visited[x].id << " " << nodes_visited[x].parent->id << endl;

                current_edge.aff1 = nodes_visited[x].id;
                current_edge.aff2 = nodes_visited[x].parent->id;
                current_edge.weight = nodes_visited[x].weight;

                path_new.push_back(current_edge);

              x = nodes_visited[x].parent->id;
            }
            std::reverse( path_new.begin(), path_new.end() );
            for (auto &edge : path_new)
            {
                std::swap(edge.aff1, edge.aff2);
            }
            //cout << "size " << path_new.size() << " " << current_path_length<< endl;

            path = path_new;
            return path;
            }

    }
    //flag_edges = false;

    return path;
}

*/



PathWithDist Datastructures::get_shortest_path(AffiliationID source, AffiliationID target)
{
   // cout << "***************************************" << endl;
    PathWithDist path;
    Connection current_edge;

    auto it = allAff_.find(source);
    auto it1 = allAff_.find(target);
    if (it == allAff_.end() or it1 == allAff_.end())
    {
        return path ;
    }
    unordered_map<AffiliationID, bool> visited;
    unordered_map<AffiliationID, bool> black;
    unordered_map<AffiliationID, Node> nodes_visited;

    queue<AffiliationID> queue_vert;

    Node source_node;
    source_node.id = source;
    source_node.parent = nullptr;

    // Initialize distances with infinite values
    std::unordered_map<AffiliationID, double> distances;
    for (const auto& aff : allAff_)
    {
        distances[aff.first] = std::numeric_limits<double>::infinity();
        visited [aff.first] = false;
        black [aff.first] = false;
    }
    distances[source] = 0.0;

    std::unordered_map<AffiliationID, double> old_distances;
    for (const auto& aff : allAff_)
    {
        old_distances[aff.first] = std::numeric_limits<double>::infinity();
    }
    old_distances[source] = 0.0;
    // ///////////////////////////

    // Priority queue to store nodes based on their distances
    std::priority_queue<std::pair<double, AffiliationID>,
            std::vector<std::pair<double, AffiliationID>>, std::greater <>> pq;

    pq.push({0.0, source});
    nodes_visited[source] = source_node;
    visited[source] = true;

    while (!pq.empty())
    {            
        auto x = pq.top().second;
     //  auto x_d = pq.top().first;

       //  cout << x << " " << x_d << endl;

        pq.pop();

        if (flag_edges)
        {
            auto graph_current = get_connected_affiliations(x);
            all_node_connections_[x] = graph_current;
        }

        for (auto &attached_edges : all_node_connections_[x])
        {
            auto y = attached_edges.aff2;
            old_distances[y] = distances[y];

            if ( distances[y] > distances[x] + get_distance(allAff_, attached_edges))
            {
                distances[y] = distances[x] + get_distance(allAff_, attached_edges) ;
            }

            if (! visited[y] and ! black[y])
            {
                visited[y] = true;

                Node current_node;
                current_node.id = y;
                        //attached_edges.aff2;
                current_node.parent = &nodes_visited[x];
                       // &nodes_visited[attached_edges.aff1];
                current_node.weight = attached_edges.weight;
                nodes_visited[attached_edges.aff2] = current_node;

                pq.push({distances[y], y});

             //   cout<< "dist init " << y << " " << nodes_visited[y].parent->id << " "<< distances[y] << endl ;
            }
            else
            {
                if (old_distances[y] > distances[y] and !black[y])
                {
                    nodes_visited[y].weight = attached_edges.weight;

                    pq.push({distances[y], y});
                //    cout<< "dist update " << y << " " << nodes_visited[y].parent->id << " "<< distances[y] << endl;
                    nodes_visited[y].parent = &nodes_visited[x];
                //    cout<< "dist update parent " << y << " " << nodes_visited[y].parent->id << " "<< distances[y] << endl;

                }
            }
         }
        black[x] = true;

        if (x == target)
        {
            while (nodes_visited[x].parent != nullptr)
            {
                //cout << "path constr" << nodes_visited[x].id << " " << nodes_visited[x].parent->id << endl;

                current_edge.aff1 = nodes_visited[x].id;
                current_edge.aff2 = nodes_visited[x].parent->id;
                current_edge.weight = nodes_visited[x].weight;

                Distance current_edge_dist = get_distance(allAff_, current_edge);

                path.push_back(std::make_pair(current_edge, current_edge_dist));

              x = nodes_visited[x].parent->id;
            }
            std::reverse( path.begin(), path.end() );
            for (auto &edge : path)
            {
                std::swap(edge.first.aff1, edge.first.aff2);
            }
            return path;
        }

    }

    flag_edges = false;
    return path;
}


