# alphabetic-db
## Concept
Main concept of this project is database made for storing words and sentences. Because of narrow range of usability of this type of database, main purpose is optimizing and making read/write operations fast.
#### operations
- Write
You can write to DB inserting as much words/sentences you want (ENTER after everyone) until EOF (CTRL+D).
Time complexity: $ O = (nlogn + n^2 + m) $ n - user input vector, m - file length
```
    Take data from user until EOF, append in vector
    Sort vector (ignoring upper case)
```

