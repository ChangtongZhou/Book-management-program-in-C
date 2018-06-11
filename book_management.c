#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX_BOOKS 100000
#define MAX_AUTHS 100000
#define MAX_PUBS 100000


struct book {
	int id;
    char * title;
    struct author * auth; // used to assign auth_id
    struct publisher * pub; // used to assign pub_id
    float price;
    char * year;
};

struct author {
	int id;
    char * firstName;
    char  * lastName;
    int bookCnt;
};


struct publisher {
	int id;
    char * pubName;
    int bookCnt;
};

typedef struct book s_book;
typedef struct author s_auth;
typedef struct publisher s_pub;
int bookCount, authorCount, publisherCount;
int i_book = 0, i_auth = 0, i_pub = 0;
s_book * library[MAX_BOOKS];
s_auth * authors[MAX_AUTHS];
s_pub * pubs[MAX_PUBS];

void findSizes (char * file);
void loadDataFromDB(char * file);
void loadDataFromFile(char * file);
// void loadBooks(char *str, s_book ** library);
void loadBooks(char *str);
void loadAuths(char *str);
void loadPubs(char *str);
s_auth * findAuthor( int a_id);
s_pub * findPub( int p_id);
s_book * findBook( int b_id);
void findAuthorByFN (char * fn);
void findBookByTitle (char * title);

void loadBooksFromFile (char * book_id, char * author_id, char * pub_id, char * title, char * year, char * fn, char * ln, char * pub_name);
int compareBook(const void * a, const void * b);
int compareAuth(const void * a, const void * b);
int comparePub(const void * a, const void * b);
void printBooks ();
void printAuthors();
void printPubs ();
void flush();

void insertBooks ();
void insertAuthors(char * fn, char * ln);
// void deleteAuthor (int a_id, s_auth ** author_arr);
void deleteAuthor (int a_id);
void afterOperations(char * file);

void printMenu (void);
void choosedOptions(void);

int main (int argc, char * argv[]) {
	// findSizes(argv[1]);
	loadDataFromDB(argv[1]);
	printMenu();
	choosedOptions();
}

void printMenu (void) {
	printf("*****************************************************************\n");
	printf("Enter the number corresponding to operations:\n");
	printf("1) Insert author\n");
	printf("2) Delete author\n");
	printf("3) select books\n");
	printf("4) select authors \n");
	printf("5) select publishers \n");
	printf("6) Import additional books from a file\n");
	printf("7) quit\n");
	printf("*****************************************************************\n");
}

