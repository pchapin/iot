
--CREATE TABLE reading (
--    id           INTEGER PRIMARY KEY GENERATED ALWAYS AS IDENTITY,
--    system_id    BIGINT CHECK(system_id > 0),
--    reading_time TIMESTAMP WITHOUT TIME ZONE,  -- Timestamps are in UTC.
--    temperature  NUMERIC(4, 2)                 -- Degrees Celsius.
--);

CREATE TABLE reading2 (
    id           INTEGER PRIMARY KEY GENERATED ALWAYS AS IDENTITY,
    system_id    BIGINT CHECK(system_id > 0),
    reading_time TIMESTAMP WITHOUT TIME ZONE,  -- Timestamps are in UTC.
    latitude     NUMERIC(8, 6) CHECK( -90.0 <= latitude  AND latitude  <=  90.0),
    longitude    NUMERIC(9, 6) CHECK(-180.0 <= longitude AND longitude <= 180.0),
    temperature  NUMERIC(4, 2)                 -- Degrees Celsius.
);
