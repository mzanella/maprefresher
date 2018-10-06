# Example
`example.cpp` contains an example of usage of `map_refresher` class. In the
example the refreshing function double the element of the map each 5s until the first element become grater of 100. After it shows that after call to `stop` method the map will be not refreshed anymore, waiting 10s before re-printing the map. Finally it shows how `sync_refres_now` works.

## Compilation

```bash
    .../maprefresher/example$ g++ -std=c++11 example.cpp -pthread -o example
```

## Run

```bash
    .../maprefresher/example$ ./example
```