void choosedOptions(void) {
	int choice;
	char input[200];
	int a_id;
	char file[80];
	char insertFile[80];
	char deleFile[80];
	// fgets(input, 200, stdin);
	while (1) {
		puts("Please select a number from the menu.");
		scanf("%d", &choice);
		flush();
		switch (choice) {
			case 1:
				puts("Please use SQL syntax to insert an author: ");
				puts("e.g. insert into author values (\"firstname\", \"lastname\")");
				// scanf("%s\n", input);
				fgets(input, 200, stdin);
				printf("input string is: %s\n", input);
				char fname[200], lname[200];
				char *fn, *ln;
				char * p1 = strstr(input, "(");
				char * p2 = strstr(p1, ",");
				memcpy(fname, p1+2, p2-p1-3);
				fname[p2-p1-3] = '\0';
				fn = fname;
				// printf("first name: %s\n", fn);

				char * p3 = strstr(p2, " ");
				char * p4 = strstr(p3, ")");
				memcpy(lname, p3+2, p4-p3-3);
				lname[p4-p3-3] = '\0';
				ln = lname;
				// printf("last name: %s\n", ln);
				printf("you entered firstname: %s, lastname: %s\n", fn, ln);
				insertAuthors(fn,  ln);

				puts("Please enter a file name to store new data after insertion.");
				scanf("%s", insertFile);
				printf("file name is: %s\n", insertFile);
				afterOperations(insertFile);
				break;
			case 2:
				puts("Please use SQL syntax to delete an author: ");
				puts("e.g. delete author id");
				fgets(input, 200, stdin);
				sscanf(input, "%*[^0123456789]%d", &a_id);
				// printf("input string is: %s\n", input);
				printf("a_id is: %d\n", a_id);
				deleteAuthor(a_id);
				puts("Please enter a file name to store new data after deletion.");
				scanf("%s", deleFile);
				printf("file name is: %s\n", deleFile);
				afterOperations(deleFile);
				break;
			case 3:
				puts("Please use SQL syntax to select a book: ");
				puts("e.g.1. select * from books where title like '%word%'");
				puts("e.g.2. select * from books");
				fgets(input, 200, stdin);
				if (strchr(input, '*') != NULL && strchr(input, '%') != NULL) {
					puts("input has *");
					char item[20], itemName[20];

					char * p1 = strstr(input, "where ");
					char * p2 = strstr(p1, "like");
					memcpy(item, p1+6, p2-p1-7);
					item[p2-p1-7] = '\0';
					printf("look for item: %s\n", item);

					char * p3 = strstr(p2, "'%");
					char * p4 = strstr(p3, "%'");
					memcpy(itemName, p3+2, p4-p3-2);
					itemName[p4-p3-2] = '\0';
					// printf("look for item name: %s\n", itemName);
					if (strcmp(item, "title") == 0) {
						findBookByTitle (itemName);
					}


				} else if (strchr(input, '*') != NULL) {
					qsort(library, i_book, sizeof(s_book *), compareBook);
					printBooks();
				}

				break;
			case 4:
				puts("Please use SQL syntax to select authors: ");
				puts("e.g. select * from authors where firstname like '%word%'");
				puts("e.g.2. select * from authors");
				fgets(input, 200, stdin);
				if (strchr(input, '*') != NULL && strchr(input, '%') != NULL) {
					// puts("input has *");
					char item[20], itemName[20];

					char * p1 = strstr(input, "where ");
					char * p2 = strstr(p1, "like");
					memcpy(item, p1+6, p2-p1-7);
					item[p2-p1-7] = '\0';
					// printf("look for item: %s\n", item);

					char * p3 = strstr(p2, "'%");
					char * p4 = strstr(p3, "%'");
					memcpy(itemName, p3+2, p4-p3-2);
					itemName[p4-p3-2] = '\0';
					// printf("look for item name: %s\n", itemName);
					if (strcmp(item, "firstname") == 0) {
						findAuthorByFN(itemName);
					}
				} else if (strchr(input,'*') != NULL) {
					qsort(authors, i_auth, sizeof(s_auth *), compareAuth);
					printAuthors();
				}
				break;
			case 5:
				puts("Please use SQL syntax to select publishers: ");
				puts("e.g. select * from publishers");
				fgets(input, 200, stdin);
				if (strchr(input, '*') != NULL) {
					qsort(pubs, i_pub, sizeof(s_pub *), comparePub);
					printPubs();
				}
				break;
			case 6:
				puts("please type file name");
				scanf("%s", file);
				printf("file name is: %s\n", file);
				loadDataFromFile(file);
				break;
			case 7:
				printf("Quit the system. \n");
				return;
				break;

			default:
				printf ("Please input a number from 1 to 7 to choose!\n");
				break;
		}
		putchar('\n');

	}
}

void flush()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);

}

int compareBook(const void * a, const void * b) {
  s_book **b1 = a; // indirect pointers
  s_book **b2 = b;
  if ((*b1)->id == (*b2)->id) return 0;
  if ((*b1)->id < (*b2)->id) return -1;
  return 1;
}

int compareAuth(const void * a, const void * b) {
  s_auth **b1 = a; // indirect pointers
  s_auth **b2 = b;
  if ((*b1)->id == (*b2)->id) return 0;
  if ((*b1)->id < (*b2)->id) return -1;
  return 1;
}

int comparePub(const void * a, const void * b) {
  s_pub **b1 = a; // indirect pointers
  s_pub **b2 = b;
  if ((*b1)->id == (*b2)->id) return 0;
  if ((*b1)->id < (*b2)->id) return -1;
  return 1;
}

