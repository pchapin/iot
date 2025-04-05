
README
======

Some of this material was created in support of a second-year IoT class I taught at Vermont
State University. The project runs on a BeaglePlay device and is written in Python with a
partially complete alternate implementation in C.

The projects gathers (simulated) temperature data in the Winooski River Valley of Vermont. It
uploads that data to a cloud database, and then analyzes it using a simple machine learning
method to build a model of how temperatures in that region vary on a daily basis.

This system consists of two programs: `observer.py` and `archivist.py`. The observer reads the
(mock) temperature sensor and the (mock) GPS connected to the BeaglePlay and stores that data in
a local PostgreSQL database. The archivist uploads new temperature records to a PostgreSQL
database server in the cloud. Each uploaded record includes a "system id" that uniquely
identifies the BeaglePlay that made the reading. This allows data from multiple BeaglePlay
devices to store data in a common table.

The observer runs as a service under `systemd` on the BeaglePlay. However, it sleeps most of the
time and gathers data every 15 minutes. The archivist runs once per hour as a `cron` job. The
decision to use `systemd` in one case and `cron` in the other was arbitrary and done mostly for
the educational value than because of any coherent policy decision.

Currently, the observer uses "mock" hardware to get its data. This was done so VTSU software
engineering students could play with the system without dealing with real hardware.

The system pretends to be used at various locations, by multiple devices (or mobile devices), in
the Winooski River Valley of Vermont. The file `mock_data.txt` contains specific locations with
parameters used by the mock temperature sensor to simulate daily temperature variations. No
attempt is being made here to simulate seasonal temperature variations.

This sample program is written in Python. To set up for use do the following:

```bash
$ python -m venv venv
$ source venv/bin/activate
$ pip install -r requirements.txt
```

The `psycopg` library (version 3) for PostgreSQL database access and the `mypy` type checker are
part of this environment.

It is also necessary to install the PostgreSQL database server on the BeaglePlay. This can
be done with the command:

```bash
$ sudo apt install postgresql
```

Once installed, create a default database for the role `debian` and create a table in that
database using the `create-local-table.sql` script.
