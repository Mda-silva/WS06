/***********************************************************************
// OOP244 Workshop # 6
//
// File: EmailFile.cpp
// Version 1.0
// Date:
// Author: Sina Talebi Moghaddam
// Description
// This file must be completed by students
// Revision History
// -----------------------------------------------------------
// Name            Date            Reason
//
/////////////////////////////////////////////////////////////////
***********************************************************************/
#define _CRT_SECURE_NO_WARNINGS

#include <fstream>
#include <cstring>
#include "EmailFile.h"

using namespace std;
namespace seneca
{

    Email& Email::operator=(const Email& rhs) {

        if (this != &rhs && rhs.m_email && rhs.m_name) {
            delete[] m_email;
            m_email = nullptr;
            m_email = new char[strlen(rhs.m_email) + 1];
            strcpy(m_email, rhs.m_email);
            delete[] m_name;
            m_name = nullptr;
            m_name = new char[strlen(rhs.m_name) + 1];
            strcpy(m_name, rhs.m_name);
            strcpy(m_year, rhs.m_year);
        }

        return *this;
    }

    bool Email::load(std::ifstream& in) {

        char buffer[BUFFER_SIZE];
        bool result = false;

        if (in.getline(buffer, BUFFER_SIZE, ',')) {
            delete[] m_email;
            m_email = new char[strlen(buffer) + 1];
            strcpy(m_email, buffer);

            if (in.getline(buffer, BUFFER_SIZE, ',')) {
                delete[] m_name;
                m_name = new char[strlen(buffer) + 1];
                strcpy(m_name, buffer);

                if (in.getline(buffer, BUFFER_SIZE, '\n')) {
                    strcpy(m_year, buffer);
                    result = true;
                }
            }
        }

        return result;
    }

    Email::~Email() {
        delete[] m_name;
        delete[] m_email;
    }

    void EmailFile::setFilename(const char* filename)
    {
        delete[] m_filename;
        m_filename = nullptr;
        if (filename)
        {
            m_filename = new char[strlen(filename) + 1];
            m_filename[0] = '\0';
            strcpy(m_filename, filename);
        }
    }

    void EmailFile::setEmpty()
    {
        delete[] m_emailLines;
        delete[] m_filename;
        m_emailLines = nullptr;
        m_filename = nullptr;
        m_noOfEmails = 0;
    }

    bool EmailFile::setNoOfEmails() {

        bool isValid = false;
        ifstream file(m_filename);

        if (!file.is_open()) {
            cout << "Failed to open file: " << m_filename << endl;
        }
        else
        {
            m_noOfEmails = 0;
            while (file) {
                m_noOfEmails += (file.get() == '\n');
            }
            file.close();

            if (m_noOfEmails == 0)
            {
                delete[] m_filename;
                m_filename = nullptr;
            }
            else {
                m_noOfEmails++;
                isValid = true;
            }
        }
        return isValid;
    }

    void EmailFile::loadEmails()
    {
        int a = 0;

        if (m_filename != nullptr)
        {
            delete[] m_emailLines;
            m_emailLines = new Email[m_noOfEmails];
            ifstream file(m_filename);
            bool check = true;
            for (int i = 0; i < m_noOfEmails && check; i++)
            {
                if (m_emailLines[i].load(file)) check = true;
                else check = false;
                a = i;
            }

            m_noOfEmails = a;
        }



    }

    EmailFile::operator bool() const
    {
        if (m_emailLines != nullptr && m_filename != nullptr
            && m_filename[0] != '\0' && m_noOfEmails > 0)
        {
            return true;
        }

        else
        {
            return false;
        }

    }

    EmailFile::EmailFile()
    {
        setEmpty();
    }

    EmailFile::EmailFile(const char* filename)
    {
        if (filename == nullptr)
        {
            setEmpty();
        }
        else
        {
            m_filename = new char[strlen(filename) + 1];
            strcpy(m_filename, filename);
            setNoOfEmails();
            loadEmails();
        }

    }

