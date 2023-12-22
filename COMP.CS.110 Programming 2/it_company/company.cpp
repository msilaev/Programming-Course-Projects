#include "company.hh"
#include "utils.hh"
#include <iostream>
#include <set>
#include<algorithm>

Company::Company()
{
}

Company::~Company()
{    
    for (auto& iter: all_staff_) {
        delete iter.second;
    }
    all_staff_.clear();

    for (auto& iter: projects_) {
        delete iter.second;
    }
    projects_.clear();
 }

void Company::set_date(Params params)
{
    std::string day = params.at(0);
    std::string month = params.at(1);
    std::string year = params.at(2);
    if( not Utils::is_numeric(day, false) or
        not Utils::is_numeric(month, false) or
        not Utils::is_numeric(year, false) )
    {
        std::cout << NOT_NUMERIC << std::endl;
        return;
    }
    Utils::today.set(stoi(day), stoi(month), stoi(year));
    std::cout << "Date has been set to ";
    Utils::today.print();
    std::cout << std::endl;
}

void Company::advance_date(Params params)
{
    std::string amount = params.at(0);
    if( not Utils::is_numeric(amount, true) )
    {
        std::cout << NOT_NUMERIC << std::endl;
        return;
    }
    Utils::today.advance(stoi(amount));
    std::cout << "New date is ";
    Utils::today.print();
    std::cout << std::endl;
}

void Company::recruit(Params params)
{
    std::string employee_id = params.at(0);
    std::map<std::string, Employee*>::const_iterator
            iter = current_staff_.find(employee_id);

    if( iter != current_staff_.end() )
    {
        std::cout << ALREADY_EXISTS << employee_id << std::endl;
        return;
    }

    iter = all_staff_.find(employee_id);
    if( iter != all_staff_.end() )
    {
        current_staff_.insert(*iter);
        std::cout << EMPLOYEE_RECRUITED << std::endl;
        return;
    }

    Employee* new_employee = new Employee(employee_id);
    all_staff_.insert({employee_id, new_employee});
    current_staff_.insert({employee_id, new_employee});
    std::cout << EMPLOYEE_RECRUITED << std::endl;
}

void Company::leave(Params params)
{
    std::string employee_id = params.at(0);
    std::map<std::string, Employee*>::const_iterator
            iter = current_staff_.find(employee_id);
    if( iter == current_staff_.end() )
    {
        std::cout << CANT_FIND << employee_id << std::endl;
        return;
    }

    current_staff_.erase(iter); // Employee still stays in all_staff_
    std::cout << EMPLOYEE_LEFT << std::endl;
}

void Company::add_skill(Params params)
{
    std::string employee_id = params.at(0);
    std::string skill_name = params.at(1);

    std::map<std::string, Employee*>::const_iterator
            staff_iter = current_staff_.find(employee_id);
    if( staff_iter == current_staff_.end() )
    {
        std::cout << CANT_FIND << employee_id << std::endl;
        return;
    }
    staff_iter->second->add_skill(skill_name);
    std::cout << SKILL_ADDED << employee_id << std::endl;
}

void Company::print_current_staff(Params)
{
    if( current_staff_.empty() )
    {
        std::cout << "None" << std::endl;
        return;
    }

    for( auto& employee : current_staff_ )
    {
        employee.second->print_id("");
        std::cout << std::endl;
    }
}

void Company::create_project(Params params)
{
    std::string poject_id = params.at(0);

    if (projects_.find(poject_id) != projects_.end())
    {
        std::cout << ALREADY_EXISTS << poject_id << std::endl;;
        return;
    }
    Project* new_project = new Project(poject_id, Utils::today);
    new_project->set_end(Date());
    all_projectID_.push_back(poject_id);
    projects_.insert({poject_id, new_project});
    std::cout << PROJECT_CREATED << std::endl;
}

void Company::close_project(Params params)
{
    std::string project_id = params.at(0);
    auto iter = projects_.find(project_id);

    if (iter == projects_.end())
    {
        std::cout << CANT_FIND << project_id << std::endl;
        return;
    }
    iter->second->set_end(Utils::today);
    std::cout << PROJECT_CLOSED << std::endl;
}

void Company::print_projects(Params)
{
    for (auto iter = all_projectID_.begin();
         iter != all_projectID_.end(); ++iter )
    {
        auto start = projects_[*iter]->get_start() ;
        auto end = projects_[*iter]->get_end() ;

        if(end.is_default())
        {
            std::cout << *iter << " : ";
            start.print();
            std::cout << " - ";
            std::cout << std::endl;
        }
        else
        {
            std::cout << *iter << " : ";
            start.print();
            std::cout << " - ";
            end.print();
            std::cout << std::endl ;
        }
    }
}

