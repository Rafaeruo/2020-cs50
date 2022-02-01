SELECT people.name FROM people WHERE people.id IN
(SELECT stars.person_id FROM stars WHERE stars.movie_id IN
(SELECT movies.id from movies JOIN stars ON stars.movie_id = movies.id WHERE stars.person_id IN
 (SELECT people.id from people WHERE people.name="Kevin Bacon" AND people.birth=1958))AND stars.person_id NOT IN (SELECT people.id from people WHERE people.name="Kevin Bacon" AND people.birth=1958))