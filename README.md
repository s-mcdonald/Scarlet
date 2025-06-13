# Scarlet
A lightweight, in-memory key-value data store inspired by Redis. Fast, minimalist, and written in C++ for simplicity and speed


## Project goals
This is a learning exercise and definitely not intended for production use.

* Not concerned about security however I may eventually include some token based security.
* Not concerned for multi threading/concurrency however I may also include this later in development.
* Support for strings, integers, and lists (similar to Redis primitives).
* Persistent Mode - Optionally dump the in-memory store to disk and restore it later.
