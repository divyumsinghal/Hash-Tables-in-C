#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_STRING_SIZE 100
#define ARRAY_SIZE 65000

int capacity = ARRAY_SIZE;
int numTerms = 0;
int collisions = 0;
double load = 0.0;

typedef struct Person
{
	char Forename[MAX_STRING_SIZE];
	int PersonID;
	char Deposition[20];
	int Age;
	char PersonType[20];
	char Gender[20];
	char Nationality[20];
	char Religion[20];
	char Occupation[20];

	struct Person* Next;
} Person;

typedef struct LastName
{
	char Surname[MAX_STRING_SIZE];
	Person* Forename;
} LastName;

LastName hash_table[ARRAY_SIZE];

void trim(char* str)
{
	char* start = str;
	char* end;

	while (isspace((unsigned char)*start))
	{
		start++;
	}

	if (*start == 0)
	{
		str[0] = '\0';
		return;
	}

	end = start + strlen(start) - 1;
	while (end > start && isspace((unsigned char)*end))
	{
		end--;
	}

	*(end + 1) = '\0';

	memmove(str, start, end - start + 2);
}

int getline(char* buffer)
{
	if (fgets(buffer, MAX_STRING_SIZE, stdin) != NULL)
	{
		buffer[strcspn(buffer, "\n")] = 0;

		trim(buffer);

		return 1;
	}
	return 0;
}

unsigned int  hashing(char* s)
{
	unsigned int hash_value = 5381;

	while (*s) {
		hash_value = ((hash_value << 5) + hash_value) + (unsigned char)(*s);  // hash * 33 + c
		s++;
	}

	return hash_value;
}

int hash(char* str)
{

	unsigned int index = hashing(str) % ARRAY_SIZE;

	// printf("The index is: %d \n", index);  


	if (hash_table[index].Surname[0] == '\0' || strcmp(hash_table[index].Surname, str) == 0)
	{
		return index;
	}
	else
	{
		collisions++;

		for (int i = index + 1; i < ARRAY_SIZE; i += 763)
		{
			int new_index = (index + i * hashing(str)) % ARRAY_SIZE;

			if (hash_table[new_index].Surname[0] == '\0' || strcmp(hash_table[new_index].Surname, str) == 0)
			{
				return i;
			}

			collisions++;
		}
		for (int i = 0; i < index; i += 763)
		{
			int new_index = (index + i * hashing(str)) % ARRAY_SIZE;

			if (hash_table[new_index].Surname[0] == '\0' || strcmp(hash_table[new_index].Surname, str) == 0)
			{
				return i;
			}

			collisions++;
		}
	}

	return -1;
}

int next_field(FILE* csv, char* word, int max_len)
{
	int i = 0;
	bool inside_quotes = false;
	char c;

	while ((c = fgetc(csv)) != EOF)
	{
		if (i >= max_len - 1)
			break;

		if (c == '"')
		{
			inside_quotes = !inside_quotes;
			continue;
		}

		if (c == ',' && !inside_quotes)
		{
			word[i] = '\0';
			return 0;
		}

		if (c == '\n' && !inside_quotes)
		{
			word[i] = '\0';
			return 1;
		}

		if (i < max_len - 1)
		{
			word[i++] = c;
		}
	}

	word[i] = '\0';

	return feof(csv) ? -1 : 0;
}

void cleanup()
{
	for (int i = 0; i < ARRAY_SIZE; i++)
	{
		Person* current = hash_table[i].Forename;
		Person* temp;

		/*
		while (current != NULL)
		{
			temp = current;
			Person* next = current->Next;
			free(temp);
			current = next;
		}
		*/

		while (current) {
			temp = current;
			current = current->Next;
			free(temp);
		}


	}
}

