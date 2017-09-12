mapcode
=======

This postgresql extension contains functions from mapcodelib, a C library for
using [mapcodes](http://www.mapcode.com).

Build Dependencies
==================

* cmake
* pg_config
* C compiler

Building
========
To build mapcode, install the dependencies, and then just do this:

    make
    make install
    make installcheck


If you encounter an error such as:

    “Makefile”, line 8: Need an operator

You need to use GNU make, which may well be installed on your system as
`gmake`:

    gmake
    gmake install
    gmake installcheck

If you encounter an error such as:

    make: pg_config: Command not found

Be sure that you have `pg_config` installed and in your path. If you used a
package management system such as RPM to install PostgreSQL, be sure that the
`-devel` package is also installed. If necessary tell the build process where
to find it:

    env PG_CONFIG=/path/to/pg_config make && make installcheck && make install

If you encounter an error such as:

    ERROR:  must be owner of database regression

You need to run the test suite using a super user, such as the default
"postgres" super user:

    make installcheck PGUSER=postgres

Once mapcode is installed, you can add it to a database. If you're running
PostgreSQL 9.1.0 or greater, it's a simple as connecting to a database as a
super user and running:

    CREATE EXTENSION mapcode;

If you've upgraded your cluster to PostgreSQL 9.1 and already had mapcode
installed, you can upgrade it to a properly packaged extension with:

    CREATE EXTENSION mapcode FROM unpackaged;

For versions of PostgreSQL less than 9.1.0, you'll need to run the
installation script:

    psql -d mydb -f /path/to/pgsql/share/contrib/mapcode.sql

If you want to install mapcode and all of its supporting objects into a
specific schema, use the `PGOPTIONS` environment variable to specify the
schema, like so:

    PGOPTIONS=--search_path=extensions psql -d mydb -f mapcode.sql

Dependencies
------------
The `mapcode` extension has no dependencies other than
[mapcodelib](https://github.com/mapcode-foundation/mapcode-cpp), PostgreSQL,
and GCC. It requires a C compiler to compile it.

Copyright and License
---------------------

Copyright (c) 2014-2016 Martin Atukunda and Afriadd Company

This module is free software; you can redistribute it and/or modify it under
the [PostgreSQL License](http://www.opensource.org/licenses/postgresql).

Permission to use, copy, modify, and distribute this software and its
documentation for any purpose, without fee, and without a written agreement is
hereby granted, provided that the above copyright notice and this paragraph
and the following two paragraphs appear in all copies.

In no event shall Martin Atukunda or Afriadd Company be liable to any party for
direct, indirect, special, incidental, or consequential damages, including
lost profits, arising out of the use of this software and its documentation,
even if Martin Atukunda or Afriadd Company has been advised of the possibility of
such damage.

Martin Atukunda and Afriadd Company specifically disclaim any warranties,
including, but not limited to, the implied warranties of merchantability and
fitness for a particular purpose. The software provided hereunder is on an "as
is" basis, and Martin Atukunda and Afriadd Company have no obligations to provide
maintenance, support, updates, enhancements, or modifications.

