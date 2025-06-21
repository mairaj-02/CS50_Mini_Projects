-- Keep a log of any SQL queries you execute as you solve the mystery.

.schema
-- crime scene description
SELECT description 
FROM crime_scene_reports
WHERE month = 7 AND
day = 28 AND
street = "Humphrey Street";

-- time was 10:15 am
-- 3 witnesses
-- each witness has "bakery" in their description


-- interview transcripts
SELECT * 
FROM interviews 
WHERE day = 28 AND 
month = 7 AND 
transcript LIKE "%bakery%"                                                                                                          
ORDER BY transcript;

/* 
info from ruth:
thief exit the bakery parking lot between 10:15 and 10:25.

info from Eugene:
thief withdrew money from Leggett Street ATM on 28th july

info from raymond:
thief called someone for less than a minute
thief is taking earliest flight out of fiftyville on 29th july 
*/




-- getting people who left the bakery between 10:15 and 10:25
SELECT *
FROM people
WHERE license_plate IN (
    SELECT license_plate
    FROM bakery_security_logs 
    WHERE day = 28 AND 
    hour = 10 AND 
    minute BETWEEN 15 AND 25 AND
    activity = "exit");


-- Narrowing the list to people who called someone for less than a minute
SELECT *
FROM people
WHERE license_plate IN (
    SELECT license_plate
    FROM bakery_security_logs 
    WHERE day = 28 AND 
    hour = 10 AND 
    minute BETWEEN 15 AND 25 AND
    activity = "exit")
AND phone_number IN (
    SELECT caller 
    FROM phone_calls 
    WHERE day = 28 AND 
    duration < 60);


-- further narrowing down the list to people who withdrew money from Leggett Street ATM
SELECT *
FROM people
WHERE license_plate IN (
    SELECT license_plate
    FROM bakery_security_logs 
    WHERE day = 28 AND 
    hour = 10 AND 
    minute BETWEEN 15 AND 25 AND
    activity = "exit")
AND phone_number IN (
    SELECT caller 
    FROM phone_calls 
    WHERE day = 28 AND 
    duration < 60)
AND id IN (
    SELECT person_id 
    FROM bank_accounts 
    WHERE account_number IN (
        SELECT account_number
        FROM atm_transactions 
        WHERE month = 7 AND
        day = 28 AND 
        atm_location = "Leggett Street" AND 
        transaction_type = "withdraw"));

-- the only two people are Diana and Bruce (the thief is one of them)




-- Getting the Flight ID of the earliest flight which left Fiftyville on 29th July
SELECT * 
FROM flights 
WHERE month = 7 AND 
day = 29 AND 
origin_airport_id = (
    SELECT id 
    FROM airports WHERE 
    city = "Fiftyville");

-- the earliest flight is at 08:20 headed towards LeGuardia Airport New York City
-- the flight ID is 36


-- its either Bruce or Diana who is on this flight, and he/she is the thief
SELECT * 
FROM people WHERE
name = "Bruce" OR name = "Diana" AND 
passport_number IN (
    SELECT passport_number 
    FROM passengers 
    WHERE flight_id = 36);

-- Bruce is the thief


-- To get the thief's accomplice
SELECT * 
FROM people 
WHERE phone_number IN (
    SELECT receiver 
    FROM phone_calls 
    WHERE caller = (
        SELECT phone_number 
        FROM people 
        WHERE name = "Bruce")
    AND duration < 60
    AND day = 28 AND 
    month = 7);

-- Robin is accomplice