void findSizes (char * file) {
	FILE * pfile = fopen (file, "r");
	char line [80];
	if (pfile != NULL) {
		// printf ("loading file %s \n", file);
		fgets(line, sizeof(line), pfile);    // fgets read line by line in a file
		// printf("First line is: %s\n", line);
		sscanf(line, "%d, %d, %d", &bookCount, &authorCount, &publisherCount);
		fclose(pfile);
	}
}



void loadDataFromDB(char * file) {
	FILE * pfile = fopen (file, "r");
	fseek(pfile, 0L, SEEK_END);
	long size = ftell(pfile);
	rewind(pfile);
	char * filedata = malloc((int)size);
	fread(filedata, size, 1, pfile);

	char * p0 = filedata;
	if (pfile != NULL) {
		printf ("loading db %s \n", file);
		while (!feof(pfile)) {
			// library[i_book] = malloc(sizeof(s_book));

			char * p1 = strstr(p0, "author_id");
			if (p1 == NULL) break;
			char * p2  = strstr(p1, "{");
			char * p3 = strstr(p2, "}");
			char auth_block[p3-p2-1];
			memcpy(auth_block, p2+1, p3-p2-1);
			auth_block[p3-p2-1] = '\0';
			// printf("what is auth block: %s \n", auth_block);
			char * auth_str = auth_block;
			loadAuths(auth_str);

			// putchar('\n');

			p1 = strstr(p3, "publisher_id");
			if (p1 == NULL) break;
			p2 = strstr(p1, "{");
			p3 = strstr(p2, "}");
			char pub_block[p3-p2-1];
			memcpy(pub_block, p2+1, p3-p2-1);
			pub_block[p3-p2-1] = '\0';
			// printf("what is auth block: %s \n", pub_block);
			char * pub_str = pub_block;
			loadPubs(pub_str);

			p1 = strstr(p3, "book_id");
			if (p1 == NULL) break;
			p2 = strstr(p1, "{");
			p3 = strstr(p2, "}");
			char book_block[p3-p2-1];
			memcpy(book_block, p2+1, p3-p2-1);
			book_block[p3-p2-1] = '\0';
			// printf("what is book block: %s \n", book_block);
			// putchar('\n');
			char * book_str = book_block;
			loadBooks(book_str);
			// putchar('\n');


			p0 =p3;

			// printf("%s was loaded successfully. The database has %d books, %d authors, %d publishers.\n", file, bookCount, authorCount, publisherCount);

			printf("%s was loaded successfully. The database has %d books, %d authors, %d publishers.\n", file, i_book, i_auth, i_pub);
			putchar('\n');
			bookCount = i_book;
			authorCount = i_auth;
			publisherCount = i_pub;

		}
	}


}

void loadAuths(char *str) {
	char *p, *temp;
	int a_id, bookCnt;
	char fn[20];
	char ln[20];
	// i_auth = 0;
	p = strtok_r(str, "\n", &temp);
	do {
		  // printf("i_auth: %d \n", i_auth);
		  authors[i_auth] = malloc(sizeof(s_auth));

		  // printf("current line = %s\n", p);
		  sscanf(p, "%d %s %s %d", &a_id, ln, fn, &bookCnt);
		  authors[i_auth]->id = a_id;

		  authors[i_auth]->lastName = malloc(strlen(ln) + 1);
		  strcpy(authors[i_auth]->lastName, ln);

		  authors[i_auth]->firstName = malloc(strlen(fn) + 1);
		  strcpy(authors[i_auth]->firstName, fn);

		  // authors[i_auth]->bookCnt = bookCnt;
		  // printf("author id: %d\nlast name: %s\nfirst name: %s\nbook count: %d\n", authors[i_auth]->id, authors[i_auth]->lastName, authors[i_auth]->firstName, authors[i_auth]->bookCnt);
		  // putchar('\n');
		  i_auth++;
	} while ((p = strtok_r(NULL, "\n", &temp)) != NULL);
}


void loadPubs(char *str) {
	char *p, *temp;
	int p_id, bookCnt;
	char pn[20];
	// i_pub = 0
	p = strtok_r(str, "\n", &temp);
	do {
		  // printf("i_pub: %d \n", i_pub);
		  pubs[i_pub] = malloc(sizeof(s_pub));

		  // printf("current line = %s\n", p);
		  sscanf(p, "%d %s %d", &p_id, pn, &bookCnt);

		  pubs[i_pub]->id = p_id;

		  pubs[i_pub]->pubName = malloc(strlen(pn) + 1);
		  strcpy(pubs[i_pub]->pubName, pn);

		  i_pub++;
	} while ((p = strtok_r(NULL, "\n", &temp)) != NULL);
}

