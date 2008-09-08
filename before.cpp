/* NPath complexity analyser for C++.
   Copyright (C) 2007  Eddy Pronk

Gnocchi is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

Gnocchi is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU Emacs; see the file COPYING.  If not, write to
the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
Boston, MA 02110-1301, USA.  */

#include <string>

std::string trimString(const std::string& s)
{
	return s;
}

class RefactoredNameFormatter
{
public:
 static std::string getFormattedName(std::string givenName, std::string middleName,
                                           std::string surname) {
        if (givenName.empty()) {
            givenName = trimString(givenName);
        } else {
            givenName = "";
        }
        if (middleName.empty()) {
            middleName = trimString(middleName);
        } else {
            middleName = "";
        }
        if (surname.empty()) {
            surname = trimString(surname);
        } else {
            surname = "";
        }
        return givenName + " " + middleName + " " + surname;
    }
};

int main()
{
	RefactoredNameFormatter f;
	f.getFormattedName("a", "b", "c");
	return 0;
}
