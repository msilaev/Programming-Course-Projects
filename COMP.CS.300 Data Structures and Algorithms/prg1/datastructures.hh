// Datastructures.hh
//
// Student name:
// Student email:
// Student number:

#ifndef DATASTRUCTURES_HH
#define DATASTRUCTURES_HH

#include <string>
#include <vector>
#include <tuple>
#include <utility>
#include <limits>
#include <functional>
#include <exception>
#include <unordered_map>
#include<map>
#include <set>


// Types for IDs
using AffiliationID = std::string;
using PublicationID = unsigned long long int;
using Name = std::string;
using Year = unsigned short int;
using Weight = int;
using Distance = int;

// Return values for cases where required thing was not found
AffiliationID const NO_AFFILIATION = "---";
PublicationID const NO_PUBLICATION = -1;
Name const NO_NAME = "!NO_NAME!";
Year const NO_YEAR = -1;
Weight const NO_WEIGHT = -1;

// Return value for cases where integer values were not found
int const NO_VALUE = std::numeric_limits<int>::min();

// Type for a coordinate (x, y)
struct Coord
{
    int x = NO_VALUE;
    int y = NO_VALUE;
};


// Example: Defining == and hash function for Coord so that it can be used
// as key for std::unordered_map/set, if needed
inline bool operator==(Coord c1, Coord c2) { return c1.x == c2.x && c1.y == c2.y; }
inline bool operator!=(Coord c1, Coord c2) { return !(c1==c2); } // Not strictly necessary

struct CoordHash
{
    std::size_t operator()(Coord xy) const
    {
        auto hasher = std::hash<int>();
        auto xhash = hasher(xy.x);
        auto yhash = hasher(xy.y);
        // Combine hash values (magic!)
        return xhash ^ (yhash + 0x9e3779b9 + (xhash << 6) + (xhash >> 2));
    }
};

// Example: Defining < for Coord so that it can be used
// as key for std::map/set
inline bool operator<(Coord c1, Coord c2)
{
    if (c1.y < c2.y) { return true; }
    else if (c2.y < c1.y) { return false; }
    else { return c1.x < c2.x; }
}

// Return value for cases where coordinates were not found
Coord const NO_COORD = {NO_VALUE, NO_VALUE};

// Return value for cases where Distance is unknown
Distance const NO_DISTANCE = NO_VALUE;

// This exception class is there just so that the user interface can notify
// about operations which are not (yet) implemented
class NotImplemented : public std::exception
{
public:
    NotImplemented() : msg_{} {}
    explicit NotImplemented(std::string const& msg) : msg_{msg + " not implemented"} {}

    virtual const char* what() const noexcept override
    {
        return msg_.c_str();
    }
private:
    std::string msg_;
};

struct Affiliation
{
AffiliationID id;
Name name;
Coord coord;
};

struct Publication
{
PublicationID id;
Name title;
Year year ;
std::vector<AffiliationID> affPub;
std::vector<Publication*> referencing;
Publication* referenced;
};

using AllAffiliations = std::unordered_map<AffiliationID, Affiliation>;
using AllPublications = std::unordered_map<PublicationID, Publication>;

class Datastructures
{
public:
    Datastructures() ;

    ~Datastructures();

    // Estimate of performance: O(1)
    // Short rationale for estimate: map size operation has constant complexity
    unsigned int get_affiliation_count();

    // Estimate of performance: O(n)
    // Short rationale for estimate: map clear operation has linear complexity
    void clear_all();

    // Estimate of performance: O(n)
    // Short rationale for estimate: need to go through all affiliations
    std::vector<AffiliationID> get_all_affiliations();

    // Estimate of performance: O(logn)
    // Short rationale for estimate: find and [] functions have logarithmic complexity
    bool add_affiliation(AffiliationID id, Name const& name, Coord xy);

    // Estimate of performance: O(logn)
    // Short rationale for estimate: find by key function have logarithmic complexity
    Name get_affiliation_name(AffiliationID id);