    EmailFile::EmailFile(EmailFile& copy)
    {
        /*setEmpty();
        if (copy.m_emailLines != nullptr && copy.m_filename != nullptr &&
            copy.m_filename[0] != '\0' && copy.m_noOfEmails > 0)
        {
            m_filename = new char[strlen(copy.m_filename)];
            strcpy(m_filename, copy.m_filename);
            m_noOfEmails = copy.m_noOfEmails;
            m_emailLines = new Email[m_noOfEmails];
            for (int i = 0; i < m_noOfEmails; i++)
            {
                m_emailLines[i] = copy.m_emailLines[i];
            }
        }*/

        *this = copy;

    }

    EmailFile& EmailFile::operator=(const EmailFile& assignment)
    {
        // TODO: insert return statement here
        if (assignment.m_emailLines != nullptr && assignment.m_filename != nullptr &&
            assignment.m_filename[0] != '\0' && assignment.m_noOfEmails > 0)
        {
            setEmpty();
            /* m_filename = new char[strlen(assignment.m_filename) + 1];
             strcpy(m_filename, assignment.m_filename);*/
            setFilename(assignment.m_filename);
            m_noOfEmails = assignment.m_noOfEmails;
            m_emailLines = new Email[m_noOfEmails];
            for (int i = 0; i < m_noOfEmails; i++)
            {
                m_emailLines[i] = assignment.m_emailLines[i];
            }
        }
        return *this;
    }



    EmailFile::~EmailFile()
    {
        delete[] m_emailLines;
        delete[] m_filename;
        m_filename = nullptr;
        m_emailLines = nullptr;
    }


    bool EmailFile::saveToFile(const char* filename) const
    {
        ofstream p_toFile(filename);
        if (!p_toFile.is_open())
        {
            cout << "Error: Could not open file: " << filename << endl;
            return false;
        }
        else
        {
            for (int i = 0; i < m_noOfEmails; i++)
            {
                p_toFile << m_emailLines[i].m_email << "," << m_emailLines[i].m_name << "," << m_emailLines[i].m_year << endl;
            }
            p_toFile.close();
            return true;
        }

    }

    void EmailFile::fileCat(const EmailFile& obj, const char* name)
    {
        Email* newEmail;
        int totalEmails = 0;
        if (m_emailLines != nullptr && m_filename != nullptr &&
            m_filename[0] != '\0' && m_noOfEmails > 0)
        {
            if (obj.m_emailLines != nullptr && obj.m_filename != nullptr &&
                obj.m_filename[0] != '\0' && obj.m_noOfEmails > 0)
            {
                totalEmails = m_noOfEmails + obj.m_noOfEmails;
                newEmail = new Email[totalEmails];
                for (int i = 0; i < m_noOfEmails; i++)
                {
                    newEmail[i] = m_emailLines[i];
                }

                int j = 0;
                for (int i = m_noOfEmails; i < totalEmails; i++)
                {
                    newEmail[i] = obj.m_emailLines[j];
                    j++;
                }

                delete[] m_emailLines;
                m_emailLines = newEmail;
                m_noOfEmails = totalEmails;



                if (name != nullptr)
                {
                    setFilename(name);
                }

                saveToFile(m_filename);
            }
        }

        else
        {
            return;
        }

    }

    std::ostream& EmailFile::view(std::ostream& ostr) const
    {
        if (m_filename) {
            ostr << m_filename << endl;
            ostr.width(strlen(m_filename));
            ostr.fill('=');
            ostr << "=" << endl;
            ostr.fill(' ');
            for (int i = 0; i < m_noOfEmails; i++) {
                ostr.width(35);
                ostr.setf(ios::left);
                ostr << m_emailLines[i].m_email;
                ostr.width(25);
                ostr << m_emailLines[i].m_name;
                ostr << "Year = " << m_emailLines[i].m_year << endl;
            }
        }

        return ostr;
    }

    std::ostream& operator<<(std::ostream& ostr, const EmailFile& text)
    {
        return text.view(ostr);
    }

}
