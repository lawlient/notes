/* Copyright 2008, 2010, Oracle and/or its affiliates.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * There are special exceptions to the terms and conditions of the GPL
 * as it is applied to this software. View the full text of the
 * exception in file EXCEPTIONS-CONNECTOR-C++ in the directory of this
 * software distribution.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 * */

/* Standard C++ includes */
#include <iostream>

#include <gflags/gflags.h>

/*
 *   Include directly the different
 *   headers from cppconn/ and mysql_driver.h + mysql_util.h
 *   (and mysql_connection.h). This will reduce your build time!
 */
#include "mysql_connection.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

DEFINE_string(mysql_ip, "127.0.0.1", "default mysql ip address");
DEFINE_uint32(mysql_port, 3306, "default mysql port");
DEFINE_string(mysql_user_name, "root", "default mysql user's name");
DEFINE_string(mysql_password, "123456", "default mysql password");

using namespace std;

int main(int argc, char* argv[]) {
    // Parse gflags. We recommend you to use gflags as well.
    GFLAGS_NS::ParseCommandLineFlags(&argc, &argv, true);

    try {
        sql::Driver *driver;
        sql::Connection *con;
        sql::Statement *stmt;
        sql::ResultSet *res;
	int i;

        /* Create a connection */
        driver = get_driver_instance();
        std::string url = "tcp://" + FLAGS_mysql_ip + ":" + std::to_string(FLAGS_mysql_port);
        con = driver->connect(url, FLAGS_mysql_user_name, FLAGS_mysql_password);
        /* Connect to the MySQL test database */
        // con->setSchema("X");

        stmt = con->createStatement();
	stmt->execute("USE X");
        res = stmt->executeQuery("select id, day, i, s, r, b, t from time_table limit 3");
        while (res->next()) {
		std::cout << "id  = " << res->getString("id") << std::endl;
		std::cout << "day = " << res->getString("day") << std::endl;
		i = 3;
		std::cout << "i   = " << res->getInt(i++) << std::endl;
		std::cout << "s   = " << res->getInt(i++) << std::endl;
		std::cout << "r   = " << res->getInt(i++) << std::endl;
		std::cout << "b   = " << res->getInt(i++) << std::endl;
		std::cout << "t   = " << res->getInt(i++) << std::endl;
        }
        delete res;
        delete stmt;
        delete con;

    } catch (sql::SQLException &e) {
        cout << "# ERR: SQLException in " << __FILE__;
        cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
        cout << "# ERR: " << e.what();
        cout << " (MySQL error code: " << e.getErrorCode();
        cout << ", SQLState: " << e.getSQLState() << " )" << endl;
    }

    cout << endl;

    return EXIT_SUCCESS;
}
