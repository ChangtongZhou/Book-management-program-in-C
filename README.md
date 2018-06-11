This is a book management program that is used to manipulate book data from external file into internal data structures in C.

The following explains how the program is designed and used:

1.	Design library.db as txt file:
a.	First line contains current book count, author count and publisher count in database
// bookCount, authorCount, publisherCount
e.g. 200 28 5

b.	The following lines contains information about books, authors and publishers:
// book_id (2 - 201), author_id, publisher_id, book_count, price, year
e.g. 2, 5, 3, 89.99, 2010

// author_id (202 - 230), last_name, first_name, book_count

// publisher_id (230 - 235), publisher_name

2.	Newbooks.txt file:
a.	Contains information of each book needs to be added
b.	The format looks like the following:
@Book{
	book_id: {6},
	author_id: {508},
	pub_id: {805},
    title: {The Property of A Gentleman},
    author: {Helen Dickson},
    publisher: {puba},
    year: {2005}
}

3.	Internal data structures:
a.	Mainly used array of structures:
i.	Struct book
ii.	Struct author
iii.	Struct publisher

4.	Memory management technique:
a.	Malloc
b.	Memcpy

5.	Searching algorithm used:
a.	Quick sort

6.	When the program finished an insertion operation:
a.	The new data will be stored in a file that is defined by users’ input
i.	E.g. afterInsertion.txt

7.	When the program finished a deletion operation:
a.	The new data will be stored in a file that is defined by user’s input
i.	E.g. afterDeletion.txt