void loadBooks(char *str){
  char *p, *temp;
  int b_id, a_id, p_id;
  char title[20];
  float price;
  char yr[5];

  p = strtok_r(str, "\n", &temp);
  do {
  	  // printf("i_book: %d \n", i_book);
  	  library[i_book] = malloc(sizeof(s_book));
      // printf("current line = %s\n", p);
      sscanf(p, "%d %d %d %s %f %s", &b_id, &a_id, &p_id, title, &price, yr);
      library[i_book]->id = b_id;

      s_auth * newAuth = findAuthor(a_id);

      if (NULL == newAuth) {
      	// puts("the auth is NOT found in athors array");
      	library[i_book]->auth = malloc( sizeof(s_auth) );
      	library[i_book]->auth->id = a_id;
      	library[i_book]->auth->bookCnt = 1;

      	// printf("current i_auth %d\n", i_auth);
      	authors[i_auth] = library[i_book]->auth;
      	// printf("author id 3 is: %d\n", authors[3]->id);
      	i_auth++;
      } else {
      	// puts("the auth is found in athors array");
      	library[i_book]->auth = newAuth;
      	newAuth->bookCnt++;
      }

      s_pub * newPub = findPub(p_id);
      if (NULL == newPub) {
      	// puts("the pub is NOT found in pubs array");
      	library[i_book]->pub = malloc( sizeof(s_pub) );
      	library[i_book]->pub->id = p_id;
      	library[i_book]->pub->bookCnt = 1;

      	pubs[i_pub] = library[i_book]->pub;
      	// printf("pub id 2 is: %d\n", pubs[2]->id);
      	i_pub++;
      } else {
      	// puts("the pub is found in pubs array");
      	library[i_book]->pub = newPub;
      	newPub->bookCnt++;
      }
      library[i_book]->price = price;
      // printf("book price: %.2f \n", library[i_book]->price);
      library[i_book]->title = malloc(strlen(title) + 1);
      library[i_book]->year = malloc(strlen(yr) + 1);
      strcpy(library[i_book]->title, title);
      strcpy(library[i_book]->year, yr);

      // printf("book id: %d\nauthor id: %d\npub id: %d\nbook title: %s\nbook price: %.2f\nbook year: %s\n", library[i_book]->id, library[i_book]->auth->id, library[i_book]->pub->id, library[i_book]->title, library[i_book]->price, library[i_book]->year);
      // putchar('\n');
      i_book++;
  } while ((p = strtok_r(NULL, "\n", &temp)) != NULL);
}

