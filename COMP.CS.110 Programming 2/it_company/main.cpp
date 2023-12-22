/* it_company
 *
 * Desc:
 * This program implements a simple database representing it company
 * (class Company). It contains information about employees (class Emplyee)
 * and projects
 * (class Project). Using command line interface it is possible to
 * add and remove employees, strat and close projects, assign employees
 * to projects, add requirements to projects and skills to employees.
 * The program terminates when the user gives a quitting command
 * ('quit and its variations').
 *
 * Program author
 * Name: Mikhail Silaev
 * Student number: 151861415
 * UserID: xdmisi
 * E-Mail: mikhail.silaev@tuni.fi
 */


#include "cli.hh"
#include "company.hh"

const std::string PROMPT = "IT> ";

int main()
{
    Company* company = new Company();
    Cli cli(company, PROMPT);
    while( cli.exec() ){}

    delete company;
    return EXIT_SUCCESS;
}
