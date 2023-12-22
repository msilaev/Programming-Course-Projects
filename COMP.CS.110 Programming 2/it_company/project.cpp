#include "project.hh"
#include <iostream>

Project::Project(const std::string& id, const std::string& start):
    id_(id), start_(start)
{
}

Project::Project(const std::string& id, const Date &start):
    id_(id), start_(start)
{
}

Project::~Project()
{
    //std::cout << "Project " << id_ << " destructed." << std::endl;
}

void Project::set_end(const Date &end)
{
    end_ = end;
}

Date Project::get_end()
{
    return end_ ;
}

Date Project::get_start()
{
    return start_ ;
}

void Project::add_skill(const std::string &skill)
{
    skills_.insert(skill);
}

void Project::add_employee(const std::string &id)
{
    project_staff_.insert(id);
}

void Project::remove_employee(const std::string &id)
{
    project_staff_.erase(id);
}

void Project::print_requirements()
{
    std::cout << "** Requirements: " ;
    if (!skills_.empty())
    {
    for (auto it = skills_.begin(); it != prev(skills_.end()); ++it)
    {
        std::cout << *it << ", ";
    }
    std::cout << *prev(skills_.end()) << std::endl;
    }
    else
    {
        std::cout << "None" << std::endl;
    }
}

void Project::print_dates()
{
    std::cout << id_ << " : " ;    
    this->get_start().print();
    std::cout << " - ";

    if(!(this->get_end().is_default()))
    {
        this->get_end().print() ;
    }
}

std::set<std::string> Project::get_project_requirements()
{
    return skills_;
}

std::set<std::string> Project::get_project_staff()
{
    return project_staff_;
}

bool Project::has_employee(std::string emplyee_id)
{
    return (project_staff_.find(emplyee_id) != project_staff_.end());
}