    // Estimate of performance: O(logn)
    // Short rationale for estimate: find by key function have logarithmic complexity
    Coord get_affiliation_coord(AffiliationID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: need to go through all affiliations
    std::vector<AffiliationID> get_affiliations_alphabetically();

    // Estimate of performance: O(n)
    // Short rationale for estimate: need to go through all affiliations
    std::vector<AffiliationID> get_affiliations_distance_increasing();

    // Estimate of performance: O(n)
    // Short rationale for estimate: find_if has linear complexity
    AffiliationID find_affiliation_with_coord(Coord xy);

    // Estimate of performance: O(n)
    // Short rationale for estimate: find_if has linear complexity
    bool change_affiliation_coord(AffiliationID id, Coord newcoord);

    // Estimate of performance: O(n)
    // Short rationale for estimate: find by key and [] functions have logarithmic complexity
    // but in the worst case publication can have all affiliations.
    bool add_publication(PublicationID id, Name const& name,
                         Year year,  const std::vector<AffiliationID>&  affiliations);

    // Estimate of performance: O(n)
    // Short rationale for estimate: need to go through all publications
    std::vector<PublicationID> all_publications();

    // Estimate of performance: O(logn)
    // Short rationale for estimate: find by key and [] functions have logarithmic complexity
    Name get_publication_name(PublicationID id);

    // Estimate of performance: O(logn)
    // Short rationale for estimate: find by key and [] functions have logarithmic complexity
    Year get_publication_year(PublicationID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: find by key and [] functions have logarithmic complexity
    // but in the worst case publication can have all possible affiliations
    std::vector<AffiliationID> get_affiliations(PublicationID id);

    // Estimate of performance: O(logn)
    // Short rationale for estimate: find by key and [] functions have logarithmic complexity
    bool add_reference(PublicationID id, PublicationID parentid);

    // Estimate of performance: O(n)
    // Short rationale for estimate: find by key and [] functions have logarithmic complexity
    // but publication can reference all others in the worst case
    std::vector<PublicationID> get_direct_references(PublicationID id);

    // Estimate of performance: O(logn)
    // Short rationale for estimate: find by key and [] functions have logarithmic complexity
    bool add_affiliation_to_publication(AffiliationID affiliationid,
                                        PublicationID publicationid);

    // Estimate of performance: O(n)
    // Short rationale for estimate: need to go through all publications
    std::vector<PublicationID> get_publications(AffiliationID id);

    // Estimate of performance: O(logn)
    // Short rationale for estimate: find by key and [] functions have logarithmic complexity
    PublicationID get_parent(PublicationID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: find by key and [] functions have logarithmic complexity
    // but in the worst case we gett all posisble publications
    std::vector<std::pair<Year, PublicationID>> get_publications_after(AffiliationID affiliationid, Year year);

    // Estimate of performance: O(n)
    // Short rationale for estimate: in the worst case we need to go through all publications
    std::vector<PublicationID> get_referenced_by_chain(PublicationID id);


    // Non-compulsory operations

    // Estimate of performance: O(n)
    // Short rationale for estimate: in the worst case we need to go through all publications
    std::vector<PublicationID> get_all_references(PublicationID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: in the worst case we need to go through all publications
    std::vector<AffiliationID> get_affiliations_closest_to(Coord xy);

    // Estimate of performance: O(logn)
    // Short rationale for estimate: find by key and [] functions have logarithmic complexity
    bool remove_affiliation(AffiliationID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: we need to go through all pubplications
    PublicationID get_closest_common_parent(PublicationID id1, PublicationID id2);

    // Estimate of performance: O(logn)
    // Short rationale for estimate: find by key and [] functions have logarithmic complexity
    bool remove_publication(PublicationID publicationid);

private:
    // basic containers are unordered maps because they are fast filling
    AllAffiliations allAff_;
    AllPublications allPubl_;

    // auxilary vectors for storing affilitaion IDs
    std::vector<AffiliationID> allAffIDvec_;

    // auxilary vectors for optimization: fetching sorted values
    // after first call of corresponging functions
    std::vector<AffiliationID> allAffname_sort_;
    std::vector<AffiliationID> allAffcoord_sort_;

    // auxilary unodered map for sorting by coordinate
    std::unordered_map<Coord, AffiliationID, CoordHash> allAffCoord_;

    // Dirty data flags for optimization
    bool flag_Affiliation_with_coord_ = true;
    bool flag_Affiliation_distanse_incr_ = true;
    bool flag_Alphabet_ = true;

};

#endif // DATASTRUCTURES_HH
