#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#define MAX_STRING_SIZE 20
#define ARRAY_SIZE 59

typedef struct Person
{
    char name[MAX_STRING_SIZE];
    int count;

} Person;

Person hash_table[ARRAY_SIZE];
int capacity = ARRAY_SIZE;
int numTerms = 0;
int collisions = 0;
double load = 0.0;

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

// DJB2 
int hash2(char *s)
{
	unsigned long hash_value = 5381;

	while (*s) {
		hash_value = ((hash_value << 5) + hash_value) + (unsigned char)(*s);
		s++;
	}

	unsigned int  index = hash_value % ARRAY_SIZE;

	return index;
}

int hash(char *str)
{

    // int index = 0;
    // for (int i = 0; str[i] != '\0'; i++)
    // {
    //     // index += str[i];
    //     index = 1 + (index + str[i]) % (ARRAY_SIZE - 1);
    // }

    // index = index % ARRAY_SIZE;

    int index = hash2(str);

    if (hash_table[index].name[0] == '\0' || strcmp(hash_table[index].name, str) == 0)
    {
        return index;
    }
    else
    {
        collisions++;

        for (int i = index + 1; i < ARRAY_SIZE; i+= 7)
        {
            if (hash_table[i].name[0] == '\0' || strcmp(hash_table[i].name, str) == 0)
            {
                return i;
            }

            collisions++;
        }
        for (int i = 0; i < index; i+= 7)
        {
            if (hash_table[i].name[0] == '\0' || strcmp(hash_table[i].name, str) == 0)
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

int main()
{
    int index = 0;

    FILE* csv = fopen("data/names.csv", "r");

    if (!csv)
    {
        printf("Error opening file.\n");
        return 1;
    }
    else
    {
        printf("names.csv loaded!\n");
    }

    char word[MAX_STRING_SIZE];

    while (!feof(csv))
    {
        next_field(csv, word, 1024);

        index = hash(word);

        // printf("%s: %d\n", word, index);

        if (hash_table[index].name[0] == '\0' && index != -1)
        {
            strcpy(hash_table[index].name, word);
            hash_table[index].count = 1;
            numTerms++;
        }
        else if (strcmp(hash_table[index].name, word) == 0)
        {
            hash_table[index].count++;
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
        if (hash_table[i].name[0] != '\0')
        {
            printf("%d: %s: %d\n", i, hash_table[i], hash_table[i].count);
        }
    }
    */


   char input[MAX_STRING_SIZE];

    printf("Enter a name to search: ");

    getline(input);

    while (strcmp(input, "quit") != 0)
    {
        index = hash(input);

        if (strcmp(hash_table[index].name, input) == 0)
        {
            printf("%s found at index %d, frequency: %d\n", input, index, hash_table[index].count);
        
        }
        else
        {
            printf("%s not in table\n", input);
        }

        printf("Enter a name to search: ");
        getline(input);
    }

    fclose(csv);
    return 0;
}

// END Code