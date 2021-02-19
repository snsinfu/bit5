#

```console
$ vagrant up
```

Test connection and also observe that the vagrant user can CREATEROLE and
CREATEDB:

```console
$ vagrant ssh
$ psql postgres
psql (13.2 (Debian 13.2-1.pgdg100+1))
Type "help" for help.

postgres=> \du
                                   List of roles
 Role name |                         Attributes                         | Member of
-----------+------------------------------------------------------------+-----------
 postgres  | Superuser, Create role, Create DB, Replication, Bypass RLS | {}
 vagrant   | Create role, Create DB                                     | {}

```

Create a DB and a full-access role for the DB:

```console
$ ./mkdatabase.sh forum
CREATE DATABASE
CREATE ROLE
GRANT
Database 'forum' created with owner password: 783235e845948c53e632209554b86a55
```

This creates a database named `forum` and a user with the same name, with a
randomly-generated password. The user has all privileges on the database.
