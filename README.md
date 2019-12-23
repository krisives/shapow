
# shapow

Find a SHA-256 hash that ends with a given suffix.

## Building

```
make update # fetches the PicoSHA2 header
make
```

## Usage

```
./shapow abcdef
hash e38450c7008711d86a4d6c2039c8633a1ed637281b96888d7d9ff257aaabcdef
4cbab1bbb03b4a10aef586b6
```

The value that matches the hash suffix will be printed to standard output with
standard error getting the hash itself.
