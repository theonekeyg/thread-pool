# Simple thread pool implementation

## Demo
Example usage exists in `src/main.c`. Interactive demo works by adding sample
task to thread pool to distribute between threads on each received line in
stdin. To test it yourself, simply do:
```sh
$ make && ./main
```
And push a couple of new lines to stdin.
