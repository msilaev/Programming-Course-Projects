#include "employee.hh"
#include <iostream>
#include <map>

Employee::Employee()
{
}

Employee::Employee(const std::string &id):
    id_(id)
{
    date_of_birth_ = Date();
}

Employee::Employee(const std::string& id, const std::string& date_of_birth):
    id_(id), date_of_birth_(date_of_birth)
{
}

Employee::~Employee()
{
    //std::cout << "Employee " << id_ << " destructed." << std::endl;
}

std::string Employee::get_id() const
{
    return id_;
}

void Employee::add_skill(const std::string& skill)
{
    skills_.insert(skill);
}

void Employee::add_project(const std::string &project)
{
    projects_.insert(project);
}

bool Employee::has_skill(const std::string &skill) const
{
    for( std::string skill_item : skills_ )
    {
        if( skill_item == skill )
        {
            return true;
        }
    }
    return false;
}

bool Employee::is_qualified(const std::set<std::string> &requirements) const
{
    if (requirements.empty())
    {
        return true;
    }
    for (auto it = requirements.begin(); it != requirements.end(); ++it )
    {
        if ( skills_.find(*it) != skills_.end())
        {
            return true;
        }
    }
    return false;
}

void Employee::print_id(const std::string& pre_text) const
{
    std::cout << pre_text << id_;
    //date_of_birth_.print();
    //std::cout << std::endl;
}

void Employee::print_skills() const
{
    std::cout << "Skills: ";
    if( skills_.empty() )
    {
        std::cout << "None" << std::endl;
        return;
    }

    std::set<std::string>::const_iterator iter = skills_.begin();
    std::cout << *iter; // Printing the first one
    ++iter;
    while( iter != skills_.end() )
    {
        std::cout << ", " << *iter; // Printing the rest
        ++iter;
    }
    std::cout << std::endl;
}

std::set<std::string> Employee::get_projects() const
{
    return projects_;
}

bool Employee::operator<(const Employee &rhs) const
{
    return id_ < rhs.id_;
}