// 3.1: import additional books into the library. example: import books2.txt
void loadDataFromFile(char * file) {
	FILE * pfile = fopen (file, "r");
	fseek(pfile, 0L, SEEK_END);
	long size = ftell(pfile);
	rewind(pfile);
	char * filedata = malloc((int)size);
	fread(filedata, size, 1, pfile);

	char * p0 = filedata;
	// char line [80];
	if (pfile != NULL) {
		printf ("loading file %s \n", file);
		while (!feof(pfile)) {
			library[i_book] = malloc(sizeof(s_book));
			char * p1 = strstr(p0, "@Book");
			if (p1 == NULL) break;
			char * p2 = strstr(p1, "book_id");
			char * p3 = strstr(p2, "{");
			char * p4 = strstr(p3, "}");
			char book_id[80];
			memcpy(book_id, p3+1, p4-p3-1);
			book_id[p4-p3-1] = '\0';


			p2 = strstr(p4, "author_id");
			p3 = strstr(p2, "{");
			p4 = strstr(p3, "}");
			char author_id[200];
			memcpy(author_id, p3+1, p4-p3-1); // memory copy from source to destiniation i.e. memcpy(destination, source_begin, source_len)
			author_id[p4-p3-1] = '\0';


			p2 = strstr(p4, "pub_id");
			p3 = strstr(p2, "{");
			p4 = strstr(p3, "}");
			char pub_id[200];
			memcpy(pub_id, p3+1, p4-p3-1); // memory copy from source to destiniation i.e. memcpy(destination, source_begin, source_len)
			pub_id[p4-p3-1] = '\0';


			p2 = strstr(p4, "title");
			p3 = strstr(p2, "{");
			p4 = strstr(p3, "}");
			char title[200];
			memcpy(title, p3+1, p4-p3-1); // memory copy from source to destiniation i.e. memcpy(destination, source_begin, source_len)
			title[p4-p3-1] = '\0';


			p2 = strstr(p4, "author");
			p3 = strstr(p2, "{");
			p4 = strstr(p3, "}");
			char author[200];
			char * fn, *ln;
			memcpy(author, p3+1, p4-p3-1); // memory copy from source to destiniation i.e. memcpy(destination, source_begin, source_len)
			author[p4-p3-1] = '\0';
			fn = author;
			char * p = strstr(fn, " ");
			*p = 0;
			ln = p + 1;

			p2 = strstr(p4, "publisher");
			p3 = strstr(p2, "{");
			p4 = strstr(p3, "}");
			char publisher[200];
			memcpy(publisher, p3+1, p4-p3-1); // memory copy from source to destiniation i.e. memcpy(destination, source_begin, source_len)
			publisher[p4-p3-1] = '\0';

			p2 = strstr(p4, "year");
			p3 = strstr(p2, "{");
			p4 = strstr(p3, "}");
			char year[200];
			memcpy(year, p3+1, p4-p3-1); // memory copy from source to destiniation i.e. memcpy(destination, source_begin, source_len)
			year[p4-p3-1] = '\0';


			p0 = p4;
			loadBooksFromFile(book_id, author_id, pub_id, title, year, fn, ln, publisher);
			fclose(pfile);
		}

		printf("%d books, %d authors, and %d publishers were imported. ", i_book - bookCount, i_auth - authorCount, i_pub - publisherCount);
		printf("The library now has %d books, %d authors, %d publishers\n", i_book, i_auth, i_pub);
		putchar('\n');
		bookCount = i_book;
		authorCount = i_auth;
		publisherCount = i_pub;
	}
}

void loadBooksFromFile (char * book_id, char * author_id, char * pub_id, char * title, char * year, char * fn, char * ln, char * pub_name) {
	int b_id = atoi(book_id);
	int a_id = atoi(author_id);
	int p_id = atoi(pub_id);

	s_book * newBook = findBook(b_id);
	if (NULL == newBook) {
	// puts("The new book is NOT found in library array");
		library[i_book] = malloc( sizeof(s_book) );
		library[i_book]->id = b_id;
		// printf("book price: %.2f \n", library[i_book]->price);
		library[i_book]->title = malloc(strlen(title) + 1);
		library[i_book]->year = malloc(strlen(year) + 1);
		strcpy(library[i_book]->title, title);
		strcpy(library[i_book]->year, year);
		library[i_book]->price = 10.0 + rand() * 89.99 / RAND_MAX;



	} else {
	// puts("The new book is found in library array");
		library[i_book] = newBook;
	}

	s_auth * newAuth = findAuthor(a_id);

	if (NULL == newAuth) {
		// puts("the auth is NOT found in athors array");
		library[i_book]->auth = malloc( sizeof(s_auth) );
		library[i_book]->auth->id = a_id;
		library[i_book]->auth->bookCnt = 1;

		library[i_book]->auth->firstName = malloc( strlen(fn) + 1 );
    	library[i_book]->auth->lastName = malloc( strlen(ln) + 1 );
      	strcpy(library[i_book]->auth->firstName, fn);
      	strcpy(library[i_book]->auth->lastName, ln);

		// printf("current i_auth %d\n", i_auth);
		authors[i_auth] = library[i_book]->auth;
		// printf("author id 3 is: %d\n", authors[3]->id);
		i_auth++;
	} else {
		// puts("the auth is found in athors array");

		if (newAuth->firstName == NULL || newAuth->lastName == NULL) {
			newAuth->firstName =  malloc(strlen(fn) + 1);
			newAuth->lastName = malloc(strlen(ln) + 1);
			strcpy(newAuth->firstName, fn);
			strcpy(newAuth->lastName, ln);
		}

		library[i_book]->auth = newAuth;
		newAuth->bookCnt++;



	}

	 s_pub * newPub = findPub(p_id);
	  if (NULL == newPub) {
	  	// puts("the pub is NOT found in pubs array");
	  	library[i_book]->pub = malloc( sizeof(s_pub) );
	  	library[i_book]->pub->id = p_id;
	  	library[i_book]->pub->bookCnt = 1;
	  	library[i_book]->pub->pubName = malloc(strlen(pub_name) + 1);
	  	strcpy(library[i_book]->pub->pubName, pub_name);
	  	pubs[i_pub] = library[i_book]->pub;
	  	// printf("pub id 2 is: %d\n", pubs[2]->id);
	  	i_pub++;
	  } else {
	  	// puts("the pub is found in pubs array");
	  	library[i_book]->pub = newPub;
	  	if (newPub->pubName == NULL) {
			newPub->pubName =  malloc(strlen(pub_name) + 1);
			strcpy(newPub->pubName, pub_name);
		}

		library[i_book]->pub = newPub;
	  	newPub->bookCnt++;
	  }

	i_book++;
}