int main()
{
	// Initializing the hash table
	for (int i = 0; i < ARRAY_SIZE; i++)
	{
		hash_table[i].Surname[0] = '\0';
		hash_table[i].Forename = NULL;
	}

	printf("Loading people.csv...\n");

	int index = 0;

	char depositionID[MAX_STRING_SIZE];
	int personID;

	FILE* csv = fopen("data/people.csv", "r");

	if (!csv)
	{
		printf("Error opening file.\n");
		return 1;
	}
	else
	{
		printf("people.csv loaded!\n");
	}

	char word[MAX_STRING_SIZE];

	while (!feof(csv))
	{
		next_field(csv, word, MAX_STRING_SIZE);
		personID = atoi(word);

		next_field(csv, word, MAX_STRING_SIZE);
		strcpy(depositionID, word);

		next_field(csv, word, MAX_STRING_SIZE);
		index = hash(word);

		// printing person ID, deposition ID, name and index
		// printf("Person ID: %d, Deposition ID: %s, Name: %s, Index: %d\n", personID, depositionID, word, index);

		if (index != -1)
		{

			Person* newPerson = (Person*)malloc(sizeof(Person));

			if (!newPerson)
			{
				printf("Error allocating memory for new person\n");
				return 1;
			}

			strcpy(hash_table[index].Surname, word);

			next_field(csv, word, MAX_STRING_SIZE);
			strcpy(newPerson->Forename, word);

			next_field(csv, word, MAX_STRING_SIZE);
			newPerson->Age = atoi(word);

			next_field(csv, word, MAX_STRING_SIZE);
			strcpy(newPerson->PersonType, word);

			next_field(csv, word, MAX_STRING_SIZE);
			strcpy(newPerson->Gender, word);

			next_field(csv, word, MAX_STRING_SIZE);
			strcpy(newPerson->Nationality, word);

			next_field(csv, word, MAX_STRING_SIZE);
			strcpy(newPerson->Religion, word);

			next_field(csv, word, MAX_STRING_SIZE);
			strcpy(newPerson->Occupation, word);

			strcpy(newPerson->Deposition, depositionID);
			newPerson->PersonID = personID;

			if (!hash_table[index].Forename)
				numTerms++;

			newPerson->Next = hash_table[index].Forename;
			hash_table[index].Forename = newPerson;

		}
		else
		{
			printf("Error inserting %s at index %d\n", word, index);
		}
	}

	printf("Capacity : %d\n", capacity);
	printf("Num Terms : %d\n", numTerms);
	printf("Collisions : %d\n", collisions);
	load = (double)numTerms / capacity * 100;
	printf("Load : %.2f%%\n", load);

	/*
	for (int i = 0; i < ARRAY_SIZE; i++)
	{
		if (hash_table[i].Surname[0] != '\0')
		{
			printf("%d: %s: %d\n", i, hash_table[i].Surname, hash_table[i].count);
		}
	}
	*/

	char input[MAX_STRING_SIZE];

	printf("Enter a name to search: ");

	getline(input);

	while (strcmp(input, "quit") != 0)
	{
		index = hash(input);

		if (strcmp(hash_table[index].Surname, input) == 0)
		{
			// Printing all the details of everyone with the input surname as a table
			printf("Surname: %s\n", hash_table[index].Surname);

			// Table header with proper alignment
			printf("%-12s %-4s %-12s %-8s %-12s %-10s %-15s %-12s %-8s\n",
				"Forename", "Age", "PersonType", "Gender", "Nationality",
				"Religion", "Occupation", "Deposition", "PersonID");

			// Print the table rows with formatting
			for (Person* i = hash_table[index].Forename; i != NULL; i = i->Next)
			{
				printf("%-12s %-4d %-12s %-8s %-12s %-10s %-15s %-12s %-8d\n",
					i->Forename,
					i->Age,
					i->PersonType,
					i->Gender,
					i->Nationality,
					i->Religion,
					i->Occupation,
					i->Deposition,
					i->PersonID);
			}
		}

		else
		{
			printf("%s not in table\n", input);
		}

		printf("Enter a name to search: ");
		getline(input);
	}

	fclose(csv);

	// cleanup();

	printf("Code Over\n");

	return 0;
}

// END Code