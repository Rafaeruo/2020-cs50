SELECT movies.title from movies WHERE movies.id IN
(SELECT stars.movie_id FROM stars WHERE stars.person_id IN (SELECT people.id FROM people WHERE people.name="Helena Bonham Carter")) AND movies.id IN
(SELECT stars.movie_id FROM stars WHERE stars.person_id IN (SELECT people.id FROM people WHERE people.name="Johnny Depp"))