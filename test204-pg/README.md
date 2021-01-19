Creating a postgresql database and a user from script. Maybe useful in an
auto-deploying git hook to automatically create a database if it does not
exist.

The generated password is saved to a file this example, for manual
inspection of the created database. In actual environment, the password
would be stored in other places like kubernetes secret.
