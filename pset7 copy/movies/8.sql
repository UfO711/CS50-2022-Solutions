-- List the names of all people who starred in Toy Story
SELECT name FROM movies, stars, people WHERE movies.id = stars.movie_id AND stars.person_id = people.id AND title = "Toy Story";