# Assignment 1: Hash Tables  
**Trinity College Dublin**  

---

## Introduction

- This assignment revolved around using hash tables to efficiently store data and retrieve it.
- The data was a file of Irish surnames. 
- In part four, we used people csv, which was a file of Irish surnames, fornames and a bunch of other data per person.
- This was stored efficiently in a hash table. 
- In the last part, alongside with a hash table, linked lists were used to store values for each table. 
- Structs called people surname and Person were made to efficiently store the data. 

---

## Task 1: Hash Table Implementation with Linear Probing (5 Marks)

In this task, a very simple hash function was implemented. This was taken from the assignment as outlined. It was named hash 1 and simply revolved around using the ascii number of each character, and then adding it to the hash, which was an integer that was the summation of all the ascii keys before it. Then the remainder was taken upon division with array size in order to ensure that the final number was between zero and array size one, so that it could be stored as a key inside the hash map. Finally, the hash was returned. This was really simple, but the issue is that a lot of similar names can give you the same key. Therefore, linear probing was used after that in order to make sure that we could take care of collisions. 

### Code Highlights:
- **Array-based Storage**: An array was used to store keys (surnames) and values (counts). 
- **Linear Probing**: For collision handling, linear probing was applied.

### Results:

The result upon running the code is given below. 

  ```  cmd
names.csv loaded!
Capacity : 59
Num Terms : 39
Collisions : 45
Load : 66.10%
Enter a name to search: Keaton
Name found at index 43, count: 4
Enter a name to search: mc Cody
Name found at index 8, count: 2
Enter a name to search: Divyum
Divyum not in table
Enter a name to search: fhei
fhei not in table
Enter a name to search: quit
  ```


---

## Task 2: Improved Hash Function (3 Marks)

In this task, I implemented a better hash function, as I found online called **DJB2**. 


### Code Highlights:

- **Improved Hash Function**: The dj B-2 hash function starts with an arbitrary hash key. After that, each character is multiplied by 32 before moving on to the next character and adding it to it. This means that the hash key is more unique and reduces the number of collisions. 


### Results:


The result upon running the code is given below. 

  ```  cmd

  names.csv loaded!
Capacity : 59
Num Terms : 39
Collisions : 34
Load : 66.10%
Enter a name to search: Keaton
Keaton found at index 30, frequency: 4
Enter a name to search: mc Cody
mc Cody found at index 25, frequency: 2
Enter a name to search: Divyum
Divyum not in table
Enter a name to search: fhei
fhei not in table
Enter a name to search: quit

 ```

### References:

https://www.reddit.com/r/cs50/comments/ggvgvm/pset5_djb2_hash_function/
http://www.cse.yorku.ca/~oz/hash.html

---

## Task 3: Double Hashing (2 Marks)

In this function, instead of linear probing, double hashing was used. This is a more efficient way of dealing with collisions as it reduces collisions in the long run by avoiding things like clustering and making the code more efficient. This means that when we go back to retrieve the value at an index, then we don't have to deal with clusters. And this reduces our time complexity in insertion and retrieval. 

### Code Highlights:
- **Double Hashing**: The way double hashing works is that first we get an index using hashing functions that we already have. I use the function that I had implemented in task 2. After this, we use a function to get a step size, which is unique to our key value. This was already given to us in the assignment. After that, we use steps of this unique size, as opposed to linear probing, where we use the same step size for all key values. This means that our step sizes for each given input is unique. And this. helps us to avoid clustering and makes the code more efficient. 


### Results:

The result upon running the code is given below. 

  ```  cmd

  names.csv loaded!
Capacity : 59
Num Terms : 39
Collisions : 19
Load : 66.10%
Enter a name to search: Keaton
Keaton found at index 29, frequency: 4
Enter a name to search: mc Cody
mc Cody found at index 25, frequency: 2
Enter a name to search: Divyum
Divyum not in table
Enter a name to search: fhei
fhei not in table
Enter a name to search: quit

 ```

 ### References:
 https://www.youtube.com/watch?v=EEjdu-85fWQ

---

## Task 4: Hash Table for Information Retrieval

