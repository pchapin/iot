#!/usr/bin/env python

# You will want to set up a Python virtual environment on your BeaglePlay. Follow these steps:
#
# 1. Go into the folder where you want the virtual environment to be located (e.g., cis-2730).
#
# 2. Use the command: `python -m venv venv`. This command runs the `venv` module to create a
#    virtual environment and puts that environment into a folder that is also named `venv`.
#
# 3. Activate the virtual environment using: `source venv/bin/activate`. You should see the
#    prompt chqange to reflect the name of the virtual environment. Now allow Python operations
#    will use the packages installed in that environment.
#
# 4. Install the psycopg module using: `pip install psycopg`.
#
# When you are done using the virtual environment, you should deactivate it using the command:
# `deactivate`. The next time you want to use the virtual environment, you will have to re-
# activate it, but you won't need to install the psycopg package again.

import psycopg    # For PostgreSQL access.
import os         # For the getenv() function.
from datetime import datetime

from typing import Any

# Return a unique identifier for this device.
def get_system_id() -> int:
    # Later this function will do something real (like look up a MAC address).
    return 1

# Return the current date/time.
def get_timestamp() -> datetime:
    return datetime.utcnow()

# Return a fake temperature.
def get_temperature() -> float:
    # Later this function will do something real (like consult a temperature sensor).
    return 25.0

def get_location() -> tuple[float, float]:
    # Later this function might do something real (like consult a GPS).
    return (45.0, -75.0)

# Database connection parameters
DB_NAME = "debian"
DB_USER = "debian"
DB_PASSWORD = os.getenv("DB_PASSWORD")
if not DB_PASSWORD:
    raise ValueError("No database password found in the environment!")
DB_HOST = "localhost"   # Could also be a remote host.
DB_PORT = "5432"        # The standard PostgreSQL port.
connection_string=f"dbname={DB_NAME} user={DB_USER} password={DB_PASSWORD} host={DB_HOST} port={DB_PORT}"

try:
    # Use `with` to manage the database connection and the corresponding cursor.
    with (psycopg.connect(connection_string) as connection,
          connection.cursor(row_factory=psycopg.rows.dict_row) as cursor):

        # Insert some data for fun.
        insert_query = """
        INSERT INTO reading (latitude, longitude, reading_time, temperature)
          VALUES (%s, %s, %s, %s)
        """
        (latitude, longitude) = get_location()
        cursor.execute(insert_query, (latitude, longitude, get_timestamp(), get_temperature()))
        print("Data inserted successfully.")

        # Fetch and display inserted data to check it.
        print("\nCurrent stored data:")
        cursor.execute("SELECT * FROM reading")
        rows: list[dict[str, Any]] = cursor.fetchall()   # Type annotation as documentation.

        # Just display one field from each row to show how individual fields can be accessed.
        for row in rows:
            print(f"temperature = {row['temperature']}")

except Exception as error:
    print(f"Error: {error}")

