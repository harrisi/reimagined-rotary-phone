# reimagined-rotary-phone
Simple music library manager for Computer Science II project.

# Pre-requisite
The song "database" is read from a file `songs.txt` in the directory the
executable is run in. The format of the file is:

```
song title;song artist;mm;ss;song album
```

# Basic usage
When the program is first run, the song database is read into memory in one
pass. After this, all modifications seen are made to the in-memory
representation of the data until program termination, when the in-memory
data overwrites the `songs.txt` file.

## Adding songs
Songs can be added with the `add` option. When adding songs, the user must
enter information in a pre-determined order: title, artist, duration,
album. Title, artist, and album are read in for 1000 characters or a
newline is encountered, and can include (almost) any characters. Duration
is entered in the format of `mm:ss`, where `mm` is the minutes of the song
and `ss` is the seconds of the song. There is no limit (other than limits
on the size of `unsigned int`) for minutes, but seconds are required to
satisfy `0 <= ss < 60`.

## Removing songs
Songs can be removed with the `remove` option. When removing a song, the
user must enter the index of the song (which is shown anytime a song is
displayed). The index must be a valid song index.

## Searching for songs
Songs can be searched for with a number of different options. The basic
flow of searching for something is as follows:

```
Selection> s
Enter search query (or ":?" for help): pink floyd
Index: 42
Title: Us and Them
Arist: Pink Floyd
Duration: 7:49
Album: The Dark Side of the Moon
...
```

There are multiple specifiers the user can supply, as well. From the `:?`
option for searching:

```
This is the help message for searching.
Searching follows the basic format of:
	[FIELD]: QUERY
Where field is an optional field selector of:
	title
	artist
	time
	album
And QUERY is the search query. Using one of these fields as the start of
the query allows for more specific search functionality. If the field
selector is omitted, the default behavior is to search for every field for
each item. This is slower, but is still useful.
For specific help with a FIELD, try :help FIELD
```

Title, artist, and album all follow the same format:

```
Selection> s
Enter search query (or ":?" for help): title: us and them
Index: 42
Title: Us and Them
Arist: Pink Floyd
Duration: 7:49
Album: The Dark Side of the Moon
...
```

Time offers a different approach. When passed the `time` field, the search
results will include all songs that have a duration that satisfy a given
predicate. As an example:

```
Selection> s
Enter search query (or ":?" for help): time: > 7:00
Index: 42
Title: Us and Them
Arist: Pink Floyd
Duration: 7:49
Album: The Dark Side of the Moon
...
```

The valid comparison operators are: `<`, `<=`, `>`, `>=`, `=`, `==`, `!`,
and `!=`. They all function as would be expected (hopefully). More
information available in the search submenu with `:help time`.

## Viewing songs
To view all songs, the `view` option is used. It simply displays all of the
song entries.

# Miscellaneous
At the "main menu," only the first character is actually used for any
functional purpose:

```
Selection> applesauce
Song title: ...
...
```

The above has a major consequence, and will likely change somewhat in the
future. It currently doesn't allow "quick" usage tricks, such as:

```
Selection> remove 42
```

Which would remove index 42 without needing to go into the remove submenu
before entering 42.

A more useful reason to allow queries similar to the above is for viewing.
A potential future release will include the ability to do the following:

```
Selection> view 42
Index: 42
Title: Us and Them
Arist: Pink Floyd
Duration: 7:49
Album: The Dark Side of the Moon
...
```

It would also be nice to allow passing a range of indexes to display (e.g.,
`40-50`).

The above would also make searching quicker for power users:

```
Selection> search time: > 7:00
Index: 42
Title: Us and Them
Arist: Pink Floyd
Duration: 7:49
Album: The Dark Side of the Moon
...
```
