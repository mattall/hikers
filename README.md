

## Scenario

Hikers are going through a series of bridges. Only two hikers can cross at once. They cross at the slower persons speed. They have a single torch, and that must be used every time anyone cross the bridge. As the hikers cross each bridge, more people join their group. 

## Optimal solution

The optimal solution is to have the fastest person cross the bridge each time, ferrying each slower person across with them. 

This strategy minimizes the overall waiting time required for the torch to return to the contingent of hikers who have not crossed the bridge yet.

## Dependencies 

Tested with 

`g++ version 9.4.0`

Uses 

 `yaml-cpp`

https://github.com/jbeder/yaml-cpp

## Expected input

Expects a YAML file describing the bridges and hikers encountered. 

e.g., 

    bridge:
        - id      : 0
        length  : 100
        hikers  : 
            - name  : A
            pace  : 2.5
        - id      : 1
        length  : 100
        hikers  : 
            - name  : B
            pace  : 5
        
Bridge ids *must* be ascending and indexed from 0. 

Hiker names are arbitrary, primarily there for readability.

All fields are required.

`length` is the length of a bridge in feet. 
`hikers` have a `pace` in feet per minute.

## Build and Run

    g++ -o hikers hikers.cpp -lyaml-cpp
    ./hikers [input_file]

Runs with `test_2.yml` when no input is given. 

## Tests

- `test_1.yml`
Single bridge with one hiker    

- `test_2.yml`
Multiple bridges with varying lengths and hikers
    Test with multiple bridges where each bridge has different lengths and varying numbers of hikers.    

- `test_3.yml`
Multiple bridges with the same length but different hikers
    Test with multiple bridges
        - all bridges have the same length
        - all bridges have different sets of hikers with varying paces.
        - hikers arrival is progressively **slower** with each bridge. 

- `test_4.yml`
Multiple bridges with the same length but different hikers
    Hikers arrival is progressively **faster** with each bridge. 

