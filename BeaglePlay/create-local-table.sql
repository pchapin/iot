
-- This table is used by the local database for storing data temporarily.
CREATE TABLE reading (
    id           INTEGER PRIMARY KEY GENERATED ALWAYS AS IDENTITY,
    latitude     NUMERIC(8, 6) CHECK( -90.0 <= latitude  AND latitude  <=  90.0),
    longitude    NUMERIC(9, 6) CHECK(-180.0 <= longitude AND longitude <= 180.0),
    reading_time TIMESTAMP WITHOUT TIME ZONE,  -- Timestamps are in UTC.
    temperature  NUMERIC(4, 2)                 -- Degrees Celsius.
);
