HEALPix for MySQL
=================

MySQL wrapper of some of the functions provided by the HEALPix library.


Installation
------------

- Make sure you have MySQL (>=5.5) sources. Also, make sure
  that ``mysql_config`` and ``mysqlbug`` are available.
- Edit CMakeList.txt and point it to the directory where mysql and its
  sources are installed.
- Compile:

.. code-block:: bash

    mkdir build
    cd build
    cmake ..
    make
    make install 

- Log into mysql server as root and run:

.. code-block:: bash

    source install_healpix.sql

- If you feel like you don't like it for some reason, uninstall it with

.. code-block:: bash

    source uninstall_healpix.sql


Usage
-----

Currently, only functions for computing the HEALPix index from a pair
or coordinates and 3D vectors, and their inverses are supported. Both work
for the RING ordering (default) as well as the NESTED ordering. In addition,
given a pixel number and ordering scheme, all 8 neighbours can be computed.

To get a HEALPix value for a set of two coordinates:

.. code-block:: mysql
    
    SELECT ang2pix(64, 0.23, 1.3);
    SELECT ang2pix(64, 0.23, 1.3, 'nest');

If the polar angle is out of bounds ([0, PI]), the function returns -1.

As a more real-world example we might want to to compute the HEALPix indices
using ``nside=128`` for the catalog inside the ``tycho2`` table:

.. code-block:: mysql

    SELECT RA. DE, ang2pix(128, RADIANS(DE + 90), RADIANS(RA)) AS healpix FROM tycho2;

Note that the first argument to ``ang2pix()`` is the polar angle and runs
between 0 and PI, therefore we need to add 90 degrees to the declination
column.

To add them permanently to the ``tycho2`` table, we can create a new column
and update it with computed HEALPix values.

.. code-block:: mysql

    ALTER TABLE tycho2 ADD COLUMN healpix INT(11) AFTER ...;
    UPDATE tycho2 SET healpix=ang2pix(128, RADIANS(DE + 90), RADIANS(RA));

Going the other way is more annoying because MySQL doesn't allow outputing
multiple values so multiple function calls are necessary. To get coordinates
of a pixel:

.. code-block:: mysql
    
    SELECT pix2ang(128, 3312, 0), pix2ang(128, 3312, 1);

or

.. code-block:: mysql
    
    SELECT pix2ang(128, 3312, 0, 'nest'), pix2ang(128, 3312, 1, 'nest');

for NESTED ordering.

We can also get the ``i``-th nighbour of any pixel by issuing

.. code-block:: mysql
    
    SELECT neighbours(128, 3312, i);
    SELECT neighbours(128, 3312, i, 'nest');

Note that not all pixels always have 8 neighbours. In those cases -1 is
returned as the neighbour pixel value.

For a few more supported functions see the ``test.sql``.


Credits & License
-----------------

Written by Gal Matijevic at the E-Science group of the `AIP <https://www.aip.de>`_

Based on the `mysql_sphere <https://escience.aip.de/mysql-sphere>`_ package by
Adrian M. Partl.

Original HEALPix library by Krzysztof M. Gorski, Eric Hivon, Martin Reinecke,
Benjamin D. Wandelt, Anthony J. Banday, Matthias Bartelmann,
Reza Ansari & Kenneth M. Ganga

C++ HEALPix port by Martin Reinecke, Max-Planck-Society

Licensed under the GPL License.
