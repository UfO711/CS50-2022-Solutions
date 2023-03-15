-- Keep a log of any SQL queries you execute as you solve the mystery.
-- Start from crime_scene_reports to get scene description
SELECT description FROM crime_scene_reports
WHERE year = 2021
AND month = 7
AND day = 28
AND street = "Humphrey Street";

-- Theft tooke place at 10:15

-- Search interview's transcripts that contain word "bakery"
SELECT transcript FROM interviews WHERE transcript LIKE "%bakery%" AND year = 2021 AND month = 7 AND day = 28;

-- Notice: bakery parking lot, car, Emma's bakery, ATM, Leggett Street, call, the earliest flight
-- Find theif's car license plate
SELECT license_plate FROM bakery_security_logs WHERE year = 2021 AND month = 7 AND day = 28 AND hour = 10 AND minute > 15 AND minute < 35 AND activity = "exit";

-- Suspects based on license plate
SELECT passport_number FROM people WHERE license_plate IN (SELECT license_plate FROM bakery_security_logs WHERE year = 2021 AND month = 7 AND day = 28 AND hour = 10 AND minute > 15 AND minute < 35 AND activity = "exit");

-- Find ATM account
SELECT account_number FROM atm_transactions WHERE year = 2021 AND month = 7 AND day = 28 AND atm_location = "Leggett Street" AND transaction_type = "withdraw";

-- From bank account table, match person's passport
-- Suspects based on account number
SELECT people.passport_number FROM people JOIN bank_accounts ON people.id = bank_accounts.person_id WHERE bank_accounts.account_number IN (SELECT account_number FROM atm_transactions WHERE year = 2021 AND month = 7 AND day = 28 AND atm_location = "Leggett Street" AND transaction_type = "withdraw");

-- Find phone call information in that day with duration less than 60 sec
SELECT caller FROM phone_calls WHERE year = 2021 AND month = 7 AND day = 28 AND duration < 60;

-- Match the suspects who's phone number and license plate are listed in previous result
-- Suspects based on phone calls
SELECT passport_number FROM people WHERE phone_number IN (SELECT caller FROM phone_calls WHERE year = 2021 AND month = 7 AND day = 28 AND duration < 60);




-- Fiftyville
SELECT flights.id, flights.hour, flights.minute
FROM flights
JOIN airports
ON airports.id = flights.origin_airport_id
WHERE city = "Fiftyville"
AND year = 2021
AND month = 7
AND day = 29
ORDER BY flights.hour ASC;


-- The earlish flight out of Fiftyville is with id 36
-- Suspects based on flights
SELECT passport_number FROM passengers WHERE flight_id = 36;

SELECT people.name
FROM people
JOIN passengers
ON people.passport_number = passengers.passport_number
WHERE people.passport_number
IN (SELECT DISTINCT people.passport_number
    FROM people
    JOIN phone_calls ON phone_calls.caller = people.phone_number
    WHERE people.license_plate
    IN (SELECT license_plate
        FROM bakery_security_logs
        WHERE year = 2021
        AND month = 7 AND day = 28 AND hour = 10 AND (minute > 15 AND minute < 35) AND activity = "exit"))
AND passengers.flight_id = 36;

SELECT people.name
FROM people
JOIN passengers
ON people.passport_number = passengers.passport_number
WHERE people.passport_number
IN (SELECT people.passport_number FROM people JOIN bank_accounts ON people.id = bank_accounts.person_id WHERE bank_accounts.account_number IN (SELECT account_number FROM atm_transactions WHERE year = 2021 AND month = 7 AND day = 28 AND atm_location = "Leggett Street" AND transaction_type = "withdraw"))
AND passengers.flight_id = 36;

-- Identify the city they fly to
SELECT airports.city
FROM airports
    JOIN flights
    ON flights.destination_airport_id = airports.id
WHERE flights.id = 36;


SELECT passport_number AS ref_lp FROM people WHERE license_plate IN (SELECT license_plate FROM bakery_security_logs WHERE year = 2021 AND month = 7 AND day = 28 AND hour = 10 AND (minute > 15 AND minute < 35) AND activity = "exit")
INTERSECT
SELECT passport_number AS ref_lp FROM passengers WHERE flight_id = 36;

SELECT passport_number AS ref_bank FROM people JOIN bank_accounts ON people.id = bank_accounts.person_id WHERE bank_accounts.account_number IN (SELECT account_number FROM atm_transactions WHERE year = 2021 AND month = 7 AND day = 28 AND atm_location = "Leggett Street" AND transaction_type = "withdraw")
INTERSECT
SELECT passport_number AS ref_bank FROM passengers WHERE flight_id = 36;

SELECT passport_number AS ref_call FROM people WHERE phone_number IN (SELECT caller FROM phone_calls WHERE year = 2021 AND month = 7 AND day = 28 AND duration < 60)
INTERSECT
SELECT passport_number AS ref_call FROM passengers WHERE flight_id = 36;

-- Bruce
SELECT name FROM people WHERE passport_number =
(
SELECT passport_number FROM people WHERE license_plate IN (SELECT license_plate FROM bakery_security_logs WHERE year = 2021 AND month = 7 AND day = 28 AND hour = 10 AND minute > 15 AND minute < 25 AND activity = "exit")
INTERSECT
SELECT passport_number FROM passengers WHERE flight_id = 36
INTERSECT
SELECT passport_number FROM people JOIN bank_accounts ON people.id = bank_accounts.person_id WHERE bank_accounts.account_number IN (SELECT account_number FROM atm_transactions WHERE year = 2021 AND month = 7 AND day = 28 AND atm_location = "Leggett Street" AND transaction_type = "withdraw")
INTERSECT
SELECT passport_number AS ref_call FROM people WHERE phone_number IN (SELECT caller FROM phone_calls WHERE year = 2021 AND month = 7 AND day = 28 AND duration < 60)
);

SELECT phone_number FROM people WHERE name = "Bruce";
SELECT name FROM people WHERE phone_number IN (SELECT receiver FROM phone_calls WHERE day = 28 AND month = 7 AND year = 2021 AND caller = "(367) 555-5533" AND duration < 60);


-- sofia
SELECT name FROM people WHERE passport_number =
(
SELECT passport_number FROM people WHERE license_plate IN (SELECT license_plate FROM bakery_security_logs WHERE year = 2021 AND month = 7 AND day = 28 AND hour = 10 AND (minute > 15 AND minute < 35) AND activity = "exit")
INTERSECT
SELECT passport_number FROM passengers WHERE flight_id = 36
INTERSECT
SELECT passport_number AS ref_call FROM people WHERE phone_number IN (SELECT caller FROM phone_calls WHERE year = 2021 AND month = 7 AND day = 28 AND duration < 60)
);

SELECT name FROM people WHERE passport_number =
(
SELECT passport_number FROM passengers WHERE flight_id = 36
INTERSECT
SELECT passport_number AS ref_call FROM people WHERE phone_number IN (SELECT caller FROM phone_calls WHERE year = 2021 AND month = 7 AND day = 28 AND duration < 60)
);