- This time we have a people.csv, which has a bunch of names, and we're going to group them by surname. 
- Each person has a set of details, like deposition, age, person type, etc, which needs to be stored. 
- They will be grouped by their surname and then stored inside the hash table as linked lists. 
- So the key will be the hashing of the surname and then the value will be a struct that stores the surname and the linked list instead of just one name. 

### Approach:

- Up until now, our people struck had only two members, the first was the name itself, and the 2nd one was the count.
- But now we have a lot of information, so we will group each person by their surname. 
- So we will have a struct called surname. And this will have its own members. 
- The 1st 1 will be a linked list of each name that comes underneath it, the next Surname itself. 
- So the linked list of each person now also has more members, as we have information like the deposition, age, person type, gender, nationality, religion as well. 
- So in this linked list, we store each person one by one.

### Results:

The result upon running the code is given below. 

  ```  cmd
  Loading people.csv...
people.csv loaded!
Capacity : 65000
Num Terms : 16743
Collisions : 5719
Load : 25.76%
Enter a name to search: Keaton
Surname: Keaton
Forename     Age  PersonType   Gender   Nationality  Religion   Occupation      Deposition   PersonID
Richard      0    Debtor       Male     Unknown      Unknown    Gentleman       828203r294   43467
Edward       0    Unknown      Unknown  Unknown      Unknown    Unknown         818077r099   1322
Mortagh      0    Unknown      Unknown  Unknown      Unknown    Unknown         815273r348   471
Redmond      0    Unknown      Unknown  Unknown      Unknown    Unknown         815273r348   470
Redmond      0    Unknown      Unknown  Unknown      Unknown    Unknown         815273r348   469
James        0    Unknown      Unknown  Unknown      Unknown    Unknown         815273r348   468
Enter a name to search: Synott
Surname: Synott
Forename     Age  PersonType   Gender   Nationality  Religion   Occupation      Deposition   PersonID
Richard      0    Victim       Unknown  Unknown      Unknown    Unknown         810325r339   17426
Richard      0    Mentioned    Unknown  Unknown      Unknown    Unknown         815156r239   7234
*            0    Unknown      Unknown  Unknown      Unknown    Unknown         818107r120   2145
Edmond       0    Unknown      Unknown  Unknown      Unknown    Unknown         818064r090   1121
Enter a name to search: Singhal
Singhal not in table
Enter a name to search: mc Cody
Surname: mc Cody
Forename     Age  PersonType   Gender   Nationality  Religion   Occupation      Deposition   PersonID
Richard      0    Unknown      Male     Unknown      Unknown    Sergeant        812137r136   42304
Rich:        0    Unknown      Unknown  Unknown      Unknown    Unknown         818253v232   688
Richard      0    Unknown      Unknown  Unknown      Unknown    Unknown         818248v226   505
Rich:        0    Unknown      Unknown  Unknown      Unknown    Unknown         818247r225   486
Enter a name to search: quit
Code Over
  ```

---

# Further Improvements

- The code in task four can be further improved, as mentioned in the assignment. 
- Like, for example, we have several names that are similar to each other, but are not stored inside the same structure.
- This can be improved by, instead of using a simple hashing function that returns one integer as a key, we can use arrays or vectors to store each hash
- This is used by popular search engines such as Google, Bing, etc. 
- This is something that we covered last year in Engineering maths 3 about how this kind of vectors can be used to symbolically store data in an N-th dimensional space. 
- The same technology is currently used by popular LLMs like Open AI's GPT, in which each name is actually stored as a vector, and that is then fed into neural networks and then further computation is done.
- Doing this can also make a retrieval faster, as we don't have to waste a lot of space, like we have to do with hash tables. 
- And this is important for some bigger search engines like Google, in which they have to go through millions and millions of data sets within milliseconds in order to give immediate search results. 
- This can also help us make sure that we can deal with similar names as now, instead of this exact 1 vector, we will be searching for vectors within a reasonable space. 
- And by doing a probability distribution, we can return all the names which not only match the vector exactly, but also vectors that are in a reasonably close space.
- This can make sure that if I search for a name like Synot, I will get the search results for similar ones, like Synnot, Synnott and so on. 

