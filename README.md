# Autocomplete Functionality Implementation

This project provides an efficient implementation of autocomplete functionality that quickly retrieves the top matching terms based on a query string. The task is to process large files of terms and their associated importance weights, sort the terms lexicographically, and efficiently retrieve the best matches for a given query string.

## Features

- **Fast Retrieval**  
  Utilizes binary search to quickly locate the start and end positions of matching terms in a sorted list.
  
- **Case-sensitive Matching**  
  Only the beginning of the string is matched, and the matches are case-sensitive.
  
- **Sorting by Weight**  
  After retrieving the matching terms, they are sorted based on their associated weights to return the most important terms.

- **Efficient Querying**  
  The sorted list of terms is reused for multiple queries, ensuring efficient performance for multiple lookups.

## How It Works

1. **Input File**  
   The system reads a file containing terms and their associated importance weights. Each term and weight are stored for efficient processing.
   
2. **Sorting Terms**  
   The terms are sorted lexicographically for fast searching.

3. **Binary Search**  
   Binary search is used to find the range of terms that match the query string, minimizing search time.

4. **Extract Matching Terms**  
   Once the matching terms are identified, the system sorts them by weight and returns the top terms.

5. **Return Results**  
   The system returns the top N matching terms, where N is determined based on the specified threshold or the number of results you want to see.

## Example

Given the following terms with weights:

```
"Engineering" 50
"English" 40
"EngSci" 60
"Science" 30
"Engaged" 10
```

If the query string is `"Eng"`, the system will:

1. Locate all terms that start with "Eng".
2. Extract the matching terms: `"Engineering"`, `"English"`, and `"EngSci"`.
3. Sort these terms by their weights: `"EngSci"`, `"Engineering"`, `"English"`.
4. Return the top results based on the weight order.