void printBooks () {
	for (int i = 0; i < i_book; i++) {
		printf("book id: %d\n", library[i]->id);
	    printf("book title: %s\n", library[i]->title);
	    printf("book year: %s\n", library[i]->year);
	    printf("book price: %.2f\n", library[i]->price);
	    printf("author name: %s %s\n", library[i]->auth->firstName, library[i]->auth->lastName);
	    // printf("The book author is firstName: %s, lastName: %s\n", library[i]->auth->firstName, library[i]->auth->lastName);
	    // printf("The book price is: %f\n", library[i]->price);
	    putchar('\n');
	  }
}

void printAuthors () {
	for (int i = 0; i < i_auth; i++) {
		printf("author id: %d\n", authors[i]->id);
	    printf("author firstName: %s\n", authors[i]->firstName);
	    printf("author lastName: %s\n", authors[i]->lastName);
	    printf("# of books wrote: %d\n", authors[i]->bookCnt);
	    putchar('\n');
  	}
}

void printPubs () {
	for (int i = 0; i < i_pub; i++) {
		printf("publisher id: %d\n", pubs[i]->id);
	    printf("publisher name: %s\n", pubs[i]->pubName);
	    printf("# of books published: %d\n", pubs[i]->bookCnt);
	    putchar('\n');
  	}
}

s_book * findBook( int b_id) {
    for(int i=0; i<i_book; i++) {
        if ( b_id == (library[i]->id) )
            return library[i];
    }
    return NULL;
}

s_auth * findAuthor( int a_id) {
    for(int i=0; i<i_auth; i++) {
        if ( a_id == (authors[i]->id) )
            return authors[i];
    }
    return NULL;
}

s_pub * findPub( int p_id) {
    for(int i=0; i<i_pub; i++) {
        if ( p_id == (pubs[i]->id) )
            return pubs[i];
    }
    return NULL;
}

void findAuthorByFN (char * fn) {
	int found = 0;
	int pos = -1;
	for (int i = 0; i < i_auth; i++) {
		if (strcmp(authors[i]->firstName, fn) == 0) {
			found = 1;
			pos = i;
			break;
		}
	}
	if (found == 1) {
		puts("Author found:");
		printf("author id: %d\n", authors[pos]->id);
		printf("author name: %s, %s\n", authors[pos]->firstName, authors[pos]->lastName);
		printf("# of books wrote: %d\n", authors[pos]->bookCnt);
	} else {
		puts("Author NOT found.");
	}
}

void findBookByTitle (char * title) {
	int found = 0;
	int pos = -1;
	for (int i = 0; i < i_book; i++) {
		if (strcmp(library[i]->title, title) == 0) {
			found = 1;
			pos = i;
			break;
		}
	}

	if (found == 1) {
		puts("Book found:");
		printf("book id: %d\n", library[pos]->id);
	    printf("book title: %s\n", library[pos]->title);
	    printf("book year: %s\n", library[pos]->year);
	    printf("book price: %.2f\n", library[pos]->price);
	    printf("author name: %s %s\n", library[pos]->auth->firstName, library[pos]->auth->lastName);
	} else {
		puts("Book NOT found.");
	}
}

