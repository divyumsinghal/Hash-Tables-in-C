#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_STRING_SIZE 100
#define ARRAY_SIZE 80000

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

    struct Person *Next;
} Person;

typedef struct LastName
{
    char Surname[MAX_STRING_SIZE];
    Person *Forename;
} LastName;

void trim(char *str)
{
    char *start = str;
    char *end;

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

int getline(char *buffer)
{
    if (fgets(buffer, MAX_STRING_SIZE, stdin) != NULL)
    {
        buffer[strcspn(buffer, "\n")] = 0;

        trim(buffer);

        return 1;
    }
    return 0;
}

int hash4 (char* str) {
    unsigned long hash = 5381;
    int c;

    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }

    return hash % ARRAY_SIZE;
}


int hash(char *str, LastName * hash_table)
{
    int index = hash4(str);

    if (hash_table[index].Surname[0] == '\0' || strcmp(hash_table[index].Surname, str) == 0)
    {
        return index;
    }
    else
    {
        collisions++;

        for (int i = index + 1; i < ARRAY_SIZE; i += 763)
        {
            if (hash_table[i].Surname[0] == '\0' || strcmp(hash_table[i].Surname, str) == 0)
            {
                return i;
            }

            collisions++;
        }
        for (int i = 0; i < index; i += 763)
        {
            if (hash_table[i].Surname[0] == '\0' || strcmp(hash_table[i].Surname, str) == 0)
            {
                return i;
            }

            collisions++;
        }
    }

    return -1;
}

int next_field(FILE *csv, char *word, int max_len)
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

void createHashTable(FILE *data, LastName *Table)
{
    int index = 0;

    char depositionID[MAX_STRING_SIZE];
    int personID;

    char word[MAX_STRING_SIZE];

    while (!feof(data))
    {
        next_field(data, word, MAX_STRING_SIZE);
        personID = atoi(word);

        next_field(data, word, MAX_STRING_SIZE);
        strcpy(depositionID, word);

        next_field(data, word, MAX_STRING_SIZE);
        index = hash(word, Table);

        // printing person ID, deposition ID, name and index
        // printf("Person ID: %d, Deposition ID: %s, Name: %s, Index: %d\n", personID, depositionID, word, index);

        if (index != -1)
        {
            Person *newPerson = (Person *)malloc(sizeof(Person));

            if (!newPerson)
            {
                printf("Error allocating memory for new person\n");
            }

            strcpy(Table[index].Surname, word);

            next_field(data, word, MAX_STRING_SIZE);
            strcpy(newPerson->Forename, word);

            next_field(data, word, MAX_STRING_SIZE);
            newPerson->Age = atoi(word);

            next_field(data, word, MAX_STRING_SIZE);
            strcpy(newPerson->PersonType, word);

            next_field(data, word, MAX_STRING_SIZE);
            strcpy(newPerson->Gender, word);

            next_field(data, word, MAX_STRING_SIZE);
            strcpy(newPerson->Nationality, word);

            next_field(data, word, MAX_STRING_SIZE);
            strcpy(newPerson->Religion, word);

            next_field(data, word, MAX_STRING_SIZE);
            strcpy(newPerson->Occupation, word);

            strcpy(newPerson->Deposition, depositionID);
            newPerson->PersonID = personID;

            newPerson->Next = Table[index].Forename;
            Table[index].Forename = newPerson;

            if (newPerson->Next != NULL)
                numTerms++;
        }
        else
        {
            printf("Error inserting %s at index %d\n", word, index);
        }
    }
}

void printHashTable(LastName *Table)
{
    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        if (Table[i].Surname[0] != '\0')
        {
            printf("%d: %s\n", i, Table[i].Surname);
        }
    }
}

void lookup(LastName *Table)
{
    char input[MAX_STRING_SIZE];

    printf("Enter a name to search: ");

    getline(input);

    while (strcmp(input, "quit") != 0)
    {
        int index = hash(input, Table);

        if (strcmp(Table[index].Surname, input) == 0)
        {
            // Printing all the details of everyone with the input surname as a table
            printf("Surname: %s\n", Table[index].Surname);

            // Table header with proper alignment
            printf("%-12s %-4s %-12s %-8s %-12s %-10s %-15s %-12s %-8s\n",
                   "Forename", "Age", "PersonType", "Gender", "Nationality",
                   "Religion", "Occupation", "Deposition", "PersonID");

            // Print the table rows with formatting
            for (Person *i = Table[index].Forename; i != NULL; i = i->Next)
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
}

void cleanup(LastName *hash_table)
{
    for (int i = 0; i < ARRAY_SIZE; ++i)
    {
        if (hash_table[i].Forename)
        {
            Person *current = hash_table[i].Forename;
            Person *temp;

            while (current)
            {
                temp = current;
                current = current->Next;

                free(temp);
                temp = NULL;
            }

            hash_table[i].Forename = NULL;
        }
    }
}

int main()
{
    LastName* HashMap = (LastName*)malloc(sizeof(LastName) * ARRAY_SIZE);

    printf("Loading people.csv...\n");

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

    createHashTable(csv, HashMap);

    printf("Capacity : %d\n", capacity);
    printf("Num Terms : %d\n", numTerms);
    printf("Collisions : %d\n", collisions);
    load = (double)numTerms / capacity * 100;
    printf("Load : %.2f%%\n", load);

    lookup(HashMap);

    fclose(csv);

    cleanup(HashMap);

    printf("Code Over\n");

    return 0;
}

// END Code