void Company::add_requirement(Params params)
{
    std::string project_id = params.at(0);
    std::string skill = params.at(1);

    if (projects_.find(project_id) == projects_.end())
    {
        std::cout << CANT_FIND  << project_id << std::endl;
        return;
    }
    auto previous_reqs = projects_[project_id]->get_project_requirements();
    projects_[project_id]->add_skill(skill);
    std::vector<std::string> non_qualified;
    std::set<std::string> project_staff =
            projects_[project_id]->get_project_staff();

    for (auto it = project_staff.begin(); it!= project_staff.end(); ++it)
    {
        auto empoyee = current_staff_[*it];

        if (!empoyee->has_skill(skill) and previous_reqs.empty())
        {
            non_qualified.push_back(*it);
            projects_[project_id]->remove_employee(*it);
        }
    }
    if (!non_qualified.empty())
    {
        std::cout << NOT_QUALIFIED;
        for (auto it = non_qualified.begin();
             it != non_qualified.end()-1 ; ++it)
        {
            std::cout << *it << ", ";
        }
        std::cout << *(non_qualified.end()-1) << std::endl;
    }
    std::cout << REQUIREMENT_ADDED << project_id << std::endl;
}

void Company::assign(Params params)
{
    std::string project_id = params.at(1);
    std::string empl_id = params.at(0);   

    if (current_staff_.find(empl_id) == current_staff_.end() )
    {
        std::cout << CANT_FIND  << empl_id << std::endl;
        return;
    }

    if (projects_[project_id]->has_employee(empl_id))
    {
        std::cout << CANT_ASSIGN << empl_id << std::endl;
        return;
    }

    auto project_reqs = projects_[project_id]->get_project_requirements();

    if (! current_staff_[empl_id]->is_qualified(project_reqs) )
    {
        std::cout << CANT_ASSIGN << empl_id << std::endl;
        return;
    }
    projects_[project_id]->add_employee(empl_id);
    current_staff_[empl_id]->add_project(project_id);
    std::cout << STAFF_ASSIGNED << project_id << std::endl;
}


void Company::print_project_info(Params params)
{
    std::string project_id = params.at(0);
    if (projects_.find(project_id) == projects_.end())
    {
        std::cout << CANT_FIND << project_id << std::endl;
        return;
    }
 // print dates
    projects_[project_id]->print_dates();
    std::cout << std::endl;

 // print Requirements
    projects_[project_id]->print_requirements();

 // print Staff
    auto staff = projects_[project_id]->get_project_staff();
    auto count = staff.size();

    std::cout << "** Staff: " ;
    if (!staff.empty())
    {
    for (auto it = staff.begin(); it != prev(staff.end()); ++it)
    {
        ++count;
        std::cout << *it << ", ";
    }    
    std::cout << *prev(staff.end()) << std::endl;
    }
    else
    {
        std::cout << "None" << std::endl;
    }
}

void Company::print_employee_info(Params params)
{    
    auto employee_id = params.at(0);
    if (current_staff_.find(employee_id) == current_staff_.end())
    {
        std::cout << CANT_FIND << employee_id << std::endl;
        return;
    }
    // print skills
    current_staff_[employee_id]->print_skills();

    // print projects
    auto projects_empl= current_staff_[employee_id]->get_projects();
    std::cout << "Projects: "  ;
    if( projects_empl.empty() )
    {
        std::cout << "None" ;
    }
    else
    {
        std::set<std::string>::const_iterator iter = projects_empl.begin();
        while( iter != projects_empl.end() )
        {
            std::cout << std::endl ;
            //std::cout << "** " << *iter << " : " ;
            std::cout << "** ";
            projects_[*iter]->print_dates() ;
            ++iter;
        }
    }
    std::cout << std::endl;
 }

void Company::print_active_staff(Params)
{
    int count_tot=0;

 for (auto it = all_staff_.begin(); it != all_staff_.end(); ++it)
 {
     int count=0;

     for (auto it_proj = projects_.begin(); it_proj != (projects_.end()); ++ it_proj)
     {
         auto staff = it_proj->second->get_project_staff();

         auto it1 = std::find( staff.begin(), staff.end(), it->first);

         if(it1 != staff.end())
         {
             ++count;
             ++ count_tot;
         }
     }
     if (count > 0)
     {
        std::cout << it->first << std::endl;
     }
 }
 if (count_tot == 0)
 {
     std::cout << "None" << std::endl;
 }
}