// insert author into lib
void insertAuthors(char * fn, char * ln){

	authors[i_auth] = malloc(sizeof(s_auth));
	authors[i_auth]->id = authors[i_auth - 1]->id + 1;
	authors[i_auth]->firstName = malloc(strlen(fn) + 1);
	strcpy(authors[i_auth]->firstName, fn);
	authors[i_auth]->lastName = malloc(strlen(ln) + 1);
	strcpy(authors[i_auth]->lastName, ln);
	authors[i_auth]->bookCnt = 0;
	i_auth++;
	authorCount = i_auth;
	printf("Element inserted successfully\n");
}

// delete author from lib
void deleteAuthor (int a_id) {
	// shift the elements
	int found = 0, pos = -1;
	for ( int i = 0; i < i_auth; i++) {
		if(authors[i]->id == a_id) {
			found = 1;
			pos = i;
			break;
		}
	}

	if (found == 1) {
		for ( int i = pos; i < i_auth - 1; i++) {
			authors[i] = authors[i+1];
		}
		i_auth--;
		printf("Element removed successfully\n");
		authorCount = i_auth;
	} else {
		printf("Author id %d is not found in the authors array\n", a_id);
	}

}



void afterOperations(char * file) {
	FILE * fp;
	if ((fp = fopen(file, "w")) == NULL)
	{
		fprintf(stdout,"Can't open \"afterInsertion.txt\" file.\n");
		exit(EXIT_FAILURE);
	}

	char * author_txt = "// author_id (501 - 800), last_name, first_name, book_count";
	char * pub_txt = "// publisher_id (801 - 1000), publisher_name, book_count";
	char * book_txt = "// book_id (1 - 500), author_id, publisher_id, title, publisher_id, price, year";

	fprintf(fp, "%d %d %d\n", bookCount, authorCount, publisherCount);
	fprintf(fp, "\n");

	fprintf(fp, "%s\n", author_txt);
	fprintf(fp, "{");
	for (int i = 0; i < i_auth; i++) {
		if (i == i_auth - 1)
			fprintf(fp, "%d %s %s %d", authors[i]->id, authors[i]->lastName, authors[i]->firstName, authors[i]->bookCnt);
		else
			fprintf(fp, "%d %s %s %d\n", authors[i]->id, authors[i]->lastName, authors[i]->firstName, authors[i]->bookCnt);
	}
	fprintf(fp, "}\n");
	fprintf(fp, "\n");

	fprintf(fp, "%s\n", pub_txt);
	fprintf(fp, "{");
	for (int i = 0; i < i_pub; i++) {
		// fprintf(fp, "%d %s %s %d\n", authors[i]->id, authors[i]->lastName, authors[i]->firstName, authors[i]->bookCnt);
		if (i == i_pub - 1)
			fprintf(fp, "%d %s %d", pubs[i]->id, pubs[i]->pubName, pubs[i]->bookCnt);
		else
			fprintf(fp, "%d %s %d\n", pubs[i]->id, pubs[i]->pubName, pubs[i]->bookCnt);
	}
	fprintf(fp, "}\n");
	fprintf(fp, "\n");

	fprintf(fp, "%s\n", book_txt);
	fprintf(fp, "{");
	for (int i = 0; i < i_book; i++) {
		if (i == i_book - 1)
			fprintf(fp, "%d %d %d %s %.2f %s", library[i]->id, library[i]->auth->id, library[i]->pub->id, library[i]->title, library[i]->price, library[i]->year);
		else
			fprintf(fp, "%d %d %d %s %.2f %s\n", library[i]->id, library[i]->auth->id, library[i]->pub->id, library[i]->title, library[i]->price, library[i]->year);
	}
	fprintf(fp, "}\n");
	fprintf(fp, "\n");

	if (fclose(fp) != 0)
		fprintf(stderr,"Error closing file\n");

	puts("Successfully loaded new data into your file after operations.");

}
