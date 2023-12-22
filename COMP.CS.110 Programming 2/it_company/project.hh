/* Class Project
 * ----------
 * COMP.CS.110 FALL 2023
 * ----------
 * Class for describing a project in an IT company.
 *
 * File author
 * Name: Mikhail Silaev
 * Student number: 151861415
 * UserID: xdmisi
 * E-Mail: mikhail.silaev@tuni.fi
*/

#ifndef PROJECT_HH
#define PROJECT_HH

#include "employee.hh"
#include "date.hh"
#include "utils.hh"
#include <string>
#include <map>
#include <set>

const std::string NOT_QUALIFIED = "Not qualified any more: ";

class Project
{
public:
    /**
     * @brief Project constructor
     * @param : id
     * @param : start (given as string ddmmyyyy)
     */
    Project(const std::string& id, const std::string& start);

    /**
     * @brief Project constructor
     * @param : id
     * @param : start (given as Date object)
     */
    Project(const std::string& id, const Date& start);

    /**
     * @brief Project destructor
     */
    ~Project();

    /**
     * @brief set_end
     * @param : Date class object
     * Sets the end date of the project
     */
    void set_end(const Date& end );

    /**
     * @brief get_end
     * @param : -
     * Returns the end date of the project
     */
    Date get_end() ;

    /**
     * @brief get_start
     * @param : -
     * Returns the start date of the project
     */
    Date get_start() ;

    /**
     * @brief add_skill
     * @param : string giving name of skill
     * Adds skill to the set of skills
     */
    void add_skill(const std::string& skill) ;

    /**
     * @brief add_employee
     * @param : string giving id name of employee
     * Adds employee to the set of employees
     */
    void add_employee(const std::string &id);

    /**
     * @brief remove_employee
     * @param : string giving id name of employee
     * Removes employee from the set of employees
     */
    void remove_employee(const std::string &id);

    /**
     * @brief print_requirements
     * @param : -
     * Prints comma-separated skills required in the project
     */
    void print_requirements();

    /**
     * @brief print_dates
     * @param : -
     * Prints start and end dates. If project is ongoing only start date is
     * printed
     */
    void print_dates();

    /**
     * @brief get_project_requirements
     * @param : -
     * Return set of skills required for the project
     */
    std::set<std::string> get_project_requirements();

    /**
     * @brief get_project_staff
     * @param : -
     * Return set of staff names assigned to the project
     */
    std::set<std::string> get_project_staff();

    /**
     * @brief has_employee
     * @param : -
     * Returns true is the project has employee with emplyee_id
     */
    bool has_employee(std::string emplyee_id);

private:
    /**
     * @brief obvious attributes
     */
    std::string id_; // Can be a name or any other identifier
    Date start_;
    Date end_;
    std::set<std::string> project_staff_;
    std::set<std::string> skills_;
};

#endif // PROJECT_HH
