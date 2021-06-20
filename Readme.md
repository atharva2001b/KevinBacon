## Explaination
- suppose each movie has a score, and score of the person is addition of scores of his movies. This is the basic logic.
- now if we extend the logic.. the score of the preson is sum of scores of movies of people that participated in the movie with the person.
- now a tree starts forming of movies and people, where root node being the perosn whose score we want to find given depth of the tree.

## Algorithm
- the above can be easily achieved with BFS algorithm

## Implementation
- we have STL liabrary, wherin we can use vectors, maps, multimaps and iterators to iterate over them.
- fstream can be used to write to output file
- sstream can be used to break the strings into relevant data (Data Analysis)

## Data Source
- one can get below files from IDMB website (https://www.imdb.com/interfaces/)
- Four files are used:
1. name.tsv: information on each person: nconst, primaryName,
birthyear,deathYear,primaryProfession, knownForTitles. nconst is the unique identifier. Some title do not have a birthYear, instead it is “\N”. When a person does not have a birthYear, the
year 1900 is used.

2. title.tsv: information on each movie: tconst, titleType, primaryTitle, originalTitle, isAdult,
startYear, endYear, runtimeMinutes, genres. tconst is the unique identifier. Some title do not
have a startYear, instead it is “\N”. When a movie does not have a startYear, the year 2200 is
used. Some genres can be empty, with the value “\N”.

3. principals.tsv: person and their role per title: tconst, ordering, nconst, category, job, characters.
Per movie (tconst), per person(nconst) the category(actress-actor, director, producer, editor..)
they had.

4. ratings.tsv: rating per movie: tconst, averageRating, numVotes

## Note:
- Read readme.txt for more clear understanding.

