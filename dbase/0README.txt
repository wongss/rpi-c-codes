sqlite3 is used in this example.

Environment
-----------
Make sure this was done:
sudo apt-get sqlite3 libsqlite3-dev

Compiling & Linking
-------------------
During linking this option "-lsqlite3" must be in.
Refer to the Makefile

Creating database
-----------------
You can create a database using steps here:

Create folder & cd to “~/prj/db”

Run “sqlite3 first.db” & it waits for your commands

Execute these in sqlite3: (Do not enter comments below preceded by '#')
# creat a table of  4 fields of given names "id", "name", "cardinfo1" and "cardinfo2" of specific types
> CREATE TABLE user(id INT, name TEXT, cardinfo1 INT, cardinfo2 INT);
# add a new record into the table
> INSERT INTO user VALUES(123,”John K”,568,890);
# displays all records
> SELECT * FROM user;
# this quits from sqlite3
>.exit

Observe that file first.db created

Running the program
-------------------
After reporting the
- tables in the database
- the fields in the 1st table (if found)
- all data in the 1st table (if any)
it let you enter any valid sql statements for the database and show any data returned.
Type "exit" to quit the program